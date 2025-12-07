#include "Lobby.h"
bool Lobby::addPlayer(const std::string& name) {
	if (status != LobbyStatus::Waiting) {
		return false;
	}
	if (players.size() >= MAX_PLAYERS) {
		return false;
	}
	players.emplace_back(name);
	return true;
}

void Lobby::removePlayer(const std::string & name)
{
	players.erase(
		std::remove_if(players.begin(), players.end(),
			[&name](const Player& p) {
				return p.getName() == name;
			}),
		players.end()
	);
}

bool Lobby::sendChatMessage(const std::string& sender, const std::string& content) {
	if (content.empty() || sender.empty()) return false;

	chatHistory.push_back({
		sender,
		content,
		});
	if (chatHistory.size() > MAX_CHAT_MESSAGES) {
		chatHistory.erase(chatHistory.begin());
	}
	return true;
}

std::vector<ChatMessage> Lobby::getChatHistory() const {
	return chatHistory;
}