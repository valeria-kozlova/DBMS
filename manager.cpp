#include "manager.h"
#include <iostream>
#include <fstream>

Manager::Manager(const char &key, const std::string &path) {
    KEY = key;
    this->path = path;
    if (!path.empty()) {
        readDatabase();
    }
}

void Manager::setKey() {
    char key = readUint("������� ����� ���� (�� 0 - ��� ���������� �� 254 ������������) ��� -1 ��� ������", 255);
    if (key == -1) {
        return;
    }
    KEY = key;
}

void Manager::manageDatabase() {
    unsigned code;
    do {
        db.print();
        code = cmd.switchCommand();
        switch (code) {
            case CMD::KEY:
                setKey();
                break;
            case CMD::READ:
                readDatabase();
                break;
            case CMD::ADD:
                addRecord();
                break;
            case CMD::EDIT:
                editRecord();
                break;
            case CMD::REMOVE:
                removeRecord();
                break;
            case CMD::REMOVEALL:
                removeAllRecords();
                break;
            case CMD::SORT:
                sortRecords();
                break;
            case CMD::FILTER:
                filterRecords();
                break;
            case CMD::REMOVEFILTERS:
                removeAllFilters();
                break;
            case CMD::PROCESSING:
                processData();
                break;
            case CMD::SAVE:
                saveDatabase();
                break;
        }
    } while (code != CMD::EXIT);
}

bool Manager::readString(std::string &str, const std::string &print) {
    puts(print.c_str());
    fflush(stdin);
    std::getline(std::cin, str);
    if (str.empty()) {
        puts("������");
        return false;
    }
    return true;
}

void Manager::readDatabase() { // ������ ���� ������
    do {
        std::string p;
        if (!path.empty()) {
            printf("���� ��� �����: %s\n", path.c_str());
            if (readString(p, "������� ����� ���� � ����� ��� ������ ������, ����� ������������ ������")) {
                path = p;
            }
        } else if (!readString(path, "������� ���� � ����� ��� ������ ������ ��� ������")) {
            break;
        }
        printf("������ ���� �� ����: %s\n", path.c_str());
        std::ifstream is(path);
        std::string encrypted;
        std::string decrypted;
        if (is.is_open()) {
            while (std::getline(is, encrypted)) {
                for (char ch: encrypted) {
                    decrypted += (char) (ch - KEY);
                }
                auto rec = Record::read(decrypted);
                decrypted.clear();
                if (rec) {
                    db.add(*rec);
                }
            }
            break;
        } else {
            printf("������ ������ ����� �� ����: %s\n", path.c_str());
            path.clear();
        }
    } while (true);
}

bool fileExists(const std::string &path) {
    if (FILE *file = fopen(path.c_str(), "r")) {
        fclose(file);
        return true;
    } else {
        return false;
    }
}

void Manager::saveDatabase() { // ��������� ���� ������
    if (db.isEmpty()) {
        return;
    }
    do {
        std::string p;
        if (!path.empty()) {
            printf("���� ��� �����: %s\n", path.c_str());
            if (readString(p, "������� ����� ���� � ����� ��� ������ ������, ����� ������������ ������")) {
                path = p;
            }
        } else if (!readString(path, "������� ���� � ����� ��� ������ ������, ����� ������������ ������")) {
            break;
        }
        if (fileExists(path)) {
            printf("���� %s ��� ����������\n", path.c_str());
            if (readString(p, "������� ����� ���� � ����� ��� ������ ������, ����� ������������ ����")) {
                path = p;
            }
        }
        printf("���������� � ���� �� ����: %s\n", path.c_str());
        std::ofstream os(path);
        if (os.is_open()) {
            for (int i = 0; i < db.getCount(); i++) {
                for (auto ch: db[i].toString()) {
                    os << (char) (ch + KEY);
                }
                os << std::endl;
            }
            break;
        } else {
            printf("������ ���������� ����� �� ����: %s\n", path.c_str());
            path.clear();
        }
    } while (true);
}

unsigned Manager::readUint(const std::string &name, const unsigned &lessThan) {
    puts(name.c_str());
    unsigned val = -1; // -1 � uint ��� UINT_MAX - �� ������������
    do {
        std::cin.clear();
        fflush(stdin);
        std::cin >> val;
        if (val == -1) {
            puts("������");
            break;
        } else if (std::cin.fail() || val >= lessThan) {
            puts("������������ �����");
        } else break;
    } while (true);
    return val;
}

