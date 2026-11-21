#include <stdexcept>
#include <limits>
#include"SellerListing.h"

SellerListing:: SellerListing(int productId, int sellerId, double price)
    : productId(productId), sellerId(sellerId), stock(0), price(price), isActive(true){
        if(productId<=0 || sellerId<=0|| price <=0){
            throw std::invalid_argument("Inavlid information");
        }

}


int SellerListing::getProductId() const{
    return productId;
}

int SellerListing::getSellerId() const{
    return sellerId;
}

int SellerListing::getStock() const{
    return stock;
}

double SellerListing::getPrice() const{
    return price;

}


void SellerListing::setPrice(double price){
    if(price<=0){
        throw std::invalid_argument("Invalid price");
    }

    this->price = price;
}

void SellerListing::restock(int quantity){
    if(quantity <=0){
        throw std::invalid_argument("Invalid quantity");
    }

    if(quantity > std::numeric_limits<int>::max() - stock) {
        throw std::overflow_error("Stock value would exceed the maximum integer value");
    }
    stock +=quantity;


}


void SellerListing::sell(int quantity){
    if(quantity<=0){
        throw std::invalid_argument("Invalid quantity");
    }

    if(quantity> stock){
        throw std::out_of_range("Quantity exceeds available stock");

    }
    stock -= quantity;
}


double SellerListing::getValue() const{
    return price*stock;
}

void SellerListing::activate(){
    isActive = true;
}

void SellerListing::deactivate(){
    isActive= false;
}

bool SellerListing::IsActive() const{
    return isActive;

}



std::string SellerListing::toString() const{
    return "Product ID: " + std::to_string(productId) +
            "\nSeller ID: " + std::to_string(sellerId) +
            "\nPrice: " + std::to_string(price) +
            "\nStock: " + std::to_string(stock) +
            "\nTotal Value: " + std::to_string(getValue());

}