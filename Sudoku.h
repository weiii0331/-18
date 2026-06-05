#ifndef SUDOKU_H
#define SUDOKU_H

// Sudoku 類別負責處理數獨的核心邏輯：題目生成、規則檢查、以及正誤判斷
class Sudoku {
public:
    static const int SIZE = 9;

    Sudoku();

    // 依據難度（1:簡單、2:普通、3:困難）隨機生成題目與對應答案
    void generatePuzzle(int difficulty);

    // 取得特定格子的數值
    int getCell(int r, int c) const;

    // 檢查輸入是否符合數獨規則（用於生成階段與安全防錯）
    bool isSafe(int r, int c, int val) const;

    // 核心功能：檢查輸入是否等於此局隨機答案，正確才更新盤面
    bool validateAndSet(int r, int c, int val);

    // 檢查是否全盤完成
    bool isComplete() const;

private:
    int grid[SIZE][SIZE];       // 玩家看到的盤面（包含 0 空格）
    int solution[SIZE][SIZE];   // 該局隨機產生的正確唯一解答

    // 隨機數生成與回溯生成演算法函式
    void fillDiagonal();
    void fillBox(int row, int col);
    bool fillRemaining(int i, int j);
    void removeKDigits(int k);

    // 規則檢查輔助函式
    bool checkRow(int r, int val) const;
    bool checkCol(int c, int val) const;
    bool checkBox(int startRow, int startCol, int val) const;
};

#endif