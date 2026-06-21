#include "Product.h"
#include "History.h"
#include "Review.h"
#include "Order.h"
#include "Cart.h"
#include "Favorite.h"
#include <iostream>
#include <fstream>
#include <iomanip>
#include <limits>

using namespace std;

// 从 products.txt 读取商品列表
vector<Product> readProducts() {
    vector<Product> products;
    ifstream fin("products.txt");
    Product p;
    while (fin >> p.id >> p.name >> p.kind >> p.price >> p.num) {
        products.push_back(p);
    }
    fin.close();
    return products;
}

// 保存商品列表，修改库存、添加、删除商品后都会调用
void saveProducts(vector<Product> products) {
    ofstream fout("products.txt");
    for (int i = 0; i < (int)products.size(); i++) {
        fout << products[i].id << " " << products[i].name << " " << products[i].kind
             << " " << products[i].price << " " << products[i].num << endl;
    }
    fout.close();
}

// 分页显示商品。普通用户浏览时可以直接购买、加购物车或收藏
void showProducts(string userName) {
    vector<Product> products = readProducts();
    if (products.size() == 0) {
        cout << "现在还没有商品。" << endl;
        return;
    }

    int page = 1;
    int pageSize = 5;
    int pageCount = ((int)products.size() + pageSize - 1) / pageSize;
    char ch;

    while (true) {
        int start = (page - 1) * pageSize;
        int end = start + pageSize;
        if (end > (int)products.size()) end = (int)products.size();

        cout << endl;
        cout << "====== 商品列表 第 " << page << " / " << pageCount << " 页 ======" << endl;
        cout << left << setw(8) << "编号" << setw(18) << "名称" << setw(12)
             << "分类" << setw(10) << "价格" << setw(8) << "库存" << endl;
        for (int i = start; i < end; i++) {
            cout << left << setw(8) << products[i].id << setw(18) << products[i].name
                 << setw(12) << products[i].kind << setw(10) << products[i].price
                 << setw(8) << products[i].num << endl;
        }

        if (userName == "") {
            cout << "n.下一页  p.上一页  j.跳页  q.返回" << endl;
        } else {
            cout << "n.下一页  p.上一页  j.跳页  b.购买  a.加购物车  f.收藏  q.返回" << endl;
        }
        cout << "请选择: ";
        cin >> ch;

        if (ch == 'n' || ch == 'N') {
            if (page < pageCount) page++;
            else cout << "已经是最后一页了。" << endl;
        } else if (ch == 'p' || ch == 'P') {
            if (page > 1) page--;
            else cout << "已经是第一页了。" << endl;
        } else if (ch == 'j' || ch == 'J') {
            int newPage;
            cout << "请输入页码(1-" << pageCount << "): ";
            while (!(cin >> newPage)) {
                cout << "输入无效，请输入数字页码: ";
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
            }
            if (newPage >= 1 && newPage <= pageCount) {
                page = newPage;
            } else {
                cout << "页码不正确。" << endl;
            }
        } else if ((ch == 'b' || ch == 'B') && userName != "") {
            buyNow(userName);
            products = readProducts();
            pageCount = ((int)products.size() + pageSize - 1) / pageSize;
            if (page > pageCount) page = pageCount;
        } else if ((ch == 'a' || ch == 'A') && userName != "") {
            int id;
            cout << "请输入要加入购物车的商品编号: ";
            while (!(cin >> id)) {
                cout << "输入无效，请重新输入商品编号: ";
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
            }
            addCartById(userName, id);
        } else if ((ch == 'f' || ch == 'F') && userName != "") {
            int id;
            cout << "请输入要收藏的商品编号: ";
            while (!(cin >> id)) {
                cout << "输入无效，请重新输入商品编号: ";
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
            }
            addFavoriteById(userName, id);
        } else if (ch == 'q' || ch == 'Q') {
            break;
        } else {
            cout << "输入错误。" << endl;
        }
    }
}

// 按商品名称关键字搜索
void searchProduct() {
    vector<Product> products = readProducts();
    string key;
    cout << "请输入商品名关键字: ";
    cin >> key;
    for (int i = 0; i < (int)products.size(); i++) {
        if (products[i].name.find(key) != string::npos) {
            cout << products[i].id << " " << products[i].name << " "
                 << products[i].kind << " " << products[i].price
                 << " 库存:" << products[i].num << endl;
        }
    }
}

