#ifndef USER_H
#define USER_H

#include <string>
#include <vector>

using namespace std;

struct User {
    string name;      // 用户名
    string password;  // 登录密码
    string phone;     // 手机号
    double money;     // 累计消费金额，用于计算用户等级
    double balance;   // 支付余额，下单时从这里扣款
    string address;   // 默认收货地址
};

vector<User> readUsers();
void saveUsers(vector<User> users);
bool registerUser();
bool userLogin(string &nowUser);
bool adminLogin();
void showAllUsers();
void resetUserPassword();
void changeMyPassword(string nowUser);
void showMyInfo(string nowUser);
void changeMyAddress(string nowUser);
void rechargeBalance(string nowUser);
double getUserMoney(string name);
void addUserMoney(string name, double money);
double getUserBalance(string name);
bool payUserMoney(string name, double money);

#endif
