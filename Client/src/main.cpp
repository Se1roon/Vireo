/// @file Client/main.cpp
/// @brief Main file of the Client.

#include <iostream>
#include <charconv>
#include <system_error>

#include "Client.hpp"
#include "ConfigParser.hpp"


int main() {
	std::string login; 
	std::cout << "Login or Register? (L or R): ";
	std::cin >> login; 
	std::cout << '\n';

	ConfigParser conf_parser("./cfg/client.conf");
	std::string v = conf_parser.get_value("server_port");
	int server_port = 0;
	auto [ptr, ec] = std::from_chars(v.data(), v.data() + v.size(), server_port);
	
	if (ec != std::errc()) {
		std::cout << "Invalid config file!\n";
		return 0;
	}

	try {
		Client client({127, 0, 0, 1}, server_port);

		client.load_user(login[0]);
	} catch (std::exception& e) {
		std::cout << e.what() << '\n';
	}
}

