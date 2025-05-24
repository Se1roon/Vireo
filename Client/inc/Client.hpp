/// @file Client.hpp
/// @brief Declaration of Client class.

#pragma once

#include <memory>
#include <optional>

#include <SFML/Network.hpp>

#include "User.hpp"
#include "PacketManager.hpp"
#include "Chat.hpp"


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
		void send_new_message_request(std::string msg_content, Chat& chat);

		User receive_login_response(sf::Packet& packet);

	public:
		unsigned int window_width = 1000;
		unsigned int window_height = 800;

		Client(sf::IpAddress addr, unsigned int s_port);
		~Client();
		
		void load_user(char op);
		void send_message(std::string content, Chat& chat);

		void render_hello_message();
		std::optional<Chat> render_chat_list();
		void render_chat(Chat& chat);
};

