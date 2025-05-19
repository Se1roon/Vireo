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

