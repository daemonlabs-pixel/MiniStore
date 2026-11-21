#ifndef STORE_H
#define STORE_H

#include <vector>
#include <memory>

#include "Product.h"
#include "SellerListing.h"

class Store {
private:
    std::vector<std::unique_ptr<Product>> products;
    std::vector<std::unique_ptr<SellerListing>> listings;
    std::vector<std::unique_ptr<Product>> pendingProducts;

public:
    void addProduct(std::unique_ptr<Product> product);
    Product* findProduct(int productId);
    const Product* findProduct(int productId) const;

    void addListing(std::unique_ptr<SellerListing> listing);

    bool removeListing(int productId, int sellerId);

    SellerListing* findListing(int productId, int sellerId);
    const SellerListing* findListing(int productId, int sellerId) const;

    void displayProducts() const;
    void displayListings() const;
    void displaySellerListings(int sellerId) const;

    void proposeProduct(std::unique_ptr<Product> product);
    bool approveProduct(int productId);
};

#endif