/// @file Server.cpp
/// @brief Implementation of the Server class.

// TODO: Fix MongoDB issue

#include <iostream>
#include <memory>
#include <optional>

#include <SFML/Network.hpp>

#include <bsoncxx/builder/basic/document.hpp>
#include <bsoncxx/types.hpp>
#include <bsoncxx/json.hpp>
#include <mongocxx/instance.hpp>
#include <mongocxx/client.hpp>
#include <mongocxx/uri.hpp>
#include <mongocxx/options/client.hpp>

#include "Server.hpp"
#include "ServerException.hpp"
#include "User.hpp"
#include "PacketManager.hpp"


using bsoncxx::builder::basic::kvp;
using bsoncxx::builder::basic::make_document;


Server::Server(int listen_port, std::string db_connection_s)  {
	this->listen_port = listen_port;
	start_listener();
	init_db_connection(db_connection_s);
}


void Server::start_listener() {
	if (listener.listen(listen_port) != sf::Socket::Status::Done)
		throw ServerException("Unable to start the listener. Port may be taken.\n");

	listener.setBlocking(false);
	selector.add(listener);
}

void Server::init_db_connection(std::string connection_string) {
	mongocxx::options::client client_options;
	mongocxx::options::tls tls_options;

	mongocxx::uri uri(connection_string);
	database_connection = std::make_unique<mongocxx::client>(uri);
}


void Server::handle_new_client() {
	sf::TcpSocket client_socket;

	if (listener.accept(client_socket) == sf::Socket::Status::Done) {
		client_socket.setBlocking(false);
		selector.add(client_socket);

		clients.emplace_back(std::move(client_socket), "");
	}
}


bool Server::handle_login_request(LoginData& ldata, sf::TcpSocket& client_s) {
	auto db_users = (*database_connection)["Vireo"]["Users"];
	auto user = db_users.find_one(make_document(kvp("username", ldata.username)));
	if (user) {
		auto user_view = user->view();
		if (user_view["password"].get_string().value == ldata.password) {
			std::cout << "Logged in!\n";
			return true;
		} else {
			std::cout << "Incorrect password!\n";
		}
	}

	return false;
}

bool Server::handle_register_request(RegisterData& rdata, sf::TcpSocket& client_s) {
	// TODO: Implement check for already existing user
	auto db_users = (*database_connection)["Vireo"]["Users"];

	auto user_doc = user_to_document(rdata);
	auto r = db_users.insert_one(user_doc.view());

	if (r) {
		std::cout << "Inserted [" << rdata.username << ":" << rdata.password << "] into the database.\n";
		return true;
	} else {
		std::cout << "Something went wrong...\n";
	}

	return false;
}


void Server::handle_requests() {
	if (selector.wait(sf::seconds(0.1f))) {
		if (selector.isReady(listener)) {
			handle_new_client();
			std::cout << "New client added!\n";
		}

		for (auto c = clients.begin(); c != clients.end(); ) {
			if (selector.isReady(c->socket)) {
				sf::Packet packet;
				sf::Socket::Status status = c->socket.receive(packet);

				if (status == sf::Socket::Status::Done) {
					std::string request_type;
					packet >> request_type;

					if (request_type == "L") {
						std::cout << "Login attempt\n";

						auto ldata = packet_manager.extract_login_packet(packet);
						if (ldata) {
							if (handle_login_request(*ldata, c->socket)){
								c->username = ldata->username;
								// Send response
							}
						}
					}
					else if (request_type == "R") {
						std::cout << "Register attempt\n";

						auto rdata = packet_manager.extract_register_packet(packet);
						if (rdata) {
							if (handle_register_request(*rdata, c->socket)) {
								// Maybe go back to login/register?
								c->username = rdata->username;
								// Send response
							}
						}
					}

					++c;
				} else if (status == sf::Socket::Status::Disconnected) {
					std::cout << "Client has disconnected!\n";
					selector.remove(c->socket);
					c = clients.erase(c);
				}
			} else {
				++c;
			}
		}
	}
}


bsoncxx::document::value Server::user_to_document(RegisterData& rdata) {
	return bsoncxx::builder::basic::make_document(
		kvp("username", rdata.username),
		kvp("email", rdata.email),
		kvp("password", rdata.password)
	);
}

bsoncxx::document::value Server::user_to_document(User& user) {
	return bsoncxx::builder::basic::make_document(
		kvp("username", user.getUsername()),
		kvp("email", user.getEmail()),
		kvp("password", user.getPassword())
	);
}

