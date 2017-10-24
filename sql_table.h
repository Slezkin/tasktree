#ifndef SQL_TABLE_H
#define SQL_TABLE_H

#include <iostream>
#include <initializer_list>
#include <vector>
#include <functional>
#include "sqlite3.h"

enum class TableName
{
    Tasks = 0,
    Subjects,
    Days,
    TableNames
};

struct Table
{
  const char name[32];
  const char descr[1024];
};


class table;

class sql_w
{
    friend class table;
    sqlite3 *db_;

public:
    sql_w(std::string filename);
    ~sql_w();
};

class StrAdapter
{
    std::string _str;
public:
    StrAdapter(int i)
        : _str(std::to_string(i))
    {}

    StrAdapter(const char *s)
        : _str(s)
    {}

    const std::string& get_str() const
    {
        return _str;
    }
};

class table
{
    sqlite3 * db_;
    TableName name_;

    void check_create(TableName name);
public:
    table(sql_w& wrp, TableName name);

    bool insert(const char *fields, std::initializer_list<StrAdapter> il);
    bool select(const char *fields, const char *settings, std::vector<std::vector<std::string>> &results);
    bool update(int id, std::initializer_list<std::pair<std::string, StrAdapter>> il);
};

#endif // SQL_TABLE_H
