#ifndef GAME_H
#define GAME_H

#include "Sudoku.h"

class Game {
public:
    void start();

private:
    Sudoku board;
    int wrongCount; // °lÂÜ¿ù»~¦¸¼Æ
    void display();
    void clear();
};

#endif