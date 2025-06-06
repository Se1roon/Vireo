/// @file DatabaseHandler.cpp
/// @brief Implementation of the DatabaseHandler class.

#include <vector>
#include <string>
#include <memory>
#include <optional>

#include <bsoncxx/builder/basic/document.hpp>
#include <bsoncxx/builder/basic/array.hpp>
#include <bsoncxx/builder/stream/document.hpp>
#include <bsoncxx/builder/stream/array.hpp>
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

		return user;
	}
	
	return std::nullopt;
}

std::vector<std::string> DatabaseHandler::get_usernames(std::string prefix) {
	auto db_users = (*client)["Vireo"]["Users"];

	std::string pattern = "^" + prefix;
	auto cursor = db_users.find(make_document(kvp("username", make_document(kvp("$regex", pattern)))));

	std::vector<std::string> usernames;
	for (auto&& doc : cursor) {
		std::string uname(doc["username"].get_string().value);
		usernames.push_back(uname);
	}

	return usernames;
}

bool DatabaseHandler::insert_user(User& user) {
	auto db_users = (*client)["Vireo"]["Users"];
	auto user_doc = user_to_document(user);

	if (db_users.insert_one(user_doc.view())) {
		std::cout << "INFO: Inserted [" << user.getUsername() << ":" << user.getPassword() << "] into the database.\n";
		return true;
	}
	return false;
}

bool DatabaseHandler::insert_message(std::string author, std::string content, std::string chat_name) {
	// Get a chat
	// Insert a message
	
	auto chats_col = (*client)["Vireo"]["Chats"];

	bsoncxx::builder::stream::document filter_builder;
	filter_builder << "name" << chat_name;
	bsoncxx::document::value filter_doc = filter_builder << bsoncxx::builder::stream::finalize;

	bsoncxx::builder::stream::document update_builder;
	update_builder << "$push"
				   << bsoncxx::builder::stream::open_document
						<< "messages"
						<< bsoncxx::builder::stream::open_document
							<< "author" << author
							<< "content" << content
						<< bsoncxx::builder::stream::close_document
					<< bsoncxx::builder::stream::close_document;

	bsoncxx::document::value update_doc = update_builder << bsoncxx::builder::stream::finalize;

	auto result = chats_col.update_one(filter_doc.view(), update_doc.view());
	if (!result) {
		std::cout << "ERROR: An error occured while inserting message!\n";
		return false;
	}

	return true;

}

bsoncxx::document::value DatabaseHandler::user_to_document(User& user) {
	return make_document(
		kvp("username", user.getUsername()),
		kvp("email", user.getEmail()),
		kvp("password", user.getPassword()),
		kvp("chats", make_array())
	);
}

bool DatabaseHandler::create_chat(std::string creator, std::string peer) {
	auto db_chats = (*client)["Vireo"]["Chats"];

	auto chat_doc = make_document(
			kvp("name", creator + " & " + peer),
			kvp("members", make_array(creator, peer)),
			kvp("messages", make_array()));

	if (db_chats.insert_one(chat_doc.view())) {
		std::cout << "INFO: Inserted [" << creator << " & " << peer << "] chat into the database.\n";

		auto db_users = (*client)["Vireo"]["Users"];

		auto creator_doc = db_users.find_one(make_document(kvp("username", creator)));
		auto peer_doc = db_users.find_one(make_document(kvp("username", peer)));

		auto update_push = make_document(
				kvp("$push", make_document(kvp("chats", db_chats.find_one(make_document(kvp("name", creator + " & " + peer)))->view()["_id"].get_oid().value))));

		db_users.update_one(creator_doc->view(), update_push.view());
		db_users.update_one(peer_doc->view(), update_push.view());

		return true;
	}

	return false;
}

