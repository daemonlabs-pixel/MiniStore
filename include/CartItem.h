#ifndef CARTITEM_H
#define CARTITEM_H

#include <string>

class CartItem{
    private:
        int productId;
        int sellerId;
        int quantity;

    public:
        CartItem(int productId, int sellerId, int quantity);
        int getProductId() const;
        int getSellerId() const;
        int getQuantity() const;

        void setQuantity(int quantity);
        void increaseQuantity(int quantity);
        void decreaseQuantity(int quantity);

        std::string toString() const;
};


#endif