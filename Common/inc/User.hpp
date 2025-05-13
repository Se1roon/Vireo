/// @file User.hpp
/// @brief Declaration of the User class.

#pragma once 

#include <string>

#include <SFML/Network.hpp>


/**
 * @class User
 * @brief Responsible for holding necessary data related to the user.
 *
 */
class User {
	private:
		std::string username;
		std::string password;
		std::string email;

	public:
		User(std::string username, std::string password, std::string email);

		std::string getUsername();
		std::string getPassword();
		std::string getEmail();

		void setUsername(std::string username);
		void setPassword(std::string password);
		void setEmail(std::string email);

		User operator=(User const& u) const;
};

