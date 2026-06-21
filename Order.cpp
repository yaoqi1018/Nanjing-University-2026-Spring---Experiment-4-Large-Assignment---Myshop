#include "Order.h"
#include "Cart.h"
#include "Product.h"
#include "User.h"
#include <iostream>
#include <fstream>
#include <limits>
#include <iomanip>
#include <sstream>

using namespace std;

// 从 orders.txt 读取所有订单
vector<Order> readOrders() {
    vector<Order> orders;
    ifstream fin("orders.txt");
    string line;
    while (getline(fin, line)) {
        if (line.size() == 0) continue;
        stringstream ss(line);
        Order o;
        // 兼容旧版 orders.txt：旧订单没有物流字段时，自动显示暂无物流
        o.logistics = "暂无物流";
        ss >> o.orderId >> o.userName >> o.productId >> o.productName
           >> o.count >> o.money >> o.status;
        if (ss >> o.logistics) {
        }
        orders.push_back(o);
    }
    fin.close();
    return orders;
}

// 保存订单列表
void saveOrders(vector<Order> orders) {
    ofstream fout("orders.txt");
    for (int i = 0; i < (int)orders.size(); i++) {
        fout << orders[i].orderId << " " << orders[i].userName << " "
             << orders[i].productId << " " << orders[i].productName << " "
             << orders[i].count << " " << orders[i].money << " "
             << orders[i].status << " " << orders[i].logistics << endl;
    }
    fout.close();
}

// 根据用户累计消费金额返回折扣
double getDiscount(string userName) {
    double money = getUserMoney(userName);
    if (money >= 1000) return 0.85;
    if (money >= 500) return 0.90;
    if (money >= 200) return 0.95;
    return 1.0;
}

// 把折扣数字转换成容易看懂的文字
string getDiscountWords(double discount) {
    if (discount == 0.85) return "钻石会员85折";
    if (discount == 0.90) return "黄金会员9折";
    if (discount == 0.95) return "白银会员95折";
    return "普通会员无折扣";
}

// 把购物车中的商品全部生成订单，并从用户余额中完成支付
void makeOrder(string userName) {
    vector<CartItem> carts = readCart();
    vector<Order> orders = readOrders();
    int maxId = 10000;
    bool hasThing = false;
    double allMoney = 0;

    for (int i = 0; i < (int)orders.size(); i++) {
        if (orders[i].orderId > maxId) maxId = orders[i].orderId;
    }

    for (int i = 0; i < (int)carts.size(); i++) {
        if (carts[i].userName == userName) {
            Product p = findProductById(carts[i].productId);
            if (p.id == -1 || p.num < carts[i].count) {
                cout << "商品 " << carts[i].productId << " 库存不足，不能下单。" << endl;
                return;
            }
        }
    }

    double discount = getDiscount(userName);
    double needPay = 0;
    for (int i = 0; i < (int)carts.size(); i++) {
        if (carts[i].userName == userName) {
            Product p = findProductById(carts[i].productId);
            needPay += p.price * carts[i].count * discount;
        }
    }

    // 先计算总金额并扣款，余额不足时不生成订单也不扣库存
    cout << "本次购物车订单需要支付: " << needPay << endl;
    cout << "当前账户余额: " << getUserBalance(userName) << endl;
    if (!payUserMoney(userName, needPay)) {
        cout << "余额不足，不能支付订单。请先到个人中心充值。" << endl;
        return;
    }

    for (int i = 0; i < (int)carts.size(); i++) {
        if (carts[i].userName == userName) {
            Product p = findProductById(carts[i].productId);
            reduceProductNum(p.id, carts[i].count);
            Order o;
            o.orderId = ++maxId;
            o.userName = userName;
            o.productId = p.id;
            o.productName = p.name;
            o.count = carts[i].count;
            o.money = p.price * carts[i].count * discount;
            o.status = "已支付";
            o.logistics = "等待发货";
            orders.push_back(o);
            allMoney += o.money;
            hasThing = true;
        }
    }

    if (!hasThing) {
        cout << "购物车是空的。" << endl;
        return;
    }

    saveOrders(orders);
    clearCart(userName);
    addUserMoney(userName, allMoney);
    cout << "支付成功，本次实付 " << allMoney << " 元。" << endl;
}

