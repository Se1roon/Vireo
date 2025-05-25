/// @file DatabaseHandler.cpp
/// @brief Implementation of the DatabaseHandler class.

#include <vector>
#include <string>
#include <memory>
#include <optional>

#include <bsoncxx/builder/basic/document.hpp>
#include <bsoncxx/builder/basic/array.hpp>
#include <bsoncxx/types.hpp>
#include <bsoncxx/json.hpp>
#include <mongocxx/uri.hpp>
#include <mongocxx/client.hpp>

#include "DatabaseHandler.hpp"
#include "User.hpp"
#include "Chat.hpp"
#include "Message.hpp"


using bsoncxx::builder::basic::kvp;
using bsoncxx::builder::basic::make_document;
using bsoncxx::builder::basic::make_array;


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

		User user(username, password, email);

		auto chats = user_view["chats"].get_array().value;
		for (auto&& chat : chats) {
			std::cout << "Execute?\n";
			auto oid = chat.get_oid().value;
			auto chat_doc = (*client)["Vireo"]["Chats"].find_one(make_document(kvp("_id", oid)));
			if (chat_doc) {
				auto chat_view = chat_doc->view();
				
				std::vector<std::string> usernames;
				auto members = chat_view["members"].get_array().value;
				for (auto&& member : members) {
					std::string member_username = std::string(member.get_string().value);
					usernames.push_back(member_username);
				}

				std::vector<Message> messages_;
				auto messages = chat_view["messages"].get_array().value;
				for (auto&& message : messages) {
					auto message_doc = message.get_document().view();

					std::string author(message_doc["author"].get_string().value);
					std::string content(message_doc["content"].get_string().value);

					messages_.push_back({content, author});
				}
				
				std::string chat_name(chat_view["name"].get_string().value);
				Chat c(chat_name, usernames, messages_);
				user.joinChat(c);
			}
		}

		std::cout << "Chats: " << user.getChats().size() << '\n';

		return user;
	}
	
	return std::nullopt;
}

bool DatabaseHandler::insert_user(User& user) {
	auto db_users = (*client)["Vireo"]["Users"];
	auto user_doc = user_to_document(user);

	if (db_users.insert_one(user_doc.view())) {
		std::cout << "Inserted [" << user.getUsername() << ":" << user.getPassword() << "] into the database.\n";
		return true;
	}
	return false;
}

bsoncxx::document::value DatabaseHandler::user_to_document(User& user) {
	return make_document(
		kvp("username", user.getUsername()),
		kvp("email", user.getEmail()),
		kvp("password", user.getPassword()),
		kvp("chats", make_array())
	);
}

