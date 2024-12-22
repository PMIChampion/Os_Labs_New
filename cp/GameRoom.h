#ifndef GAMEROOM_H
#define GAMEROOM_H

#include <string>
#include <vector>
#include <unordered_map>
#include "Board.h"

class GameRoom {
public:
    std::string name;
    std::vector<std::string> players; // ровно 2 игрока
    bool started = false;
    std::unordered_map<std::string, Board> boards;
    int currentPlayerIndex = 0; // чей ход

    bool allBoardsSet() const;
    bool isPlayerTurn(const std::string &player) const;
    void nextTurn();
    std::string getOpponent(const std::string &player) const;
};

#endif // GAMEROOM_H
