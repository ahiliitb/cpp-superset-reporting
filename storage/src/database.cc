#include "database.h"
#include <stdexcept>
#include <memory>

#include <pqxx/pqxx>
#include <vector>
#include <string>
#include <stdexcept>
#include <memory>

DatabaseConnectionPool::DatabaseConnectionPool(const std::string &username,
                                               const std::string &password,
                                               const std::string &host,
                                               const std::string &port,
                                               const std::string &dbname,
                                               const std::string &dbtype,
                                               size_t maxConns) : maxConnections(maxConns)
{
    connectionString = "dbname=" + dbname +
                       " user=" + username +
                       " password=" + password +
                       " hostaddr=" + host +
                       " port=" + port;

    for (size_t i = 0; i < maxConnections; ++i)
    {
        try
        {
            auto *conn = new pqxx::connection(connectionString);
            if (conn->is_open())
            {
                connectionPool.push_back(conn);
            }
            else
            {
                delete conn;
                throw std::runtime_error("Failed to open database connection");
            }
        }
        catch (const std::exception &e)
        {
            throw std::runtime_error(std::string("Error initializing connection: ") + e.what());
        }
    }
}

pqxx::connection *DatabaseConnectionPool::getConnection()
{
    if (connectionPool.empty())
    {
        throw std::runtime_error("No available connections in the pool.");
    }

    pqxx::connection *conn = connectionPool.back();
    connectionPool.pop_back();
    return conn;
}

void DatabaseConnectionPool::releaseConnection(pqxx::connection *conn)
{
    if (!conn)
        return;

    if (connectionPool.size() < maxConnections)
    {
        if (conn->is_open())
        {
            connectionPool.push_back(conn);
        }
        else
        {
            try
            {
                delete conn;
                auto *newConn = new pqxx::connection(connectionString);
                connectionPool.push_back(newConn);
            }
            catch (const std::exception &e)
            {
                // Log error or handle reconnection failure
            }
        }
    }
    else
    {
        delete conn; // Free connection if pool is full
    }
}

std::vector<std::vector<std::string>> DatabaseConnectionPool::executeCommand(const std::string &sqlCommand, const std::vector<std::string> &values)
{
    pqxx::connection *conn = getConnection();
    std::vector<std::vector<std::string>> result;

    try
    {
        pqxx::work txn(*conn);

        // Prepare the SQL command with placeholders for values
        std::string preparedCommand = sqlCommand;
        for (size_t i = 0; i < values.size(); ++i)
        {
            std::string placeholder = "$" + std::to_string(i + 1);
            size_t pos = preparedCommand.find(placeholder);
            if (pos != std::string::npos)
            {
                preparedCommand.replace(pos, placeholder.length(), values[i]);
            }
        }

        // Execute the query
        pqxx::result res = txn.exec(preparedCommand);

        // Convert the result set into a vector of vectors
        for (const auto &row : res)
        {
            std::vector<std::string> rowData;
            for (const auto &field : row)
            {
                rowData.push_back(field.is_null() ? "" : field.c_str());
            }
            result.push_back(std::move(rowData));
        }

        txn.commit();
    }
    catch (const std::exception &e)
    {
        conn->disconnect(); // Rollback by disconnecting on failure
        releaseConnection(conn);
        throw std::runtime_error("Error executing SQL command: " + std::string(e.what()));
    }

    releaseConnection(conn);
    return result;
}

std::vector<std::vector<std::string>> DatabaseConnectionPool::executeMultipleCommands(
    const std::vector<std::pair<std::string, std::vector<std::string>>> &queries)
{
    pqxx::connection *conn = getConnection();
    std::vector<std::vector<std::string>> result;

    try
    {
        pqxx::work txn(*conn);

        for (const auto &query : queries)
        {
            std::string sqlCommand = query.first;
            const std::vector<std::string> &values = query.second;

            // Prepare the SQL command with placeholders for values
            for (size_t i = 0; i < values.size(); ++i)
            {
                std::string placeholder = "$" + std::to_string(i + 1);
                size_t pos = sqlCommand.find(placeholder);
                if (pos != std::string::npos)
                {
                    sqlCommand.replace(pos, placeholder.length(), values[i]); // Escape value
                }
            }

            // Execute the query
            pqxx::result res = txn.exec(sqlCommand);

            // Convert the result set into a vector of vectors (if needed)
            for (const auto &row : res)
            {
                std::vector<std::string> rowData;
                for (const auto &field : row)
                {
                    rowData.push_back(field.is_null() ? "" : field.c_str());
                }
                result.push_back(std::move(rowData));
            }
        }

        txn.commit(); // Commit all queries together
    }
    catch (const std::exception &e)
    {
        conn->disconnect(); // Rollback by disconnecting on failure
        releaseConnection(conn);
        throw std::runtime_error("Error executing SQL commands: " + std::string(e.what()));
    }

    releaseConnection(conn);
    return result;
}

