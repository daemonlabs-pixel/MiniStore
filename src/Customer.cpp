
#include <iostream>
#include <stdexcept>

#include "Customer.h"
#include "Store.h"
#include "AccountManager.h"


// Constructs a Customer and initializes its Account base class.
Customer::Customer(int id, const std::string& username,
                   const std::string& password)
    : Account(id, username, password) {}


// Returns the role of this account.
std::string Customer::getRole() const {
    return "Customer";
}


// Adds an item to the customer's shopping cart.
// ShoppingCart handles product, listing, activity and stock validation.
void Customer::addItemToCart(const CartItem& item, const Store& store) {
    cart.addItem(item, store);
}


// Removes a specific item identified by its product and seller IDs.
bool Customer::removeItem(int productId, int sellerId) {
    return cart.removeItem(productId, sellerId);
}


// Removes all items belonging to a particular seller.
bool Customer::removeSellerItems(int sellerId) {
    return cart.removeSellerItems(sellerId);
}


// Removes all listings associated with a particular product.
bool Customer::removeProductItems(int productId) {
    return cart.removeProductItems(productId);
}


// Replaces the current quantity with a new quantity.
bool Customer::updateQuantity(int productId, int sellerId,
                              int quantity, const Store& store) {
    return cart.updateQuantity(productId, sellerId, quantity, store);
}


// Increases the quantity of an existing cart item.
bool Customer::increaseQuantity(int productId, int sellerId,
                                int amount, const Store& store) {
    return cart.increaseQuantity(productId, sellerId, amount, store);
}


// Decreases the quantity of an existing cart item.
bool Customer::decreaseQuantity(int productId, int sellerId, int amount) {
    return cart.decreaseQuantity(productId, sellerId, amount);
}


// Displays the complete shopping cart, including product and seller details.
void Customer::viewCart(const Store& store, AccountManager& manager) const {
    cart.displayCart(store, manager);
}


// Displays information about one specific cart item.
void Customer::viewItem(int productId, int sellerId) const {

    const CartItem* item = cart.findItem(productId, sellerId);

    // Check whether the requested item exists.
    if (item == nullptr) {
        std::cout << "Item not found in your cart.\n";
        return;
    }

    std::cout << item->toString() << '\n';
}


// Removes all items from the customer's cart.
void Customer::clearCart() {

    if (cart.clear()) {
        std::cout << "Your shopping cart was cleared successfully.\n";
    }
    else {
        std::cout << "Your shopping cart is already empty.\n";
    }
}
