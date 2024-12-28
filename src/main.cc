#include "log_parser.h"

int main(int argc, char* argv[])
{

    if(argc == 2 && std::string(argv[1]) == "clear-database")
    {
        // ./log_parser clear-database

        DatabaseConnectionPool dcp("admin", "safesquid", "127.0.0.1", "5432", "safesquid_logs", "postgresql", 1); 

        dcp.clear_database();
    }
    else if(argc == 3 && std::string(argv[1]) == "create-database" && std::string(argv[2]) == "extended")
    {
        // ./log_parser create-database extended

        DatabaseConnectionPool dcp("admin", "safesquid", "127.0.0.1", "5432", "safesquid_logs", "postgresql", 1); 

        LogParser basic(dcp, (std::string(argv[2]) + "_logs"));
        basic.initialise_schemas(std::string(argv[2]), "log_structure.xml");

        basic.create_table();
    }
    else if(argc == 2 && std::string(argv[1]) == "extended")
    {
        // ./log_parser extended

        DatabaseConnectionPool dcp("admin", "safesquid", "127.0.0.1", "5432", "safesquid_logs", "postgresql", 1); 
        LogParser basic(dcp, (std::string(std::string(argv[1])) + "_logs"));
        basic.initialise_schemas(std::string(argv[1]), "log_structure.xml");

        long long initial_size = basic.database_size();
        basic.insert_all_log_files({"20241202175557-extended.log"});
        long long final_size = basic.database_size();

        std::cout<<"Database Size is: "<<(final_size-initial_size)/1024<< " KB" <<std::endl;
    }
    else if(argc == 4 && std::string(argv[1]) == "analyse-table")
    {
        // ./log_parser analyse-table table_name first_n_lines

        DatabaseConnectionPool dcp("admin", "safesquid", "127.0.0.1", "5432", "safesquid_logs", "postgresql", 1); 
        dcp.print_table_content(std::string(argv[2]), std::stoi(std::string(argv[3]))); // table_name and first x line
    }
    else if(argc == 2 && std::string(argv[1]) == "analyse-database")
    {
        // ./log_parser analyse-database

        DatabaseConnectionPool dcp("admin", "safesquid", "127.0.0.1", "5432", "safesquid_logs", "postgresql", 1); 
        dcp.print_all_table_schema(); 
    }
    else
    {
        std::cerr << "ERROR! Insufficient Arguments" << std::endl;
        return 1;
    }

    return 0;
}