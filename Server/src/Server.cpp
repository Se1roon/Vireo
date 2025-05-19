/// @file Server.cpp
/// @brief Implementation of the Server class.

#include <iostream>
#include <memory>
#include <optional>

#include <SFML/Network.hpp>

#include <mongocxx/instance.hpp>
#include <mongocxx/client.hpp>
#include <mongocxx/uri.hpp>
#include <mongocxx/options/client.hpp>

#include "Server.hpp"
#include "ServerException.hpp"
#include "DatabaseHandler.hpp"
#include "User.hpp"
#include "PacketManager.hpp"


Server::Server(int listen_port, std::string db_connection_s)  {
	this->listen_port = listen_port;
	start_listener();
	db_handler = std::make_unique<DatabaseHandler>(db_connection_s);
}


void Server::start_listener() {
	if (listener.listen(listen_port) != sf::Socket::Status::Done)
		throw ServerException("Unable to start the listener. Port may be taken.\n");

	listener.setBlocking(false);
	selector.add(listener);
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
	auto user = db_handler->fetch_user(ldata.username);

	if (user) {
		if (user->getPassword() == ldata.password) {
			std::cout << "Logged in!\n";

			sf::Packet r_packet = packet_manager.create_login_response_packet(*user);
			if (client_s.send(r_packet) == sf::Socket::Status::Done) {
				return true;
			} else {
				std::cout << "Error while sending data to client\n";
				return false;
			}
		} else {
			std::cout << "Incorrect password!\n";
		}
	} 

	std::cout << "User doesn't exist\n";
	return false;
}

bool Server::handle_register_request(RegisterData& rdata, sf::TcpSocket& client_s) {
	// TODO: Implement check for already existing user

	User user(rdata.username, rdata.password, rdata.email);

	db_handler->insert_user(user);

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
							sf::Packet response_packet;
							if (handle_login_request(*ldata, c->socket)){
								c->username = ldata->username;
								std::cout << "Sent User data to [" << c->username << "]\n";
							} else {
								response_packet << "F";
								c->socket.send(response_packet);
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

