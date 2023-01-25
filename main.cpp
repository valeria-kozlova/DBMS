#include "manager.h"
#include <cstdio>
#include <windows.h>

int main(int argc, char* argv[]) {
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    std::string path;
    char key = 10;
    if (argc > 1) {
        path = argv[1];
        if (argc > 2) {
            key = atoi(argv[2]);
        }
    }

    Manager manager{key, path};
    manager.manageDatabase();
    puts("Работа закончена");
    system("pause");
    return 0;
}
