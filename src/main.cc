#include "log_parser.h"

int main()
{
    DatabaseConnectionPool dcp("admin", "safesquid", "127.0.0.1", "5432", "safesquid_logs", "postgresql", 1); 

    dcp.clear_database();
    
    // std::map<std::string, std::vector<std::pair<std::string, std::string>>> schema = dcp.fetch_all_table_schema();
    // std::cout<<schema["extended_logs"].size();

    // std::unordered_set<std::string> table_names = dcp.fetch_table_names();
    // std::cout<<table_names.size()<<std::endl;

    LogParser basic(dcp, "extended_logs");
    basic.initialise_schemas("extended", "log_structure.xml");
    // std::cout << basic.get_log_schema_length() << std::endl;
    basic.create_table();


    long long initial_size = basic.database_size();

    // // basic.print_table_schema();
    basic.insert_all_log_files({"20241202175557-extended.log"});

    long long final_size = basic.database_size();

    std::cout<<"Database Size is: "<<(final_size-initial_size)/1024<< " KB" <<std::endl;
    // basic.print_all_table_schema();
    // basic.print_table_content("extended_logs", 4);
    // basic.print_table_content("request_profiles", 5);
    // basic.print_table_content("filter_name", 3);
    // // basic.print_table_content("download_content_types", 3);

    return 0;
}