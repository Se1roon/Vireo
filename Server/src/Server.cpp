/// @file Server.cpp
/// @brief Implementation of the Server class.

#include <iostream>
#include <memory>
#include <optional>

#include <SFML/Network.hpp>
#include <SFML/Network/IpAddress.hpp>

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
			std::cout << "INFO: A user logged in!\n";

			sf::Packet r_packet = packet_manager.create_login_response_packet(*user);
			if (client_s.send(r_packet) == sf::Socket::Status::Done) {
				return true;
			} else {
				std::cout << "ERROR: Unable to send data to client!\n";
				return false;
			}
		} else {
			std::cout << "INFO: Incorrect password provided\n";
		}
	} 

	std::cout << "ERROR: User doesn't exist!\n";

	return false;
}

bool Server::handle_register_request(RegisterData& rdata, sf::TcpSocket& client_s) {
	// TODO: Implement check for already existing user

	User user(rdata.username, rdata.password, rdata.email);

	sf::Packet r_packet;
	if (db_handler->insert_user(user))
		r_packet = packet_manager.create_register_response_packet(true);
	else
		r_packet = packet_manager.create_register_response_packet(false);

	if (client_s.send(r_packet) == sf::Socket::Status::Done)
		return true;
	
	std::cout << "ERROR: Unable to insert a user!\n";
	return false;
}

bool Server::handle_search_request(std::string search_term, sf::TcpSocket& client_s) {
	std::vector<std::string> usernames = db_handler->get_usernames(search_term);

	sf::Packet response_packet = packet_manager.create_search_response_packet(usernames);
	if (client_s.send(response_packet) == sf::Socket::Status::Done)
		return true;

	std::cout << "ERROR: An error occured while searching for users!\n";
	return false;
}

bool Server::handle_new_chat_request(std::string username, std::string peer, sf::TcpSocket& client_s, sf::TcpSocket* peer_s) {
	sf::Packet response_packet;

	if (db_handler->create_chat(username, peer)) {
		std::vector<std::string> members = { username, peer };
		std::vector<Message> messages;

		Chat new_chat(username + " & " + peer, members, messages);

		std::cout << "INFO: Inserted new chat [" << new_chat.getName() << "] into the database\n";

		response_packet = packet_manager.create_new_chat_response_packet(new_chat);
	}
	else {
		// Handle db chat insertion error
		std::cout << "ERROR: Unable to insert new chat!\n";
	}

	if (client_s.send(response_packet) != sf::Socket::Status::Done) return false;
	if (peer_s)
		if (peer_s->send(response_packet) != sf::Socket::Status::Done)
			std::cout << "INFO: Unable to send information to peer, might be offline\n";

	return true;
}

bool Server::handle_new_messsage_request(NewMessageData& nmdata, sf::TcpSocket& client_s, sf::TcpSocket* peer_s) {
	sf::Packet response_packet;

	Message msg(nmdata.msg_content, nmdata.msg_author);

	if (db_handler->insert_message(nmdata.msg_author, nmdata.msg_content, nmdata.chat_name)) {
		response_packet = packet_manager.create_new_message_response_packet(nmdata.chat_name, msg);
	}

	if (client_s.send(response_packet) != sf::Socket::Status::Done) return false;
	if (peer_s != NULL)
		if (peer_s->send(response_packet) != sf::Socket::Status::Done)
			std::cout << "INFO: Unable to send information to peer, might be offline\n";

	return true;
}


void Server::handle_requests() {
	if (selector.wait(sf::seconds(0.1f))) {
		if (selector.isReady(listener)) {
			handle_new_client();
			std::cout << "INFO: New client added!\n";
		}

		for (auto c = clients.begin(); c != clients.end(); ) {
			if (selector.isReady(c->socket)) {
				sf::Packet packet;
				sf::Socket::Status status = c->socket.receive(packet);

				if (status == sf::Socket::Status::Done) {
					std::string request_type;
					packet >> request_type;

					if (request_type == "L") {
						std::cout << "LOG: Login attempt\n";

						auto ldata = packet_manager.extract_login_packet(packet);
						if (ldata) {
							sf::Packet response_packet;
							if (handle_login_request(*ldata, c->socket)){
								c->username = ldata->username;
								std::cout << "LOG: Sent User data to [" << c->username << "]\n";
							} else {
								response_packet << "F";
								c->socket.send(response_packet);
							}
						}
					}
					else if (request_type == "R") {
						std::cout << "LOG: Register attempt\n";

						auto rdata = packet_manager.extract_register_packet(packet);
						if (rdata) {
							if (handle_register_request(*rdata, c->socket))
								c->username = rdata->username;
						}
					}
					else if (request_type == "NM") {
						std::cout << "LOG: New Message came\n";
						
						auto nmdata = packet_manager.extract_new_message_packet(packet);
						if (nmdata) {
							sf::TcpSocket* peer_socket = NULL;
							for (auto peer = clients.begin(); peer != clients.end(); peer++) {
								if (peer->username == nmdata->peer_username) peer_socket = &peer->socket;
							}

							if (!handle_new_messsage_request(*nmdata, c->socket, peer_socket)) {
								std::cout << "ERROR: handle_new_message_request error\n";
							}
						}
					}
					else if (request_type == "S") {
						std::cout << "LOG: Search request\n";

						auto search_term = packet_manager.extract_search_packet(packet);
						if (search_term) {
							if (!handle_search_request(*search_term, c->socket)) {
								std::cout << "ERROR: error happened :(\n";
							}
						}
					}
					else if (request_type == "NH") {
						std::cout << "LOG: New chat request!\n";

						auto peer_username = packet_manager.extract_new_chat_packet(packet);
						if (peer_username) {
							sf::TcpSocket* peer_socket = NULL;
							for (auto peer = clients.begin(); peer != clients.end(); peer++) {
								if (peer->username == peer_username) peer_socket = &peer->socket;
							}

							if (!handle_new_chat_request(c->username, *peer_username, c->socket, peer_socket)) {
								std::cout << "ERROR: handle_new_chat_request errror\n";
							}
						}
					}
					
					++c;
				} else if (status == sf::Socket::Status::Disconnected) {
					std::cout << "LOG: Client has disconnected!\n";
					selector.remove(c->socket);
					c = clients.erase(c);
				}
			} else {
				++c;
			}
		}
	}
}

