#ifndef DATABASE_H
#define DATABASE_H

#include <bits/stdc++.h>

class TableColumn
{
public:
    std::string name, datatype, data_format;
    bool isArray, isPrimary;

    TableColumn(std::string name, std::string datatype, std::string data_format = "NULL", bool isArray = false, bool isPrimary = false)
    {
        this->name = name;
        this->datatype = datatype;
        this->data_format = data_format;
        this->isArray = isArray;
        this->isPrimary = isPrimary;
    }

};


class DatabaseConnectionPool
{
    public:
        std::map<std::string, std::vector<std::vector<std::string>>> table_name_map;
        std::map<std::string, std::vector<TableColumn>> table_schema_map;

        void create_table(std::string table_name, std::vector<TableColumn> table_schema);

        std::vector<TableColumn> fetch_table_schema(std::string table_name);

        std::unordered_set<std::string> fetch_table_names();

        std::map<std::string, int> fetch_lookup_table_as_dict(std::string single_log_col_name);

        void insert_data(std::string table_name, std::vector<std::string> value_to_store);

        void print_table_content(std::string table_name, int first_x_line);

        void print_total_table_no();
};


#endif // DATABASE_H