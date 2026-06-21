#ifndef PRODUCT_H
#define PRODUCT_H

#include <string>
#include <vector>

using namespace std;

struct Product {
    int id;          // 商品编号
    string name;     // 商品名称
    string kind;     // 商品分类
    double price;    // 商品单价
    int num;         // 商品库存
};

vector<Product> readProducts();
void saveProducts(vector<Product> products);
void showProducts(string userName = "");
void searchProduct();
void searchKind();
void searchPriceRange();
void sortProductByPrice();
void showOneProduct(string userName);
bool reduceProductNum(int id, int count);
void addProductNum(int id, int count);
Product findProductById(int id);
void addProduct();
void changeProduct();
void deleteProduct();

#endif
