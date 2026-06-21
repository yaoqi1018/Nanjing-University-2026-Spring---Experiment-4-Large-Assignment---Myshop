#include "History.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <ctime>

using namespace std;

// 浏览历史记录，timeNum 用来表示浏览时间
struct HistoryItem {
    string userName;
    int productId;
    string productName;
    long long timeNum;
};

// 查看商品详情时自动增加一条浏览记录
void addHistory(string userName, int productId, string productName) {
    ofstream fout("history.txt", ios::app);
    fout << userName << " " << productId << " " << productName << " " << time(0) << endl;
    fout.close();
}

// 显示最近 10 条浏览记录，新的记录排在前面
void showHistory(string userName) {
    ifstream fin("history.txt");
    vector<HistoryItem> arr;
    HistoryItem h;
    while (fin >> h.userName >> h.productId >> h.productName >> h.timeNum) {
        if (h.userName == userName) arr.push_back(h);
    }
    fin.close();

    cout << "====== 最近浏览记录 ======" << endl;
    int cnt = 0;
    for (int i = (int)arr.size() - 1; i >= 0 && cnt < 10; i--, cnt++) {
        cout << arr[i].productId << " " << arr[i].productName << endl;
    }
    if (cnt == 0) cout << "还没有浏览记录。" << endl;
}
