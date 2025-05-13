/// @file PacketExceptions.hpp
/// @brief Declaration & Implementation of the PacketManagerError class.

#pragma once

#include <stdexcept>
#include <string>


/**
 * @class PacketManagerError
 * @brief Handles exceptions related to PacketManager class.
 */
class PacketManagerError : public std::exception {
	private:
		std::string message;
	
	public:
		PacketManagerError(const char* msg) : message(msg) {}

		const char* what() const throw() {
			return message.c_str();
		}
};

