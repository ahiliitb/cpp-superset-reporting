#ifndef ARGUMENT_H
#define ARGUMENT_H

#include "appcontext.h"

namespace ARGUMENT
{
    inline void clear_database(AppContext &appcontext, int argc, char* argv[])
    {
        appcontext.database->clear_database();
    }

    inline void create_database(AppContext &appcontext, int argc, char* argv[])
    {
        appcontext.load_parser(std::string(argv[2]));
        appcontext.logparser->create_table();
    }

    inline void analyse_table(AppContext &appcontext, int argc, char* argv[])
    {
        appcontext.database->print_table_content(std::string(argv[2]), std::stoi(std::string(argv[3]))); // table_name and first x line
    }

    inline void analyse_database(AppContext &appcontext, int argc, char* argv[])
    {
        appcontext.database->print_all_table_schema();
    }

    inline void insert(AppContext &appcontext, int argc, char* argv[])
    {
        appcontext.load_parser(std::string(argv[2]));

        std::vector<std::string> log_files;
        for (size_t i = 3; i < argc; i++)
        {

            if (std::filesystem::is_directory(argv[i]))
            {
                for (const auto &entry : std::filesystem::directory_iterator(argv[i]))
                {
                    if (UTILS::check_format(entry.path().filename().string()))
                    {
                        log_files.push_back(entry.path().string());
                    }
                }
            }
            else if (std::filesystem::is_regular_file(argv[i]) && UTILS::check_format(argv[i]))
            {
                log_files.push_back(argv[i]);
            }
            else
            {
                std::cerr << argv[i] << ": Invalid log file format. Please provide a valid log file." << std::endl;
            }
        }

        long long initial_size = appcontext.logparser->database_size();
        appcontext.logparser->insert_all_log_files(log_files);
        long long final_size = appcontext.logparser->database_size();

        std::cout << "Database Size is: " << (final_size - initial_size) / 1024 << " KB" << std::endl;
    }

}

#endif // ARGUMENT_H