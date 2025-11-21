#pragma once

#include <vector>
#include <string>
#include <map>


struct PileInfo {
    int id;             
    std::string type;   
    int topValue;       
    int cardCount;      
};

struct OtherPlayerInfo {
    std::string name;
    int cardCount;          
    bool canMakeMove; // Server-calculated. If false while it's this player's turn -> Game Over (Teammate stuck).
    bool isCurrentPlayer; // UI Logic: Highlight this player. If false for local user -> Disable input/buttons.
};

struct GameSnapshot {
    std::vector<int> myHand; 

    int cardsPlayedThisTurn; // Display progress (e.g., "Played: 1").
    int minCardsToPlay;     // Enable "End Turn" button ONLY IF cardsPlayed >= minCards.
    
    std::vector<OtherPlayerInfo> opponents;

    
    std::vector<PileInfo> piles; 
    int deckSize;                

    
    std::string message;   // Global Message
    bool isGameOver;      // If true -> Switch to EndScreen immediately.
    bool playerWon;      // If GameOver is true: true = "VICTORY Screen", false = "DEFEAT Screen".

               
};