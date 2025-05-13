/// @file Server.hpp
/// @brief Declaration of the Server class and structures related to it.

#pragma once

#include <vector>
#include <memory>
#include <optional>
#include <string>

#include <bsoncxx/builder/basic/document.hpp>

#include <mongocxx/instance.hpp>
#include <mongocxx/client.hpp>

#include <SFML/Network.hpp>

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
	inline static mongocxx::instance _inst{};

	private:
		int listen_port;
		sf::TcpListener listener;
		sf::SocketSelector selector;
		PacketManager packet_manager;
		std::vector<ClientSession> clients;
		std::unique_ptr<mongocxx::client> database_connection;

		void start_listener();
		void init_db_connection(std::string connection_string);

		void handle_new_client();

		bool handle_login_request(LoginData& ldata, sf::TcpSocket& client_s);
		bool handle_register_request(RegisterData& rdata, sf::TcpSocket& client_s);

		bsoncxx::document::value user_to_document(RegisterData& rdata);
		bsoncxx::document::value user_to_document(User& user);
	public:
		Server(int listen_port, std::string db_connection_s);

		void handle_requests();
};

