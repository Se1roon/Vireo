/// @file Client/main.cpp
/// @brief Main file of the Client.

#include <iostream>
#include <charconv>
#include <system_error>

#include <SFML/Graphics.hpp>

#include "Client.hpp"
#include "ConfigParser.hpp"
#include "Chat.hpp"


int main() {
	/*
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

		std::cout << '\n';
		client.render_hello_message();
		std::cout << '\n';

		auto selected_chat = client.render_chat_list();

		if (selected_chat) {
			std::cout << '\n';
			client.render_chat(*selected_chat);
		} else {
			std::cout << ":(\n";
		}

		std::string message;
		std::cout << "\nType something: " << std::flush;
		std::getline(std::cin >> std::ws, message);
		
		client.send_message(message, *selected_chat);
	} catch (std::exception& e) {
		std::cout << e.what() << '\n';
	}
	*/

	ConfigParser conf_parser("./cfg/client.conf");
	std::string v = conf_parser.get_value("server_port");
	int server_port = 0;
	auto [ptr, ec] = std::from_chars(v.data(), v.data() + v.size(), server_port);
	
	if (ec != std::errc()) {
		std::cout << "Invalid config file!\n";
		return 0;
	}	

	Client client({127, 0, 0, 1}, server_port);

	sf::RenderWindow main_window(sf::VideoMode({client.window_width, client.window_height}), "Vireo");

	sf::Font main_font("fonts/SourceSansPro-Regular.ttf");
	sf::Font title_font("fonts/Inter-Regular.otf");

	while (main_window.isOpen()) {
		while (const std::optional event = main_window.pollEvent()) {
			if (event->is<sf::Event::Closed>())
				main_window.close();
		}

		main_window.clear({ MAIN_COLOR_R, MAIN_COLOR_G, MAIN_COLOR_B });

		client.render_login(main_window, main_font, title_font);

		main_window.display();
	}
}

