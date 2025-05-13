/// @file Client.hpp
/// @brief Declaration of Client class.

#pragma once

#include <memory>

#include <SFML/Network.hpp>

#include "User.hpp"
#include "PacketManager.hpp"


/**
 * @class Client
 * @brief Handles communication with the server and everything related to it (GUI later).
 *
 */
class Client {
	private:
		sf::TcpSocket server_socket;
		PacketManager packet_manager;
		std::unique_ptr<User> user;

		void send_login_request();
		void send_register_request();

	public:
		Client(sf::IpAddress addr, unsigned int s_port);
		~Client();
		
		void load_user(char op);
};

