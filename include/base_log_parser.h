#ifndef BASE_LOG_PARSER_H
#define BASE_LOG_PARSER_H

#include "database.h"
#include "../thirdparty/pugixml-1.14/src/pugixml.hpp"
#include <zlib.h> // For handling gzipped files
#include "utils.h"
#include "log_column.h"
#include "config.h"

class BaseLogParser
{

    public:
        DatabaseConnectionPool &database;
        std::vector<LogColumn> log_schema;
        std::string file_type;
        
        BaseLogParser(DatabaseConnectionPool &database, std::string file_type = "tsv");

        void load_log_schema(std::string log_type, std::string xml_file_name);

        virtual void insert_log(const std::vector<std::string> &line) = 0;

        virtual void create_table() = 0;

        void insert_log_file(const std::string &file_path);

        void insert_log_files(const std::vector<std::string> log_file_name_list);

        std::vector<std::string> parse_log_line(const std::string &log_line);

        int get_log_schema_length();
};

#endif // BASE_LOG_PARSER_H