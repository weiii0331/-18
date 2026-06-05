#ifndef GAME_H
#define GAME_H

#include "Sudoku.h"
#include <chrono>


// Game 類別負責處理遊戲介面的顯示、錯誤狀態計數、與使用者輸入流程
class Game {
public:
    void start();

private:
    Sudoku board;
    int wrongCount; // 追蹤錯誤次數

    // ------ 新增時間相關變數 ------
    std::chrono::time_point<std::chrono::steady_clock> startTime; // 記錄開始時間
    int timeLimitInSeconds; // 總限時（秒）
    // ----------------------------

    void display();
    void clear();
    int getValidDifficulty(); // 取得玩家選擇的難度
};

#endif
