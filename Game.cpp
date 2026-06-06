#include "Game.h"
#include <iostream>
#include <string>
#include <sstream>
#include <conio.h>
#include <thread>

using namespace std;

void Game::clear() {
    // \033[?25l：隱藏游標（避免在重繪盤面時游標閃爍亂跑）
    // \033[1;1H：將游標移回左上角
    cout << "\033[?25l\033[1;1H";
}

int Game::getValidDifficulty() {
    int choice;
    while (true) {
        cout << "請選擇遊戲難度:\n";
        cout << " [1] 簡單 (挖空 30 格, 容錯 3 次, 提示 3 次, 不限時)\n";
        cout << " [2] 普通 (挖空 42 格, 容錯 3 次, 提示 3 次, 不限時)\n";
        cout << " [3] 困難 (挖空 54 格, 容錯 3 次, 提示 3 次, 不限時)\n";
        cout << " [4] 極限 (挖空 54 格, 容錯 3 次, 提示 3 次, 限時 5 分鐘)\n";
        cout << "請輸入選擇 (1-4): ";

        if (cin >> choice && choice >= 1 && choice <= 4) {
            cin.ignore(100, '\n');
            return choice;
        }
        else {
            cout << ">> [錯誤] 輸入無效，請輸入 1, 2, 3 或 4！\n\n";
            cin.clear();
            cin.ignore(100, '\n');
        }
    }
}

void Game::display() {
    // 顯示頂部行號 (Column)，確保使用純 ASCII 半形空白
    cout << "         1 2 3   4 5 6   7 8 9\n";
    cout << "       +-------+-------+-------+\n";

    for (int i = 0; i < 9; i++) {
        if (i > 0 && i % 3 == 0) {
            cout << "       +-------+-------+-------+\n";
        }

        // 左側顯示純數字列號 (Row)
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

    int difficulty = getValidDifficulty();
    board.generatePuzzle(difficulty);

    wrongCount = 0;
    hintCount = 0;
    maxWrong = 3;
    maxHints = 3;

    if (difficulty == 4) {
        timeLimitInSeconds = 300;
    }
    else {
        timeLimitInSeconds = 0;
    }

    startTime = std::chrono::steady_clock::now();

    string inputBuffer = "";
    string systemMessage = "";
    bool timeOut = false;

    // 在進入遊戲前，強制清空底層鍵盤緩衝區
    while (_kbhit()) { _getch(); }

    cout << "\033[2J\033[1;1H";

    while (!board.isComplete() && wrongCount < maxWrong) {

        bool inputComplete = false;
        while (true) {
            auto currentTime = chrono::steady_clock::now();
            int elapsedSeconds = chrono::duration_cast<chrono::seconds>(currentTime - startTime).count();
            int remainingTime = timeLimitInSeconds - elapsedSeconds;

            if (timeLimitInSeconds > 0 && remainingTime <= 0) {
                timeOut = true;
                break;
            }

            clear();

            cout << "====== 隨機生成數獨挑戰賽 ======\n";
            cout << " 目前錯誤次數: " << wrongCount << " / " << maxWrong << "\033[K\n\n";

            if (timeLimitInSeconds > 0) {
                cout << " [極限挑戰] 剩餘時間: " << remainingTime / 60 << " 分 " << remainingTime % 60 << " 秒\033[K\n\n";
            }
            else {
                cout << " [計時模式] 已遊玩時間: " << elapsedSeconds / 60 << " 分 " << elapsedSeconds % 60 << " 秒\033[K\n\n";
            }

            display();

            if (!systemMessage.empty()) {
                cout << "\n" << systemMessage << "\033[K\n";
            }
            else {
                cout << "\n\033[K\n";
            }

            cout << "\n請輸入作答 [列] [行] [數字] (例如: 1 3 4): " << inputBuffer << "\033[K";
            cout << flush;

            if (_kbhit()) {
                char ch = _getch();

                if (ch == '\r') {
                    if (!inputBuffer.empty()) {
                        inputComplete = true;
                        break;
                    }
                }
                else if (ch == '\b') {
                    if (!inputBuffer.empty()) {
                        inputBuffer.pop_back();
                    }
                }
                else if (ch >= 32 && ch <= 126) {
                    inputBuffer += ch;
                }
            }

            this_thread::sleep_for(chrono::milliseconds(20));
        }

        if (timeOut) break;

        int r, c, v;
        stringstream ss(inputBuffer);
        inputBuffer = "";
        systemMessage = "";

        if (!(ss >> r >> c >> v)) {
            systemMessage = ">> [提示] 輸入格式錯誤！請輸入三個數字（以空格區隔）。";
            continue;
        }

        // 智慧提示功能
        if (r == 0 && c == 0 && v == 0) {
            if (hintCount < maxHints) {
                int hr, hc, hv;
                if (board.getHint(hr, hc, hv)) {
                    hintCount++;
                    systemMessage = ">> [提示] 系統已自動在第 " + to_string(hr + 1) + " 列、第 " + to_string(hc + 1) + " 行填入正確數字: " + to_string(hv);
                }
                else {
                    systemMessage = ">> [提示] 盤面上已經沒有空格了！";
                }
            }
            else {
                systemMessage = ">> [錯誤] 本局提示次數已用盡！";
            }
            continue;
        }

        r--; c--;

        if (r < 0 || r >= 9 || c < 0 || c >= 9) {
            systemMessage = ">> [提示] 位置超出範圍(1-9)！";
            continue;
        }

        if (board.getCell(r, c) != 0) {
            systemMessage = ">> [提示] 該處已經有數字了！";
            continue;
        }

        if (!board.validateAndSet(r, c, v)) {
            wrongCount++;
            systemMessage = ">> [錯誤] 數字 " + to_string(v) + " 不是正確答案！(剩餘機會: " + to_string(maxWrong - wrongCount) + ")";
        }
        else {
            systemMessage = ">> [成功] 填入正確！";
        }
    }

    cout << "\033[2J\033[1;1H";
    cout << "====== 遊戲結束 ======\n";
    display();

    auto endTime = chrono::steady_clock::now();
    int totalTime = chrono::duration_cast<chrono::seconds>(endTime - startTime).count();

    if (board.isComplete() && !timeOut && wrongCount < maxWrong) {
        cout << "\n恭喜你完成了這則隨機產生的數獨！太強了！\n";
        cout << "總共花費時間: " << totalTime / 60 << " 分 " << totalTime % 60 << " 秒\n";
    }
    else if (timeOut) {
        cout << "\n很遺憾，時間到！挑戰失敗。\n";
    }
    else if (wrongCount >= maxWrong) {
        cout << "\n很遺憾，你已經達到了 3 次錯誤，挑戰失敗。\n";
    }
}