#ifndef LOBBY_H
#define LOBBY_H

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
    explicit Lobby(int id, const std::string& hostName, LobbyStatus status = LobbyStatus::Waiting)
        : id{ id }, status{ status }
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
    Game* getGame() { return game.get(); }
    const Game* getGame() const { return game.get(); }
    const std::vector<PlayerInfo>& getPlayers() const;
    LobbyStatus getStatus() const;
    void setMAX_PLAYERS(int max);
    crow::json::wvalue getStateJSON() const;
    void resetGame();

private:
    int id;
    std::unique_ptr<Game> game;
    std::vector<PlayerInfo> players;
    std::vector<ChatMessage> chatHistory;
    LobbyStatus status;
    static const size_t MAX_PLAYERS = 5;
    static const size_t MAX_CHAT_MESSAGES = 100;
};

#endif