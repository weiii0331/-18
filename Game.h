#ifndef GAME_H
#define GAME_H

#include "Sudoku.h"

// Game 類別負責處理遊戲介面的顯示、錯誤狀態計數、與使用者輸入流程
class Game {
public:
    void start();

private:
    Sudoku board;
    int wrongCount; // 追蹤錯誤次數
    void display();
    void clear();
    int getValidDifficulty(); // 取得玩家選擇的難度
};

#endif
