#ifndef SELLERLISTING_H
#define SELLERLISTING_H

#include <string>

class SellerListing {
private:
    const int productId;
    const int sellerId;
    int stock;
    double price;
    bool isActive;

public:
    SellerListing(int productId, int sellerId, double price);

    int getProductId() const;
    int getSellerId() const;
    int getStock() const;
    double getPrice() const;

    void setPrice(double price);

    void restock(int quantity);
    void sell(int quantity);

    void activate();
    void deactivate();
    bool IsActive() const;

    double getValue() const;
    std::string toString() const;
};

#endif