void Manager::editOrAdd(const bool &edit) {
    auto id = readUint("������� id ��� -1 ��� ������", edit ? db.getCount() : database::MAX);
    if (id == -1) {
        return;
    }
    std::string line;
    float fnumber;
    int valid = 0;
    Record* rec = new Record();
    if (readString(line, "������� '��������' ��� ������ ������ ��� ������")) {
        if (rec->setRestaurant(line.c_str())) {
            printf("��������: %s\n", rec->getRestaurant());
            valid++;
        }
    }
    while (valid == 1) {
        if (readString(line, "������� '�����' ��� ������ ������ ��� ������")) {
            if (rec->setMeal(line.c_str())) {
                printf("�����: %s\n", rec->getMeal());
                valid++;
                break;
            }
        } else {
            break;
        }
    }
    while (valid == 2) {
        if (readString(line, "������� '���' ��� ������ ������ ��� ������")) {
            if (rec->setType(line.c_str())) {
                printf("���: %s\n", rec->getType());
                valid++;
                break;
            }
        } else {
            break;
        }
    }
    while (valid == 3) {
        if (readFloat(fnumber, "������� '���' ��� -1 ��� ������")) {
            if (rec->setWeight(fnumber)) {
                printf("���: %9.3f\n", rec->getWeight());
                valid++;
                break;
            }
        } else {
            break;
        }
    }
    while (valid == 4) {
        if (readFloat(fnumber, "������� '����' ��� -1 ��� ������")) {
            if (rec->setPrice(fnumber)) {
                printf("����: %10.3f\n", rec->getPrice());
                valid++;
                break;
            }
        } else {
            break;
        }
    }
    if (valid != 5) {
        delete rec;
    } else {
        if (edit) {
            db.edit(id, *rec);
        } else {
            db.add(id, *rec);
        }
    }
}

void Manager::editRecord() {
    editOrAdd();
}

void Manager::addRecord() {
    editOrAdd(false);
}

void Manager::removeRecord() {
    auto id = readUint("������� id ��� -1 ��� ������", db.getCount());
    if (id == -1) {
        return;
    }
    db.remove(id);
}

void Manager::removeAllRecords() {
    if (db.isEmpty()) {
        return;
    }
    std::string line;
    if (readString(line, "������� ����� ������ ��� ������������� ��� ������ - ��� ������")) {
        if (readString(line, "������� ����� ������ ��� ������������� ��� ������ - ��� ������")) {
            if (readString(line, "������� ����� ������ ��� ������������� ��� ������ - ��� ������")) {
                db.removeAll();
            }
        }
    }
}

void Manager::sortRecords() {
    if (db.isEmpty()) {
        return;
    }
    char c = readUint("������� ����� ������� ��� -1 ��� ������", 5);
    if (c == -1) {
        return;
    }
    db.sort(static_cast<database::COL>(c));
}

bool Manager::readFloat(float &val, const std::string &print) {
    puts(print.c_str());
    std::cin.clear();
    fflush(stdin);
    std::cin >> val;
    if (val == -1) {
        puts("������");
        return false;
    }
    return true;
}

void Manager::filterRecords() {
    if (db.isEmpty()) {
        return;
    }
    char c = readUint("������� ����� ������� ��� -1 ��� ������", 5);
    if (c == -1) {
        return;
    }
    auto col = static_cast<database::COL>(c);
    Record temp{};
    std::string line;
    float val;
    do {
        switch (col) {
            case database::COL::RESTAURANT:
                if (!readString(line, "������� ������ ��� '��������' ��� ������ ������ ��� ������")) {
                    return;
                }
                if (temp.setRestaurant(line.c_str())) {
                    db.addRemoveFilter(col, temp);
                    return;
                }
                break;
            case database::COL::MEAL:
                if (!readString(line, "������� ������ ��� '�����' ��� ������ ������ ��� ������")) {
                    return;
                }
                if (temp.setMeal(line.c_str())) {
                    db.addRemoveFilter(col, temp);
                    return;
                }
                break;
            case database::COL::TYPE:
                if (!readString(line, "������� ������ ��� '���' ��� ������ ������ ��� ������")) {
                    return;
                }
                if (temp.setType(line.c_str())) {
                    db.addRemoveFilter(col, temp);
                    return;
                }
                break;
            case database::COL::WEIGHT:
                if (!readFloat(val, "������� ������ ��� '���' ��� -1 ��� ������")) {
                    return;
                }
                if (temp.setWeight(val)) {
                    db.addRemoveFilter(col, temp);
                    return;
                }
                break;
            case database::COL::PRICE:
                if (!readFloat(val, "������� ������ ��� '����' ��� -1 ��� ������")) {
                    return;
                }
                if (temp.setPrice(val)) {
                    db.addRemoveFilter(col, temp);
                    return;
                }
                break;
        }
    } while (true);
}

void Manager::removeAllFilters() {
    std::string line;
    if (readString(line, "������� ����� ������ ��� ������������� ��� ������ - ��� ������")) {
        db.removeAllFilters();
    }
}

void Manager::processData() {
    if (db.isEmpty()) {
        return;
    }
    db.mean();
}