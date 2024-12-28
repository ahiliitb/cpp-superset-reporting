#ifndef LOG_PARSER_WITHOUT_LOOKUP_H
#define LOG_PARSER_WITHOUT_LOOKUP_H

#include <bits/stdc++.h>
#include "base_log_parser.h"


class LogParserWithoutLookup: public LogParser
{
    public:
        std::string main_table_name;
        std::vector<TableColumn> main_table_schema;

        LogParserWithoutLookup(DatabaseConnectionPool &database, std::string main_table, std::string file_type = "tsv");

        void create_table();

        void set_main_table_schema();

        // Declared in derived log parser
        void insert_log(const std::string &line);

        void initialise_schemas(std::string log_type, std::string xml_file_name);

        void insert_all_log_files(std::vector<std::string> log_lists);
 
        // void print_table_schema(std::string table_name);

        // void print_all_table_schema();

        // void print_table_content(std::string table_name, int first_n_line);

};

#endif // LOG_PARSER_WITHOUT_LOOKUP_H