/// @file User.cpp
/// @brief Implementation of the User class.

#include <string>

#include <SFML/Network.hpp>

#include "User.hpp"


User::User(std::string username, std::string password, std::string email) {
	setUsername(username);
	setPassword(password);
	setEmail(email);
}


void User::setUsername(std::string username) {
	this->username = username;
}

void User::setPassword(std::string password) {
	// TODO: Add some password protection feature (minLength, complexity itp).
	this->password = password;
}

void User::setEmail(std::string email) {
	// TODO: Validate if it is indeed an email address.
	this->email = email; 
}


std::string User::getUsername() {
	return username;
}

std::string User::getPassword() {
	return password;
}

std::string User::getEmail() {
	return email;
}


User User::operator=(User const& u) const {
	return { u.username, u.password, u.email };
}

