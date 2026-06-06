#ifndef GAME_H
#define GAME_H

#include "Sudoku.h"
#include <chrono>

class Game {
public:
    void start();

private:
    Sudoku board;
    int wrongCount;
    int maxWrong;
    int hintCount;
    int maxHints;
    int timeLimitInSeconds;
    std::chrono::steady_clock::time_point startTime;

    void display();
    void clear();
    int getValidDifficulty();
};

#endif