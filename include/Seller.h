#ifndef SELLER_H
#define SELLER_H

#include <string>
#include <memory>

#include "Account.h"
#include "Store.h"


class Seller : public Account {
public:
    Seller(int id, const std::string& username,
           const std::string& password);

    std::string getRole() const override;

    void createListing(int productId, double price, Store& store);

    void proposeProduct(std::unique_ptr<Product> product, Store& store);

    bool removeListing(int productId, Store& store);

    void updateListingPrice(int productId, double price, Store& store);

    void restockListing(int productId, int quantity, Store& store);

    void displayListings(const Store& store) const;
};

#endif