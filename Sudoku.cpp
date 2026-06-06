#include "Sudoku.h"
#include <iostream>
#include <ctime>

// 使用自定義線性同餘法 (LCG) 生成隨機數，避免平台相依性問題
static unsigned long long next_seed = 1;

int getManualRand(int limit) {
    next_seed = next_seed * 1103515245 + 12345;
    int result = (unsigned int)(next_seed / 65536) % 32768;
    return result % limit;
}

void setManualSrand(unsigned int seed) {
    next_seed = seed;
}

Sudoku::Sudoku() {
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            grid[i][j] = 0;
            solution[i][j] = 0;
        }
    }
    setManualSrand((unsigned int)std::time(0));
}

void Sudoku::generatePuzzle(int difficulty) {
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            grid[i][j] = 0;
            solution[i][j] = 0;
        }
    }

    fillDiagonal();
    fillRemaining(0, 3);

    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            solution[i][j] = grid[i][j];
        }
    }

    // 1:簡單(30) | 2:普通(42) | 3:困難(54) | 4:極限(54)
    int k = 30;
    if (difficulty == 2) k = 42;
    else if (difficulty == 3 || difficulty == 4) k = 54;

    removeKDigits(k);
}

void Sudoku::fillDiagonal() {
    for (int i = 0; i < SIZE; i = i + 3) {
        fillBox(i, i);
    }
}

void Sudoku::fillBox(int row, int col) {
    int num;
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            do {
                num = getManualRand(SIZE) + 1;
            } while (!checkBox(row, col, num));
            grid[row + i][col + j] = num;
        }
    }
}

bool Sudoku::fillRemaining(int i, int j) {
    if (j >= SIZE && i < SIZE - 1) {
        i = i + 1;
        j = 0;
    }
    if (i >= SIZE && j >= SIZE) return true;
    if (i < 3) {
        if (j < 3) j = 3;
    }
    else if (i < SIZE - 3) {
        if (j == (int)(i / 3) * 3) j = j + 3;
    }
    else {
        if (j == SIZE - 3) {
            i = i + 1;
            j = 0;
            if (i >= SIZE) return true;
        }
    }
    for (int num = 1; num <= SIZE; num++) {
        if (isSafe(i, j, num)) {
            grid[i][j] = num;
            if (fillRemaining(i, j + 1)) return true;
            grid[i][j] = 0;
        }
    }
    return false;
}

void Sudoku::removeKDigits(int k) {
    int count = k;
    while (count != 0) {
        int cellId = getManualRand(SIZE * SIZE);
        int i = cellId / SIZE;
        int j = cellId % SIZE;
        if (grid[i][j] != 0) {
            count--;
            grid[i][j] = 0;
        }
    }
}

int Sudoku::getCell(int r, int c) const {
    return grid[r][c];
}

bool Sudoku::checkRow(int r, int val) const {
    for (int c = 0; c < 9; c++) {
        if (grid[r][c] == val) return false;
    }
    return true;
}

bool Sudoku::checkCol(int c, int val) const {
    for (int r = 0; r < 9; r++) {
        if (grid[r][c] == val) return false;
    }
    return true;
}

bool Sudoku::checkBox(int startRow, int startCol, int val) const {
    for (int r = 0; r < 3; r++) {
        for (int c = 0; c < 3; c++) {
            if (grid[startRow + r][startCol + c] == val) return false;
        }
    }
    return true;
}

bool Sudoku::isSafe(int r, int c, int val) const {
    return checkRow(r, val) &&
        checkCol(c, val) &&
        checkBox(r - r % 3, c - c % 3, val);
}

bool Sudoku::validateAndSet(int r, int c, int val) {
    if (solution[r][c] == val) {
        grid[r][c] = val;
        return true;
    }
    return false;
}

bool Sudoku::getHint(int& outR, int& outC, int& outVal) {
    int emptyCount = 0;
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            if (grid[i][j] == 0) emptyCount++;
        }
    }
    if (emptyCount == 0) return false;

    int target = getManualRand(emptyCount);
    int current = 0;
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            if (grid[i][j] == 0) {
                if (current == target) {
                    grid[i][j] = solution[i][j];
                    outR = i;
                    outC = j;
                    outVal = solution[i][j];
                    return true;
                }
                current++;
            }
        }
    }
    return false;
}

bool Sudoku::isComplete() const {
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            if (grid[i][j] == 0) return false;
        }
    }
    return true;
}