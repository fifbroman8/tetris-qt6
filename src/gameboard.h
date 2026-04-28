#ifndef GAMEBOARD_H
#define GAMEBOARD_H

#include "tetromino.h"
#include <vector>
#include <array>

class GameBoard
{
public:
    static constexpr int BOARD_WIDTH = 10;
    static constexpr int BOARD_HEIGHT = 20;

    GameBoard();

    // Game logic
    bool canMove(const Tetromino& piece, int row, int col) const;
    bool canRotate(const Tetromino& piece, int row, int col) const;
    void placePiece(const Tetromino& piece, int row, int col);
    int clearLines();
    bool isGameOver() const;

    // Getters
    int getCellColor(int row, int col) const;
    const Tetromino& getCurrentPiece() const { return currentPiece; }
    const Tetromino& getNextPiece() const { return nextPiece; }
    int getCurrentRow() const { return currentRow; }
    int getCurrentCol() const { return currentCol; }
    int getScore() const { return score; }
    int getLevel() const { return level; }
    int getLines() const { return lines; }
    bool isPaused() const { return paused; }

    // Game control
    void newGame();
    void moveLeft();
    void moveRight();
    void moveDown();
    void dropDown();
    void rotatePiece();
    void togglePause();
    void update(); // Called on timer tick

private:
    std::vector<std::vector<int>> board; // 0 = empty, 1-7 = color
    Tetromino currentPiece;
    Tetromino nextPiece;
    int currentRow = 0;
    int currentCol = 0;
    int score = 0;
    int lines = 0;
    int level = 1;
    bool paused = false;
    int dropCounter = 0;

    void spawnNewPiece();
    bool isLineFull(int row) const;
    void removeLine(int row);
};

#endif // GAMEBOARD_H
