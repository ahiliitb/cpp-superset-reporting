#include "log_parser.h"


LogParser::LogParser(DatabaseConnectionPool &database, const std::string main_table, const bool single_lookup_table, const bool lookup, const std::string file_type): BaseLogParser(database, file_type)
{
    this->main_table_name = main_table;
    this->single_lookup_table = single_lookup_table;
    this->lookup = lookup;
}

void LogParser::create_table()
{
    this->database.create_table(this->main_table_name, this->main_table_schema);
}

void LogParser::set_main_table_schema()
{
    for (LogColumn log_col: this->log_schema)
    {
        std::string datatype = log_col.datatype;
        bool isArray = log_col.isArray;

        if(this->columnindex.count(log_col.name) != 0)
        {
            if(log_col.isArray) isArray = false;
            else datatype = "INTEGER";            
        }
        this->main_table_schema.emplace_back(TableColumn(log_col.name, datatype, log_col.data_format, isArray, log_col.isPrimary));
    }
}

void LogParser::set_lookup_table_schema()
{
    this->lookuptable_schema.push_back(TableColumn("key", "TEXT"));
    this->lookuptable_schema.push_back(TableColumn("value", "INTEGER"));
}

// Declared in derived log parser
void LogParser::insert_log(const std::string &line)
{
    std::vector<std::string> line_field = this->parse_log_line(line);
    if(line_field.size() != this->main_table_schema.size())return;
    line_field = this->update_log_line(line_field);
    this->database.insert_data(this->main_table_name, this->main_table_schema, line_field);
}

// // Declared in derived log parser
// void LogParser::insert_log(const std::vector<std::string> &lines)
// {
//     std::vector<std::vector<std::string>> updated_logs;
//     for(std::string line: lines)
//     {
//         std::vector<std::string> line_field = this->parse_log_line(line);
//         if(line_field.size() != this->main_table_schema.size())continue;
//         line_field = this->update_log_line(line_field);
//         updated_logs.emplace_back(line_field);
//     }
//     this->database.insert_multiple_data(this->main_table_name, this->main_table_schema, updated_logs);
// }

void LogParser::initialise_lookup()
{
    this->set_lookup_table_schema();
    this->initialise_logcolumn_index_and_logcolumn();
    this->initialise_lookuptables();
}

void LogParser::initialise_schemas(std::string log_type, std::string xml_file_name)
{
    this->load_log_schema(log_type, xml_file_name);
    if(this->lookup) this->initialise_lookup();
    this->set_main_table_schema();
}

int LogParser::get_logcolumn_index_and_col(std::string column_name)
{
    for(int i = 0; i < this->log_schema.size(); i++)
    {
        LogColumn log_col = this->log_schema[i];
        if(log_col.name == column_name) return i;
    }
    return -1;
}

void LogParser::initialise_logcolumn_index_and_logcolumn()
{
    for(LogColumn log_col: this->log_schema)
    {
        if (log_col.isLookupColumn)
        {
            int index = this->get_logcolumn_index_and_col(log_col.name);
            if(log_col.isArray) this->columnindex[log_col.name] = -(index+1);
            else this->columnindex[log_col.name] = (index+1);
            this->lookup_columns.emplace_back(log_col);
        }
    }
}

std::map<std::string, int> LogParser::initialise_single_lookuptable(std::string log_col_name)
{
    std::unordered_set<std::string> all_tables_name = this->database.fetch_table_names();
    std::map<std::string, int> single_lookup_dict;
    if(all_tables_name.count(log_col_name) == 0)
    {
        this->database.create_table(log_col_name, this->lookuptable_schema);
    }
    else
    {
        single_lookup_dict = this->database.fetchLookupTableAsDict(log_col_name, this->lookuptable_schema);
    }
    return single_lookup_dict;
}

void LogParser::initialise_lookuptables()
{
    for(LogColumn log_col: this->lookup_columns)
    {
        std::string lookuptable_name;
        if(this->single_lookup_table) lookuptable_name = "lookuptable";
        else lookuptable_name = log_col.name;
        this->lookuptables[lookuptable_name] = initialise_single_lookuptable(lookuptable_name);
    }
}

int LogParser::update_string_column(std::string log_one_col_val, std::string lookuptable_name)
{

    std::map<std::string, int> lookup_dict = this->lookuptables[lookuptable_name];
    if (lookup_dict.count(log_one_col_val) == 0)
    {
        std::string key = log_one_col_val;
        size_t value = lookup_dict.size();
        this->lookuptables[lookuptable_name][key] = value;
        return value;
    }
    else
    {
        return lookup_dict[lookuptable_name];
    }
    return -1;
}

std::string LogParser::update_array_column(std::string log_one_col_val, std::string lookuptable_name)
{
    if(log_one_col_val.size() == 0 || log_one_col_val[0] != '{')
    {
        std::string value = "{" + std::to_string(this->update_string_column(log_one_col_val, lookuptable_name)) + "}";
        
        return value;
    }
    std::string comma_sep = log_one_col_val.substr(1, log_one_col_val.size()-2);   
    std::vector<std::string> split_to_vector = split(comma_sep, ',');

    std::vector<std::string> lookuptable_value;
    for(std::string arrayvalue: split_to_vector)
    {
        lookuptable_value.push_back(std::to_string(this->update_string_column(arrayvalue, lookuptable_name)));
    }
    std::string value = join(lookuptable_value, ",");

    return "{" + value + "}";
}

std::vector<std::string> LogParser::update_log_line(std::vector<std::string> log_line)
{
    int i = 0;
    for(auto iter: this->columnindex)
    {
        std::string log_col_name = iter.first;
        int col_index = iter.second;

        std::string lookuptable_name;
        if(this->single_lookup_table) lookuptable_name = "lookuptable";
        else lookuptable_name = log_col_name;

        if (log_line.size() == this->log_schema.size())
        {
            if(col_index > 0)
            {
                // just for testing i am pushing it as string, idealy it should be int
                log_line[abs(col_index)-1] = std::to_string(this->update_string_column(log_line[abs(col_index)-1], lookuptable_name));
            }
            else if(col_index < 0)
            {
                log_line[abs(col_index)-1] = this->update_array_column(log_line[abs(col_index)-1], lookuptable_name);
            }
        }
        else
        {
            return log_line;
        }
        
    }
    return log_line;
}

void LogParser::insert_all_log_files(std::vector<std::string> log_lists)
{
    this->insert_log_files(log_lists);
    for(auto lookuptable: this->lookuptables)
    {
        for(auto column: lookuptable.second)
        {
            this->database.insert_data(lookuptable.first, this->lookuptable_schema, {column.first, std::to_string(column.second)});
        }
    }
}

void LogParser::print_table_schema(std::string table_name)
{
    std::map<std::string, std::vector<std::pair<std::string, std::string>>> schemas = this->database.fetch_all_table_schema();
    std::cout<<"Table Name is "<<table_name<<std::endl;
    std::cout<<"Table Column Size is "<<schemas[table_name].size()<<std::endl;
    std::cout<<"Columns are: "<<std::endl;
    for(auto table_col: schemas[table_name])
    {
        std::cout<<"==> Column Name: "<<table_col.first<<" | Datatype: "<<table_col.second<<std::endl;
    }
}

void LogParser::print_all_table_schema()
{
    std::unordered_set<std::string> table_names = this->database.fetch_table_names();
    for(std::string table_name: table_names)
    {
        print_table_schema(table_name);
        std::cout<<"------------------------------"<<std::endl;
    }
}

void LogParser::print_table_content(std::string table_name, int first_n_line)
{
    this->database.print_table_content(table_name, first_n_line);
}

