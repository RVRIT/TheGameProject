#include "GameSnapshot.h"

crow::json::wvalue toJson(const GameSnapshot& snapshot) {
    crow::json::wvalue j;

    crow::json::wvalue myHandArr;
    for (size_t i = 0; i < snapshot.myHand.size(); ++i) {
        myHandArr[i] = snapshot.myHand[i];
    }
    j["myHand"] = std::move(myHandArr);

    j["cardsPlayedThisTurn"] = snapshot.cardsPlayedThisTurn;
    j["minCardsToPlay"] = snapshot.minCardsToPlay;

    crow::json::wvalue oppArr;
    for (size_t i = 0; i < snapshot.opponents.size(); ++i) {
        oppArr[i]["name"] = snapshot.opponents[i].name;
        oppArr[i]["cardCount"] = snapshot.opponents[i].cardCount;
        oppArr[i]["canMakeMove"] = snapshot.opponents[i].canMakeMove;
        oppArr[i]["isCurrentPlayer"] = snapshot.opponents[i].isCurrentPlayer;
    }
    j["opponents"] = std::move(oppArr);

    crow::json::wvalue pilesArr;
    for (size_t i = 0; i < snapshot.piles.size(); ++i) {
        pilesArr[i]["id"] = snapshot.piles[i].id;
        pilesArr[i]["type"] = snapshot.piles[i].type;
        pilesArr[i]["topValue"] = snapshot.piles[i].topValue;
        pilesArr[i]["cardCount"] = snapshot.piles[i].cardCount;
    }
    j["piles"] = std::move(pilesArr);

    j["deckSize"] = snapshot.deckSize;

    crow::json::wvalue lbArr;
    for (size_t i = 0; i < snapshot.leaderboard.size(); ++i) {
        lbArr[i]["name"] = snapshot.leaderboard[i].name;
        lbArr[i]["score"] = snapshot.leaderboard[i].score;
    }
    j["leaderboard"] = std::move(lbArr);

    j["message"] = snapshot.message;
    j["isGameOver"] = snapshot.isGameOver;
    j["playerWon"] = snapshot.playerWon;

    return j;
}