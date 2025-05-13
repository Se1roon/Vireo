/// @file ConfigParserException.hpp
/// @brief Declaration & Implementation of the ConfigParserException class.

#pragma once

#include <exception>
#include <string>


/**
 * @class ConfigParserException
 * @brief Handles exceptions related to the ConfigParser class.
 */
class ConfigParserException : public std::exception {
	private:
		std::string message;

	public:
		ConfigParserException(const char* msg) : message(msg) {}

		const char* what() const throw() {
			return message.c_str();
		}
};

