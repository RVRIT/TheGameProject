#include "lobby.h"
#include <algorithm>

Lobby::Lobby() : maxPlayers(4), gameStarted(false) {}

Lobby::~Lobby() {}

bool Lobby::AddPlayer(int id, const std::string& name) {
    if (gameStarted || players.size() >= maxPlayers) {
        return false;
    }

    for (const auto& p : players) {
        if (p.id == id) return false;
    }

    players.push_back({ id, name, false });
    return true;
}

void Lobby::RemovePlayer(int id) {
    players.erase(
        std::remove_if(players.begin(), players.end(),
            [id](const PlayerInfo& p) { return p.id == id; }),
        players.end()
    );
}

void Lobby::SetPlayerReady(int id, bool ready) {
    for (auto& p : players) {
        if (p.id == id) {
            p.isReady = ready;
            break;
        }
    }
}

bool Lobby::IsAllReady() const {
    if (players.empty()) return false;
    for (const auto& p : players) {
        if (!p.isReady) return false;
    }
    return true;
}

GameSnapshot Lobby::CreateGameSnapshot() {
    if (!IsAllReady()) {
        return GameSnapshot();
    }

    gameStarted = true;
    return GameSnapshot(players);
}

const std::vector<PlayerInfo>& Lobby::GetPlayers() const {
    return players;
}

void Lobby::SetMaxPlayers(int max) {
    maxPlayers = max;
}