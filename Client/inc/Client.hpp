/// @file Client.hpp
/// @brief Declaration of Client class.

#pragma once

#include <memory>
#include <optional>

#include <SFML/Network.hpp>
#include <SFML/Graphics.hpp>

#include "User.hpp"
#include "PacketManager.hpp"
#include "Chat.hpp"

// Colors
#define MAIN_COLOR_R 230
#define MAIN_COLOR_G 230
#define MAIN_COLOR_B 230

#define ACCENT_COLOR_R 108
#define ACCENT_COLOR_G 0
#define ACCENT_COLOR_B 162

#define DIMM_COLOR_R 180
#define DIMM_COLOR_G 180
#define DIMM_COLOR_B 180


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

		void render_login(sf::RenderWindow& window, sf::Font& main_font, sf::Font& title_font);
};

