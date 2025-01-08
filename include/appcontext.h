#ifndef APPCONTEXT_H
#define APPCONTEXT_H

#include "log_parser.h"

class AppContext
{
public:
    DatabaseConnectionPool *database;
    LogParser *logparser;

    void initilise_context(std::string log_type)
    {
        this->load_database();
        this->load_parser(log_type);
    }

    void load_database()
    {
        std::unordered_map<std::string, std::string> credentials = UTILS::jsonToDict(CONFIG::CONFIG_FILE_PATH);

        // "postgresql" or "timescaledb" ==> database options
        this->database = new DatabaseConnectionPool(credentials["username"], credentials["password"], credentials["host"], credentials["port"], credentials["dbname"], "timescaledb", stoi(credentials["maxconns"]));
    }

    void load_parser(std::string log_type)
    {
        this->logparser = new LogParser(*database, log_type + "_logs");
        logparser->initialise_schemas(log_type, CONFIG::LOG_STRUCTURE);
    }
    ~AppContext()
    {
        delete database;
        delete logparser;
    }
};

#endif // APPCONTEXT_H