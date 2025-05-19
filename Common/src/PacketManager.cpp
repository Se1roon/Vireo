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

	packet << "LR" << user.getUsername() << user.getEmail() << user.getPassword()
		<< chats.size();

	for (Chat& chat : chats) {
		packet << chat.getName();

		auto members = chat.getMembers();
		packet << members.size();
		for (std::string member : members)
			packet << member;

		auto messages = chat.getMessages();
		packet << messages.size();
		for (Message& message : messages)
			packet << message.getAuthor() << message.getContent();
	}

	return packet;
}

std::optional<ResponseLoginData> PacketManager::extract_login_response_packet(sf::Packet& packet) {
	ResponseLoginData lrdata;

	packet >> lrdata.username >> lrdata.email >> lrdata.password;

	int chat_count = 0;
	int members_count = 0;
	int message_count = 0;

	packet >> chat_count;

	std::vector<Chat> chats;
	for (int i = 0; i < chat_count; i++) {
		std::vector<std::string> members;
		std::vector<Message> messages;

		std::string chat_name;
		packet >> chat_name >> members_count;

		for (int j = 0; j < members_count; j++) {
			std::string username;
			packet >> username;
			members.push_back(username);
		}
		packet >> message_count;
		for (int k = 0; k < message_count; k++) {
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