// 用户输入商品编号后直接购买
void buyNow(string userName) {
    int id;
    cout << "请输入要购买的商品编号: ";
    while (!(cin >> id)) {
        cout << "输入无效，请重新输入编号: ";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
    buyNowById(userName, id);
}

// 已经知道商品编号时直接购买，购买前显示折扣、实付金额和账户余额
void buyNowById(string userName, int id) {
    vector<Order> orders = readOrders();
    int count;
    int maxId = 10000;

    Product p = findProductById(id);
    if (p.id == -1) {
        cout << "没有找到这个商品。" << endl;
        return;
    }

    cout << "请输入购买数量: ";
    while (!(cin >> count)) {
        cout << "输入无效，请重新输入数量: ";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
    if (count <= 0) {
        cout << "购买数量必须大于0。" << endl;
        return;
    }
    if (p.num < count) {
        cout << "库存不够，现在库存只有 " << p.num << " 件。" << endl;
        return;
    }

    for (int i = 0; i < (int)orders.size(); i++) {
        if (orders[i].orderId > maxId) maxId = orders[i].orderId;
    }

    double discount = getDiscount(userName);
    double oldMoney = p.price * count;
    double newMoney = oldMoney * discount;

    cout << endl;
    cout << "====== 确认订单 ======" << endl;
    cout << "商品名称: " << p.name << endl;
    cout << "商品单价: " << p.price << endl;
    cout << "购买数量: " << count << endl;
    cout << "原价合计: " << oldMoney << endl;
    cout << "当前折扣: " << getDiscountWords(discount) << endl;
    cout << "实付金额: " << newMoney << endl;
    cout << "当前账户余额: " << getUserBalance(userName) << endl;
    cout << "确认下单吗？(y/n): ";
    char sure;
    cin >> sure;
    if (sure != 'y' && sure != 'Y') {
        cout << "已经取消下单。" << endl;
        return;
    }

    Order o;
    o.orderId = maxId + 1;
    o.userName = userName;
    o.productId = p.id;
    o.productName = p.name;
    o.count = count;
    o.money = newMoney;
    o.status = "已支付";
    o.logistics = "等待发货";

    // 支付成功后才扣库存、写订单，避免余额不足还生成订单
    if (!payUserMoney(userName, newMoney)) {
        cout << "余额不足，不能支付订单。请先到个人中心充值。" << endl;
        return;
    }
    reduceProductNum(p.id, count);
    orders.push_back(o);
    saveOrders(orders);
    addUserMoney(userName, o.money);

    cout << "直接下单并支付成功！" << endl;
    cout << "订单号: " << o.orderId << endl;
    cout << "商品: " << o.productName << endl;
    cout << "数量: " << o.count << endl;
    cout << "实付金额: " << o.money << endl;
}

// 用户查看自己的历史订单
void showMyOrders(string userName) {
    vector<Order> orders = readOrders();
    cout << "====== 我的订单 ======" << endl;
    for (int i = 0; i < (int)orders.size(); i++) {
        if (orders[i].userName == userName) {
            cout << "订单号:" << orders[i].orderId << " 商品:" << orders[i].productName
                 << " 数量:" << orders[i].count << " 金额:" << orders[i].money
                 << " 状态:" << orders[i].status
                 << " 物流:" << orders[i].logistics << endl;
        }
    }
}

// 管理员查看全部订单
void showAllOrders() {
    vector<Order> orders = readOrders();
    cout << "====== 全部订单 ======" << endl;
    for (int i = 0; i < (int)orders.size(); i++) {
        cout << "订单号:" << orders[i].orderId << " 用户:" << orders[i].userName
             << " 商品:" << orders[i].productName << " 数量:" << orders[i].count
             << " 金额:" << orders[i].money << " 状态:" << orders[i].status
             << " 物流:" << orders[i].logistics << endl;
    }
}

// 管理员修改订单状态
void changeOrderStatus() {
    vector<Order> orders = readOrders();
    int id;
    string status;
    cout << "请输入订单号: ";
    while (!(cin >> id)) {
        cout << "输入无效，请重新输入订单号: ";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
    cout << "请输入新状态(已支付/已发货/已收货/已取消): ";
    cin >> status;
    if (status != "已支付" && status != "已发货" && status != "已收货" && status != "已取消") {
        cout << "订单状态不正确，请输入：已支付/已发货/已收货/已取消。" << endl;
        return;
    }
    for (int i = 0; i < (int)orders.size(); i++) {
        if (orders[i].orderId == id) {
            orders[i].status = status;
            saveOrders(orders);
            cout << "修改成功。" << endl;
            return;
        }
    }
    cout << "没有找到订单。" << endl;
}

// 用户确认收货。只有已发货订单才允许确认，避免跳过物流流程
void confirmReceive(string userName) {
    vector<Order> orders = readOrders();
    int id;
    cout << "请输入要确认收货的订单号: ";
    while (!(cin >> id)) {
        cout << "输入无效，请重新输入订单号: ";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
    for (int i = 0; i < (int)orders.size(); i++) {
        if (orders[i].orderId == id && orders[i].userName == userName) {
            if (orders[i].status != "已发货") {
                cout << "只有已发货订单才能确认收货。" << endl;
                return;
            }
            orders[i].status = "已收货";
            orders[i].logistics = "买家已签收";
            saveOrders(orders);
            cout << "确认收货成功。" << endl;
            return;
        }
    }
    cout << "没有找到这个订单。" << endl;
}

// 管理员维护订单物流。维护物流后，如果订单已支付，则自动变成已发货
void changeLogistics() {
    vector<Order> orders = readOrders();
    int id;
    string info;
    cout << "请输入订单号: ";
    while (!(cin >> id)) {
        cout << "输入无效，请重新输入订单号: ";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
    cout << "请输入物流信息(不要输入空格): ";
    cin >> info;
    for (int i = 0; i < (int)orders.size(); i++) {
        if (orders[i].orderId == id) {
            orders[i].logistics = info;
            if (orders[i].status == "已支付") orders[i].status = "已发货";
            saveOrders(orders);
            cout << "物流维护成功。" << endl;
            return;
        }
    }
    cout << "没有找到这个订单。" << endl;
}

// 判断用户是否买过某商品，评价商品时使用
bool hasBought(string userName, int productId) {
    vector<Order> orders = readOrders();
    for (int i = 0; i < (int)orders.size(); i++) {
        if (orders[i].userName == userName && orders[i].productId == productId) {
            return true;
        }
    }
    return false;
}

// 用户提交退款申请，提交后订单状态变为“退款中”，避免重复申请
void applyRefund(string userName) {
    vector<Order> orders = readOrders();
    int id;
    string reason;
    cout << "请输入要退款的订单号: ";
    while (!(cin >> id)) {
        cout << "输入无效，请重新输入订单号: ";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
    cout << "请输入退款原因(不要输入空格): ";
    cin >> reason;
    if (reason.empty()) {
        cout << "退款原因不能为空。" << endl;
        return;
    }
    for (int i = 0; i < (int)orders.size(); i++) {
        if (orders[i].orderId == id && orders[i].userName == userName) {
            if (orders[i].status == "已收货" || orders[i].status == "已取消") {
                cout << "这个订单不能申请退款。" << endl;
                return;
            }
            if (orders[i].status == "退款中" || orders[i].status == "已退款") {
                cout << "这个订单已经申请过退款了。" << endl;
                return;
            }
            ofstream fout("refunds.txt", ios::app);
            fout << id << " " << userName << " " << reason << " 待审核" << endl;
            fout.close();
            orders[i].status = "退款中";
            saveOrders(orders);
            cout << "退款申请已经提交。" << endl;
            return;
        }
    }
    cout << "没有找到这个订单。" << endl;
}

// 管理员查看并审核退款申请
void checkRefund() {
    ifstream fin("refunds.txt");
    vector<int> ids;
    vector<string> users, reasons, states;
    string line;
    while (getline(fin, line)) {
        if (line.size() == 0) continue;
        if (line.size() >= 3 && (unsigned char)line[0] == 239 &&
            (unsigned char)line[1] == 187 && (unsigned char)line[2] == 191) {
            line = line.substr(3);
        }

        stringstream ss(line);
        int id;
        string user, reason, state;
        if (!(ss >> id >> user >> reason >> state)) continue;

        ids.push_back(id);
        users.push_back(user);
        reasons.push_back(reason);
        states.push_back(state);
    }
    fin.close();

    cout << endl;
    cout << "====== 退款申请列表 ======" << endl;
    if (ids.size() == 0) {
        cout << "现在没有退款申请。" << endl;
        return;
    }

    vector<Order> showOrders = readOrders();
    for (int i = 0; i < (int)ids.size(); i++) {
        Order thisOrder;
        thisOrder.orderId = -1;
        for (int j = 0; j < (int)showOrders.size(); j++) {
            if (showOrders[j].orderId == ids[i]) {
                thisOrder = showOrders[j];
                break;
            }
        }

        string productName = "订单不存在";
        int count = 0;
        double money = 0;
        string orderStatus = "无";
        if (thisOrder.orderId != -1) {
            productName = thisOrder.productName;
            count = thisOrder.count;
            money = thisOrder.money;
            orderStatus = thisOrder.status;
        }

        cout << "------------------------------" << endl;
        cout << "订单号: " << ids[i] << endl;
        cout << "申请用户: " << users[i] << endl;
        cout << "商品: " << productName << endl;
        cout << "数量: " << count << endl;
        cout << "金额: " << money << endl;
        cout << "订单状态: " << orderStatus << endl;
        cout << "申请状态: " << states[i] << endl;
        cout << "退款原因: " << reasons[i] << endl;
    }
    cout << "------------------------------" << endl;

    cout << "输入要处理的订单号(0返回): ";
    int id;
    while (!(cin >> id)) {
        cout << "输入无效，请重新输入订单号: ";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
    if (id == 0) return;

    bool foundRefund = false;
    int refundIndex = -1;
    for (int i = 0; i < (int)ids.size(); i++) {
        if (ids[i] == id) {
            foundRefund = true;
            refundIndex = i;
            break;
        }
    }
    if (!foundRefund) {
        cout << "没有找到这个退款申请。" << endl;
        return;
    }
    if (states[refundIndex] != "待审核") {
        cout << "这个退款申请已经处理过了，不能重复审核。" << endl;
        return;
    }

    cout << "1.通过  2.拒绝: ";
    int choose;
    while (!(cin >> choose) || (choose != 1 && choose != 2)) {
        cout << "输入无效，请输入1(通过)或2(拒绝): ";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }

    vector<Order> orders = readOrders();
    cout << "确认处理这个退款申请吗？(y/n): ";
    char sure;
    cin >> sure;
    if (sure != 'y' && sure != 'Y') {
        cout << "已经取消审核。" << endl;
        return;
    }

    if (choose == 1) states[refundIndex] = "已通过";
    else states[refundIndex] = "已拒绝";

    for (int i = 0; i < (int)orders.size(); i++) {
        if (orders[i].orderId == id && choose == 1) {
            orders[i].status = "已退款";
            addProductNum(orders[i].productId, orders[i].count);
        }
    }
    saveOrders(orders);

    ofstream fout("refunds.txt");
    for (int i = 0; i < (int)ids.size(); i++) {
        fout << ids[i] << " " << users[i] << " " << reasons[i] << " " << states[i] << endl;
    }
    fout.close();
    cout << "处理完成。" << endl;
}
