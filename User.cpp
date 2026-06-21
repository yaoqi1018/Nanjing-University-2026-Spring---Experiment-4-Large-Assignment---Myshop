#include "User.h"
#include <iostream>
#include <fstream>
#include <iomanip>
#include <limits>
#include <sstream>

using namespace std;

// 从 users.txt 读取所有普通用户信息
vector<User> readUsers() {
    vector<User> users;
    ifstream fin("users.txt");
    string line;
    while (getline(fin, line)) {
        if (line.size() == 0) continue;
        User u;
        stringstream ss(line);
        // 兼容旧版 users.txt：旧数据没有余额和地址时，自动补默认值
        u.balance = 10000;
        u.address = "未填写";
        ss >> u.name >> u.password >> u.phone >> u.money;
        if (ss >> u.balance >> u.address) {
        }
        if (u.name == "") continue;
        users.push_back(u);
    }
    fin.close();
    return users;
}

// 把用户列表重新保存到 users.txt
void saveUsers(vector<User> users) {
    ofstream fout("users.txt");
    for (int i = 0; i < (int)users.size(); i++) {
        fout << users[i].name << " " << users[i].password << " "
             << users[i].phone << " " << users[i].money << " "
             << users[i].balance << " " << users[i].address << endl;
    }
    fout.close();
}

// 普通用户注册，要求用户名不能重复
bool registerUser() {
    vector<User> users = readUsers();
    User u;
    cout << "请输入用户名: ";
    cin >> u.name;

    if (u.name.empty()) {
        cout << "用户名不能为空。" << endl;
        return false;
    }

    for (int i = 0; i < (int)users.size(); i++) {
        if (users[i].name == u.name) {
            cout << "这个用户名已经存在了。" << endl;
            return false;
        }
    }

    cout << "请输入密码: ";
    cin >> u.password;
    if (u.password.empty()) {
        cout << "密码不能为空。" << endl;
        return false;
    }

    cout << "请输入手机号: ";
    cin >> u.phone;
    // 验证手机号是否全为数字
    bool valid = !u.phone.empty();
    for (int i = 0; i < (int)u.phone.size(); i++) {
        if (u.phone[i] < '0' || u.phone[i] > '9') {
            valid = false;
            break;
        }
    }
    if (!valid) {
        cout << "手机号格式不正确，只能包含数字。" << endl;
        return false;
    }

    u.money = 0;
    u.balance = 10000;
    cout << "请输入收货地址(不要输入空格): ";
    cin >> u.address;
    if (u.address.empty()) u.address = "未填写";
    users.push_back(u);
    saveUsers(users);
    cout << "注册成功！" << endl;
    return true;
}

// 普通用户登录，登录成功后把用户名保存到 nowUser
bool userLogin(string &nowUser) {
    vector<User> users = readUsers();
    string name, password;
    cout << "用户名: ";
    cin >> name;
    cout << "密码: ";
    cin >> password;

    if (name.empty() || password.empty()) {
        cout << "用户名和密码不能为空。" << endl;
        return false;
    }

    for (int i = 0; i < (int)users.size(); i++) {
        if (users[i].name == name && users[i].password == password) {
            nowUser = name;
            cout << "登录成功，欢迎你，" << nowUser << "！" << endl;
            return true;
        }
    }
    cout << "用户名或密码错误。" << endl;
    return false;
}

// 管理员账号是固定的，不能通过注册得到
bool adminLogin() {
    string name, password;
    cout << "管理员账号: ";
    cin >> name;
    cout << "管理员密码: ";
    cin >> password;

    if (name.empty() || password.empty()) {
        cout << "管理员账号或密码错误。" << endl;
        return false;
    }

    if (name == "admin" && password == "123456") {
        cout << "管理员登录成功。" << endl;
        return true;
    }
    cout << "管理员账号或密码错误。" << endl;
    return false;
}

// 管理员查看全部普通用户
void showAllUsers() {
    vector<User> users = readUsers();
    if (users.size() == 0) {
        cout << "目前还没有注册用户。" << endl;
        return;
    }
    cout << left << setw(15) << "用户名" << setw(15) << "手机号"
         << setw(12) << "消费金额" << setw(12) << "余额" << "地址" << endl;
    for (int i = 0; i < (int)users.size(); i++) {
        cout << left << setw(15) << users[i].name << setw(15) << users[i].phone
             << setw(12) << users[i].money << setw(12) << users[i].balance
             << users[i].address << endl;
    }
}

