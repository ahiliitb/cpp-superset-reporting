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

    std::string string_repr()
    {
        std::string array_repr = "";
        if(this->isArray)array_repr = "[]";

        std::string primarykey_repr = "";
        if(this->isPrimary)primarykey_repr = " PRIMARY KEY";
        std::string repr = this->name + " " + this->datatype + array_repr + primarykey_repr;

        return repr;
    }

};

#endif // TABLE_COLUMN_H