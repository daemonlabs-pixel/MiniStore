#include <stdexcept>
#include <utility>
#include <iostream>

#include "Store.h"

// ---------------- PRODUCTS ----------------

void Store::addProduct(std::unique_ptr<Product> product) {
    if (!product) {
        throw std::invalid_argument("Invalid product");
    }

    if (findProduct(product->getId()) != nullptr) {
        throw std::invalid_argument("Product already exists");
    }

    for (const auto& pending : pendingProducts) {
        if (pending->getId() == product->getId()) {
            throw std::invalid_argument("Product is already pending approval");
        }
    }

    products.push_back(std::move(product));
}


Product* Store::findProduct(int productId) {
    if (productId <= 0) {
        throw std::invalid_argument("Invalid product ID");
    }

    for (const auto& product : products) {
        if (product->getId() == productId) {
            return product.get();
        }
    }

    return nullptr;
}


const Product* Store::findProduct(int productId) const {
    if (productId <= 0) {
        throw std::invalid_argument("Invalid product ID");
    }

    for (const auto& product : products) {
        if (product->getId() == productId) {
            return product.get();
        }
    }

    return nullptr;
}


// ---------------- SELLER LISTINGS ----------------

void Store::addListing(std::unique_ptr<SellerListing> listing) {
    if (!listing) {
        throw std::invalid_argument("Invalid listing");
    }

    if (findProduct(listing->getProductId()) == nullptr) {
        throw std::invalid_argument("Product does not exist");
    }

    if (findListing(listing->getProductId(), listing->getSellerId()) != nullptr) {
        throw std::invalid_argument("Seller already has a listing for this product");
    }

    listings.push_back(std::move(listing));
}


bool Store::removeListing(int productId, int sellerId) {
    if (productId <= 0 || sellerId <= 0) {
        throw std::invalid_argument("Invalid ID");
    }

    for (auto it = listings.begin(); it != listings.end(); ++it) {
        if ((*it)->getProductId() == productId &&
            (*it)->getSellerId() == sellerId) {

            listings.erase(it);
            return true;
        }
    }

    return false;
}


SellerListing* Store::findListing(int productId, int sellerId) {
    if (productId <= 0 || sellerId <= 0) {
        throw std::invalid_argument("Invalid ID");
    }

    for (const auto& listing : listings) {
        if (listing->getProductId() == productId &&
            listing->getSellerId() == sellerId) {

            return listing.get();
        }
    }

    return nullptr;
}


const SellerListing* Store::findListing(int productId, int sellerId) const {
    if (productId <= 0 || sellerId <= 0) {
        throw std::invalid_argument("Invalid ID");
    }

    for (const auto& listing : listings) {
        if (listing->getProductId() == productId &&
            listing->getSellerId() == sellerId) {

            return listing.get();
        }
    }

    return nullptr;
}


// ---------------- PRODUCT PROPOSALS ----------------

void Store::proposeProduct(std::unique_ptr<Product> product) {
    if (!product) {
        throw std::invalid_argument("Invalid product");
    }

    if (findProduct(product->getId()) != nullptr) {
        throw std::invalid_argument("Product already exists in the catalog");
    }

    for (const auto& pending : pendingProducts) {
        if (pending->getId() == product->getId()) {
            throw std::invalid_argument("Product is already pending approval");
        }
    }

    pendingProducts.push_back(std::move(product));
}


bool Store::approveProduct(int productId) {
    if (productId <= 0) {
        throw std::invalid_argument("Invalid ID");
    }

    for (auto it = pendingProducts.begin(); it != pendingProducts.end(); ++it) {
        if ((*it)->getId() == productId) {
            products.push_back(std::move(*it));
            pendingProducts.erase(it);
            return true;
        }
    }

    return false;
}


// ---------------- DISPLAY ----------------

void Store::displayListings() const {
    for (const auto& listing : listings) {
        std::cout << listing->toString() << '\n';
    }
}


void Store::displayProducts() const {
    for (const auto& product : products) {
        std::cout << product->toString() << '\n';
    }
}


void Store::displaySellerListings(int sellerId) const {
    if (sellerId <= 0) {
        throw std::invalid_argument("Invalid seller ID");
    }

    bool found = false;

    for (const auto& listing : listings) {
        if (listing->getSellerId() == sellerId) {
            std::cout << listing->toString() << '\n';
            found = true;
        }
    }

    if (!found) {
        std::cout << "No listings found for this seller.\n";
    }
}