// 管理员把指定用户密码重置为 123456
void resetUserPassword() {
    vector<User> users = readUsers();
    string name;
    cout << "请输入要重置密码的用户名: ";
    cin >> name;
    if (name.empty()) {
        cout << "用户名不能为空。" << endl;
        return;
    }
    for (int i = 0; i < (int)users.size(); i++) {
        if (users[i].name == name) {
            users[i].password = "123456";
            saveUsers(users);
            cout << "密码已经重置为 123456。" << endl;
            return;
        }
    }
    cout << "没有找到这个用户。" << endl;
}

// 用户修改自己的密码，需要先验证原密码
void changeMyPassword(string nowUser) {
    vector<User> users = readUsers();
    string oldPass, newPass;
    cout << "请输入原密码: ";
    cin >> oldPass;
    cout << "请输入新密码: ";
    cin >> newPass;

    if (oldPass.empty() || newPass.empty()) {
        cout << "密码不能为空。" << endl;
        return;
    }

    for (int i = 0; i < (int)users.size(); i++) {
        if (users[i].name == nowUser) {
            if (users[i].password != oldPass) {
                cout << "原密码错误。" << endl;
                return;
            }
            users[i].password = newPass;
            saveUsers(users);
            cout << "密码修改成功。" << endl;
            return;
        }
    }
}

// 显示个人资料、支付余额、收货地址、累计消费和会员等级
void showMyInfo(string nowUser) {
    vector<User> users = readUsers();
    for (int i = 0; i < (int)users.size(); i++) {
        if (users[i].name == nowUser) {
            cout << "用户名: " << users[i].name << endl;
            cout << "手机号: " << users[i].phone << endl;
            cout << "支付余额: " << users[i].balance << endl;
            cout << "收货地址: " << users[i].address << endl;
            cout << "累计消费: " << users[i].money << endl;
            if (users[i].money >= 1000) cout << "会员等级: 钻石会员，享受85折" << endl;
            else if (users[i].money >= 500) cout << "会员等级: 黄金会员，享受9折" << endl;
            else if (users[i].money >= 200) cout << "会员等级: 白银会员，享受95折" << endl;
            else cout << "会员等级: 普通会员，无折扣" << endl;
            return;
        }
    }
}

// 修改收货地址。地址中不支持空格，方便继续使用空格分隔的文本文件保存
void changeMyAddress(string nowUser) {
    vector<User> users = readUsers();
    string address;
    cout << "请输入新的收货地址(不要输入空格): ";
    cin >> address;
    if (address.empty()) {
        cout << "地址不能为空。" << endl;
        return;
    }
    for (int i = 0; i < (int)users.size(); i++) {
        if (users[i].name == nowUser) {
            users[i].address = address;
            saveUsers(users);
            cout << "收货地址修改成功。" << endl;
            return;
        }
    }
}

// 用户给自己的支付余额充值，充值金额必须为正数
void rechargeBalance(string nowUser) {
    vector<User> users = readUsers();
    double money;
    cout << "请输入充值金额: ";
    while (!(cin >> money)) {
        cout << "输入无效，请重新输入金额: ";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
    if (money <= 0) {
        cout << "充值金额必须大于0。" << endl;
        return;
    }
    for (int i = 0; i < (int)users.size(); i++) {
        if (users[i].name == nowUser) {
            users[i].balance += money;
            saveUsers(users);
            cout << "充值成功，当前余额: " << users[i].balance << endl;
            return;
        }
    }
}

// 查询用户累计消费金额，用于计算会员折扣
double getUserMoney(string name) {
    vector<User> users = readUsers();
    for (int i = 0; i < (int)users.size(); i++) {
        if (users[i].name == name) return users[i].money;
    }
    return 0;
}

// 下单成功后增加用户累计消费金额
void addUserMoney(string name, double money) {
    vector<User> users = readUsers();
    for (int i = 0; i < (int)users.size(); i++) {
        if (users[i].name == name) {
            users[i].money += money;
            saveUsers(users);
            return;
        }
    }
}

// 查询用户支付余额
double getUserBalance(string name) {
    vector<User> users = readUsers();
    for (int i = 0; i < (int)users.size(); i++) {
        if (users[i].name == name) return users[i].balance;
    }
    return 0;
}

// 支付订单时扣除用户余额。余额不足时返回 false，订单不会生成
bool payUserMoney(string name, double money) {
    vector<User> users = readUsers();
    for (int i = 0; i < (int)users.size(); i++) {
        if (users[i].name == name) {
            if (users[i].balance < money) return false;
            users[i].balance -= money;
            saveUsers(users);
            return true;
        }
    }
    return false;
}
