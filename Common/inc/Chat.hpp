/// @file Chat.hpp
/// @brief Declaration of the Chat class.

#pragma once

#include <string>
#include <vector>

#include "Message.hpp"

/**
 * @class Chat
 * @brief Stores data related to chats.
 */
class Chat {
	private:
		std::string name;
		std::vector<std::string> members; // Just the usernames
		std::vector<Message> messages;

	public:
		Chat(std::string name, std::vector<std::string> members, std::vector<Message> messages);
		
		void setName(std::string name);
		std::string getName();
		
		std::vector<std::string> getMembers();
		void addMember(std::string username);
		void delMember(std::string username);

		std::vector<Message> getMessages();
		void addMessage(Message& message);
};

