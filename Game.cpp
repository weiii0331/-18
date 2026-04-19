#include "Game.h"
#include <iostream>
using namespace std;

void Game::clear() {
    // 簡單清屏指令
    cout << "\033[2J\033[1;1H";
}

void Game::display() {
    // 顯示頂部行號
    cout << "         1 2 3   4 5 6   7 8 9\n";
    cout << "       +-------+-------+-------+\n";

    for (int i = 0; i < 9; i++) {
        if (i > 0 && i % 3 == 0) {
            cout << "       +-------+-------+-------+\n";
        }

        // 左側顯示純數字列號
        cout << "     " << i + 1 << " | ";

        for (int j = 0; j < 9; j++) {
            int v = board.getCell(i, j);
            if (v == 0) cout << ". ";
            else cout << v << " ";

            if ((j + 1) % 3 == 0) cout << "| ";
        }
        cout << "\n";
    }
    cout << "       +-------+-------+-------+\n";
}

void Game::start() {
    wrongCount = 0; // 初始化錯誤次數
    const int MAX_WRONG = 3;

    while (!board.isComplete() && wrongCount < MAX_WRONG) {
        clear();
        cout << "====== 簡易數獨遊戲 (挑戰版) ======\n";
        cout << " 目前錯誤次數: " << wrongCount << " / " << MAX_WRONG << "\n\n";
        display();

        int r, c, v;
        cout << "\n請輸入作答 [列] [行] [數字] (例如: 1 3 4): ";

        if (!(cin >> r >> c >> v)) {
            cin.clear();
            cin.ignore(100, '\n');
            continue;
        }

        // 座標轉換
        r--; c--;

        // 範圍檢查
        if (r < 0 || r >= 9 || c < 0 || c >= 9) {
            cout << ">> [提示] 位置超出範圍！按 Enter 鍵繼續...";
            cin.ignore(100, '\n'); cin.get();
            continue;
        }

        // 檢查該處是否已經填寫
        if (board.getCell(r, c) != 0) {
            cout << ">> [提示] 該處已經有數字了！按 Enter 鍵繼續...";
            cin.ignore(100, '\n'); cin.get();
            continue;
        }

        // 判斷是否正確並填入
        if (!board.validateAndSet(r, c, v)) {
            wrongCount++;
            cout << ">> [錯誤] 數字 " << v << " 不正確！(剩餘機會: " << (MAX_WRONG - wrongCount) << ")\n";
            cout << ">> 請按 Enter 鍵繼續...";
            cin.ignore(100, '\n'); cin.get();
        }
    }

    clear();
    cout << "====== 遊戲結束 ======\n";
    display();

    if (board.isComplete()) {
        cout << "\n恭喜你完成了這則數獨！\n";
    }
    else if (wrongCount >= MAX_WRONG) {
        cout << "\n很遺憾，你已經達到了 3 次錯誤，挑戰失敗。\n";
    }
}