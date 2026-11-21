#include <stdexcept>
#include <limits>
#include <string>

#include "CartItem.h"


CartItem::CartItem(int productId, int sellerId, int quantity)
    : productId(productId), sellerId(sellerId), quantity(quantity) {

    if (productId <= 0 || sellerId <= 0 || quantity <= 0) {
        throw std::invalid_argument("Invalid cart item information");
    }
}


int CartItem::getProductId() const {
    return productId;
}


int CartItem::getSellerId() const {
    return sellerId;
}


int CartItem::getQuantity() const {
    return quantity;
}


void CartItem::setQuantity(int quantity) {
    if (quantity <= 0) {
        throw std::invalid_argument("Invalid quantity");
    }

    this->quantity = quantity;
}


void CartItem::increaseQuantity(int quantity) {
    if (quantity <= 0) {
        throw std::invalid_argument("Invalid quantity");
    }

    if (quantity > std::numeric_limits<int>::max() - this->quantity) {
        throw std::overflow_error("Quantity would exceed the maximum integer value");
    }

    this->quantity += quantity;
}


void CartItem::decreaseQuantity(int quantity) {
    if (quantity <= 0) {
        throw std::invalid_argument("Invalid quantity");
    }

    if (quantity >= this->quantity) {
        throw std::out_of_range("Quantity cannot become zero or negative");
    }

    this->quantity -= quantity;
}


std::string CartItem::toString() const {
    return "Product ID: " + std::to_string(productId) +
           "\nSeller ID: " + std::to_string(sellerId) +
           "\nQuantity: " + std::to_string(quantity);
}