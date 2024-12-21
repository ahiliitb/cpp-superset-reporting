#include<bits/stdc++.h>

class LogColumn
{
public:
    std::string name, datatype, data_format;
    bool isArray, isPrimary, isLookupColumn;

    LogColumn(std::string name, std::string datatype, std::string data_format = NULL, bool isArray = false, bool isPrimary = false, bool isLookupColumn = false)
    {
        this->name = name;
        this->datatype = datatype;
        this->data_format = data_format;
        this->isArray = isArray;
        this->isPrimary = isPrimary;
        this->isLookupColumn = isLookupColumn;
    }
    ~LogColumn() {}
};