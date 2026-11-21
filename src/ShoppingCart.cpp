
#include <iostream>
#include <stdexcept>
#include <algorithm>
#include <string>
#include <iomanip>
#include <limits>

#include "ShoppingCart.h"
#include "Store.h"
#include "AccountManager.h"


// Adds a new item or increases the quantity of an existing one.
void ShoppingCart::addItem(const CartItem& item, const Store& store) {

    // Check whether the product exists.
    if (store.findProduct(item.getProductId()) == nullptr) {
        throw std::invalid_argument("The product doesn't exist.");
    }

    // Retrieve the seller's listing.
    const SellerListing* listing =
        store.findListing(item.getProductId(), item.getSellerId());

    if (listing == nullptr) {
        throw std::invalid_argument("No listing was found for this product and seller.");
    }

    // An inactive listing cannot be added to the cart.
    if (!listing->IsActive()) {
        throw std::runtime_error("This listing is currently inactive.");
    }

    // Find out whether this item is already in the cart.
    CartItem* existingItem =
        findItem(item.getProductId(), item.getSellerId());

    int currentQuantity = 0;

    if (existingItem != nullptr) {
        currentQuantity = existingItem->getQuantity();
    }

    // Ensure the requested quantity doesn't exceed available stock.
    if (currentQuantity > listing->getStock() ||
        item.getQuantity() > listing->getStock() - currentQuantity) {
        throw std::out_of_range("Not enough stock available.");
    }

    // Avoid creating duplicate entries for the same seller and product.
    if (existingItem != nullptr) {
        existingItem->increaseQuantity(item.getQuantity());
    }
    else {
        items.push_back(item);
    }
}


// Removes one specific cart item.
bool ShoppingCart::removeItem(int productId, int sellerId) {

    if (productId <= 0 || sellerId <= 0) {
        throw std::invalid_argument("Invalid ID.");
    }

    for (auto it = items.begin(); it != items.end(); ++it) {

        if (it->getProductId() == productId &&
            it->getSellerId() == sellerId) {

            items.erase(it);
            return true;
        }
    }

    return false;
}


// Removes every item belonging to a seller.
bool ShoppingCart::removeSellerItems(int sellerId) {

    if (sellerId <= 0) {
        throw std::invalid_argument("Invalid seller ID.");
    }

    auto newEnd = std::remove_if(
        items.begin(), items.end(),
        [sellerId](const CartItem& item) {
            return item.getSellerId() == sellerId;
        }
    );

    // Check whether anything was actually removed.
    bool removed = newEnd != items.end();

    items.erase(newEnd, items.end());

    return removed;
}


// Removes every listing associated with a product.
bool ShoppingCart::removeProductItems(int productId) {

    if (productId <= 0) {
        throw std::invalid_argument("Invalid product ID.");
    }

    auto newEnd = std::remove_if(
        items.begin(), items.end(),
        [productId](const CartItem& item) {
            return item.getProductId() == productId;
        }
    );

    bool removed = newEnd != items.end();

    items.erase(newEnd, items.end());

    return removed;
}


// Sets the quantity to a specific value.
bool ShoppingCart::updateQuantity(
    int productId, int sellerId, int newQuantity, const Store& store) {

    if (productId <= 0 || sellerId <= 0) {
        throw std::invalid_argument("Invalid ID.");
    }

    if (newQuantity < 0) {
        throw std::invalid_argument("Quantity cannot be negative.");
    }

    CartItem* item = findItem(productId, sellerId);

    if (item == nullptr) {
        return false;
    }

    // A quantity of zero means removing the item.
    if (newQuantity == 0) {
        return removeItem(productId, sellerId);
    }

    const SellerListing* listing = store.findListing(productId, sellerId);

    if (listing == nullptr || !listing->IsActive()) {
        throw std::runtime_error("This listing is no longer available.");
    }

    if (newQuantity > listing->getStock()) {
        throw std::out_of_range("Not enough stock available.");
    }

    item->setQuantity(newQuantity);

    return true;
}


// Increases the quantity of an existing cart item.
bool ShoppingCart::increaseQuantity(
    int productId, int sellerId, int amount, const Store& store) {

    if (productId <= 0 || sellerId <= 0) {
        throw std::invalid_argument("Invalid ID.");
    }

    if (amount <= 0) {
        throw std::invalid_argument("Increase amount must be positive.");
    }

    CartItem* item = findItem(productId, sellerId);

    if (item == nullptr) {
        return false;
    }

    const SellerListing* listing = store.findListing(productId, sellerId);

    if (listing == nullptr || !listing->IsActive()) {
        throw std::runtime_error("This listing is no longer available.");
    }

    int currentQuantity = item->getQuantity();

    if (currentQuantity > listing->getStock() ||
        amount > listing->getStock() - currentQuantity) {
        throw std::out_of_range("Not enough stock available.");
    }

    item->increaseQuantity(amount);

    return true;
}


