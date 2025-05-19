/// @file User.hpp
/// @brief Declaration of the User class.

#pragma once 

#include <string>
#include <vector>

#include <SFML/Network.hpp>

#include "Chat.hpp"

/**
 * @class User
 * @brief Responsible for holding necessary data related to the user.
 *
 */
class User {
	private:
		std::string username;
		std::string password;
		std::string email;
		std::vector<Chat> chats;

	public:
		User(std::string username, std::string password, std::string email);

		std::string getUsername();
		std::string getPassword();
		std::string getEmail();
		std::vector<Chat> getChats();

		void setUsername(std::string username);
		void setPassword(std::string password);
		void setEmail(std::string email);
		
		void joinChat(Chat& chat);
		void leaveChat(Chat& chat);

		User operator=(User const& u) const;
};

