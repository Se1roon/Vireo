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

void Client::load_user(char op) {
	std::string i_username;
	std::string i_password;

	switch (op) {
		case 'L': {
			std::cout << "Provide username: ";
			std::cin >> i_username;
			std::cout << "Provide password: ";
			std::cin >> i_password;

			user = std::make_unique<User>(i_username, i_password, "");
			send_login_request();

			sf::Packet response_packet;
			server_socket.receive(response_packet);

			user = std::make_unique<User>(receive_login_response(response_packet));

			break;
		}
		case 'R': {
			std::string i_email;
			std::string i_password_c;

			std::cout << "Provide username: ";
			std::cin >> i_username;
			std::cout << "Provide email: ";
			std::cin >> i_email;

			do {
				std::cout << "Provide password: ";
				std::cin >> i_password;
				std::cout << "Confirm password: ";
				std::cin >> i_password_c;
				if (i_password != i_password_c)
					std::cout << "Passwords don't match!\n";
			} while (i_password != i_password_c);

			user = std::make_unique<User>(i_username, i_password, i_email);
			send_register_request();

			break;
		}
	}
}

void Client::render_chat(Chat& chat) {
	for (Message& message : chat.getMessages())
		std::cout << "[" << message.getAuthor() << "]\n" << message.getContent() << "\n\n";
}

void Client::render_hello_message() {
	std::cout << "Welcome back " << user->getUsername() << "!\n";
}

std::optional<Chat> Client::render_chat_list() {
	std::vector<Chat> chats = user->getChats();

	std::cout << "Your chats:\n\n";

	int i = 1;
	for (Chat& chat : chats)
		std::cout << i << "- [" << chat.getName() << "]\n";

	int chat_num = 1;
	std::cout << "\nEnter chat number: ";
	std::cin >> chat_num;

	try {
		return chats.at(chat_num - 1);
	} catch (std::out_of_range& e) {
		std::cout << "There is no chat with number " << chat_num << ".\n";
	}

	return std::nullopt;
}

