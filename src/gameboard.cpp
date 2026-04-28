#include "gameboard.h"
#include <algorithm>

GameBoard::GameBoard()
    : board(BOARD_HEIGHT, std::vector<int>(BOARD_WIDTH, 0)),
      currentPiece(Tetromino::Type::None),
      nextPiece(Tetromino::createRandom())
{
    newGame();
}

void GameBoard::newGame()
{
    for (auto& row : board) {
        std::fill(row.begin(), row.end(), 0);
    }
    score = 0;
    lines = 0;
    level = 1;
    paused = false;
    dropCounter = 0;
    spawnNewPiece();
}

void GameBoard::spawnNewPiece()
{
    currentPiece = nextPiece;
    nextPiece = Tetromino::createRandom();
    currentRow = 0;
    currentCol = BOARD_WIDTH / 2 - 2;
    dropCounter = 0;
}

bool GameBoard::canMove(const Tetromino& piece, int row, int col) const
{
    for (const auto& block : piece.getBlocks()) {
        int newRow = row + block[0];
        int newCol = col + block[1];

        // Check boundaries
        if (newRow < 0 || newRow >= BOARD_HEIGHT ||
            newCol < 0 || newCol >= BOARD_WIDTH) {
            return false;
        }

        // Check collision with placed pieces
        if (board[newRow][newCol] != 0) {
            return false;
        }
    }
    return true;
}

bool GameBoard::canRotate(const Tetromino& piece, int row, int col) const
{
    Tetromino rotated = piece;
    rotated.rotate();
    return canMove(rotated, row, col);
}

void GameBoard::placePiece(const Tetromino& piece, int row, int col)
{
    int color = piece.getColor();
    for (const auto& block : piece.getBlocks()) {
        int newRow = row + block[0];
        int newCol = col + block[1];
        if (newRow >= 0 && newRow < BOARD_HEIGHT &&
            newCol >= 0 && newCol < BOARD_WIDTH) {
            board[newRow][newCol] = color;
        }
    }
}

bool GameBoard::isLineFull(int row) const
{
    for (int col = 0; col < BOARD_WIDTH; ++col) {
        if (board[row][col] == 0) {
            return false;
        }
    }
    return true;
}

void GameBoard::removeLine(int row)
{
    board.erase(board.begin() + row);
    board.insert(board.begin(), std::vector<int>(BOARD_WIDTH, 0));
}

int GameBoard::clearLines()
{
    int linesCleared = 0;
    for (int row = BOARD_HEIGHT - 1; row >= 0; --row) {
        if (isLineFull(row)) {
            removeLine(row);
            linesCleared++;
            row++; // Check this row again
        }
    }

    if (linesCleared > 0) {
        lines += linesCleared;
        // Scoring: 1 line = 100, 2 lines = 300, 3 lines = 500, 4 lines = 800
        int scoreTable[] = {0, 100, 300, 500, 800};
        score += scoreTable[std::min(linesCleared, 4)] * level;
        level = 1 + lines / 10;
    }

    return linesCleared;
}

bool GameBoard::isGameOver() const
{
    for (int col = 0; col < BOARD_WIDTH; ++col) {
        if (board[0][col] != 0) {
            return true;
        }
    }
    return false;
}

void GameBoard::moveLeft()
{
    if (canMove(currentPiece, currentRow, currentCol - 1)) {
        currentCol--;
    }
}

void GameBoard::moveRight()
{
    if (canMove(currentPiece, currentRow, currentCol + 1)) {
        currentCol++;
    }
}

void GameBoard::moveDown()
{
    if (canMove(currentPiece, currentRow + 1, currentCol)) {
        currentRow++;
    } else {
        placePiece(currentPiece, currentRow, currentCol);
        clearLines();
        spawnNewPiece();
    }
}

void GameBoard::dropDown()
{
    while (canMove(currentPiece, currentRow + 1, currentCol)) {
        currentRow++;
    }
    placePiece(currentPiece, currentRow, currentCol);
    clearLines();
    spawnNewPiece();
}

void GameBoard::rotatePiece()
{
    if (canRotate(currentPiece, currentRow, currentCol)) {
        currentPiece.rotate();
    }
}

void GameBoard::togglePause()
{
    paused = !paused;
}

void GameBoard::update()
{
    if (paused) return;

    dropCounter++;
    int dropSpeed = std::max(1, 10 - level); // Faster as level increases

    if (dropCounter >= dropSpeed) {
        moveDown();
        dropCounter = 0;
    }
}

int GameBoard::getCellColor(int row, int col) const
{
    if (row < 0 || row >= BOARD_HEIGHT || col < 0 || col >= BOARD_WIDTH) {
        return 0;
    }

    // Check if current piece occupies this cell
    for (const auto& block : currentPiece.getBlocks()) {
        if (currentRow + block[0] == row && currentCol + block[1] == col) {
            return currentPiece.getColor();
        }
    }

    return board[row][col];
}