// Decreases the quantity of an existing cart item.
bool ShoppingCart::decreaseQuantity(int productId, int sellerId, int amount) {

    if (productId <= 0 || sellerId <= 0) {
        throw std::invalid_argument("Invalid ID.");
    }

    if (amount <= 0) {
        throw std::invalid_argument("Decrease amount must be positive.");
    }

    CartItem* item = findItem(productId, sellerId);

    if (item == nullptr) {
        return false;
    }

    int currentQuantity = item->getQuantity();

    if (amount > currentQuantity) {
        throw std::out_of_range("Cannot decrease by more than the current quantity.");
    }

    // Remove the item if the decrease reaches zero.
    if (amount == currentQuantity) {
        return removeItem(productId, sellerId);
    }

    item->decreaseQuantity(amount);

    return true;
}


// Finds an item using both product ID and seller ID.
CartItem* ShoppingCart::findItem(int productId, int sellerId) {

    if (productId <= 0 || sellerId <= 0) {
        throw std::invalid_argument("Invalid ID.");
    }

    for (auto& item : items) {

        if (item.getProductId() == productId &&
            item.getSellerId() == sellerId) {
            return &item;
        }
    }

    return nullptr;
}


// Const version of findItem.
const CartItem* ShoppingCart::findItem(int productId, int sellerId) const {

    if (productId <= 0 || sellerId <= 0) {
        throw std::invalid_argument("Invalid ID.");
    }

    for (const auto& item : items) {

        if (item.getProductId() == productId &&
            item.getSellerId() == sellerId) {
            return &item;
        }
    }

    return nullptr;
}


// Displays the contents of the shopping cart.
void ShoppingCart::displayCart(
    const Store& store, AccountManager& manager) const {

    if (items.empty()) {
        std::cout << "Your shopping cart is empty.\n";
        return;
    }

    for (const auto& item : items) {

        const Product* product =
            store.findProduct(item.getProductId());

        const SellerListing* listing =
            store.findListing(item.getProductId(), item.getSellerId());

        Account* seller =
            manager.findAccount(item.getSellerId());

        std::cout << "\n------------------------------\n";

        // Display product information.
        if (product != nullptr) {
            std::cout << "Product: " << product->getName() << '\n';
        }
        else {
            std::cout << "Product: Unknown product\n";
        }

        // Display seller information.
        if (seller != nullptr) {
            std::cout << "Seller: " << seller->getUsername() << '\n';
        }
        else {
            std::cout << "Seller: Unknown seller\n";
        }

        std::cout << "Quantity: " << item.getQuantity() << '\n';

        // Display listing information.
        if (listing != nullptr) {

            double price = listing->getPrice();

            std::cout << std::fixed << std::setprecision(2);

            std::cout << "Unit price: " << price << '\n';
            std::cout << "Subtotal: "
                      << price * item.getQuantity() << '\n';

            if (!listing->IsActive()) {
                std::cout << "Availability: Inactive\n";
            }
            else if (item.getQuantity() > listing->getStock()) {
                std::cout << "Availability: Insufficient stock\n";
            }
            else {
                std::cout << "Availability: Available\n";
            }
        }
        else {
            std::cout << "Listing: No longer available\n";
        }
    }

    std::cout << "\n------------------------------\n";

    displayTotals(store);
}


// Displays the total quantity and total price.
void ShoppingCart::displayTotals(const Store& store) const {

    std::cout << "\n========== CART TOTALS ==========\n";
    std::cout << "Total items: " << getTotalItems() << '\n';

    try {
        std::cout << std::fixed << std::setprecision(2);
        std::cout << "Total price: " << getTotalPrice(store) << '\n';
    }
    catch (const std::runtime_error& e) {
        std::cout << "Total price unavailable: " << e.what() << '\n';
    }
}

// Clears all items from the cart.
bool ShoppingCart::clear() {

    bool hadItems = !items.empty();

    items.clear();

    return hadItems;
}


// Calculates the total price of the cart.
double ShoppingCart::getTotalPrice(const Store& store) const {

    double totalPrice = 0.0;

    for (const auto& item : items) {

        const SellerListing* listing =
            store.findListing(item.getProductId(), item.getSellerId());

        if (listing == nullptr) {
            throw std::runtime_error(
                "Cannot calculate total: a listing is no longer available."
            );
        }

        totalPrice += listing->getPrice() * item.getQuantity();
    }

    return totalPrice;
}


// Calculates the sum of all quantities in the cart.
std::size_t ShoppingCart::getTotalItems() const {

    std::size_t totalItems = 0;

    for (const auto& item : items) {
        totalItems += static_cast<std::size_t>(item.getQuantity());
    }

    return totalItems;
}
