/// @file PacketManager.hpp
/// @brief Declaration of PacketManager class and several structures for each packet type. 

#pragma once

#include <string>
#include <optional>

#include <SFML/Network.hpp>

#include "User.hpp"


struct LoginData {
	std::string username;
	std::string password;
};

struct RegisterData {
	std::string username;
	std::string email;
	std::string password;
};

struct ResponseLoginData {
	std::string username;
	std::string email;
	std::string password;
	std::vector<Chat> chats;
};


/**
 * @class PacketManager
 * @brief Handles packing and unpacking of network packets.
 *
 */
class PacketManager {
	public:
		sf::Packet create_login_packet(User& user);
		sf::Packet create_register_packet(User& user);

		sf::Packet create_login_response_packet(User& user);

		std::optional<ResponseLoginData> extract_login_response_packet(sf::Packet& packet);

		std::optional<LoginData> extract_login_packet(sf::Packet& packet);
		std::optional<RegisterData> extract_register_packet(sf::Packet& packet);
};

