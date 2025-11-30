#pragma once
#pragma once
#include "Player.h"
#include <vector>
#include <memory>
#include "Game.h"
class Lobby
{
public:
	enum class LobbyStatus
	{
		Waiting,
		InProgress,
		Finished
	};
	Lobby() = default;
	explicit Lobby(int id, const std::string& hostName, LobbyStatus status = LobbyStatus::Waiting)
		: id{ id }, hostingPlayer{ hostName }, status{ status }
	{
		addPlayer(hostName);
	}
	bool addPlayer(const std::string& name) {
		if (players.size() >= MAX_PLAYERS) { return false; }
		players.emplace_back(name);
		return true;
	}
	void removePlayer(const std::string& name)
	{
		players.erase(
			std::remove_if(players.begin(), players.end(),
				[&name](const Player& p) {
					return p.getName() == name;
				}),
			players.end()
		);
	}
	std::vector<Player> getPlayerList() const { return players; }
	int getId() const { return id; }
	Game* getGame() const { return game.get(); }
	LobbyStatus getStatus() const { return status; }
private:
	int id;
	std::string hostingPlayer;
	std::vector<Player> players;
	std::unique_ptr<Game> game;
	static const size_t MAX_PLAYERS = 5;
	Lobby::LobbyStatus status = Lobby::LobbyStatus::Waiting;

};

