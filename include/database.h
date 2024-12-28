#ifndef DATABASE_H
#define DATABASE_H

#include "table_column.h"
#include "utils.h"
#include <pqxx/pqxx>

class DatabaseConnectionPool
{
    private:
        size_t maxConnections;
        std::vector<pqxx::connection*> connectionPool; // Raw pointers for connections
        std::string connectionString;
        
    public:

        DatabaseConnectionPool(const std::string &username, const std::string &password,
        const std::string &host, const std::string &port, const std::string &dbname, const std::string &dbtype, size_t maxConns = 10);

        ~DatabaseConnectionPool();

        pqxx::connection *getConnection();

        void releaseConnection(pqxx::connection *conn);

        void closeConnections();

        std::vector<std::vector<std::string>> executeCommand(const std::string &sqlCommand, const std::vector<std::string> &values = {});

        std::vector<std::vector<std::string>> executeMultipleCommands(const std::vector<std::pair<std::string, std::vector<std::string>>> &queries);

        void create_table(std::string table_name, std::vector<TableColumn> table_schema);

        std::map<std::string, std::vector<std::pair<std::string, std::string>>> fetch_all_table_schema();

        void print_table_schema(std::string table_name);

        void print_all_table_schema();

        std::unordered_set<std::string> fetch_table_names();

        void insert_data(std::string table_name, std::vector<TableColumn> table_schema, std::vector<std::string> value_to_store);

        void insert_multiple_data(std::string table_name, std::vector<TableColumn> table_schema, std::vector<std::vector<std::string>> values_to_store);

        void clear_database(std::string db_name = "NULL");

        std::map<std::string, int> fetchLookupTableAsDict(const std::string &table_name, const std::vector<TableColumn> &schema);

        std::vector<std::unordered_map<std::string, std::string>> fetchAllTableData(const std::string& tableName);
        
        void print_table_content(std::string table_name, int first_x_line);

        long long get_database_size();

        // void print_total_table_no();
};


#endif // DATABASE_H