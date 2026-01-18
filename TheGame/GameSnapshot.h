#pragma once

#include <vector>
#include <string>
#include <map>
#include <crow.h>
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

    std::vector<OtherPlayerInfo> players;

    std::vector<PileInfo> piles;
    int deckSize;

    std::vector<LeaderboardEntry> leaderboard;

    std::string message;
    bool isGameOver;
    bool playerWon;
    crow::json::wvalue toJson(const GameSnapshot& snapshot);
};
