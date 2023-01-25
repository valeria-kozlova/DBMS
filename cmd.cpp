#include <iostream>
#include "cmd.h"

CMD::CMD() {
    commands = { "�������� ���� ����������",
                 "��������� � �������� �� � �����",
                 "��������",
                 "�������������",
                 "�������",
                 "������� ���",
                 "����������� �� ����������� ��� ��������",
                 "�������� ��� ������� ������",
                 "������� ��� �������",
                 "��������� ������� ��� � ���� ��� ��������������� ��������",
                 "���������",
                 "����� �� ���������"};
}

void CMD::printCommands() {
    puts("�������� ��������:");
    for (int i = 0; i < commands.size(); i++) {
        printf("%d - %s\n", i, commands[i].c_str());
    }
}

unsigned CMD::switchCommand() {
    unsigned code = NONE;
    printCommands();
    do {
        printf("��������:");
        std::cin.clear();
        unsigned opt;
        fflush(stdin);
        std::cin >> opt;
        if (std::cin.fail() || opt < 0 || opt >= commands.size()) {
            code = NONE;
            puts("��� ������ ��������");
        } else {
            code = opt;
        }
    } while (code == NONE);
    return code;
}