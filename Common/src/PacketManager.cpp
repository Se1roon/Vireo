/// @file PacketManager.cpp
/// @brief Implementation of PacketManager class.

#include <iostream>
#include <string>
#include <optional>

#include <SFML/Network.hpp>

#include "PacketManager.hpp"
#include "PacketExceptions.hpp"
#include "User.hpp"


sf::Packet PacketManager::create_login_packet(User& user) {
	sf::Packet packet;

	packet << "L" << user.getUsername() << user.getPassword();

	return packet;
}

sf::Packet PacketManager::create_register_packet(User& user) {
	sf::Packet packet;

	packet << "R" << user.getUsername() << user.getEmail() << user.getPassword();

	return packet;
}


std::optional<LoginData> PacketManager::extract_login_packet(sf::Packet& packet) {
	LoginData ldata;

	if ((packet >> ldata.username >> ldata.password)) {
		packet.clear();
		return ldata;
	}

	return std::nullopt;	
}

std::optional<RegisterData> PacketManager::extract_register_packet(sf::Packet& packet) {
	RegisterData rdata;

	if ((packet >> rdata.username >> rdata.email >> rdata.password)) {
		packet.clear();
		return rdata;
	}

	return std::nullopt;
}

