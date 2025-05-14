/// @file Chat.cpp
/// @brief Implementation of the Chat class.

#include <string>
#include <vector>
#include <algorithm>

#include "Chat.hpp"
#include "Message.hpp"


void Chat::setName(std::string name) {
	this->name = name;
}

std::vector<std::string> Chat::getMembers() {
	return members;
}

void Chat::addMember(std::string username) {
	auto exists = std::find(members.begin(), members.end(), username);
	if (exists == v.end()) // Doesn't exist, add it
		members.push_back(username);
	// It already is a member, do nothing
}

void Chat::delMember(std::string username) {
	auto user = std::find(members.begin(), members.end(), username);
	if (user != v.end()) // Is a member, delete it
		members.erase(user);
	// Isn't part of the chat anyway, do nothing
}

std::vector<Message> Chat::getMessages() {
	return messages;
}

void Chat::addMessage(Message& message) {
	messages.push_back(message);
}

