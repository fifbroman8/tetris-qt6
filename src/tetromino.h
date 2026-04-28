#ifndef TETROMINO_H
#define TETROMINO_H

#include <vector>
#include <array>

class Tetromino
{
public:
    enum class Type
    {
        I, O, T, S, Z, J, L, None
    };

    Tetromino(Type type = Type::None);

    Type getType() const { return type; }
    int getColor() const;
    const std::vector<std::array<int, 2>>& getBlocks() const { return blocks; }
    std::vector<std::array<int, 2>> getRotatedBlocks() const;
    void rotate();

    static Tetromino createRandom();

private:
    Type type;
    std::vector<std::array<int, 2>> blocks; // {row, col} relative positions
    int rotationState = 0;

    void initializeBlocks();
};

#endif // TETROMINO_H
