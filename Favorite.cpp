#include "Favorite.h"
#include "Product.h"
#include "Cart.h"
#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

// 收藏记录，只保存用户名和商品编号
struct FavoriteItem {
    string userName;
    int productId;
};

// 读取收藏文件
vector<FavoriteItem> readFavorite() {
    vector<FavoriteItem> fs;
    ifstream fin("favorites.txt");
    FavoriteItem f;
    while (fin >> f.userName >> f.productId) {
        fs.push_back(f);
    }
    fin.close();
    return fs;
}

// 保存收藏文件
void saveFavorite(vector<FavoriteItem> fs) {
    ofstream fout("favorites.txt");
    for (int i = 0; i < (int)fs.size(); i++) {
        fout << fs[i].userName << " " << fs[i].productId << endl;
    }
    fout.close();
}

// 用户输入商品编号进行收藏
void addFavorite(string userName) {
    int id;
    cout << "请输入要收藏的商品编号: ";
    cin >> id;
    addFavoriteById(userName, id);
}

// 已经知道商品编号时直接收藏
void addFavoriteById(string userName, int id) {
    vector<FavoriteItem> fs = readFavorite();
    if (findProductById(id).id == -1) {
        cout << "没有这个商品。" << endl;
        return;
    }
    for (int i = 0; i < (int)fs.size(); i++) {
        if (fs[i].userName == userName && fs[i].productId == id) {
            cout << "已经收藏过了。" << endl;
            return;
        }
    }
    FavoriteItem f;
    f.userName = userName;
    f.productId = id;
    fs.push_back(f);
    saveFavorite(fs);
    cout << "收藏成功。" << endl;
}

// 取消收藏
void deleteFavorite(string userName) {
    vector<FavoriteItem> fs = readFavorite();
    int id;
    cout << "请输入要取消收藏的商品编号: ";
    cin >> id;
    for (int i = 0; i < (int)fs.size(); i++) {
        if (fs[i].userName == userName && fs[i].productId == id) {
            fs.erase(fs.begin() + i);
            saveFavorite(fs);
            cout << "已取消收藏。" << endl;
            return;
        }
    }
    cout << "收藏夹里没有这个商品。" << endl;
}

// 查看当前用户的收藏列表
void showFavorite(string userName) {
    vector<FavoriteItem> fs = readFavorite();
    cout << "====== 我的收藏 ======" << endl;
    for (int i = 0; i < (int)fs.size(); i++) {
        if (fs[i].userName == userName) {
            Product p = findProductById(fs[i].productId);
            if (p.id != -1) {
                cout << p.id << " " << p.name << " " << p.price << endl;
            }
        }
    }
}

// 从收藏列表中选择商品加入购物车
void favoriteToCart(string userName) {
    showFavorite(userName);
    cout << "下面进入加入购物车操作。" << endl;
    addCart(userName);
}
