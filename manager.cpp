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
    char key = readUint("Введите новый ключ (от 0 - без шифрования до 254 включительно) или -1 для отмены", 255);
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
        puts("Отмена");
        return false;
    }
    return true;
}

void Manager::readDatabase() { // Читаем базу данных
    do {
        std::string p;
        if (!path.empty()) {
            printf("Путь уже задан: %s\n", path.c_str());
            if (readString(p, "Введите новый путь к файлу или пустую строку, чтобы использовать старый")) {
                path = p;
            }
        } else if (!readString(path, "Введите путь к файлу или пустую строку для отмены")) {
            break;
        }
        printf("Читаем файл по пути: %s\n", path.c_str());
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
            printf("Ошибка чтения файла по пути: %s\n", path.c_str());
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

void Manager::saveDatabase() { // Сохраняем базу данных
    if (db.isEmpty()) {
        return;
    }
    do {
        std::string p;
        if (!path.empty()) {
            printf("Путь уже задан: %s\n", path.c_str());
            if (readString(p, "Введите новый путь к файлу или пустую строку, чтобы использовать старый")) {
                path = p;
            }
        } else if (!readString(path, "Введите путь к файлу или пустую строку, чтобы использовать старый")) {
            break;
        }
        if (fileExists(path)) {
            printf("Файл %s уже существует\n", path.c_str());
            if (readString(p, "Введите новый путь к файлу или пустую строку, чтобы перезаписать файл")) {
                path = p;
            }
        }
        printf("Записываем в файл по пути: %s\n", path.c_str());
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
            printf("Ошибка сохранения файла по пути: %s\n", path.c_str());
            path.clear();
        }
    } while (true);
}

unsigned Manager::readUint(const std::string &name, const unsigned &lessThan) {
    puts(name.c_str());
    unsigned val = -1; // -1 в uint это UINT_MAX - не используется
    do {
        std::cin.clear();
        fflush(stdin);
        std::cin >> val;
        if (val == -1) {
            puts("Отмена");
            break;
        } else if (std::cin.fail() || val >= lessThan) {
            puts("Некорректное число");
        } else break;
    } while (true);
    return val;
}

void Manager::editOrAdd(const bool &edit) {
    auto id = readUint("Введите id или -1 для отмены", edit ? db.getCount() : database::MAX);
    if (id == -1) {
        return;
    }
    std::string line;
    float fnumber;
    int valid = 0;
    Record* rec = new Record();
    if (readString(line, "Введите 'ресторан' или пустую строку для отмены")) {
        if (rec->setRestaurant(line.c_str())) {
            printf("Ресторан: %s\n", rec->getRestaurant());
            valid++;
        }
    }
    while (valid == 1) {
        if (readString(line, "Введите 'блюдо' или пустую строку для отмены")) {
            if (rec->setMeal(line.c_str())) {
                printf("Блюдо: %s\n", rec->getMeal());
                valid++;
                break;
            }
        } else {
            break;
        }
    }
    while (valid == 2) {
        if (readString(line, "Введите 'тип' или пустую строку для отмены")) {
            if (rec->setType(line.c_str())) {
                printf("Тип: %s\n", rec->getType());
                valid++;
                break;
            }
        } else {
            break;
        }
    }
    while (valid == 3) {
        if (readFloat(fnumber, "Введите 'вес' или -1 для отмены")) {
            if (rec->setWeight(fnumber)) {
                printf("Вес: %9.3f\n", rec->getWeight());
                valid++;
                break;
            }
        } else {
            break;
        }
    }
    while (valid == 4) {
        if (readFloat(fnumber, "Введите 'цену' или -1 для отмены")) {
            if (rec->setPrice(fnumber)) {
                printf("Цена: %10.3f\n", rec->getPrice());
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
    auto id = readUint("Введите id или -1 для отмены", db.getCount());
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
    if (readString(line, "Введите любую строку для подтверждения или пустую - для отмены")) {
        if (readString(line, "Введите любую строку для подтверждения или пустую - для отмены")) {
            if (readString(line, "Введите любую строку для подтверждения или пустую - для отмены")) {
                db.removeAll();
            }
        }
    }
}

void Manager::sortRecords() {
    if (db.isEmpty()) {
        return;
    }
    char c = readUint("Введите номер колонки или -1 для отмены", 5);
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
        puts("Отмена");
        return false;
    }
    return true;
}

void Manager::filterRecords() {
    if (db.isEmpty()) {
        return;
    }
    char c = readUint("Введите номер колонки или -1 для отмены", 5);
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
                if (!readString(line, "Введите фильтр для 'ресторан' или пустую строку для отмены")) {
                    return;
                }
                if (temp.setRestaurant(line.c_str())) {
                    db.addRemoveFilter(col, temp);
                    return;
                }
                break;
            case database::COL::MEAL:
                if (!readString(line, "Введите фильтр для 'блюдо' или пустую строку для отмены")) {
                    return;
                }
                if (temp.setMeal(line.c_str())) {
                    db.addRemoveFilter(col, temp);
                    return;
                }
                break;
            case database::COL::TYPE:
                if (!readString(line, "Введите фильтр для 'тип' или пустую строку для отмены")) {
                    return;
                }
                if (temp.setType(line.c_str())) {
                    db.addRemoveFilter(col, temp);
                    return;
                }
                break;
            case database::COL::WEIGHT:
                if (!readFloat(val, "Введите фильтр для 'вес' или -1 для отмены")) {
                    return;
                }
                if (temp.setWeight(val)) {
                    db.addRemoveFilter(col, temp);
                    return;
                }
                break;
            case database::COL::PRICE:
                if (!readFloat(val, "Введите фильтр для 'цену' или -1 для отмены")) {
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
    if (readString(line, "Введите любую строку для подтверждения или пустую - для отмены")) {
        db.removeAllFilters();
    }
}

void Manager::processData() {
    if (db.isEmpty()) {
        return;
    }
    db.mean();
}