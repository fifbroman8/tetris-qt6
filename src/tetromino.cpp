#include "tetromino.h"
#include <cstdlib>
#include <ctime>

Tetromino::Tetromino(Type type) : type(type), rotationState(0)
{
    if (type != Type::None) {
        initializeBlocks();
    }
}

void Tetromino::initializeBlocks()
{
    blocks.clear();
    switch (type) {
    case Type::I:
        blocks = {{0, 0}, {0, 1}, {0, 2}, {0, 3}};
        break;
    case Type::O:
        blocks = {{0, 0}, {0, 1}, {1, 0}, {1, 1}};
        break;
    case Type::T:
        blocks = {{0, 1}, {1, 0}, {1, 1}, {1, 2}};
        break;
    case Type::S:
        blocks = {{0, 1}, {0, 2}, {1, 0}, {1, 1}};
        break;
    case Type::Z:
        blocks = {{0, 0}, {0, 1}, {1, 1}, {1, 2}};
        break;
    case Type::J:
        blocks = {{0, 0}, {1, 0}, {1, 1}, {1, 2}};
        break;
    case Type::L:
        blocks = {{0, 2}, {1, 0}, {1, 1}, {1, 2}};
        break;
    default:
        break;
    }
}

int Tetromino::getColor() const
{
    switch (type) {
    case Type::I: return 0x00F0F0; // Cyan
    case Type::O: return 0xFFFF00; // Yellow
    case Type::T: return 0xA000F0; // Purple
    case Type::S: return 0x00F000; // Green
    case Type::Z: return 0xF00000; // Red
    case Type::J: return 0x0000F0; // Blue
    case Type::L: return 0xFFA500; // Orange
    default: return 0xFFFFFF; // White
    }
}

std::vector<std::array<int, 2>> Tetromino::getRotatedBlocks() const
{
    std::vector<std::array<int, 2>> rotated = blocks;

    // Rotate 90 degrees clockwise: (r, c) -> (c, -r)
    for (auto& block : rotated) {
        int temp = block[0];
        block[0] = block[1];
        block[1] = -temp;
    }

    return rotated;
}

void Tetromino::rotate()
{
    if (type == Type::O) return; // O doesn't rotate

    auto rotated = getRotatedBlocks();
    blocks = rotated;
    rotationState = (rotationState + 1) % 4;
}

Tetromino Tetromino::createRandom()
{
    static bool initialized = false;
    if (!initialized) {
        srand(static_cast<unsigned>(time(nullptr)));
        initialized = true;
    }

    Type types[] = {Type::I, Type::O, Type::T, Type::S, Type::Z, Type::J, Type::L};
    return Tetromino(types[rand() % 7]);
}
