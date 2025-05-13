// @file ClientException.hpp
// @brief Declaration & Implementation of the ClientException class.

#pragma once

#include <exception>
#include <string>


/**
 * @class ClientException
 * @brief Handles exceptions related to the Client class.
 *
 */
class ClientException : public std::exception {
	private:
		std::string message;
	
	public:
		ClientException(const char* msg) : message(msg) {}

		const char* what() const throw() {
			return message.c_str();
		}
};

