/// @file DatabaseHandler.cpp
/// @brief Implementation of the DatabaseHandler class.

#include <string>
#include <memory>
#include <optional>

#include <bsoncxx/builder/basic/document.hpp>
#include <bsoncxx/types.hpp>
#include <bsoncxx/json.hpp>
#include <mongocxx/uri.hpp>
#include <mongocxx/client.hpp>

#include "DatabaseHandler.hpp"


using bsoncxx::builder::basic::kvp;
using bsoncxx::builder::basic::make_document;


DatabaseHandler::DatabaseHandler(std::string connection_string) {
	mongocxx::uri uri(connection_string);
	client = std::make_unique<mongocxx::client>(uri);
}


std::optional<User> DatabaseHandler::fetch_user(std::string username) {
	auto db_users = (*client)["Vireo"]["Users"];
	auto user_doc = db_users.find_one(make_document(kvp("username", username)));
	
	if (user_doc) {
		auto user_view = user_doc->view();
		std::string username(user_view["username"].get_string().value);
		std::string password(user_view["password"].get_string().value);
		std::string email(user_view["email"].get_string().value);

		return User{username, password, email};
	}
	
	return std::nullopt;
}

void DatabaseHandler::insert_user(User& user) {
	auto db_users = (*client)["Vireo"]["Users"];
	auto user_doc = user_to_document(user);

	if (db_users.insert_one(user_doc.view())) {
		std::cout << "Inserted [" << user.getUsername() << ":" << user.getPassword() << "] into the database.\n";
	} else {
		std::cout << "Exception\n";
	}
}

bsoncxx::document::value DatabaseHandler::user_to_document(User& user) {
	return make_document(
		kvp("username", user.getUsername()),
		kvp("email", user.getEmail()),
		kvp("password", user.getPassword())
	);
}

