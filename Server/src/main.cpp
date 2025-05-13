/// @file Server/main.cpp
/// @brief Main file of the Server.

#include <iostream>
#include <charconv>
#include <system_error>

#include "Server.hpp"
#include "ConfigParser.hpp"


int main() {
	ConfigParser conf_parser("./cfg/server.conf");
	std::string v = conf_parser.get_value("listening_port");
	int l_port = 0;
	auto [ptr, ec] = std::from_chars(v.data(), v.data() + v.size(), l_port);

	if (ec != std::errc()) {
		std::cout << "Invalid config file!\n";
		return 0;
	}
	
	std::string connection_string = conf_parser.get_value("connection_string");
	if (connection_string == "") {
		std::cout << "Couldn't find connection_string key in the config file!\n";
		return 0;
	}

	Server s(l_port, connection_string);
	
	while (true) {
		s.handle_requests();
	}
}

