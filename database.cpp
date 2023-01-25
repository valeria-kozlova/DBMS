#include "database.h"
#include <algorithm>

const unsigned database::MAX = UINT_MAX;

database::database() {
    char buf[128];
    sprintf(buf, "%32s | %32s | %16s | %9s | %10s", "ресторан-0",
            "блюдо-1", "тип-2", "вес-3", "цена-4");
    header = buf;
    data = new Record[size];
}

database::~database() {
    delete[] data;
}

Record &database::operator[](unsigned id) {
    if (id > count) {
        id = count;
    }
    return data[id];
}

unsigned database::getCount() const {
    return count;
}

bool database::isEmpty() const {
    if (filteredIds.empty()) {
        puts("Нет записей в базе");
        return true;
    }
    return false;
}

void database::print() {
    auto idlen = snprintf(nullptr, 0, "%u", count);
    if (idlen < 3) {
        idlen = 3;
    }
    std::string format = "%" + std::to_string(idlen) + "s | %s\n";
    unsigned len = printf(format.c_str(), "id", header.c_str());
    puts(std::string(len, '-').c_str());
    if (isEmpty()) {
        return;
    }
    format = "%" + std::to_string(idlen) + "u | %s\n";

    std::vector<std::vector<std::string>> res(3);
    std::vector<std::vector<float>> res2(2);
    fillFilters(res, res2);

    for (unsigned i = 0; i < count; i++) {
        if (filteredIds.find(i) != filteredIds.end()) {
            printf(format.c_str(), i, data[i].toFormattedString().c_str());
        }
    }
    puts(std::string(len, '-').c_str());
    printMean(idlen, len);
    printFilters(res, res2, idlen, len);
}

void database::printFilters(std::vector<std::vector<std::string>> &res, std::vector<std::vector<float>> &res2,
                            const unsigned &idlen, const unsigned &len) {
    unsigned rows = countMaxFilters();
    if (rows == 0) {
        return;
    }
    printf("%s%s\n", std::string(len / 2 - 4, ' ').c_str(), "Фильтры");
    puts(std::string(len, '-').c_str());
    for (int i = 0; i < rows; i++) {
        printf((std::string(idlen, ' ') + " | ").c_str());
        printf("%32s | ", res[RESTAURANT].size() > i ? res[RESTAURANT][i].c_str() : "");
        printf("%32s | ", res[MEAL].size() > i ? res[MEAL][i].c_str() : "");
        printf("%16s | ", res[TYPE].size() > i ? res[TYPE][i].c_str() : "");
        if (res2[WEIGHT - 3].size() > i) {
            printf("%9.3f | ", res2[WEIGHT - 3][i]);
        } else {
            printf("%9s | ", "");
        }
        if (res2[PRICE - 3].size() > i) {
            printf("%10.3f\n", res2[PRICE - 3][i]);
        } else {
            printf("%10s\n", "");
        }
    }
    puts(std::string(len, '-').c_str());
}

unsigned database::countMaxFilters() {
    unsigned size = 0;
    for (int i = 0; i < 5; i++) {
        if (i < 3) {
            if (filters[i].sfilter.size() > size) {
                size = filters[i].sfilter.size();
            }
        } else {
            if (filters[i].ffilter.size() > size) {
                size = filters[i].ffilter.size();
            }
        }
    }
    return size;
}

void database::fillFilters(std::vector<std::vector<std::string>> &res, std::vector<std::vector<float>> &res2) {
    for (int i = 0; i < 5; i++) {
        if (i < 3) {
            for (const auto& pair : filters[i].sfilter) {
                res[i].push_back(pair);
            }
        } else {
            for (const auto& pair : filters[i].ffilter) {
                res2[i - 3].push_back(pair);
            }
        }
    }
}

void database::printMean(const unsigned &idlen, const unsigned &len) {
    if (meanWeight != 0) {
        std::string format = "%" + std::to_string(idlen) + "s | ";
        printf(format.c_str(), "AVG");
        printf((std::string(32, ' ') + " | " +
                std::string(32, ' ') + " | " +
                std::string(16, ' ') + " | ").c_str());
        printf("%9.3f | ", meanWeight);
        printf("%10.3f\n", meanPrice);
        puts(std::string(len, '-').c_str());
    }
}

void database::add(const Record &rec) {
    add(count, rec);
}

void database::add(unsigned &id, const Record &rec) {
    if (id > count) {
        id = count;
    }
    if (count < MAX) {
        if (count == size) {
            size = size <= MAX - grow ? size + grow : MAX;
            auto newData = new Record[size];
            for (unsigned i = 0; i < id; i++) {
                newData[i] = data[i];
            }
            for (unsigned i = count; i > id; i--) {
                newData[i] = data[i - 1];
            }
            newData[id] = rec;
            printf("Запись добавлена по id: %u\n", id);
            count++;
            delete[] data;
            data = newData;
            update();
        } else {
            for (unsigned i = count; i > id; i--) {
                data[i] = data[i - 1];
            }
            data[id] = rec;
            printf("Запись добавлена по id: %u\n", id);
            count++;
            update();
        }
    }
}

void database::edit(const unsigned &id, const Record &rec) {
    data[id] = rec;
    printf("Запись изменена по id: %u\n", id);
    update();
}

void database::remove(const unsigned &id) {
    for (unsigned i = id; i < count; i++) {
        data[i] = data[i + 1];
    }
    printf("Запись удалена по id: %u\n", id);
    count--;
    update(false);
}

