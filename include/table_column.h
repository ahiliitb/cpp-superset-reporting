#ifndef TABLE_COLUMN_H
#define TABLE_COLUMN_H

#include <bits/stdc++.h>

class TableColumn
{
public:
    std::string name, datatype, data_format;
    bool isArray, isPrimary;

    TableColumn(std::string name, std::string datatype, std::string data_format = "NULL", bool isArray = false, bool isPrimary = false)
    {
        this->name = name;
        this->datatype = datatype;
        this->data_format = data_format;
        this->isArray = isArray;
        this->isPrimary = isPrimary;
    }

};

#endif // TABLE_COLUMN_H