#pragma once


#include <vector>
#include <string>
#include <memory>
#include "Game.h"
#include "GameSnapshot.h"
#include <crow.h>


struct PlayerInfo {
    int id;
    std::string name;
    bool isReady;
    float rating;
};

struct ChatMessage {
    std::string sender;
    std::string content;
};

enum class LobbyStatus {
    Waiting,
    InProgress,
    Finished
};

class Lobby {
public:
    Lobby() = default;
    explicit Lobby(int id, const std::string& hostName, float hostRating, LobbyStatus m_status = LobbyStatus::Waiting);
      
    ~Lobby() = default;

    bool addPlayer(const std::string& name, float playerRating);
    void removePlayer(int id);
    bool setPlayerReady(int id, bool ready);
    bool isAllReady() const;
    bool tryStartGame(int requestPlayerId);

    bool sendChatMessage(const std::string& sender, const std::string& content);
    std::vector<ChatMessage> getChatHistory() const;

    GameSnapshot CreateGameSnapshot();
    Game* getGame();
    const Game* getGame() const;
    const std::vector<PlayerInfo>& getPlayers() const;
    bool isPlayerInLobby(const std::string& playerName);
    LobbyStatus getStatus() const;
    crow::json::wvalue getStateJson() const;
    float getAverageRating() const; 
    void resetGame();

private:
    int id;
    std::unique_ptr<Game> m_game;
    std::vector<PlayerInfo> m_players;
    std::vector<ChatMessage> m_chatHistory;
    float m_averageRating;
    LobbyStatus m_status;
    static const size_t MAX_PLAYERS = 5;
    static const size_t MAX_CHAT_MESSAGES = 100;
};