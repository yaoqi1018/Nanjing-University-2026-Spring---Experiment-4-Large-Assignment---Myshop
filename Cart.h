#ifndef CART_H
#define CART_H

#include <string>
#include <vector>

using namespace std;

struct CartItem {
    string userName;
    int productId;
    int count;
};

vector<CartItem> readCart();
void saveCart(vector<CartItem> carts);
void addCart(string userName);
void addCartById(string userName, int id);
void showCart(string userName);
void changeCart(string userName);
void deleteCart(string userName);
void clearCart(string userName);

#endif
