#ifndef DB_RECORD_H
#define DB_RECORD_H

#include <string>

class Record {
public:
    char* getRestaurant();
    char* getMeal();
    char* getType();
    float getWeight() const;
    float getPrice() const;

    bool setRestaurant(const char* val);
    bool setMeal(const char* val);
    bool setType(const char* val);
    bool setWeight(const float &val);
    bool setPrice(const float &val);

    static bool compareRestaurant(const Record &a, const Record &b);
    static bool compareMeal(const Record &a, const Record &b);
    static bool compareType(const Record &a, const Record &b);
    static bool compareWeight(const Record &a, const Record &b);
    static bool comparePrice(const Record &a, const Record &b);

    static Record* read(const std::string &str, const char &sep = ',');
    std::string toString(const std::string &sep = ",");
    std::string toFormattedString();

private:
    static const unsigned RESTAURANT = 32;
    static const unsigned MEAL = 32;
    static const unsigned TYPE = 16;
    char restaurant[RESTAURANT];
    char meal[MEAL];
    char type[TYPE];
    float weight;
    float price;
};

#endif //DB_RECORD_H
