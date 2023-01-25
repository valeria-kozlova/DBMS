#include <iostream>
#include "cmd.h"

CMD::CMD() {
    commands = { "Изменить ключ шифрования",
                 "Прочитать и добавить всё в конец",
                 "Добавить",
                 "Редактировать",
                 "Удалить",
                 "Удалить все",
                 "Сортировать по возрастанию или убыванию",
                 "Добавить или удалить фильтр",
                 "Удалить все фильтры",
                 "Посчитать среднюю вес и цену для отфильтрованных значений",
                 "Сохранить",
                 "Выход из программы"};
}

void CMD::printCommands() {
    puts("Выберите действие:");
    for (int i = 0; i < commands.size(); i++) {
        printf("%d - %s\n", i, commands[i].c_str());
    }
}

unsigned CMD::switchCommand() {
    unsigned code = NONE;
    printCommands();
    do {
        printf("Действие:");
        std::cin.clear();
        unsigned opt;
        fflush(stdin);
        std::cin >> opt;
        if (std::cin.fail() || opt < 0 || opt >= commands.size()) {
            code = NONE;
            puts("Нет такого действия");
        } else {
            code = opt;
        }
    } while (code == NONE);
    return code;
}
