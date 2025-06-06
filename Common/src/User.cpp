/// @file User.cpp
/// @brief Implementation of the User class.

#include <iostream>
#include <vector>
#include <string>
#include <optional>

#include <SFML/Network.hpp>

#include "User.hpp"
#include "Chat.hpp"


User::User(std::string username, std::string password, std::string email) {
	setUsername(username);
	setPassword(password);
	setEmail(email);
}


void User::setUsername(std::string username) {
	this->username = username;
}

void User::setPassword(std::string password) {
	// TODO: Add some password protection feature (minLength, complexity itp).
	this->password = password;
}

void User::setEmail(std::string email) {
	// TODO: Validate if it is indeed an email address.
	this->email = email; 
}

void User::setChats(std::vector<Chat> chats) {
	this->chats = chats;
}


std::string User::getUsername() {
	return username;
}

std::string User::getPassword() {
	return password;
}

std::string User::getEmail() {
	return email;
}

std::vector<Chat> User::getChats() {
	return chats;
}

Chat& User::getChat(std::string chat_name) {
	for (auto& chat : chats)
		if (chat.getName() == chat_name)
			return chat;
}


void User::joinChat(Chat& chat) {
	chats.push_back(chat);
}

void User::leaveChat(Chat& chat) {
	std::cout << "INFO: Leaving chat not implemented!\n";
}


User User::operator=(User const& u) const {
	return { u.username, u.password, u.email };
}

