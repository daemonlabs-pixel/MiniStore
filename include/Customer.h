#ifndef CUSTOMER_H
#define CUSTOMER_H

#include <string>

#include "Account.h"
#include "ShoppingCart.h"
#include <iostream>

class Store;
class AccountManager;

class Customer : public Account {
private:
    ShoppingCart cart;

public:
    Customer(int id, const std::string& username,
             const std::string& password);

    std::string getRole() const override;

    void addItemToCart(const CartItem& item, const Store& store);

    bool removeItem(int productId, int sellerId);
    bool removeSellerItems(int sellerId);
    bool removeProductItems(int productId);

    bool updateQuantity(int productId, int sellerId,
                        int quantity, const Store& store);
    bool increaseQuantity(int productId, int sellerId,
                          int amount, const Store& store);
    bool decreaseQuantity(int productId, int sellerId, int amount);

    void viewCart(const Store& store, AccountManager& manager) const;
    void viewItem(int productId, int sellerId) const;
    void clearCart();
};

#endif