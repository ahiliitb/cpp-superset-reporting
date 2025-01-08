#ifndef CONFIG_H
#define CONFIG_H

#include<bits/stdc++.h>

namespace CONFIG
{
    inline std::string PROJECT_PATH = "/home/ahilkhanjnv/MYWORK/safesquid/cpp-superset-reporting/";
    inline size_t NUM_QUERIES = 500;
    inline bool LOOKUP = true;
    inline bool SINGLE_LOOKUP_TABLE = false;
    inline std::string LOG_STRUCTURE = PROJECT_PATH + "log_structure.xml";
    inline std::string CONFIG_FILE_PATH = PROJECT_PATH + "config.json";

}

#endif // CONFIG_H