#ifndef DB_CMD_H
#define DB_CMD_H

#include <string>
#include <vector>

class CMD {
    void printCommands();
protected:
    std::vector<std::string> commands;
public:
    CMD();
    enum {NONE = -1, KEY, READ, ADD, EDIT, REMOVE, REMOVEALL, SORT, FILTER, REMOVEFILTERS, PROCESSING, SAVE, EXIT};
    unsigned switchCommand();
};

#endif // DB_CMD_H
