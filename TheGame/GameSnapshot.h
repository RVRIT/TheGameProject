#pragma once

#include <vector>
#include <string>
#include <map>
#include "json.hpp"

using json = nlohmann::json;

struct LeaderboardEntry {
    std::string name;
    int score;
};

struct PileInfo {
    int id;
    std::string type;
    int topValue;
    int cardCount;
};

struct OtherPlayerInfo {
    std::string name;
    int cardCount;
    bool canMakeMove;
    bool isCurrentPlayer;
};

struct GameSnapshot {
    std::vector<int> myHand;
    int cardsPlayedThisTurn;
    int minCardsToPlay;

    std::vector<OtherPlayerInfo> opponents;

    std::vector<PileInfo> piles;
    int deckSize;

    std::vector<LeaderboardEntry> leaderboard;

    std::string message;
    bool isGameOver;
    bool playerWon;
};

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(LeaderboardEntry, name, score)
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(PileInfo, id, type, topValue, cardCount)
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(OtherPlayerInfo, name, cardCount, canMakeMove, isCurrentPlayer)
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(GameSnapshot, myHand, cardsPlayedThisTurn, minCardsToPlay, opponents, piles, deckSize, leaderboard, message, isGameOver, playerWon)