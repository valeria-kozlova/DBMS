#ifndef DB_DATABASE_H
#define DB_DATABASE_H

#include "record.h"
#include <unordered_set>
#include <vector>

struct filter {
    std::unordered_set<std::string> sfilter;
    std::unordered_set<float> ffilter;
};

class database {
private:
    std::string header;
    Record* data;
    unsigned count = 0;
    unsigned size = 4;
    const unsigned grow = 4;
    char sorted = -1;
    filter filters[5]{};
    std::unordered_set<unsigned> filteredIds;
    float meanWeight = 0.0;
    float meanPrice = 0.0;

public:
    enum COL{RESTAURANT = 0, MEAL, TYPE, WEIGHT, PRICE};
    static const unsigned MAX;
    database();
    ~database();
    Record& operator[](unsigned);
    unsigned getCount() const;
    bool isEmpty() const;
    void print();
    void printFilters(std::vector<std::vector<std::string>> &res, std::vector<std::vector<float>> &res2,
                      const unsigned &idlen, const unsigned &len);
    unsigned countMaxFilters();
    void fillFilters(std::vector<std::vector<std::string>> &res, std::vector<std::vector<float>> &res2);
    void printMean(const unsigned &idlen, const unsigned &len);
    void add(const Record &rec);
    void add(unsigned &id, const Record &rec);
    void edit(const unsigned &id, const Record &rec);
    void remove(const unsigned &id);
    void removeAll();
    void sort(const COL &col);
    void sortInternal(const bool &ascending, const std::string &colname, bool (*cmp)(const Record &a, const Record &b));
    void update(const bool &sort = true);
    void updateFilteredIds();
    void addRemoveFilter(const COL &col, Record &temp);
    void removeAllFilters();
    void mean();
};

#endif //DB_DATABASE_H
