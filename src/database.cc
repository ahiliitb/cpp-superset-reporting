#include "database.h"


void DatabaseConnectionPool::create_table(std::string table_name, std::vector<TableColumn> table_schema)
{
    this->table_name_map[table_name] = {};
    this->table_schema_map[table_name];
    for(TableColumn table_col: table_schema)
    {
        this->table_schema_map[table_name].emplace_back(table_col);
    }

}

std::vector<TableColumn> DatabaseConnectionPool::fetch_table_schema(std::string table_name)
{
    return table_schema_map[table_name];
}

std::unordered_set<std::string> DatabaseConnectionPool::fetch_table_names()
{
    std::unordered_set<std::string> table_names;
    for(auto key_value: table_name_map)
    {
        table_names.insert(key_value.first);
    }
    return table_names;
}

std::map<std::string, int> DatabaseConnectionPool::fetch_lookup_table_as_dict(std::string single_log_col_name)
{
    std::map<std::string, int> single_lookup_table;
    for(auto iter: this->table_name_map[single_log_col_name])
    {
        std::string key = iter[0];
        int value = std::stoi(iter[1]);
        single_lookup_table[key] = value;
    }
    return single_lookup_table;
}

void DatabaseConnectionPool::insert_data(std::string table_name, std::vector<std::string> value_to_store)
{
    this->table_name_map[table_name].emplace_back(value_to_store);
}

void DatabaseConnectionPool::print_table_content(std::string table_name, int first_x_line)
{
    for(auto row: this->table_name_map[table_name])
    {
        if(first_x_line == 0)break;
        for(auto column: row)
        {
            std::cout<<column<<" ";
        }
        std::cout<<std::endl;

        first_x_line--;
    }
}

void DatabaseConnectionPool::print_total_table_no()
{
    std::cout<<"Total Number of tables are: "<<this->table_name_map.size()<<std::endl;
}