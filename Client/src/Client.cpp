/// @file Client.cpp
/// @brief Implementation of Client class.

#include <iostream>
#include <iomanip>
#include <vector>
#include <optional>
#include <memory>

#include <SFML/Network.hpp>
#include <SFML/Graphics.hpp>

#include "Client.hpp"
#include "ClientException.hpp"
#include "PacketManager.hpp"
#include "User.hpp"
#include "Chat.hpp"
#include "Message.hpp"


Client::Client(sf::IpAddress s_addr, unsigned int s_port) {
	sf::Socket::Status status = server_socket.connect(s_addr, s_port);
	if (status != sf::Socket::Status::Done)
		throw ClientException("Unable to contact the Server.");
}

Client::~Client() {
	server_socket.disconnect();
}


User& Client::getUser() {
	return *user;
}


void Client::send_login_request() {
	sf::Packet lpacket = packet_manager.create_login_packet(*user);
	if (server_socket.send(lpacket) != sf::Socket::Status::Done)
		throw ClientException("Unable to send login credentials\n");
}

void Client::send_register_request() {
	sf::Packet rpacket = packet_manager.create_register_packet(*user);
	if (server_socket.send(rpacket) != sf::Socket::Status::Done)
		throw ClientException("Unable to register an account!\n");
}

void Client::send_new_message_request(std::string msg_content, Chat& chat) {
	sf::Packet nmpacket = packet_manager.create_new_message_packet(msg_content, chat);
	if (server_socket.send(nmpacket) != sf::Socket::Status::Done)
		throw ClientException("Unable to send new message to the server!\n");
}

void Client::send_search_request(std::string search_term) {
	sf::Packet spacket = packet_manager.create_search_packet(search_term);
	if (server_socket.send(spacket) != sf::Socket::Status::Done)
		throw ClientException("Unable to query server for seraching!\n");
}

void Client::send_new_chat_request(std::string username) {
	sf::Packet nhpacket = packet_manager.create_new_chat_packet(username);
	if (server_socket.send(nhpacket) != sf::Socket::Status::Done)
		throw ClientException("Unable to create new chat!\n");
}

User Client::receive_login_response(sf::Packet& packet) {
	auto response_data = packet_manager.extract_login_response_packet(packet);
	if (!response_data) {
		std::cout << "Error!\n";
		return User{"", "", ""};
	}

	User user(response_data->username, response_data->password, response_data->email);
	user.setChats(response_data->chats);

	return user;
}

void Client::send_message(std::string content, Chat& chat) {
	send_new_message_request(content, chat);
}


std::optional<std::vector<std::string>> Client::search_users(std::string prefix) {
	send_search_request(prefix);

	sf::Packet response_packet;
	if (server_socket.receive(response_packet) != sf::Socket::Status::Done)
		return std::nullopt;

	auto usernames = packet_manager.extract_search_response_packet(response_packet);
	if (usernames)
		return *usernames;

	return std::nullopt;
}

bool Client::load_user(std::string username, std::string password) {
	user = std::make_unique<User>(username, password, "");
	send_login_request();

	sf::Packet response_packet;
	if (server_socket.receive(response_packet) != sf::Socket::Status::Done)
		return false;

	user = std::make_unique<User>(receive_login_response(response_packet));
	return true;
}

bool Client::load_user(std::string username, std::string email, std::string password, std::string password_conf) {
	user = std::make_unique<User>(username, password, email);
	send_register_request();

	sf::Packet response_packet;
	if (server_socket.receive(response_packet) != sf::Socket::Status::Done)
		return false;

	return packet_manager.extract_register_response_packet(response_packet);
}

void Client::create_new_chat(std::string username) {
	std::vector<std::string> members = { user->getUsername(), username };
	std::vector<Message> messages;

	Chat new_chat(user->getUsername() + " & " + username, members, messages);

	send_new_chat_request(username);
	
	std::cout << "This blocs?\n";
	sf::Packet response_packet;
	if (server_socket.receive(response_packet) != sf::Socket::Status::Done) {
		std::cout << "error\n";
		return;
	}

	if (packet_manager.extract_new_chat_response_packet(response_packet)) {
		std::cout << "Success - joining new chat!\n";
		user->joinChat(new_chat);
	}
	else std::cout << "err\n";

	return;
}
