#ifndef DB_MANAGER_H
#define DB_MANAGER_H

#include "record.h"
#include "cmd.h"
#include "database.h"

class Manager {
    database db;
    CMD cmd;
    char KEY = 10;
    std::string path;

public:
    Manager(const char &key = 10, const std::string &path = "");
    bool readString(std::string &str, const std::string &print);
    unsigned readUint(const std::string &name, const unsigned &lessThan = database::MAX);
    bool readFloat(float &val, const std::string &print);
    void setKey();
    void manageDatabase();
    void readDatabase();
    void saveDatabase();
    void editOrAdd(const bool &edit = true);
    void editRecord();
    void addRecord();
    void removeRecord();
    void removeAllRecords();
    void sortRecords();
    void filterRecords();
    void processData();
    void removeAllFilters();
};

#endif // DB_MANAGER_H
