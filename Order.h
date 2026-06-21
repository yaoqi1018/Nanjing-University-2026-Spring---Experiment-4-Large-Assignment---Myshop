#ifndef ORDER_H
#define ORDER_H

#include <string>
#include <vector>

using namespace std;

struct Order {
    int orderId;          // 订单号
    string userName;      // 下单用户
    int productId;        // 商品编号
    string productName;   // 商品名称，保存一份方便订单展示
    int count;            // 购买数量
    double money;         // 实付金额
    string status;        // 订单状态：已支付、已发货、已收货、退款中等
    string logistics;     // 物流信息
};

vector<Order> readOrders();
void saveOrders(vector<Order> orders);
void makeOrder(string userName);
void buyNow(string userName);
void buyNowById(string userName, int id);
void showMyOrders(string userName);
void showAllOrders();
void changeOrderStatus();
void confirmReceive(string userName);
void changeLogistics();
bool hasBought(string userName, int productId);
void applyRefund(string userName);
void checkRefund();

#endif
