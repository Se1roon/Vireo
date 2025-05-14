/// @file Message.hpp
/// @brief Declaration of the Message class.

#pragma once

#include <string>
#include <chrono>


/**
 * @class Message
 * @brief Holds data related to message.
 */
class Message {
	private:
		std::string content;
		std::string author_name;
		TimePoint timestamp; // May need to think about storing dates more.

	public:
		Message(std::string content, std::string author, TimePoint date);

		std::string getContent();
		std::string getAuthor();
		TimePoint& getDate();
};

