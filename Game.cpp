#include "Game.h"
#include <iostream>
#include <string>
#include <sstream>
#include <conio.h>
#include <thread>
using namespace std;

void Game::clear() {
    // 簡單清屏指令（\033[1;1H 代表游標移回左上角）
    cout << "\033[1;1H";
}

int Game::getValidDifficulty() {
    int choice;
    while (true) {
        cout << "請選擇遊戲難度 [1] 簡單  [2] 普通  [3] 困難: ";
        if (cin >> choice && choice >= 1 && choice <= 3) {
            cin.ignore(100, '\n');
            return choice;
        }
        else {
            cout << ">> [錯誤] 輸入無效，請輸入 1, 2 或 3！\n";
            cin.clear();
            cin.ignore(100, '\n');
        }
    }
}

void Game::display() {
    // 顯示頂部行號 (Column)
    cout << "         1 2 3   4 5 6   7 8 9\n";
    cout << "       +-------+-------+-------+\n";

    for (int i = 0; i < 9; i++) {
        if (i > 0 && i % 3 == 0) {
            cout << "       +-------+-------+-------+\n";
        }

        // 左側顯示純數字列號 (Row)，完美對齊頂部風格
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
    cout << "\033[2J\033[1;1H";
    cout << "====== 隨機生成數獨挑戰賽 ======\n\n";

    // 取得難度並隨機生成題目
    int difficulty = getValidDifficulty();
    board.generatePuzzle(difficulty);

    wrongCount = 0; // 初始化錯誤次數
    const int MAX_WRONG = 3;

    // 設定限時並初始化開始時間 
    timeLimitInSeconds = 300; // 限時 5 分鐘 (300秒)
    startTime = std::chrono::steady_clock::now();

    string inputBuffer = ""; // 用來暫存玩家目前正在輸入的文字
    string systemMessage = ""; // 用來存放系統提示訊息，避免畫面閃爍被洗掉
    bool timeOut = false; // 標記是否超時

    // 在進入遊戲前，強制清空底層鍵盤緩衝區，防止難度選擇的 Enter 殘留
    while (_kbhit()) { _getch(); }

    // 在進入遊戲前，先徹底清空一次
    cout << "\033[2J\033[1;1H";

    // 迴圈條件加上時間檢查
    while (!board.isComplete() && wrongCount < MAX_WRONG) {
        
        // 【核心控制：微型刷新迴圈】
        
        // 只要玩家還沒按下 Enter 送出答案，這個迴圈就會每 0.1 秒瘋狂重新整理
        bool inputComplete = false;
        while (true) {
            // 計算剩餘時間
            auto currentTime = chrono::steady_clock::now();
            // 計算從開始到現在過了幾秒
            int elapsedSeconds = chrono::duration_cast<chrono::seconds>(currentTime - startTime).count();
            int remainingTime = timeLimitInSeconds - elapsedSeconds;

            // 如果超時，直接打破迴圈結束遊戲
            if (remainingTime <= 0) {
                timeOut = true;
                break;
            }

            clear();

            // 即時重繪畫面，\033[K 可以清除殘影
            cout << "====== 隨機生成數獨挑戰賽 ======\n";
            cout << " 目前錯誤次數: " << wrongCount << " / " << MAX_WRONG << "\033[K\n\n";
            cout << " 剩餘時間: " << remainingTime / 60 << " 分 " << remainingTime % 60 << " 秒\033[K\n\n";

            display();

            if (!systemMessage.empty()) {
                cout << "\n" << systemMessage << "\033[K\n";
            }
            else {
                cout << "\n\033[K\n";
            }

            cout << "\n請輸入作答 [列] [行] [數字] (例如: 1 3 4): " << inputBuffer << "\033[K";
            cout << flush;

            // 偵測鍵盤有沒有被按下
            if (_kbhit()) {
                char ch = _getch();

                if (ch == '\r') { // 玩家按了 Enter 鍵
                    if (!inputBuffer.empty()) {
                        inputComplete = true; // 標記輸入完成，準備解析
                        break;
                    }
                }
                else if (ch == '\b') { // 玩家按了 Backspace 鍵
                    if (!inputBuffer.empty()) {
                        inputBuffer.pop_back(); // 刪除最後一個字元
                    }
                }
                else if (ch >= 32 && ch <= 126) { // 玩家按了一般可見字元（數字、空格等）
                    inputBuffer += ch; // 串接到暫存字串中
                }
            }

            // 20 毫秒隔間，畫面更新率為 50 FPS
            this_thread::sleep_for(chrono::milliseconds(20));
        }

        // 如果超時，直接打破迴圈結束遊戲
        if (timeOut) break;

        // 解析玩家輸入的字串
        int r, c, v;
        stringstream ss(inputBuffer);
        inputBuffer = "";
        systemMessage = "";

        if (!(ss >> r >> c >> v)) {
            systemMessage = ">> [提示] 輸入格式錯誤！請輸入三個數字（以空格區隔）。";
            continue;
        }

        // 座標轉換 (1-9 轉為陣列索引 0-8)
        r--; c--;

        // 範圍檢查
        if (r < 0 || r >= 9 || c < 0 || c >= 9) {
            systemMessage =  ">> [提示] 位置超出範圍(1-9)！";
            continue;
        }

        // 檢查該處是否已經填寫
        if (board.getCell(r, c) != 0) {
            systemMessage =  ">> [提示] 該處已經有數字了！";
            continue;
        }

        // 判斷是否正確並填入
        if (!board.validateAndSet(r, c, v)) {
            wrongCount++;
            systemMessage =  ">> [錯誤] 數字 " + to_string(v) + " 不是正確答案！(剩餘機會: " + to_string(MAX_WRONG - wrongCount) + ")";
        }
        else {
            systemMessage = ">> [成功] 填入正確！";
        }
    }

    cout << "\033[2J\033[1;1H";
    cout << "====== 遊戲結束 ======\n";
    display();

    if (board.isComplete()) {
        cout << "\n恭喜你完成了這則隨機產生的數獨！太強了！\n";
    }
    else if (timeOut) {
        cout << "\n很遺憾，時間到！挑戰失敗。\n";
    }
    else if (wrongCount >= MAX_WRONG) {
        cout << "\n很遺憾，你已經達到了 3 次錯誤，挑戰失敗。\n";
    }
}