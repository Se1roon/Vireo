/// @file DatabaseHandler.hpp
/// @brief Declaration of the DatabaseHandler class.

#pragma once

#include <string>
#include <memory>
#include <optional>
#include <vector>

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
		std::vector<std::string> get_usernames(std::string prefix);
		bool insert_user(User& user);
		bool create_chat(std::string creator, std::string peer);

		bsoncxx::document::value user_to_document(User& user);
};

