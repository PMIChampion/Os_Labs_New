#ifndef BOARD_H
#define BOARD_H

#include <string>

enum class CellState {
    EMPTY,
    SHIP,
    HIT,
    MISS
};

struct ShipConfig {
    int ships4 = 1;
    int ships3 = 2;

    bool done() const {
        return ships4 == 0 && ships3 == 0;
    }
};

class Board {
public:
    static const int SIZE = 10;
    CellState cells[SIZE][SIZE];
    ShipConfig config;
    int totalShipCells = 0; // количество клеток кораблей на поле

    Board();

    bool placeShip(int sx, int sy, int ex, int ey);
    bool allShipsSet() const;
    bool isAllSunk() const;
    bool inRange(int x, int y) const;
    std::string printBoard() const;
};

#endif // BOARD_H
