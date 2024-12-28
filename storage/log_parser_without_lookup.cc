#include "log_parser_without_lookup.h"


LogParserWithoutLookup::LogParserWithoutLookup(DatabaseConnectionPool &database, std::string main_table, std::string file_type): LogParser(database, file_type)
{
    this->main_table_name = main_table;
}

void LogParserWithoutLookup::create_table()
{
    this->database.create_table(this->main_table_name, this->main_table_schema);
}

void LogParserWithoutLookup::set_main_table_schema()
{
    for (LogColumn log_col: this->log_schema)
    {
        this->main_table_schema.emplace_back(TableColumn(log_col.name, log_col.datatype, log_col.data_format, log_col.isArray, log_col.isPrimary));
    }
}


// Declared in derived log parser
void LogParserWithoutLookup::insert_log(const std::string &line)
{
    std::vector<std::string> line_field = this->parse_log_line(line);
    if(line_field.size() != this->main_table_schema.size())return;
    this->database.insert_data(this->main_table_name, this->main_table_schema, line_field);
}

void LogParserWithoutLookup::initialise_schemas(std::string log_type, std::string xml_file_name)
{
    this->load_log_schema(log_type, xml_file_name);
    this->set_main_table_schema();
}

void LogParserWithoutLookup::insert_all_log_files(std::vector<std::string> log_lists)
{
    this->insert_log_files(log_lists);
}

// void LogParserWithoutLookup::print_table_schema(std::string table_name)
// {
//     std::cout<<"Table Name is "<<table_name<<std::endl;
//     std::cout<<"Table Column Size is "<<this->database.table_schema_map[table_name].size()<<std::endl;
//     std::cout<<"Table Row Size is "<<this->database.table_name_map[table_name].size()<<std::endl;
//     std::cout<<"Columns are: "<<std::endl;
//     for(TableColumn table_col: this->database.table_schema_map[table_name])
//     {
//         std::cout<<"==> Column Name: "<<table_col.name<<" | Datatype: "<<table_col.datatype<<" | isArray: "<<table_col.isArray<<std::endl;
//     }
// }

// void LogParserWithoutLookup::print_all_table_schema()
// {
//     std::unordered_set<std::string> table_names = this->database.fetch_table_names();
//     for(std::string table_name: table_names)
//     {
//         print_table_schema(table_name);
//         std::cout<<"------------------------------"<<std::endl;
//     }
// }

// void LogParserWithoutLookup::print_table_content(std::string table_name, int first_n_line)
// {
//     this->database.print_table_content(table_name, first_n_line);
// }

