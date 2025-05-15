/// @file DatabaseHandler.hpp
/// @brief Declaration of the DatabaseHandler class.

#pragma once

#include <string>
#include <memory>
#include <optional>

#include <mongocxx/instance.hpp>
#include <mongocxx/client.hpp>

#include "User.hpp"

/**
 * @class DatabaseHandler
 * @brief Handles connection to the database, fetching, updating, inserting documents.
 *
 */
class DatabaseHandler {
	inline static mongocxx::instance _inst{};

	private:
		std::unique_ptr<mongocxx::client> client;

	public:
		DatabaseHandler(std::string connection_string);

		std::optional<User> fetch_user(std::string username);
		void insert_user(User& user); // Throw an exception if failed

		bsoncxx::document::value user_to_document(User& user);
};

