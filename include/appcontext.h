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
        // "postgresql" or "timescaledb" ==> database options
        database = new DatabaseConnectionPool("admin", "safesquid", "127.0.0.1", "5432", "safesquid_logs", "timescaledb", 1);
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