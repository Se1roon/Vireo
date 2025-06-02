/// @file PacketManager.cpp
/// @brief Implementation of PacketManager class.

#include <iostream>
#include <string>
#include <optional>

#include <SFML/Network.hpp>

#include "PacketManager.hpp"
#include "PacketExceptions.hpp"
#include "User.hpp"
#include "Chat.hpp"
#include "Message.hpp"


sf::Packet PacketManager::create_login_packet(User& user) {
	sf::Packet packet;

	packet << "L" << user.getUsername() << user.getPassword();

	return packet;
}

sf::Packet PacketManager::create_register_packet(User& user) {
	sf::Packet packet;

	packet << "R" << user.getUsername() << user.getEmail() << user.getPassword();

	return packet;
}


sf::Packet PacketManager::create_login_response_packet(User& user) {
	sf::Packet packet;

	std::vector<Chat> chats = user.getChats();

	packet << "LR" << user.getUsername() << user.getEmail() << user.getPassword();

	std::uint32_t chats_count = static_cast<std::uint32_t>(chats.size());

	packet << chats_count;

	for (Chat& chat : chats) {
		packet << chat.getName();

		auto members = chat.getMembers();
		std::uint32_t members_count = static_cast<std::uint32_t>(members.size());
		packet << members_count;
		for (std::string member : members)
			packet << member;

		auto messages = chat.getMessages();
		std::uint32_t messages_count = static_cast<std::uint32_t>(messages.size());
		packet << messages_count;
		for (Message& message : messages)
			packet << message.getAuthor() << message.getContent();
	}

	return packet;
}

sf::Packet PacketManager::create_register_response_packet(bool success) {
	sf::Packet packet;
	packet << "RR";

	if (success)
		packet << "S";
	else
		packet << "F";

	return packet;
}

sf::Packet PacketManager::create_search_packet(std::string search_term) {
	sf::Packet packet;
	packet << "S" << search_term;

	return packet;
}

sf::Packet PacketManager::create_search_response_packet(std::vector<std::string> usernames) {
	sf::Packet packet;
	packet << "SR";

	int uname_count = usernames.size();
	packet << uname_count;

	for (std::string username : usernames)
		packet << username;

	return packet;
}

sf::Packet PacketManager::create_new_chat_packet(std::string username) {
	sf::Packet packet;
	packet << "NH" << username;

	return packet;
}

std::optional<std::string> PacketManager::extract_new_chat_packet(sf::Packet& packet) {
	std::string username;
	packet >> username;
	return username;
}

sf::Packet PacketManager::create_new_chat_response_packet(bool success) {
	sf::Packet packet;
	packet << "NHR";
	if (success) packet << "S";
	else packet << "F";

	return packet;
}

bool PacketManager::extract_new_chat_response_packet(sf::Packet& packet) {
	std::string type;
	packet >> type;
	if (type != "NHR") return false;

	std::string state;
	packet >> state;

	return state == "S";
}

std::optional<std::vector<std::string>> PacketManager::extract_search_response_packet(sf::Packet& packet) {
	std::string type;
	packet >> type;
	if (type != "SR")
		return std::nullopt;

	int count = 0;
	packet >> count;

	std::vector<std::string> usernames;
	for (int i = 0; i < count; i++) {
		std::string username;
		packet >> username;
		usernames.push_back(username);
	}

	return usernames;
}

std::optional<std::string> PacketManager::extract_search_packet(sf::Packet& packet) {
	std::string search_term;
	packet >> search_term;

	return search_term;
}


bool PacketManager::extract_register_response_packet(sf::Packet& packet) {
	std::string type;
	packet >> type;
	if (type != "RR") {
		std::cout << "Invalid packet type!\n";
		return false;
	}

	std::string status;
	packet >> status;

	return (status == "S");
}


sf::Packet PacketManager::create_new_message_packet(std::string content, Chat& chat) {
	sf::Packet packet;

	packet << "NM" << chat.getName() << content; // the server will know how sent it
	
	return packet;
}

std::optional<NewMessageData> PacketManager::extract_new_message_packet(sf::Packet& packet) {
	/*
	std::string type;
	packet >> type;
	if (type != "NM") {
		std::cout << "Invalid packet type\n";
		return std::nullopt;
	}
	*/

	NewMessageData nmdata;
	packet >> nmdata.chat_name >> nmdata.msg_content;

	return nmdata;
}

std::optional<ResponseLoginData> PacketManager::extract_login_response_packet(sf::Packet& packet) {
	std::string type;
	packet >> type;
	if (type != "LR") {
		std::cout << "Invalid packet type!\n";
		return std::nullopt;
	}

	ResponseLoginData lrdata;

	packet >> lrdata.username >> lrdata.email >> lrdata.password;

	std::uint32_t chat_count = 0;
	std::uint32_t members_count = 0;
	std::uint32_t message_count = 0;

	packet >> chat_count;

	std::vector<Chat> chats;
	for (std::uint32_t i = 0; i < chat_count; i++) {
		std::vector<std::string> members;
		std::vector<Message> messages;

		std::string chat_name;
		packet >> chat_name >> members_count;

		for (std::uint32_t j = 0; j < members_count; j++) {
			std::string username;
			packet >> username;
			members.push_back(username);
		}
		packet >> message_count;
		for (std::uint32_t k = 0; k < message_count; k++) {
			std::string author;
			std::string content;
			packet >> author >> content;
			messages.push_back({content, author});
		}

		chats.emplace_back(chat_name, members, messages);
	}

	lrdata.chats = chats;

	return lrdata;
}


std::optional<LoginData> PacketManager::extract_login_packet(sf::Packet& packet) {
	LoginData ldata;

	if ((packet >> ldata.username >> ldata.password)) {
		packet.clear();
		return ldata;
	}

	return std::nullopt;	
}

std::optional<RegisterData> PacketManager::extract_register_packet(sf::Packet& packet) {
	RegisterData rdata;

	if ((packet >> rdata.username >> rdata.email >> rdata.password)) {
		packet.clear();
		return rdata;
	}

	return std::nullopt;
}

