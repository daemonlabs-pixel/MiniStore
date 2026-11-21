#include <memory>
#include <utility>
#include <stdexcept>

#include "Seller.h"

Seller::Seller(int id, const std::string& username,
               const std::string& password)
    : Account(id, username, password) {}


std::string Seller::getRole() const {
    return "Seller";
}


void Seller::createListing(int productId, double price, Store& store) {
    auto listing = std::make_unique<SellerListing>(
        productId, getId(), price
    );

    store.addListing(std::move(listing));
}


void Seller::proposeProduct(std::unique_ptr<Product> product, Store& store) {
    store.proposeProduct(std::move(product));
}


bool Seller::removeListing(int productId, Store& store) {
    return store.removeListing(productId, getId());
}


void Seller::updateListingPrice(int productId, double price, Store& store) {
    SellerListing* listing = store.findListing(productId, getId());

    if (listing == nullptr) {
        throw std::out_of_range("Listing not found");
    }

    listing->setPrice(price);
}


void Seller::restockListing(int productId, int quantity, Store& store) {
    SellerListing* listing = store.findListing(productId, getId());

    if (listing == nullptr) {
        throw std::out_of_range("Listing not found");
    }

    listing->restock(quantity);
}


void Seller::displayListings(const Store& store) const {
    store.displaySellerListings(getId());
}