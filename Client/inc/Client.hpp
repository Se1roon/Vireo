/// @file Client.hpp
/// @brief Declaration of Client class.

#pragma once

#include <memory>
#include <optional>
#include <vector>

#include <SFML/Network.hpp>

#include "User.hpp"
#include "GUIManager.hpp"
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
		void send_search_request(std::string search_term);
		void send_new_chat_request(std::string username);

		User receive_login_response(sf::Packet& packet);

	public:
		unsigned int window_width = 1000;
		unsigned int window_height = 800;

		Client(sf::IpAddress addr, unsigned int s_port);
		~Client();

		User& getUser();

		void handle_request(GUI::GUIManager& gui_manager);
		
		bool load_user(std::string username, std::string password);
		bool load_user(std::string username, std::string email, std::string password, std::string password_conf);
		void send_message(std::string content, Chat& chat);
		void create_new_chat(std::string username);
		
		bool search_users(std::string prefix);

		void render_hello_message();
		std::optional<Chat> render_chat_list();
		void render_chat(Chat& chat);
};

