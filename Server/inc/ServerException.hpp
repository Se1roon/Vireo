/// @file ServerException.hpp
/// @brief Declaration & Implementation of the ServerException class.

#pragma once

#include <exception>
#include <string>

/**
 * @class ServerException
 * @brief Handles exceptions related to the Server class.
 */
class ServerException : public std::exception {
	private:
		std::string message;

	public:
		ServerException(const char* msg) : message(msg) {}

		const char* what() const throw() {
			return message.c_str();
		}
};

