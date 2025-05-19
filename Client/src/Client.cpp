/// @file Client.cpp
/// @brief Implementation of Client class.

#include <iostream>
#include <memory>

#include <SFML/Network.hpp>

#include "Client.hpp"
#include "ClientException.hpp"
#include "PacketManager.hpp"
#include "User.hpp"


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

			std::cout << "Running as " << user->getUsername() << '\n';
			std::cout << "Email: " << user->getEmail() << '\n';
			std::cout << "Dupa: " << user->getChats().size() << '\n';
			for (Chat& c : user->getChats()) {
				std::cout << "Chat [" << c.getName() << "]\n";
				for (std::string member : c.getMembers())
					std::cout << member << " ";
				std::cout << "\n";
				for (Message& m : c.getMessages()) {
					std::cout << m.getAuthor() << "said \"" << m.getContent() << "\"\n";
				}
			}
			
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

