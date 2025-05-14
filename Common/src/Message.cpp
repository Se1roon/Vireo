/// @file Message.cpp
/// @brief Implementation of the Message class.

#include <string>
#include <chrono>

#include "Message.hpp"


using namespace std::chrono;


Message::Message(std::string content, std::string author, system_clock::time_point date) :
	content(content), author_name(author), timestamp(date) {}


std::string Message::getContent() {
	return content;
}

std::string Message::getAuthor() {
	return author_name;
}

system_clock::time_point& Message::getDate() {
	return timestamp;
}

