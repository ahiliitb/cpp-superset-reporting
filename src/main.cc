#include "arg.h"


int main(int argc, char *argv[])
{
    auto appcontext = AppContext();
    appcontext.load_database();

    if (argc == 2 && std::string(argv[1]) == "clear-database")
    {
        // ./log_parser clear-database

        ARGUMENT::clear_database(appcontext, argc, argv);
    }
    else if (argc == 3 && std::string(argv[1]) == "create-database" && std::string(argv[2]) == "extended")
    {
        // ./log_parser create-database extended

        ARGUMENT::create_database(appcontext, argc, argv);
    }
    else if (argc > 3 && std::string(argv[1]) == "insert" && std::string(argv[2]) == "extended")
    {
        // ./log_parser insert extended

        ARGUMENT::insert(appcontext, argc, argv);
    }
    else if (argc == 4 && std::string(argv[1]) == "analyse-table")
    {
        // ./log_parser analyse-table table_name first_n_lines

        ARGUMENT::analyse_table(appcontext, argc, argv);
    }
    else if (argc == 2 && std::string(argv[1]) == "analyse-database")
    {
        // ./log_parser analyse-database

        ARGUMENT::analyse_database(appcontext, argc, argv);
    }
    else
    {
        std::cerr << "ERROR! Insufficient Arguments" << std::endl;
        return 1;
    }

    return 0;
}