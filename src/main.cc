#include "log_parser_with_lookup.h"

int main()
{
    DatabaseConnectionPool dcp;
    LogParserWithLookup basic = LogParserWithLookup(dcp, "extended_logs");
    basic.initialise_schemas("extended", "log_structure.xml");
    // std::cout << basic.get_log_schema_length() << std::endl;
    basic.create_table();
    // basic.print_table_schema();
    basic.insert_all_and_update_lookup({"20241202175557-extended.log"});
    basic.print_all_table_schema();
    // basic.print_table_content("download_content_types", 3);

    return 0;
}