#include "Game.h"
#include <iostream>

void Game::clear() {
    // 簡單清屏指令（ANSI 轉義序列：\033[2J 代表清空，\033[1;1H 代表游標移回左上角）
    std::cout << "\033[2J\033[1;1H";
}

int Game::getValidDifficulty() {
    int choice;
    while (true) {
        std::cout << "請選擇遊戲難度 [1] 簡單  [2] 普通  [3] 困難: ";
        if (std::cin >> choice && choice >= 1 && choice <= 3) {
            return choice;
        }
        else {
            std::cout << ">> [錯誤] 輸入無效，請輸入 1, 2 或 3！\n";
            std::cin.clear();
            std::cin.ignore(100, '\n');
        }
    }
}

void Game::display() {
    // 顯示頂部行號 (Column)
    std::cout << "         1 2 3   4 5 6   7 8 9\n";
    std::cout << "       +-------+-------+-------+\n";

    for (int i = 0; i < 9; i++) {
        if (i > 0 && i % 3 == 0) {
            std::cout << "       +-------+-------+-------+\n";
        }

        // 左側顯示純數字列號 (Row)，完美對齊頂部風格
        std::cout << "     " << i + 1 << " | ";

        for (int j = 0; j < 9; j++) {
            int v = board.getCell(i, j);
            if (v == 0) std::cout << ". ";
            else std::cout << v << " ";

            if ((j + 1) % 3 == 0) std::cout << "| ";
        }
        std::cout << "\n";
    }
    std::cout << "       +-------+-------+-------+\n";
}

void Game::start() {
    clear();
    std::cout << "====== 隨機生成數獨挑戰賽 ======\n\n";

    // 取得難度並隨機生成題目
    int difficulty = getValidDifficulty();
    board.generatePuzzle(difficulty);

    wrongCount = 0; // 初始化錯誤次數
    const int MAX_WRONG = 3;

    while (!board.isComplete() && wrongCount < MAX_WRONG) {
        clear();
        std::cout << "====== 隨機生成數獨挑戰賽 ======\n";
        std::cout << " 目前錯誤次數: " << wrongCount << " / " << MAX_WRONG << "\n\n";
        display();

        int r, c, v;
        std::cout << "\n請輸入作答 [列] [行] [數字] (例如: 1 3 4): ";

        if (!(std::cin >> r >> c >> v)) {
            std::cin.clear();
            std::cin.ignore(100, '\n');
            continue;
        }

        // 座標轉換 (1-9 轉為陣列索引 0-8)
        r--; c--;

        // 範圍檢查
        if (r < 0 || r >= 9 || c < 0 || c >= 9) {
            std::cout << ">> [提示] 位置超出範圍(1-9)！按 Enter 鍵繼續...";
            std::cin.ignore(100, '\n'); std::cin.get();
            continue;
        }

        // 檢查該處是否已經填寫
        if (board.getCell(r, c) != 0) {
            std::cout << ">> [提示] 該處已經有數字了！按 Enter 鍵繼續...";
            std::cin.ignore(100, '\n'); std::cin.get();
            continue;
        }

        // 判斷是否正確並填入
        if (!board.validateAndSet(r, c, v)) {
            wrongCount++;
            std::cout << ">> [錯誤] 數字 " << v << " 不是正確答案！(剩餘機會: " << (MAX_WRONG - wrongCount) << ")\n";
            std::cout << ">> 請按 Enter 鍵繼續...";
            std::cin.ignore(100, '\n'); std::cin.get();
        }
    }

    clear();
    std::cout << "====== 遊戲結束 ======\n";
    display();

    if (board.isComplete()) {
        std::cout << "\n恭喜你完成了這則隨機產生的數獨！太強了！\n";
    }
    else if (wrongCount >= MAX_WRONG) {
        std::cout << "\n很遺憾，你已經達到了 3 次錯誤，挑戰失敗。\n";
    }
}