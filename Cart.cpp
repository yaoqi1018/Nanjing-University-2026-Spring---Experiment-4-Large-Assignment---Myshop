#include "Cart.h"
#include "Product.h"
#include <iostream>
#include <fstream>
#include <limits>

using namespace std;

// 读取购物车文件，每条记录属于某一个用户
vector<CartItem> readCart() {
    vector<CartItem> carts;
    ifstream fin("cart.txt");
    CartItem c;
    while (fin >> c.userName >> c.productId >> c.count) {
        carts.push_back(c);
    }
    fin.close();
    return carts;
}

// 保存购物车内容
void saveCart(vector<CartItem> carts) {
    ofstream fout("cart.txt");
    for (int i = 0; i < (int)carts.size(); i++) {
        fout << carts[i].userName << " " << carts[i].productId << " "
             << carts[i].count << endl;
    }
    fout.close();
}

// 用户手动输入商品编号加入购物车
void addCart(string userName) {
    int id;
    cout << "商品编号: ";
    while (!(cin >> id)) {
        cout << "输入无效，请重新输入商品编号: ";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
    addCartById(userName, id);
}

// 已经知道商品编号时，直接把该商品加入购物车
void addCartById(string userName, int id) {
    vector<CartItem> carts = readCart();
    int count;
    Product p = findProductById(id);
    if (p.id == -1) {
        cout << "没有这个商品。" << endl;
        return;
    }
    cout << "购买数量: ";
    while (!(cin >> count)) {
        cout << "输入无效，请重新输入数量: ";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
    if (count <= 0) {
        cout << "购买数量必须大于0。" << endl;
        return;
    }
    if (count > p.num) {
        cout << "库存不足，当前库存为 " << p.num << " 件。" << endl;
        return;
    }

    for (int i = 0; i < (int)carts.size(); i++) {
        if (carts[i].userName == userName && carts[i].productId == id) {
            carts[i].count += count;
            saveCart(carts);
            cout << "已经加入购物车。" << endl;
            return;
        }
    }

    CartItem c;
    c.userName = userName;
    c.productId = id;
    c.count = count;
    carts.push_back(c);
    saveCart(carts);
    cout << "已经加入购物车。" << endl;
}

// 查看当前用户自己的购物车
void showCart(string userName) {
    vector<CartItem> carts = readCart();
    double sum = 0;
    cout << "====== 我的购物车 ======" << endl;
    for (int i = 0; i < (int)carts.size(); i++) {
        if (carts[i].userName == userName) {
            Product p = findProductById(carts[i].productId);
            if (p.id != -1) {
                cout << p.id << " " << p.name << " 单价:" << p.price
                     << " 数量:" << carts[i].count
                     << " 小计:" << p.price * carts[i].count << endl;
                sum += p.price * carts[i].count;
            }
        }
    }
    cout << "总价: " << sum << endl;
}

// 修改购物车中某个商品的数量
void changeCart(string userName) {
    vector<CartItem> carts = readCart();
    int id, count;
    cout << "要修改的商品编号: ";
    while (!(cin >> id)) {
        cout << "输入无效，请重新输入编号: ";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
    cout << "新的数量: ";
    while (!(cin >> count)) {
        cout << "输入无效，请重新输入数量: ";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
    if (count <= 0) {
        cout << "数量必须大于0。" << endl;
        return;
    }
    for (int i = 0; i < (int)carts.size(); i++) {
        if (carts[i].userName == userName && carts[i].productId == id) {
            carts[i].count = count;
            saveCart(carts);
            cout << "修改成功。" << endl;
            return;
        }
    }
    cout << "购物车里没有这个商品。" << endl;
}

// 删除购物车中的某个商品
void deleteCart(string userName) {
    vector<CartItem> carts = readCart();
    int id;
    cout << "要删除的商品编号: ";
    cin >> id;
    for (int i = 0; i < (int)carts.size(); i++) {
        if (carts[i].userName == userName && carts[i].productId == id) {
            carts.erase(carts.begin() + i);
            saveCart(carts);
            cout << "删除成功。" << endl;
            return;
        }
    }
    cout << "购物车里没有这个商品。" << endl;
}

// 下单成功后清空当前用户的购物车
void clearCart(string userName) {
    vector<CartItem> carts = readCart();
    for (int i = (int)carts.size() - 1; i >= 0; i--) {
        if (carts[i].userName == userName) {
            carts.erase(carts.begin() + i);
        }
    }
    saveCart(carts);
}
