#ifndef SUDOKU_H
#define SUDOKU_H

class Sudoku {
public:
    static const int SIZE = 9;

    Sudoku();

    // 依據難度隨機生成題目與對應答案
    void generatePuzzle(int difficulty);

    // 取得特定格子的數值
    int getCell(int r, int c) const;

    // 檢查輸入是否符合數獨規則
    bool isSafe(int r, int c, int val) const;

    // 核心驗證功能
    bool validateAndSet(int r, int c, int val);

    // 智慧提示功能
    bool getHint(int& outR, int& outC, int& outVal);

    // 檢查是否完成
    bool isComplete() const;

private:
    int grid[SIZE][SIZE];
    int solution[SIZE][SIZE];

    // 隨機生成演算法
    void fillDiagonal();
    void fillBox(int row, int col);
    bool fillRemaining(int i, int j);
    void removeKDigits(int k);

    // 規則檢查輔助
    bool checkRow(int r, int val) const;
    bool checkCol(int c, int val) const;
    bool checkBox(int startRow, int startCol, int val) const;
};

#endif
