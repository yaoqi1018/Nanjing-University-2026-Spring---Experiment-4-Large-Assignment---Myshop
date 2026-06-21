#include "Review.h"
#include "Order.h"
#include "Product.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <limits>
#include <iomanip>
#include <sstream>

using namespace std;

// 一条商品评价记录
struct ReviewItem {
    int id;
    string userName;
    int productId;
    int score;
    string words;
    string reply; // 管理员/商家对评价的回复
};

// 从 reviews.txt 读取所有评价
vector<ReviewItem> readReviews() {
    vector<ReviewItem> rs;
    ifstream fin("reviews.txt");
    string line;
    while (getline(fin, line)) {
        if (line.size() == 0) continue;
        stringstream ss(line);
        ReviewItem r;
        // 兼容旧版 reviews.txt：旧评价没有回复时，自动显示暂无回复
        r.reply = "暂无回复";
        ss >> r.id >> r.userName >> r.productId >> r.score >> r.words;
        if (ss >> r.reply) {
        }
        if (r.id <= 0) continue;
        rs.push_back(r);
    }
    fin.close();
    return rs;
}

// 保存评价列表
void saveReviews(vector<ReviewItem> rs) {
    ofstream fout("reviews.txt");
    for (int i = 0; i < (int)rs.size(); i++) {
        fout << rs[i].id << " " << rs[i].userName << " " << rs[i].productId
             << " " << rs[i].score << " " << rs[i].words << " "
             << rs[i].reply << endl;
    }
    fout.close();
}

// 用户对已经购买过的商品进行评价，未购买的商品不能评价
void addReview(string userName) {
    vector<ReviewItem> rs = readReviews();
    ReviewItem r;
    int maxId = 0;
    for (int i = 0; i < (int)rs.size(); i++) {
        if (rs[i].id > maxId) maxId = rs[i].id;
    }
    r.id = maxId + 1;
    r.userName = userName;
    cout << "请输入商品编号: ";
    while (!(cin >> r.productId)) {
        cout << "输入无效，请重新输入商品编号: ";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
    if (!hasBought(userName, r.productId)) {
        cout << "你还没有购买过这个商品，不能评价。" << endl;
        return;
    }
    cout << "请输入评分(1-5): ";
    while (!(cin >> r.score) || r.score < 1 || r.score > 5) {
        cout << "评分无效，请输入1-5之间的整数: ";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
    cout << "请输入评价内容(不要输入空格): ";
    cin >> r.words;
    if (r.words.empty()) {
        cout << "评价内容不能为空。" << endl;
        return;
    }
    r.reply = "暂无回复";
    rs.push_back(r);
    saveReviews(rs);
    cout << "评价成功。" << endl;
}

// 查看某个商品的所有评价
void showProductReviews(int productId) {
    vector<ReviewItem> rs = readReviews();
    cout << "====== 商品评价 ======" << endl;
    bool ok = false;
    for (int i = 0; i < (int)rs.size(); i++) {
        if (rs[i].productId == productId) {
            cout << "评价编号:" << rs[i].id << " 用户:" << rs[i].userName
                 << " 评分:" << rs[i].score << " 内容:" << rs[i].words
                 << " 商家回复:" << rs[i].reply << endl;
            ok = true;
        }
    }
    if (!ok) cout << "暂无评价。" << endl;
}

// 管理员查看所有评价，方便知道要删除哪条评价
void showAllReviews() {
    vector<ReviewItem> rs = readReviews();
    cout << endl;
    cout << "====== 全部商品评价 ======" << endl;
    if (rs.size() == 0) {
        cout << "现在还没有任何评价。" << endl;
        return;
    }

    for (int i = 0; i < (int)rs.size(); i++) {
        Product p = findProductById(rs[i].productId);
        string productName = "商品已删除";
        if (p.id != -1) productName = p.name;

        cout << "------------------------------" << endl;
        cout << "评价编号: " << rs[i].id << endl;
        cout << "用户: " << rs[i].userName << endl;
        cout << "商品: " << rs[i].productId << " " << productName << endl;
        cout << "评分: " << rs[i].score << endl;
        cout << "内容: " << rs[i].words << endl;
        cout << "商家回复: " << rs[i].reply << endl;
    }
    cout << "------------------------------" << endl;
}

// 管理员回复评价，回复内容会保存到 reviews.txt 中
void replyReview() {
    vector<ReviewItem> rs = readReviews();
    int id;
    string reply;
    showAllReviews();
    if (rs.size() == 0) return;

    cout << "请输入要回复的评价编号: ";
    while (!(cin >> id)) {
        cout << "输入无效，请重新输入评价编号: ";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
    cout << "请输入商家回复内容(不要输入空格): ";
    cin >> reply;
    if (reply.empty()) {
        cout << "回复内容不能为空。" << endl;
        return;
    }
    for (int i = 0; i < (int)rs.size(); i++) {
        if (rs[i].id == id) {
            rs[i].reply = reply;
            saveReviews(rs);
            cout << "回复成功。" << endl;
            return;
        }
    }
    cout << "没有找到这条评价。" << endl;
}

// 管理员根据评价编号删除评价
void deleteReview() {
    vector<ReviewItem> rs = readReviews();
    int id;
    showAllReviews();
    if (rs.size() == 0) return;

    cout << "请输入要删除的评价编号: ";
    while (!(cin >> id)) {
        cout << "输入无效，请重新输入评价编号: ";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
    for (int i = 0; i < (int)rs.size(); i++) {
        if (rs[i].id == id) {
            cout << "准备删除这条评价：" << endl;
            cout << "用户:" << rs[i].userName << " 商品编号:" << rs[i].productId
                 << " 评分:" << rs[i].score << " 内容:" << rs[i].words << endl;
            cout << "确认删除吗？(y/n): ";
            char sure;
            cin >> sure;
            if (sure != 'y' && sure != 'Y') {
                cout << "已经取消删除。" << endl;
                return;
            }
            rs.erase(rs.begin() + i);
            saveReviews(rs);
            cout << "删除评价成功。" << endl;
            return;
        }
    }
    cout << "没有找到这条评价。" << endl;
}
