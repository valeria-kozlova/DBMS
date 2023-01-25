#ifndef DB_REC_CPP
#define DB_REC_CPP

#include "record.h"
#include <ostream>
#include <sstream>
#include <iostream>
#include <cstring>
#include <vector>

char *Record::getRestaurant() {
    return restaurant;
}

char *Record::getMeal() {
    return meal;
}

char *Record::getType() {
    return type;
}

float Record::getWeight() const {
    return weight;
}

float Record::getPrice() const {
    return price;
}

bool Record::setRestaurant(const char *val) {
    if (val[0] != '\0') {
        strncpy(restaurant, val, Record::RESTAURANT);
        restaurant[RESTAURANT - 1] = '\0';
        return true;
    }
    puts("Ресторан не может быть пустым");
    return false;
}

bool Record::setMeal(const char *val) {
    if (val[0] != '\0') {
        strncpy(meal, val, Record::MEAL);
        meal[MEAL - 1] = '\0';
        return true;
    }
    puts("Блюдо не может быть пустым");
    return false;
}

bool Record::setType(const char *val) {
    if (val[0] != '\0') {
        strncpy(type, val, Record::TYPE);
        type[TYPE - 1] = '\0';
        return true;
    }
    puts("Тип не может быть пустым");
    return false;
}

bool Record::setWeight(const float &val) {
    if (val > 0 && val <= 99999) {
        weight = val;
        return true;
    }
    puts("Вес должен быть больше 0 и небольше 99999");
    return false;
}

bool Record::setPrice(const float &val) {
    if (val > 0 && val <= 999999) {
        price = val;
        return true;
    }
    puts("Цена должна быть больше 0 и небольше 999999");
    return false;
}

std::string trim(std::string str) {
    std::stringstream ss(str);
    ss >> str;
    return str;
}

Record* Record::read(const std::string &str, const char &sep) {
    std::stringstream ss(str);
    std::vector<std::string> items;
    std::string item;
    while (std::getline(ss, item, sep)) {
        if (item.length() > 0) {
            items.push_back(item);
        }
        if (items.size() == 5) {
            break;
        }
    }
    if (items.size() == 5) {
        Record* temp = new Record();
        if (temp->setRestaurant(items[0].c_str()) && temp->setMeal(items[1].c_str())
            && temp->setType(items[2].c_str()) && temp->setWeight(atof(items[3].c_str()))
            && temp->setPrice(atof(items[4].c_str()))) {
            return temp;
        }
        delete temp;
    } else {
        puts("В строке недостаточно элементов");
    }
    return nullptr;
}

std::string Record::toString(const std::string &sep) { // Файл
    std::ostringstream ss;
    ss << restaurant << sep << meal << sep << type << sep << weight << sep << price;
    return ss.str();
}

std::string Record::toFormattedString() { // Таблица
    char buf[128];
    sprintf(buf, "%32s | %32s | %16s | %9.3f | %10.3f", restaurant, meal, type, weight, price);
    return buf;
}

bool Record::compareRestaurant(const Record &a, const Record &b) {
    return std::string(a.restaurant) < std::string(b.restaurant);
}

bool Record::compareMeal(const Record &a, const Record &b) {
    return std::string(a.meal) < std::string(b.meal);
}

bool Record::compareType(const Record &a, const Record &b) {
    return std::string(a.type) < std::string(b.type);
}

bool Record::compareWeight(const Record &a, const Record &b) {
    return a.weight < b.weight;
}

bool Record::comparePrice(const Record &a, const Record &b) {
    return a.price < b.price;
}

#endif // DB_REC_CPP
