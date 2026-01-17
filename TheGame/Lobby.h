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
    explicit Lobby(int id, const std::string& hostName, LobbyStatus m_status = LobbyStatus::Waiting)
        : id{ id }, m_status{ m_status }
    {
        addPlayer(hostName);
    }
    ~Lobby() = default;

    bool addPlayer(const std::string& name);
    void removePlayer(int id);
    bool setPlayerReady(int id, bool ready);
    bool isAllReady() const;
    bool tryStartGame(int requestPlayerId);

    bool sendChatMessage(const std::string& sender, const std::string& content);
    std::vector<ChatMessage> getChatHistory() const;

    GameSnapshot CreateGameSnapshot();
    Game* getGame() { return m_game.get(); }
    const Game* getGame() const { return m_game.get(); }
    const std::vector<PlayerInfo>& getPlayers() const;
    bool isPlayerInLobby(const std::string& playerName);
    LobbyStatus getStatus() const;
    crow::json::wvalue getStateJson() const;
    void resetGame();

private:
    int id;
    std::unique_ptr<Game> m_game;
    std::vector<PlayerInfo> m_players;
    std::vector<ChatMessage> m_chatHistory;
    LobbyStatus m_status;
    static const size_t MAX_PLAYERS = 5;
    static const size_t MAX_CHAT_MESSAGES = 100;
};