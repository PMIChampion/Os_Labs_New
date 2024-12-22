#include <gtest/gtest.h>
#include "GameRoom.h"
#include "Board.h"

// Тестирование конструктора
TEST(BoardTest, ConstructorInitializesEmptyBoard) {
    Board board;
    for (int i = 0; i < Board::SIZE; ++i) {
        for (int j = 0; j < Board::SIZE; ++j) {
            EXPECT_EQ(board.cells[i][j], CellState::EMPTY);
        }
    }
    EXPECT_EQ(board.config.ships4, 1);
    EXPECT_EQ(board.config.ships3, 2);
    EXPECT_EQ(board.totalShipCells, 0);
}

// Тестирование корректной расстановки корабля длиной 4
TEST(BoardTest, PlaceShipLength4) {
    Board board;
    bool placed = board.placeShip(1, 1, 1, 4); // Вертикально
    EXPECT_TRUE(placed);
    EXPECT_EQ(board.config.ships4, 0);
    EXPECT_EQ(board.totalShipCells, 4);
    for (int y = 1; y <= 4; ++y) {
        EXPECT_EQ(board.cells[y][1], CellState::SHIP);
    }
}

// Тестирование корректной расстановки двух кораблей длиной 3
TEST(BoardTest, PlaceTwoShipsLength3) {
    Board board;
    bool placed1 = board.placeShip(2, 2, 4, 2); // Горизонтально
    bool placed2 = board.placeShip(5, 5, 5, 7); // Вертикально
    EXPECT_TRUE(placed1);
    EXPECT_TRUE(placed2);
    EXPECT_EQ(board.config.ships3, 0);
    EXPECT_EQ(board.totalShipCells, 6);
    for (int x = 2; x <= 4; ++x) {
        EXPECT_EQ(board.cells[2][x], CellState::SHIP);
    }
    for (int y = 5; y <= 7; ++y) {
        EXPECT_EQ(board.cells[y][5], CellState::SHIP);
    }
}

// Тестирование попытки разместить корабль вне поля
TEST(BoardTest, PlaceShipOutOfRange) {
    Board board;
    bool placed = board.placeShip(8, 8, 8, 12); // Вне вертикали
    EXPECT_FALSE(placed);
    EXPECT_EQ(board.config.ships4, 1);
    EXPECT_EQ(board.totalShipCells, 0);
}

// Тестирование попытки разместить корабль по диагонали
TEST(BoardTest, PlaceShipDiagonal) {
    Board board;
    bool placed = board.placeShip(0, 0, 3, 3); // Диагонально
    EXPECT_FALSE(placed);
    EXPECT_EQ(board.config.ships4, 1);
    EXPECT_EQ(board.totalShipCells, 0);
}

// Тестирование пересечения кораблей
TEST(BoardTest, PlaceShipOverlap) {
    Board board;
    bool placed1 = board.placeShip(1, 1, 1, 4); // Вертикально
    bool placed2 = board.placeShip(1, 3, 3, 3); // Горизонтально пересекается на (1,3)
    EXPECT_TRUE(placed1);
    EXPECT_FALSE(placed2);
    EXPECT_EQ(board.config.ships4, 0);
    EXPECT_EQ(board.config.ships3, 2);
    EXPECT_EQ(board.totalShipCells, 4);
    for (int y = 1; y <= 4; ++y) {
        EXPECT_EQ(board.cells[y][1], CellState::SHIP);
    }
}

// Тестирование проверки, все ли корабли расставлены
TEST(BoardTest, AllShipsSet) {
    Board board;
    board.placeShip(0, 0, 0, 3); // 4-клеточный
    board.placeShip(2, 2, 4, 2); // 3-клеточный
    board.placeShip(5, 5, 5, 7); // 3-клеточный
    EXPECT_TRUE(board.allShipsSet());
}

// Тестирование проверки, все ли корабли потоплены
TEST(BoardTest, AllShipsSunk) {
    Board board;
    board.placeShip(0, 0, 0, 3); // 4-клеточный
    board.placeShip(2, 2, 4, 2); // 3-клеточный
    board.placeShip(5, 5, 5, 7); // 3-клеточный

    // Потопить все корабли
    for (int y = 0; y <= 3; ++y) {
        board.cells[y][0] = CellState::HIT;
    }
    for (int x = 2; x <= 4; ++x) {
        board.cells[2][x] = CellState::HIT;
    }
    for (int y = 5; y <= 7; ++y) {
        board.cells[y][5] = CellState::HIT;
    }

    board.totalShipCells = 0;
    EXPECT_TRUE(board.isAllSunk());
}

TEST(GameRoomTest, AllBoardsSet) {
    GameRoom gr;
    gr.players.push_back("Player1");
    gr.players.push_back("Player2");
    gr.boards["Player1"].placeShip(0, 0, 0, 3); // 4-клеточный
    gr.boards["Player1"].placeShip(1, 1, 3, 1); // 3-клеточный
    gr.boards["Player1"].placeShip(4, 4, 4, 6); // 3-клеточный

    gr.boards["Player2"].placeShip(5, 5, 5, 8); // 4-клеточный
    gr.boards["Player2"].placeShip(6, 6, 8, 6); // 3-клеточный
    gr.boards["Player2"].placeShip(9, 9, 9, 7); // 3-клеточный

    EXPECT_TRUE(gr.allBoardsSet());
}

// Тестирование метода isPlayerTurn
TEST(GameRoomTest, IsPlayerTurn) {
    GameRoom gr;
    gr.players.push_back("Player1");
    gr.players.push_back("Player2");
    gr.currentPlayerIndex = 0;

    EXPECT_TRUE(gr.isPlayerTurn("Player1"));
    EXPECT_FALSE(gr.isPlayerTurn("Player2"));

    gr.nextTurn();
    EXPECT_FALSE(gr.isPlayerTurn("Player1"));
    EXPECT_TRUE(gr.isPlayerTurn("Player2"));
}
