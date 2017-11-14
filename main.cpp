#include "desk.h"
#include <QApplication>
#include <iostream>
#include <fcntl.h>

#include "sql_table.h"

int main(int argc, char *argv[])
{
    sql_w sql("base.sql");
    table tasks(sql, TableName::Tasks);
    table subjects(sql, TableName::Subjects);
    table records(sql, TableName::Records);

    tasks.insert("NAME, parent_task_id", {"Abeevewfrewfrewfrc", 143223});

    std::vector<std::vector<std::string>> res;
    tasks.select("id, NAME, parent_task_id", "", res);


    for(auto &row : res)
    {
        for(auto &field : row)
            std::cout << field << ", ";
        std::cout << "\n";
    }

    tasks.update(4, {{"Name", "AA"},{"parent_task_id", "54"}});

    return 0;

    QApplication a(argc, argv);
    desk w;
    w.show();
    return a.exec();
}
