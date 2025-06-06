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
		throw ClientException("ERROR: Unable to contact the Server.");

	server_socket.setBlocking(false);
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
		throw ClientException("ERROR: Unable to send login credentials\n");
}

void Client::send_register_request() {
	sf::Packet rpacket = packet_manager.create_register_packet(*user);
	if (server_socket.send(rpacket) != sf::Socket::Status::Done)
		throw ClientException("ERROR: Unable to register an account!\n");
}

void Client::send_new_message_request(std::string msg_content, Chat& chat) {
	sf::Packet nmpacket = packet_manager.create_new_message_packet(user->getUsername(), msg_content, chat);
	if (server_socket.send(nmpacket) != sf::Socket::Status::Done)
		throw ClientException("ERROR: Unable to send new message to the server!\n");
}

void Client::send_search_request(std::string search_term) {
	sf::Packet spacket = packet_manager.create_search_packet(search_term);
	if (server_socket.send(spacket) != sf::Socket::Status::Done)
		throw ClientException("ERROR: Unable to query server for seraching!\n");
}

void Client::send_new_chat_request(std::string username) {
	sf::Packet nhpacket = packet_manager.create_new_chat_packet(username);
	if (server_socket.send(nhpacket) != sf::Socket::Status::Done)
		throw ClientException("ERROR: Unable to create new chat!\n");
}

User Client::receive_login_response(sf::Packet& packet) {
	auto response_data = packet_manager.extract_login_response_packet(packet);
	if (!response_data) {
		std::cout << "ERROR: Problem when extracting LoginResponse!\n";
		return User{"", "", ""};
	}

	User user(response_data->username, response_data->password, response_data->email);
	user.setChats(response_data->chats);

	return user;
}


void Client::handle_request(GUI::GUIManager& gui_manager) {
	sf::Packet incoming_packet;
	if (server_socket.receive(incoming_packet) == sf::Socket::Status::Done) {
		std::string packet_type;
		incoming_packet >> packet_type;
		
		if (packet_type == "LR") {
			std::cout << "LOG: LoginResponse came!\n";
			user = std::make_unique<User>(receive_login_response(incoming_packet));
		} else if (packet_type == "RR") {
			std::cout << "LOG: RegisterResponse came!\n";
			if (!packet_manager.extract_register_response_packet(incoming_packet))
				std::cout << "ERROR: Unable to register!\n";
			else std::cout << "INFO: Successfully registered!\n";
		} else if (packet_type == "SR") {
			std::cout << "LOG: SearchResponse came!\n";
			auto usernames = packet_manager.extract_search_response_packet(incoming_packet);
			if (usernames)
				gui_manager.build_search_results(*usernames);
		} else if (packet_type == "NHR") {
			std::cout << "LOG: NewChatResponse came!\n";
			Chat new_chat = packet_manager.extract_new_chat_response_packet(incoming_packet);
			user->joinChat(new_chat);
			gui_manager.build_chat_rects(*user);
		} else if (packet_type == "NMR") {
			std::cout << "LOG: NewMessageResponse came!\n";
			std::pair<Message, std::string> new_msg = packet_manager.extract_new_message_response_packet(incoming_packet);
			Chat& chat = user->getChat(new_msg.second);
			chat.addMessage(new_msg.first);
			gui_manager.build_chat_messages(chat);
		} else {
			std::cout << "ERROR: Unknown packet type!\n";
		}
	}

	return;
}


void Client::send_message(std::string content, Chat& chat) {
	send_new_message_request(content, chat);
	std::cout << "LOG: Sent NewMessage request!\n";
}


bool Client::search_users(std::string prefix) {
	send_search_request(prefix);
	std::cout << "LOG: Sent Search request!\n";
	return true;
}

bool Client::load_user(std::string username, std::string password) {
	user = std::make_unique<User>(username, password, "");
	send_login_request();
	std::cout << "LOG: Sent Login request!\n";

	return true;
}

bool Client::load_user(std::string username, std::string email, std::string password, std::string password_conf) {
	user = std::make_unique<User>(username, password, email);
	send_register_request();
	std::cout << "LOG: Sent Register request!\n";

	return true;
}

void Client::create_new_chat(std::string username) {
	send_new_chat_request(username);
	std::cout << "LOG: Sent NewChat request!\n";
	
	return;
}

