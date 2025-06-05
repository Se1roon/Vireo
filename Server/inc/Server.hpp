/// @file Server.hpp
/// @brief Declaration of the Server class and structures related to it.

#pragma once

#include <vector>
#include <memory>
#include <optional>
#include <string>

#include <mongocxx/instance.hpp>
#include <mongocxx/client.hpp>

#include <SFML/Network.hpp>

#include "DatabaseHandler.hpp"
#include "PacketManager.hpp"
#include "User.hpp"


struct ClientSession {
	sf::TcpSocket socket;
	std::string username;

	ClientSession(sf::TcpSocket&& socket, std::string username) :
		socket(std::move(socket)), username(username) {}
};


/**
 * @class Server
 * @brief Handles communication with clients and the database.
 */
class Server {
	private:
		int listen_port;
		sf::TcpListener listener;
		sf::SocketSelector selector;
		PacketManager packet_manager;
		std::vector<ClientSession> clients;
		std::unique_ptr<DatabaseHandler> db_handler;

		void start_listener();

		void handle_new_client();

		bool handle_login_request(LoginData& ldata, sf::TcpSocket& client_s);
		bool handle_register_request(RegisterData& rdata, sf::TcpSocket& client_s);
		bool handle_search_request(std::string search_term, sf::TcpSocket& client_s);
		bool handle_new_chat_request(std::string username, std::string peer, sf::TcpSocket& client_s, sf::TcpSocket& peer_s);
		bool handle_new_messsage_request(NewMessageData& nmdata, sf::TcpSocket& client_s, sf::TcpSocket& peer_s);

	public:
		Server(int listen_port, std::string db_connection_s);

		void handle_requests();
};