// 按分类筛选商品
void searchKind() {
    vector<Product> products = readProducts();
    string kind;
    cout << "请输入分类: ";
    cin >> kind;
    for (int i = 0; i < (int)products.size(); i++) {
        if (products[i].kind == kind) {
            cout << products[i].id << " " << products[i].name << " "
                 << products[i].price << " 库存:" << products[i].num << endl;
        }
    }
}

// 按价格区间筛选商品。若用户把最低价和最高价输反，程序会自动交换
void searchPriceRange() {
    vector<Product> products = readProducts();
    double low, high;
    cout << "请输入最低价: ";
    while (!(cin >> low)) {
        cout << "输入无效，请重新输入最低价: ";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
    cout << "请输入最高价: ";
    while (!(cin >> high)) {
        cout << "输入无效，请重新输入最高价: ";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
    if (low > high) {
        double t = low;
        low = high;
        high = t;
    }
    cout << "====== 价格区间商品 ======" << endl;
    for (int i = 0; i < (int)products.size(); i++) {
        if (products[i].price >= low && products[i].price <= high) {
            cout << products[i].id << " " << products[i].name << " "
                 << products[i].kind << " " << products[i].price
                 << " 库存:" << products[i].num << endl;
        }
    }
}

// 先输入分类，再把该分类下的商品按价格从低到高排序显示
void sortProductByPrice() {
    vector<Product> products = readProducts();
    vector<Product> sameKind;
    vector<string> kinds;
    string kind;

    cout << "当前已有分类: ";
    for (int i = 0; i < (int)products.size(); i++) {
        bool exists = false;
        for (int j = 0; j < (int)kinds.size(); j++) {
            if (kinds[j] == products[i].kind) {
                exists = true;
                break;
            }
        }
        if (!exists) {
            kinds.push_back(products[i].kind);
            cout << products[i].kind << " ";
        }
    }
    cout << endl;
    cout << "请输入要排序的商品分类: ";
    cin >> kind;

    for (int i = 0; i < (int)products.size(); i++) {
        if (products[i].kind == kind) {
            sameKind.push_back(products[i]);
        }
    }

    if (sameKind.size() == 0) {
        cout << "没有找到这个分类的商品。" << endl;
        return;
    }

    // 使用简单冒泡排序，便于课程展示和讲解
    for (int i = 0; i < (int)sameKind.size(); i++) {
        for (int j = 0; j + 1 < (int)sameKind.size() - i; j++) {
            if (sameKind[j].price > sameKind[j + 1].price) {
                Product t = sameKind[j];
                sameKind[j] = sameKind[j + 1];
                sameKind[j + 1] = t;
            }
        }
    }
    cout << "====== " << kind << " 类商品价格排序（从低到高） ======" << endl;
    for (int i = 0; i < (int)sameKind.size(); i++) {
        cout << sameKind[i].id << " " << sameKind[i].name << " "
             << sameKind[i].kind << " " << sameKind[i].price
             << " 库存:" << sameKind[i].num << endl;
    }
}

// 查看商品详情，同时记录浏览历史，并提供购买、加购物车、收藏操作
void showOneProduct(string userName) {
    int id;
    cout << "请输入商品编号: ";
    while (!(cin >> id)) {
        cout << "输入无效，请重新输入商品编号: ";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
    Product p = findProductById(id);
    if (p.id == -1) {
        cout << "没有找到这个商品。" << endl;
        return;
    }
    cout << "商品编号: " << p.id << endl;
    cout << "商品名称: " << p.name << endl;
    cout << "商品分类: " << p.kind << endl;
    cout << "价格: " << p.price << endl;
    cout << "库存: " << p.num << endl;
    addHistory(userName, p.id, p.name);
    showProductReviews(p.id);

    while (true) {
        int choose;
        cout << endl;
        cout << "1. 直接购买这个商品" << endl;
        cout << "2. 加入购物车" << endl;
        cout << "3. 收藏这个商品" << endl;
        cout << "0. 返回" << endl;
        cout << "请选择: ";
        while (!(cin >> choose)) {
            cout << "输入无效，请重新选择: ";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }

        if (choose == 1) {
            buyNowById(userName, p.id);
            p = findProductById(id);
            cout << "当前库存: " << p.num << endl;
        } else if (choose == 2) {
            addCartById(userName, p.id);
        } else if (choose == 3) {
            addFavoriteById(userName, p.id);
        } else if (choose == 0) {
            break;
        } else {
            cout << "输入错误。" << endl;
        }
    }
}

// 下单时扣减库存，库存不足时返回 false
bool reduceProductNum(int id, int count) {
    if (count <= 0) return false;
    vector<Product> products = readProducts();
    for (int i = 0; i < (int)products.size(); i++) {
        if (products[i].id == id) {
            if (products[i].num < count) return false;
            products[i].num -= count;
            saveProducts(products);
            return true;
        }
    }
    return false;
}

// 退款成功时恢复商品库存
void addProductNum(int id, int count) {
    vector<Product> products = readProducts();
    for (int i = 0; i < (int)products.size(); i++) {
        if (products[i].id == id) {
            products[i].num += count;
            saveProducts(products);
            return;
        }
    }
}

// 根据商品编号查找商品，找不到时返回 id 为 -1 的商品
Product findProductById(int id) {
    vector<Product> products = readProducts();
    for (int i = 0; i < (int)products.size(); i++) {
        if (products[i].id == id) return products[i];
    }
    Product p;
    p.id = -1;
    return p;
}

// 管理员添加新商品，编号自动增加
void addProduct() {
    vector<Product> products = readProducts();
    Product p;
    int maxId = 1000;
    for (int i = 0; i < (int)products.size(); i++) {
        if (products[i].id > maxId) maxId = products[i].id;
    }
    p.id = maxId + 1;
    cout << "商品名称: ";
    cin >> p.name;
    if (p.name.empty()) {
        cout << "商品名称不能为空。" << endl;
        return;
    }
    cout << "商品分类: ";
    cin >> p.kind;
    if (p.kind.empty()) {
        cout << "商品分类不能为空。" << endl;
        return;
    }
    cout << "商品价格: ";
    while (!(cin >> p.price)) {
        cout << "输入无效，请重新输入价格: ";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
    if (p.price <= 0) {
        cout << "商品价格必须大于0。" << endl;
        return;
    }
    cout << "商品库存: ";
    while (!(cin >> p.num)) {
        cout << "输入无效，请重新输入库存: ";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
    if (p.num < 0) {
        cout << "商品库存不能为负数。" << endl;
        return;
    }
    products.push_back(p);
    saveProducts(products);
    cout << "添加成功，商品编号是 " << p.id << endl;
}

// 管理员修改商品名称、分类、价格和库存
void changeProduct() {
    vector<Product> products = readProducts();
    int id;
    cout << "请输入要修改的商品编号: ";
    while (!(cin >> id)) {
        cout << "输入无效，请重新输入编号: ";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
    for (int i = 0; i < (int)products.size(); i++) {
        if (products[i].id == id) {
            cout << "新名称: ";
            cin >> products[i].name;
            if (products[i].name.empty()) {
                cout << "名称不能为空，修改取消。" << endl;
                return;
            }
            cout << "新分类: ";
            cin >> products[i].kind;
            if (products[i].kind.empty()) {
                cout << "分类不能为空，修改取消。" << endl;
                return;
            }
            cout << "新价格: ";
            while (!(cin >> products[i].price)) {
                cout << "输入无效，请重新输入价格: ";
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
            }
            if (products[i].price <= 0) {
                cout << "价格必须大于0，修改取消。" << endl;
                return;
            }
            cout << "新库存: ";
            while (!(cin >> products[i].num)) {
                cout << "输入无效，请重新输入库存: ";
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
            }
            if (products[i].num < 0) {
                cout << "库存不能为负数，修改取消。" << endl;
                return;
            }
            saveProducts(products);
            cout << "修改成功。" << endl;
            return;
        }
    }
    cout << "没有找到这个商品。" << endl;
}

// 管理员按商品编号删除商品
void deleteProduct() {
    vector<Product> products = readProducts();
    int id;
    cout << "请输入要删除的商品编号: ";
    cin >> id;
    for (int i = 0; i < (int)products.size(); i++) {
        if (products[i].id == id) {
            products.erase(products.begin() + i);
            saveProducts(products);
            cout << "删除成功。" << endl;
            return;
        }
    }
    cout << "没有找到这个商品。" << endl;
}
