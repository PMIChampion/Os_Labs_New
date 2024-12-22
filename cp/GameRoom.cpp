#include "GameRoom.h"
#include <algorithm>

bool GameRoom::allBoardsSet() const {
    if (players.size() < 2) return false;
    for (const auto &p : players) {
        if (!boards.at(p).allShipsSet()) return false;
    }
    return true;
}

bool GameRoom::isPlayerTurn(const std::string &player) const {
    if (players.empty()) return false;
    return players[currentPlayerIndex] == player;
}

void GameRoom::nextTurn() {
    currentPlayerIndex = (currentPlayerIndex + 1) % players.size();
}

std::string GameRoom::getOpponent(const std::string &player) const {
    for (const auto &p : players) {
        if (p != player) return p;
    }
    return "";
}
