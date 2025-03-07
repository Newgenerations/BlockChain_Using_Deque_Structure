#ifndef DATABASE_H
#define DATABASE_H

#include <sqlite3.h>
#include <string>

class Database
{
public:
    Database(const std::string &db_name);
    ~Database();
    bool initialize();
    bool execute(const std::string &query);
    void createTables();

private:
    sqlite3 *db;
    std::string db_name;
    bool open();
    void close();
};

#endif // DATABASE_H
