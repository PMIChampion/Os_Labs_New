#include "Board.h"
#include <algorithm>
#include <cmath>
#include <cassert>

Board::Board() {
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            cells[i][j] = CellState::EMPTY;
        }
    }
}

bool Board::placeShip(int sx, int sy, int ex, int ey) {
    if (!inRange(sx, sy) || !inRange(ex, ey)) return false;

    int length = 0;
    if (sx == ex) {
        length = std::abs(ey - sy) + 1;
    } else if (sy == ey) {
        length = std::abs(ex - sx) + 1;
    } else {
        return false; // не по прямой
    }

    if ((length != 3 && length != 4)) return false;

    // Проверка конфигурации
    if (length == 4 && config.ships4 == 0) return false;
    if (length == 3 && config.ships3 == 0) return false;

    // Проверка занятых клеток
    int stepX = (ex == sx) ? 0 : ((ex > sx) ? 1 : -1);
    int stepY = (ey == sy) ? 0 : ((ey > sy) ? 1 : -1);

    int x = sx;
    int y = sy;
    for (int i = 0; i < length; i++) {
        if (cells[y][x] != CellState::EMPTY) {
            return false;
        }
        x += stepX;
        y += stepY;
    }

    // Установка корабля
    x = sx;
    y = sy;
    for (int i = 0; i < length; i++) {
        cells[y][x] = CellState::SHIP;
        x += stepX;
        y += stepY;
    }

    totalShipCells += length;
    if (length == 4) {
        config.ships4--;
    } else {
        config.ships3--;
    }

    return true;
}

bool Board::allShipsSet() const {
    return config.done();
}

bool Board::isAllSunk() const {
    return totalShipCells == 0;
}

bool Board::inRange(int x, int y) const {
    return (x >= 0 && x < SIZE && y >= 0 && y < SIZE);
}

std::string Board::printBoard() const {
    std::string out;
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            if (cells[i][j] == CellState::SHIP) out += "S ";
            else if (cells[i][j] == CellState::HIT) out += "H ";
            else if (cells[i][j] == CellState::MISS) out += "M ";
            else out += ". ";
        }
        out += "\n";
    }
    return out;
}
