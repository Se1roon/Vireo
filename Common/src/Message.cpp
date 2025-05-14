/// @file Message.cpp
/// @brief Implementation of the Message class.

#include <string>
#include <chrono>

#include "Message.hpp"


Message::Message(std::string content, std::string author, TimePoint date) :
	content(content), author_name(author), timestamp(date) {}


std::string Message::getContent() {
	return content;
}

std::string Message::getAuthor() {
	return autor_name;
}

TimePoint& getDate() {
	return timestamp;
}

