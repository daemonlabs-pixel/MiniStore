
#ifndef SHOPPINGCART_H
#define SHOPPINGCART_H

#include <vector>
#include <cstddef>

#include "CartItem.h"

// Forward declarations: the full definitions are needed in ShoppingCart.cpp.
class Store;
class AccountManager;

class ShoppingCart {
private:
    std::vector<CartItem> items;

public:
    // Adds an item after validating its product, listing, activity and stock.
    void addItem(const CartItem& item, const Store& store);

    // Removes one specific seller's offer from the cart.
    bool removeItem(int productId, int sellerId);

    // Removes every item belonging to a particular seller.
    bool removeSellerItems(int sellerId);

    // Removes every seller's offer for a particular product.
    bool removeProductItems(int productId);

    // Sets the quantity to a specific value. Zero removes the item.
    bool updateQuantity(int productId, int sellerId,
                        int newQuantity, const Store& store);

    // Adds to the current quantity, subject to available stock.
    bool increaseQuantity(int productId, int sellerId,
                          int amount, const Store& store);

    // Subtracts from the current quantity. Reaching zero removes the item.
    bool decreaseQuantity(int productId, int sellerId, int amount);

    // Returns a pointer to the item, or nullptr if it doesn't exist.
    CartItem* findItem(int productId, int sellerId);
    const CartItem* findItem(int productId, int sellerId) const;

    // Displays item details and the cart's total information.
    void displayCart(const Store& store, AccountManager& manager) const;
    void displayTotals(const Store& store) const;

    // Clears the cart. Returns true if it previously contained items.
    bool clear();

    // Calculates the total price of all cart items.
    double getTotalPrice(const Store& store) const;

    // Calculates the sum of all item quantities.
    std::size_t getTotalItems() const;
};

#endif