void DatabaseConnectionPool::closeConnections()
{
    for (auto *conn : connectionPool)
    {
        if (conn)
        {
            conn->disconnect();
            delete conn;
        }
    }
    connectionPool.clear();
}

DatabaseConnectionPool::~DatabaseConnectionPool()
{
    closeConnections();
}

void DatabaseConnectionPool::create_table(std::string table_name, std::vector<TableColumn> table_schema)
{
    std::vector<std::string> column_vec;
    for (TableColumn table_col : table_schema)
    {
        column_vec.emplace_back(table_col.string_repr());
    }

    std::string columns = join(column_vec, ", ");

    std::string query = "CREATE TABLE " + table_name + " (" + columns + ");";
    this->executeCommand(query);
}

std::map<std::string, std::vector<std::pair<std::string, std::string>>> DatabaseConnectionPool::fetch_all_table_schema()
{
    std::string query = "SELECT table_name, column_name, data_type FROM information_schema.columns WHERE table_schema = 'public' ORDER BY table_name, ordinal_position;";

    std::map<std::string, std::vector<std::pair<std::string, std::string>>> schema;

    auto result = executeCommand(query);
    for (const auto &row : result)
    {
        std::string table_name = row[0];
        std::string column_name = row[1];
        std::string data_type = row[2];

        schema[table_name].emplace_back(column_name, data_type);
    }

    return schema;
}

std::unordered_set<std::string> DatabaseConnectionPool::fetch_table_names()
{
    std::string query = "SELECT table_name FROM information_schema.tables WHERE table_schema = 'public' AND table_type = 'BASE TABLE';";

    auto rows = this->executeCommand(query);

    std::unordered_set<std::string> table_names;
    for (auto row : rows)
    {
        table_names.insert(row[0]);
    }
    return table_names;
}

void DatabaseConnectionPool::insert_data(std::string table_name, std::vector<TableColumn> table_schema, std::vector<std::string> value_to_store)
{
    std::vector<std::string> column_vec;
    for (TableColumn table_col : table_schema)
    {
        column_vec.emplace_back(table_col.name);
    }

    std::string columns = join(column_vec, ", ");

    // Prepare placeholders and formatted values
    std::vector<std::string> placeholder;
    std::vector<std::string> formated_values;

    for (size_t i = 0; i < table_schema.size(); i++)
    {
        TableColumn tab_col = table_schema[i];
        std::string value = value_to_store[i];

        value = "'" + value + "'";

        if (tab_col.datatype == "TIMESTAMP")
        {
            if (tab_col.data_format != "NULL")
            {
                placeholder.push_back("to_timestamp($" + std::to_string(i + 1) + ", '" + tab_col.data_format + "')");
            }
            else
            {
                placeholder.push_back("to_timestamp($" + std::to_string(i + 1) + ")");
            }
            formated_values.push_back(value);
        }
        else if (tab_col.isArray)
        {
            placeholder.push_back("ARRAY[$" + std::to_string(i + 1) + "]");
            formated_values.push_back(value); // Assuming 'value' is formatted correctly for an array
        }
        else
        {
            placeholder.push_back("$" + std::to_string(i + 1));
            formated_values.push_back(value);
        }
    }

    std::string placeholder_str = join(placeholder, ", ");
    std::string query = "INSERT INTO " + table_name + " (" + columns + ") VALUES (" + placeholder_str + ") ON CONFLICT DO NOTHING";

    // Execute the query with formatted values
    this->executeCommand(query, formated_values);
}

void DatabaseConnectionPool::insert_multiple_data(std::string table_name, std::vector<TableColumn> table_schema, std::vector<std::vector<std::string>> values_to_store)
{
    std::vector<std::string> column_vec;
    for (TableColumn table_col : table_schema)
    {
        column_vec.emplace_back(table_col.name);
    }

    std::string columns = join(column_vec, ", ");

    std::vector<std::pair<std::string, std::vector<std::string>>> all_queries;
    for(auto value_to_store: values_to_store)
    {
        // Prepare placeholders and formatted values
        std::vector<std::string> placeholder;
        std::vector<std::string> formated_values;

        for (size_t i = 0; i < table_schema.size(); i++)
        {
            TableColumn tab_col = table_schema[i];
            std::string value = value_to_store[i];

            value = "'" + value + "'";

            if (tab_col.datatype == "TIMESTAMP")
            {
                if (tab_col.data_format != "NULL")
                {
                    placeholder.push_back("to_timestamp($" + std::to_string(i + 1) + ", '" + tab_col.data_format + "')");
                }
                else
                {
                    placeholder.push_back("to_timestamp($" + std::to_string(i + 1) + ")");
                }
                formated_values.push_back(value);
            }
            else if (tab_col.isArray)
            {
                placeholder.push_back("ARRAY[$" + std::to_string(i + 1) + "]");
                formated_values.push_back(value); // Assuming 'value' is formatted correctly for an array
            }
            else
            {
                placeholder.push_back("$" + std::to_string(i + 1));
                formated_values.push_back(value);
            }
        }

        std::string placeholder_str = join(placeholder, ", ");
        std::string query = "INSERT INTO " + table_name + " (" + columns + ") VALUES (" + placeholder_str + ") ON CONFLICT DO NOTHING";

        all_queries.push_back({query, formated_values});
    }

    // Execute the query with formatted values
    this->executeMultipleCommands(all_queries);
}

