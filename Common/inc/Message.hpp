/// @file Message.hpp
/// @brief Declaration of the Message class.

#pragma once

#include <string>

/**
 * @class Message
 * @brief Holds data related to message.
 */
class Message {
	private:
		std::string content;
		std::string author_name;
		// I don't want to deal with dates now
		//std::chrono::system_clock::time_point timestamp; // May need to think about storing dates more.

	public:
		Message(std::string content, std::string author);

		std::string getContent();
		std::string getAuthor();
};

