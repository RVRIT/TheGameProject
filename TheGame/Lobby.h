#ifndef LOBBY_H
#define LOBBY_H

#include <vector>
#include <string>
#include <memory>
#include "Game.h"
#include "GameSnapshot.h"

using json = nlohmann::json;

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
    ~Lobby();

    bool addPlayer(const std::string& name);
    void removePlayer(int id);
    void setPlayerReady(int id, bool ready);
    bool isAllReady() const;

    bool sendChatMessage(const std::string& sender, const std::string& content);
    std::vector<ChatMessage> getChatHistory() const;

    GameSnapshot CreateGameSnapshot();

    const std::vector<PlayerInfo>& getPlayers() const;
    LobbyStatus getStatus() const;
    void setMAX_PLAYERS(int max);
    json getStateJSON() const;

private:
    int id;
    std::unique_ptr<Game> game;
    std::vector<PlayerInfo> players;
    std::vector<ChatMessage> chatHistory;
    LobbyStatus status;
    static const size_t MAX_PLAYERS = 5;
    static const size_t MAX_CHAT_MESSAGES = 100;
};

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(PlayerInfo, id, name, isReady)
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(ChatMessage, sender, content)

#endif