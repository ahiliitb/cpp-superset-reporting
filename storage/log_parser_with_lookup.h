#ifndef LOG_PARSER_WITH_LOOKUP_H
#define LOG_PARSER_WITH_LOOKUP_H

#include <bits/stdc++.h>
#include "base_log_parser.h"


class LogParserWithLookup: public LogParser
{
    public:
        std::string main_table_name;
        std::vector<TableColumn> main_table_schema;
        std::vector<TableColumn> lookuptable_schema;
        std::vector<LogColumn> lookup_columns;
        std::map<std::string, std::map<std::string, int>> lookuptables;
        std::map<std::string, int> columnindex;
        bool single_lookup_table;

        LogParserWithLookup(DatabaseConnectionPool &database, const std::string main_table, const bool single_lookup_table = false, const std::string file_type = "tsv");

        void create_table();

        void set_main_table_schema();

        void set_lookup_table_schema();

        // Declared in derived log parser
        void insert_log(const std::string &line);

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
 
        void print_table_schema(std::string table_name);

        void print_all_table_schema();

        // void print_table_content(std::string table_name, int first_n_line);

};

#endif // LOG_PARSER_WITH_LOOKUP_H