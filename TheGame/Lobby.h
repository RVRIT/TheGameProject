#pragma once
#pragma once
#include "Player.h"
#include <vector>
#include <memory>
#include "Game.h"
#include <chrono>
struct ChatMessage
{
	std::string sender;
	std::string content;
};
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
	bool addPlayer(const std::string& name);
	void removePlayer(const std::string& name);
	std::vector<Player> getPlayerList() const { return players; }
	int getId() const { return id; }
	Game* getGame() const { return game.get(); }
	LobbyStatus getStatus() const { return status; }
	bool sendChatMessage(const std::string& sender, const std::string& content);
	std::vector<ChatMessage> getChatHistory() const;
private:
	int id;
	std::string hostingPlayer;
	std::vector<Player> players;
	std::unique_ptr<Game> game;
	static const size_t MAX_PLAYERS = 5;
	Lobby::LobbyStatus status = Lobby::LobbyStatus::Waiting;
	std::vector<ChatMessage> chatHistory;
	static const size_t MAX_CHAT_MESSAGES = 100;
};

