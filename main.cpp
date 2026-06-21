#include <iostream>
#include <limits>
#include "User.h"
#include "Product.h"
#include "Cart.h"
#include "Order.h"
#include "History.h"
#include "Favorite.h"
#include "Review.h"

#ifdef _WIN32
#include <windows.h>
#endif

using namespace std;

// 清屏，让每次进入新菜单时界面更干净
void clearScreen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

// 安全读取整数，防止输入非数字导致程序死循环
int readInt() {
    int value;
    while (!(cin >> value)) {
        cout << "输入无效，请重新输入数字: ";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
    return value;
}

// 安全读取浮点数，目前保留给后续输入价格时使用
double readDouble() {
    double value;
    while (!(cin >> value)) {
        cout << "输入无效，请重新输入数字: ";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
    return value;
}

void userMenu(string userName);
void adminMenu();
void productMenu(string userName);
void cartMenu(string userName);
void orderMenu(string userName);
void favoriteMenu(string userName);
void myCenterMenu(string userName);
void adminProductMenu();
void adminOrderMenu();
void adminUserMenu();
void adminReviewMenu();

// 暂停一下，方便用户看清本次操作结果
void waitEnter() {
    cout << endl << "按回车键继续...";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cin.get();
}

int main() {
#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
#endif
    int choose;
    while (true) {
        clearScreen();
        cout << endl;
        cout << "====== 我的网购小店 ======" << endl;
        cout << "1. 用户注册" << endl;
        cout << "2. 用户登录" << endl;
        cout << "3. 管理员登录" << endl;
        cout << "0. 退出系统" << endl;
        cout << "请选择: ";
        choose = readInt();

        if (choose == 1) {
            registerUser();
        } else if (choose == 2) {
            string userName;
            if (userLogin(userName)) userMenu(userName);
        } else if (choose == 3) {
            if (adminLogin()) adminMenu();
        } else if (choose == 0) {
            cout << "欢迎下次再来。" << endl;
            break;
        } else {
            cout << "输入错误。" << endl;
            waitEnter();
        }
    }
    return 0;
}

// 普通用户登录后的一级菜单
void userMenu(string userName) {
    int choose;
    while (true) {
        clearScreen();
        cout << endl;
        cout << "====== 用户功能区 ======" << endl;
        cout << "1. 商品功能" << endl;
        cout << "2. 购物车功能" << endl;
        cout << "3. 订单功能" << endl;
        cout << "4. 收藏功能" << endl;
        cout << "5. 个人中心" << endl;
        cout << "0. 退出登录" << endl;
        cout << "请选择: ";
        choose = readInt();

        if (choose == 1) productMenu(userName);
        else if (choose == 2) cartMenu(userName);
        else if (choose == 3) orderMenu(userName);
        else if (choose == 4) favoriteMenu(userName);
        else if (choose == 5) myCenterMenu(userName);
        else if (choose == 0) break;
        else {
            cout << "输入错误。" << endl;
            waitEnter();
        }
    }
}

// 商品功能子菜单
void productMenu(string userName) {
    int choose;
    while (true) {
        clearScreen();
        cout << endl;
        cout << "====== 商品功能 ======" << endl;
        cout << "1. 查看所有商品" << endl;
        cout << "2. 按名称搜索商品" << endl;
        cout << "3. 按分类筛选商品" << endl;
        cout << "4. 按价格区间筛选商品" << endl;
        cout << "5. 按分类进行价格排序" << endl;
        cout << "6. 查看商品详情和评价" << endl;
        cout << "7. 直接购买商品" << endl;
        cout << "8. 写商品评价" << endl;
        cout << "0. 返回上一级" << endl;
        cout << "请选择: ";
        choose = readInt();

        if (choose == 1) { showProducts(userName); waitEnter(); }
        else if (choose == 2) { searchProduct(); waitEnter(); }
        else if (choose == 3) { searchKind(); waitEnter(); }
        else if (choose == 4) { searchPriceRange(); waitEnter(); }
        else if (choose == 5) { sortProductByPrice(); waitEnter(); }
        else if (choose == 6) { showOneProduct(userName); waitEnter(); }
        else if (choose == 7) { buyNow(userName); waitEnter(); }
        else if (choose == 8) { addReview(userName); waitEnter(); }
        else if (choose == 0) break;
        else { cout << "输入错误。" << endl; waitEnter(); }
    }
}

// 购物车功能子菜单
void cartMenu(string userName) {
    int choose;
    while (true) {
        clearScreen();
        cout << endl;
        cout << "====== 购物车功能 ======" << endl;
        cout << "1. 加入购物车" << endl;
        cout << "2. 查看购物车" << endl;
        cout << "3. 修改商品数量" << endl;
        cout << "4. 删除购物车商品" << endl;
        cout << "5. 购物车下单" << endl;
        cout << "0. 返回上一级" << endl;
        cout << "请选择: ";
        choose = readInt();

        if (choose == 1) { addCart(userName); waitEnter(); }
        else if (choose == 2) { showCart(userName); waitEnter(); }
        else if (choose == 3) { changeCart(userName); waitEnter(); }
        else if (choose == 4) { deleteCart(userName); waitEnter(); }
        else if (choose == 5) { makeOrder(userName); waitEnter(); }
        else if (choose == 0) break;
        else { cout << "输入错误。" << endl; waitEnter(); }
    }
}

// 订单功能子菜单
void orderMenu(string userName) {
    int choose;
    while (true) {
        clearScreen();
        cout << endl;
        cout << "====== 订单功能 ======" << endl;
        cout << "1. 查看我的订单" << endl;
        cout << "2. 申请退款" << endl;
        cout << "3. 确认收货" << endl;
        cout << "0. 返回上一级" << endl;
        cout << "请选择: ";
        choose = readInt();

        if (choose == 1) { showMyOrders(userName); waitEnter(); }
        else if (choose == 2) { applyRefund(userName); waitEnter(); }
        else if (choose == 3) { confirmReceive(userName); waitEnter(); }
        else if (choose == 0) break;
        else { cout << "输入错误。" << endl; waitEnter(); }
    }
}

// 收藏功能子菜单
void favoriteMenu(string userName) {
    int choose;
    while (true) {
        clearScreen();
        cout << endl;
        cout << "====== 收藏功能 ======" << endl;
        cout << "1. 收藏商品" << endl;
        cout << "2. 查看收藏" << endl;
        cout << "3. 取消收藏" << endl;
        cout << "4. 从收藏加入购物车" << endl;
        cout << "0. 返回上一级" << endl;
        cout << "请选择: ";
        choose = readInt();

        if (choose == 1) { addFavorite(userName); waitEnter(); }
        else if (choose == 2) { showFavorite(userName); waitEnter(); }
        else if (choose == 3) { deleteFavorite(userName); waitEnter(); }
        else if (choose == 4) { favoriteToCart(userName); waitEnter(); }
        else if (choose == 0) break;
        else { cout << "输入错误。" << endl; waitEnter(); }
    }
}

// 个人中心子菜单
void myCenterMenu(string userName) {
    int choose;
    while (true) {
        clearScreen();
        cout << endl;
        cout << "====== 个人中心 ======" << endl;
        cout << "1. 查看个人信息和会员等级" << endl;
        cout << "2. 修改密码" << endl;
        cout << "3. 查看浏览历史" << endl;
        cout << "4. 修改收货地址" << endl;
        cout << "5. 充值支付余额" << endl;
        cout << "0. 返回上一级" << endl;
        cout << "请选择: ";
        choose = readInt();

        if (choose == 1) { showMyInfo(userName); waitEnter(); }
        else if (choose == 2) { changeMyPassword(userName); waitEnter(); }
        else if (choose == 3) { showHistory(userName); waitEnter(); }
        else if (choose == 4) { changeMyAddress(userName); waitEnter(); }
        else if (choose == 5) { rechargeBalance(userName); waitEnter(); }
        else if (choose == 0) break;
        else { cout << "输入错误。" << endl; waitEnter(); }
    }
}

// 管理员登录后的一级菜单
void adminMenu() {
    int choose;
    while (true) {
        clearScreen();
        cout << endl;
        cout << "====== 管理员功能区 ======" << endl;
        cout << "1. 商品管理" << endl;
        cout << "2. 订单管理" << endl;
        cout << "3. 用户管理" << endl;
        cout << "4. 评价管理" << endl;
        cout << "5. 退款审核" << endl;
        cout << "0. 退出登录" << endl;
        cout << "请选择: ";
        choose = readInt();

        if (choose == 1) adminProductMenu();
        else if (choose == 2) adminOrderMenu();
        else if (choose == 3) adminUserMenu();
        else if (choose == 4) adminReviewMenu();
        else if (choose == 5) checkRefund();
        else if (choose == 0) break;
        else { cout << "输入错误。" << endl; waitEnter(); }
    }
}

// 管理员商品管理子菜单
void adminProductMenu() {
    int choose;
    while (true) {
        clearScreen();
        cout << endl;
        cout << "====== 商品管理 ======" << endl;
        cout << "1. 查看商品" << endl;
        cout << "2. 添加商品" << endl;
        cout << "3. 修改商品" << endl;
        cout << "4. 删除商品" << endl;
        cout << "0. 返回上一级" << endl;
        cout << "请选择: ";
        choose = readInt();

        if (choose == 1) { showProducts(); waitEnter(); }
        else if (choose == 2) { addProduct(); waitEnter(); }
        else if (choose == 3) { changeProduct(); waitEnter(); }
        else if (choose == 4) { deleteProduct(); waitEnter(); }
        else if (choose == 0) break;
        else { cout << "输入错误。" << endl; waitEnter(); }
    }
}

// 管理员订单管理子菜单
void adminOrderMenu() {
    int choose;
    while (true) {
        clearScreen();
        cout << endl;
        cout << "====== 订单管理 ======" << endl;
        cout << "1. 查看全部订单" << endl;
        cout << "2. 修改订单状态" << endl;
        cout << "3. 维护订单物流" << endl;
        cout << "0. 返回上一级" << endl;
        cout << "请选择: ";
        choose = readInt();

        if (choose == 1) { showAllOrders(); waitEnter(); }
        else if (choose == 2) { changeOrderStatus(); waitEnter(); }
        else if (choose == 3) { changeLogistics(); waitEnter(); }
        else if (choose == 0) break;
        else { cout << "输入错误。" << endl; waitEnter(); }
    }
}

// 管理员用户管理子菜单
void adminUserMenu() {
    int choose;
    while (true) {
        clearScreen();
        cout << endl;
        cout << "====== 用户管理 ======" << endl;
        cout << "1. 查看所有用户" << endl;
        cout << "2. 重置用户密码" << endl;
        cout << "0. 返回上一级" << endl;
        cout << "请选择: ";
        choose = readInt();

        if (choose == 1) { showAllUsers(); waitEnter(); }
        else if (choose == 2) { resetUserPassword(); waitEnter(); }
        else if (choose == 0) break;
        else { cout << "输入错误。" << endl; waitEnter(); }
    }
}

// 管理员评价管理子菜单
void adminReviewMenu() {
    int choose;
    while (true) {
        clearScreen();
        cout << endl;
        cout << "====== 评价管理 ======" << endl;
        cout << "1. 查看全部评价" << endl;
        cout << "2. 回复评价" << endl;
        cout << "3. 删除评价" << endl;
        cout << "0. 返回上一级" << endl;
        cout << "请选择: ";
        choose = readInt();

        if (choose == 1) { showAllReviews(); waitEnter(); }
        else if (choose == 2) { replyReview(); waitEnter(); }
        else if (choose == 3) { deleteReview(); waitEnter(); }
        else if (choose == 0) break;
        else { cout << "输入错误。" << endl; waitEnter(); }
    }
}
