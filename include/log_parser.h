#ifndef LOG_PARSER_H
#define LOG_PARSER_H

#include <bits/stdc++.h>
#include "base_log_parser.h"


class LogParser: public BaseLogParser
{
    private:

        std::string main_table_name;
        std::vector<TableColumn> main_table_schema;
        std::vector<TableColumn> lookuptable_schema;
        std::vector<LogColumn> lookup_columns;
        std::map<std::string, std::map<std::string, int>> lookuptables;
        std::map<std::string, int> columnindex;

    public:

        LogParser(DatabaseConnectionPool &database, const std::string main_table, const std::string file_type = "tsv");

        void create_table();

        void set_main_table_schema();

        void set_lookup_table_schema();

        void insert_log(const std::vector<std::string> &lines);

        void initialise_lookup();

        void initialise_schemas(std::string log_type, std::string xml_file_name);

        int get_logcolumn_index_and_col(std::string column_name);

        void initialise_logcolumn_index_and_logcolumn();

        std::map<std::string, int> initialise_single_lookuptable(std::string log_col_name);

        void initialise_lookuptables();

        int update_string_column(std::string log_one_col_val, std::string lookuptable_name);

        std::string update_array_column(std::string log_one_col_val, std::string lookuptable_name);

        std::vector<std::string> update_log_line(std::vector<std::string> log_line);

        void insert_all_log_files(std::vector<std::string> log_lists);

        long long database_size();

};

#endif // LOG_PARSER_H