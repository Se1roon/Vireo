/// @file PacketManager.hpp
/// @brief Declaration of PacketManager class and several structures for each packet type. 

#pragma once

#include <string>
#include <optional>
#include <vector>
#include <utility>

#include <SFML/Network.hpp>

#include "User.hpp"
#include "Chat.hpp"


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

struct NewMessageData {
	std::string chat_name;
	std::string msg_author;
	std::string msg_content;
	std::string peer_username;
};


/**
 * @class PacketManager
 * @brief Handles packing and unpacking of network packets.
 *
 */
class PacketManager {
	public:
		sf::Packet create_login_packet(User& user);
		sf::Packet create_login_response_packet(User& user);
		sf::Packet create_register_packet(User& user);
		sf::Packet create_register_response_packet(bool success);
		sf::Packet create_search_packet(std::string search_term);
		sf::Packet create_search_response_packet(std::vector<std::string> usernames);
		sf::Packet create_new_chat_packet(std::string username);
		sf::Packet create_new_chat_response_packet(Chat& chat);
		sf::Packet create_new_message_packet(std::string author, std::string content, Chat& chat);
		sf::Packet create_new_message_response_packet(std::string chat_name, Message& message);

		std::optional<LoginData> extract_login_packet(sf::Packet& packet);
		std::optional<ResponseLoginData> extract_login_response_packet(sf::Packet& packet);
		std::optional<RegisterData> extract_register_packet(sf::Packet& packet);
		bool extract_register_response_packet(sf::Packet& packet);
		std::optional<NewMessageData> extract_new_message_packet(sf::Packet& packet);
		std::pair<Message, std::string> extract_new_message_response_packet(sf::Packet& packet);
		std::optional<std::string> extract_search_packet(sf::Packet& packet);
		std::optional<std::vector<std::string>> extract_search_response_packet(sf::Packet& packet);
		std::optional<std::string> extract_new_chat_packet(sf::Packet& packet);
		Chat extract_new_chat_response_packet(sf::Packet& packet);
};