void DatabaseConnectionPool::clear_database(std::string db_name)
{
    if (db_name == "NULL")
    {
        std::unordered_set<std::string> table_names = this->fetch_table_names();
        for (std::string table_name : table_names)
        {
            std::string query = "DROP TABLE " + table_name + " CASCADE";
            executeCommand(query);
        }
    }
    else
    {
        std::string query = "DROP TABLE " + db_name + " CASCADE";
        executeCommand(query);
    }
}

std::map<std::string, int> DatabaseConnectionPool::fetchLookupTableAsDict(const std::string &table_name, const std::vector<TableColumn> &schema)
{
    // Find the string and integer columns in the schema
    std::string string_column;
    std::string int_column;

    for (const auto &column : schema)
    {
        std::string datatype = column.datatype;
        std::transform(datatype.begin(), datatype.end(), datatype.begin(), ::tolower); // Convert datatype to lowercase

        if (datatype == "varchar" || datatype == "text" || datatype == "char")
        {
            string_column = column.name;
        }
        else if (datatype == "int" || datatype == "integer" || datatype == "bigint" || datatype == "smallint")
        {
            int_column = column.name;
        }

        // Break the loop if both columns are identified
        if (!string_column.empty() && !int_column.empty())
        {
            break;
        }
    }

    if (string_column.empty() || int_column.empty())
    {
        throw std::invalid_argument("Schema must contain one string column and one integer column.");
    }

    // SQL query to fetch data from the table
    std::string query = "SELECT " + string_column + ", " + int_column + " FROM " + table_name + ";";

    // Execute the query and fetch results
    auto results = executeCommand(query);

    // Convert the results into a dictionary (unordered_map in C++)
    std::map<std::string, int> lookup_dict;
    for (const auto &row : results)
    {
        if (row.size() < 2)
        {
            throw std::runtime_error("Row contains insufficient data.");
        }
        // Convert the string key and integer value
        std::string key = row[0];
        int value = std::stoi(row[1]); // Convert string to integer
        lookup_dict[key] = value;
    }

    return lookup_dict;
}

std::vector<std::unordered_map<std::string, std::string>> DatabaseConnectionPool::fetchAllTableData(const std::string &tableName)
{
    std::vector<std::string> columnNames;
    std::vector<std::unordered_map<std::string, std::string>> allData;

    try
    {
        // Query to fetch column names
        std::string columnQuery = R"(
            SELECT column_name
            FROM information_schema.columns
            WHERE table_name = ')" +
                                  tableName + R"('
            ORDER BY ordinal_position;
        )";

        std::vector<std::vector<std::string>> columnResult = this->executeCommand(columnQuery);

        // Store column names
        for (const auto &row : columnResult)
        {
            columnNames.push_back(row[0].c_str());
        }

        // Query to fetch all rows
        std::string fetchQuery = "SELECT * FROM " + tableName + ";";
        auto rowResult = executeCommand(fetchQuery);

        // Convert rows to a list of dictionaries
        for (const auto &row : rowResult)
        {
            std::unordered_map<std::string, std::string> rowData;
            for (std::size_t i = 0; i < columnNames.size(); ++i)
            {
                rowData[columnNames[i]] = row[i].c_str() ? row[i].c_str() : "NULL";
            }
            allData.push_back(rowData);
        }
    }
    catch (const std::exception &e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    return allData;
}

void DatabaseConnectionPool::print_table_content(std::string table_name, int first_x_line)
{
    std::vector<std::unordered_map<std::string, std::string>> table_data = fetchAllTableData(table_name);
    for (auto row : table_data)
    {
        if (first_x_line == 0)
            break;
        for (auto column : row)
        {
            std::cout << column.first << ":" << column.second << ", ";
        }
        std::cout << std::endl;

        first_x_line--;
    }
}

// void DatabaseConnectionPool::print_total_table_no()
// {
//     std::cout << "Total Number of tables are: " << this->table_name_map.size() << std::endl;
// }