#ifndef SUDOKU_H
#define SUDOKU_H

class Sudoku {
public:
    static const int SIZE = 9;

    Sudoku();

    // 取得格子的數值
    int getCell(int r, int c) const;

    // 核心功能：檢查輸入是否等於正確答案，正確才填入
    bool validateAndSet(int r, int c, int val);

    // 檢查是否全盤完成
    bool isComplete() const;

private:
    int grid[SIZE][SIZE];       // 玩家看到的盤面
    int solution[SIZE][SIZE];   // 預設的正確答案
};

#endif