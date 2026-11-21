#ifndef PRODUCT_H
#define PRODUCT_H

#include <string>

class Product {
private:
    const int id;
    const std::string name;

public:
    Product(int id, const std::string& name);

    int getId() const;
    std::string getName() const;

    std::string toString() const;
};

#endif