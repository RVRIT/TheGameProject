#ifndef LOBBY_H
#define LOBBY_H

#include <vector>
#include <string>
#include <memory>
#include "json.hpp"
#include "gamesnapshot.h"

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
    Lobby();
    ~Lobby();

    bool AddPlayer(int id, const std::string& name);
    void RemovePlayer(int id);
    void SetPlayerReady(int id, bool ready);
    bool IsAllReady() const;

    bool SendChatMessage(const std::string& sender, const std::string& content);
    std::vector<ChatMessage> GetChatHistory() const;

    GameSnapshot CreateGameSnapshot();

    const std::vector<PlayerInfo>& GetPlayers() const;
    LobbyStatus GetStatus() const;
    void SetMaxPlayers(int max);

    json GetStateJSON() const;

private:
    std::vector<PlayerInfo> players;
    std::vector<ChatMessage> chatHistory;
    LobbyStatus status;
    int maxPlayers;
    static const size_t MAX_CHAT_MESSAGES = 100;
};

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(PlayerInfo, id, name, isReady)
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(ChatMessage, sender, content)

#endif