#include "sql_table.h"

enum class tasks_flags
{
  is_open = 1,
  is_hierarcy_end = 2,
  is_actual = 4
};

enum class subjects_flags
{
  is_internal = 1
};

Table TableStructs[static_cast<int>(TableName::TableNames)]
{
    {
        "tasks",

        "("
        "id                     INTEGER PRIMARY KEY AUTOINCREMENT,"
        "name                   VARCHAR(255),"
        "creation_date          INTEGER,"
        "start_date             INTEGER,"
        "flags                  INTEGER,"
        "level                  INTEGER,"
        "parent_task_id         INTEGER,"
        "dependent_task_id      INTEGER,"
        "source_subject_id      INTEGER,"
        "blocking_subject_id    INTEGER,"
        "blocking_start_date    INTEGER,"
        "blocking_end_date      INTEGER,"
        "description            TEXT,"
        "descr_files            BLOB,"
        "result                 TEXT,"
        "rslt_files             BLOB,"
        "finish_date            INT"
        ")"
    },
    {
        "subjects",

        "("
        "id                 INTEGER PRIMARY KEY AUTOINCREMENT,"
        "name               VARCHAR(255),"
        "parent_id          INTEGER,"
        "flags              INTEGER"
        ")"
    },
    {
        "records",

        "("
        "id                 INTEGER PRIMARY KEY AUTOINCREMENT,"
        "task_id            INTEGER,"
        "description        TEXT,"
        "date               INTEGER,"
        "hours              INTEGER"
        ")"
    },
};

sql_w::sql_w(std::string filename)
{
    if(sqlite3_open(filename.c_str(), &db_))
        throw std::system_error();
}

sql_w::~sql_w()
{
    sqlite3_close(db_);
}


table::table(sql_w& wrp, TableName name)
        : db_(wrp.db_)
        , name_(name)
{
    char *errm;

    if(sqlite3_table_column_metadata(db_, NULL, TableStructs[static_cast<int>(name_)].name, NULL, NULL, NULL, NULL, NULL, NULL) != SQLITE_OK)
    {
        std::cout << "Info: creating not existent table: " << TableStructs[static_cast<int>(name_)].name << "\n";
        std::string create_query =
            std::string("CREATE TABLE ") +
            TableStructs[static_cast<int>(name_)].name +
            TableStructs[static_cast<int>(name_)].descr;
        if(sqlite3_exec(db_, create_query.c_str(), NULL, NULL, &errm) != SQLITE_OK)
            throw std::runtime_error(std::string("Table creation failed: ") + errm + "\nSQL:\n" + create_query);
    }
    else
        std::cout << "Info: table exists: " << TableStructs[static_cast<int>(name_)].name << "\n";
}


bool table::insert(const char *fields, std::initializer_list<StrAdapter> il)
{
    std::string values;
    char *errm;

    for(auto &sa : il)
        values += "\"" + sa.get_str() + "\",";

    values.erase(values.size() - 1, 1);

    std::string query =
        std::string("INSERT INTO ") +
        TableStructs[static_cast<int>(name_)].name +
        " (" + fields + ") VALUES (" + values + ")";

    if(sqlite3_exec(db_, query.c_str(), nullptr, nullptr, &errm) != SQLITE_OK)
    {
        std::cout << "Error: table row insertion failed: " << errm << "\nSQL:\n" << query << "\n";
        return false;
    }

    return true;
}

bool table::update(int id, std::initializer_list<std::pair<std::string, StrAdapter>> il)
{
    std::string sets;
    char *errm;

    for(auto &sa : il)
        sets += sa.first + "=\""  + sa.second.get_str() + "\",";

    sets.erase(sets.size() -1, 1);

    std::string query =
        std::string("UPDATE ") +
        TableStructs[static_cast<int>(name_)].name +
        " SET " + sets + " WHERE id=\"" + std::to_string(id) + "\"";

    if(sqlite3_exec(db_, query.c_str(), nullptr, nullptr, &errm) != SQLITE_OK)
    {
        std::cout << "Error: table row update failed: " << errm << "\nSQL:\n" << query << "\n";
        return false;
    }

    return true;
}

bool table::select(const char *fields, const char *settings, std::vector<std::vector<std::string>> &results)
{
    char *errm;


    std::string query =
        std::string("SELECT ") + fields + " FROM " +
        TableStructs[static_cast<int>(name_)].name +
        " " + settings;

    auto cb = [](void *data, int count, char **values, char **) ->int
    {
        std::vector<std::string> &&row{};

        for(int i = 0; i < count; i ++)
            row.push_back(values[i]);
        static_cast<std::vector<std::vector<std::string>>*>(data)->emplace_back(std::move(row));

        return 0;
    };

    if(sqlite3_exec(db_, query.c_str(), cb, static_cast<void *>(&results), &errm) != SQLITE_OK)
    {
        std::cout << "Error: table select failed: " << errm << "\nSQL:\n" << query << "\n";
        return false;
    }

    return true;
}
