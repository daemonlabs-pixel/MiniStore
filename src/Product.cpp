#include "Product.h"
#include <stdexcept>
#include <string>

Product::Product(int id, const std::string& name)
    : id(id), name(name) {

    if (id <= 0 ||
        name.find_first_not_of(" \t\n\r") == std::string::npos) {
        throw std::invalid_argument("Invalid product information");
    }
}

int Product::getId() const {
    return id;
}

std::string Product::getName() const {
    return name;
}

std::string Product::toString() const {
    return "ID: " + std::to_string(id) +
           "\nName: " + name;
}