void database::removeAll() {
    count = 0;
    size = 4;
    delete[] data;
    data = new Record[size];
    puts("Все записи удалены");
}

void database::update(const bool &sort) {
    if (sort) {
        sorted = -1;
    }
    updateFilteredIds();
}

void database::sort(const COL &col) {
    switch (col) {
        case RESTAURANT:
            sortInternal(sorted == RESTAURANT, "ресторан", Record::compareRestaurant);
            break;
        case MEAL:
            sortInternal(sorted == MEAL, "блюдо", Record::compareMeal);
            break;
        case TYPE:
            sortInternal(sorted == TYPE, "тип", Record::compareType);
            break;
        case WEIGHT:
            sortInternal(sorted == WEIGHT, "вес", Record::compareWeight);
            break;
        case PRICE:
            sortInternal(sorted == PRICE, "цена", Record::comparePrice);
            break;
    }
    sorted = col;
}

void database::sortInternal(const bool &ascending, const std::string &colname,
                            bool (*cmp)(const Record &a, const Record &b)) {
    if (ascending) {
        std::reverse(data, data + count);
        printf("Отсортировано по убыванию колонки %s\n", colname.c_str());
    } else {
        std::sort(data, data + count, cmp);
        printf("Отсортировано по возрастанию колонки %s\n", colname.c_str());
    }
}

void database::updateFilteredIds() {
    meanWeight = 0.0;
    meanPrice = 0.0;
    filteredIds.clear();
    if (countMaxFilters()) {
        for (unsigned i = 0; i < count; i++) {
            if (filters[RESTAURANT].sfilter.find(data[i].getRestaurant()) != filters[RESTAURANT].sfilter.end()
                || filters[MEAL].sfilter.find(data[i].getMeal()) != filters[MEAL].sfilter.end()
                || filters[TYPE].sfilter.find(data[i].getType()) != filters[TYPE].sfilter.end()
                || filters[WEIGHT].ffilter.find(data[i].getWeight()) != filters[WEIGHT].ffilter.end()
                || filters[PRICE].ffilter.find(data[i].getPrice()) != filters[PRICE].ffilter.end()) {
                filteredIds.insert(i);
            }
        }
    } else {
        for (unsigned i = 0; i < count; i++) {
            filteredIds.insert(i);
        }
    }
}

void database::addRemoveFilter(const COL &col, Record &temp) {
    switch (col) {
        case RESTAURANT:
            if (filters[RESTAURANT].sfilter.find(temp.getRestaurant()) == filters[RESTAURANT].sfilter.end()) {
                filters[RESTAURANT].sfilter.insert(temp.getRestaurant());
                printf("Добавлен фильтр по колонке ресторан: %s\n", temp.getRestaurant());
            } else {
                filters[RESTAURANT].sfilter.erase(temp.getRestaurant());
                printf("Удален фильтр по колонке ресторан: %s\n", temp.getRestaurant());
            }
            break;
        case MEAL:
            if (filters[MEAL].sfilter.find(temp.getMeal()) == filters[MEAL].sfilter.end()) {
                filters[MEAL].sfilter.insert(temp.getMeal());
                printf("Добавлен фильтр по колонке блюдо: %s\n", temp.getMeal());
            } else {
                filters[MEAL].sfilter.erase(temp.getMeal());
                printf("Удален фильтр по колонке блюдо: %s\n", temp.getMeal());
            }
            break;
        case TYPE:
            if (filters[TYPE].sfilter.find(temp.getType()) == filters[TYPE].sfilter.end()) {
                filters[TYPE].sfilter.insert(temp.getType());
                printf("Добавлен фильтр по колонке тип: %s\n", temp.getType());
            } else {
                filters[TYPE].sfilter.erase(temp.getType());
                printf("Удален фильтр по колонке тип: %s\n", temp.getType());
            }
            break;
        case WEIGHT:
            if (filters[WEIGHT].ffilter.find(temp.getWeight()) == filters[WEIGHT].ffilter.end()) {
                filters[WEIGHT].ffilter.insert(temp.getWeight());
                printf("Добавлен фильтр по колонке вес: %9.3f\n", temp.getWeight());
            } else {
                filters[WEIGHT].ffilter.erase(temp.getWeight());
                printf("Удален фильтр по колонке вес: %9.3f\n", temp.getWeight());
            }
            break;
        case PRICE:
            if (filters[PRICE].ffilter.find(temp.getPrice()) == filters[PRICE].ffilter.end()) {
                filters[PRICE].ffilter.insert(temp.getPrice());
                printf("Добавлен фильтр по колонке цена: %10.3f\n", temp.getPrice());
            } else {
                filters[PRICE].ffilter.erase(temp.getPrice());
                printf("Удален фильтр по колонке цена: %10.3f\n", temp.getPrice());
            }
            break;
    }
    updateFilteredIds();
}

void database::removeAllFilters() {
    for (auto &filter : filters) {
        filter.sfilter.clear();
        filter.ffilter.clear();
    }
    printf("Все фильтры были удалены\n");
    updateFilteredIds();
}

void database::mean() {
    float sumWeight = 0;
    float sumPrice = 0;
    for (unsigned i = 0; i < count; i++) {
        if (filteredIds.find(i) != filteredIds.end()) {
            sumWeight += data[i].getWeight();
            sumPrice += data[i].getPrice();
        }
    }
    meanWeight = sumWeight / (float)filteredIds.size();
    meanPrice = sumPrice / (float)filteredIds.size();
}
