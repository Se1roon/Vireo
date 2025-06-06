/// @file Client/main.cpp
/// @brief Main file of the Client.

#include <iostream>
#include <charconv>
#include <system_error>
#include <vector>

#include <SFML/Graphics.hpp>

#include "Client.hpp"
#include "ConfigParser.hpp"
#include "Chat.hpp" 
#include "GUIManager.hpp"
#include "PHElement.hpp"


using namespace GUI;


int main() {
	ConfigParser conf_parser("./cfg/client.conf");
	std::string v = conf_parser.get_value("server_port");
	int server_port = 0;
	auto [ptr, ec] = std::from_chars(v.data(), v.data() + v.size(), server_port);
	
	if (ec != std::errc()) {
		std::cout << "ERROR: Invalid config file!\n";
		return 0;
	}	

	Client client({127, 0, 0, 1}, server_port);

	sf::RenderWindow main_window(sf::VideoMode({client.window_width, client.window_height}), "Vireo");
	main_window.setFramerateLimit(30);

	sf::Font main_font("fonts/SourceSansPro-Regular.ttf");
	sf::Font title_font("fonts/Inter-Regular.otf");

	GUIManager gui_manager(main_window, main_font, title_font);

	bool isLoginState = true;
	bool isRegisterState = false;
	bool isChatlistState = false;
	bool isSearchState = false;

	bool username_rect_focused = false;
	bool password_rect_focused = false;
	bool email_rect_focused = false;
	bool password_c_rect_focused = false;
	bool search_rect_focused = false;
	bool new_msg_rect_focused = false;

	bool chat_entered = false;
	bool stop_rendering_chat_rects = false;

	std::string current_chat = "";

	while (main_window.isOpen()) {
		while (const std::optional event = main_window.pollEvent()) {
			if (event->is<sf::Event::Closed>())
				main_window.close();

			if (const auto* mouseButtonPressed = event->getIf<sf::Event::MouseButtonPressed>()) {
				if (mouseButtonPressed->button == sf::Mouse::Button::Left) {
					if (isLoginState) { 
						auto action = gui_manager.loginpage_action(mouseButtonPressed->position);
						if (action) {
							if (*action == LPROMPT) {
								isLoginState = false;
								isRegisterState = true;
							} else if (*action == LUSERNAME) {
								password_rect_focused = false;
								username_rect_focused = true;
							}
							else if (*action == LPASSWORD) {
								password_rect_focused = true;
								username_rect_focused = false;
							}
						} else {
							username_rect_focused = false;
							password_rect_focused = false;
						}
					} else if (isRegisterState) {
						auto action = gui_manager.registerpage_action(mouseButtonPressed->position);
						if (action) {
							if (*action == RPROMPT) {
								isLoginState = true;
								isRegisterState = false;
							} else if (*action == RUSERNAME) {
								username_rect_focused = true;
								password_rect_focused = false;
								email_rect_focused = false;
								password_c_rect_focused = false;
							} else if (*action == REMAIL) {
								email_rect_focused = true;
								username_rect_focused = false;
								password_rect_focused = false;
								password_c_rect_focused = false;
							} else if (*action == RPASSWORD) {
								password_rect_focused = true;
								username_rect_focused = false;
								email_rect_focused = false;
								password_c_rect_focused = false;
							} else if (*action == RPASSWORD_C)  {
								password_c_rect_focused = true;
								username_rect_focused = false;
								password_rect_focused = false;
								email_rect_focused = false;
							}
						} else {
							username_rect_focused = false;
							password_rect_focused = false;
							email_rect_focused = false;
							password_c_rect_focused = false;
						}
					} else if (isChatlistState) {
						ChatListPageAction action = gui_manager.chatlistpage_action(mouseButtonPressed->position);
						if (action.search) {
							search_rect_focused = true;
						} else  {
							search_rect_focused = false;
						}

						if (action.chat_name != "") {
							gui_manager.build_chat_messages(client.getUser().getChat(action.chat_name));
							chat_entered = true;
							current_chat = client.getUser().getChat(action.chat_name).getName();
						} else if (action.clicked_outside_list)
							chat_entered = false;
					} 

					if (chat_entered) {
						if (gui_manager.chatpage_new_msg(mouseButtonPressed->position))
							new_msg_rect_focused = true;
						else
							new_msg_rect_focused = false;
					}

					if (isSearchState) {
						auto selection = gui_manager.search_selection(mouseButtonPressed->position);
						if (selection) {
							client.create_new_chat(*selection);
							isSearchState = false; // Move it somewhere else because it blocks the interface
						} else 
							isSearchState = false;
					}
				}
			}

			if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()) {
				if (isLoginState && username_rect_focused)
					gui_manager.lusername_enter_key(keyPressed->shift, keyPressed->code);
				if (isLoginState && password_rect_focused)
					gui_manager.lpassword_enter_key(keyPressed->shift, keyPressed->code);

				if (isRegisterState && username_rect_focused)
					gui_manager.rusername_enter_key(keyPressed->shift, keyPressed->code);
				if (isRegisterState && email_rect_focused)
					gui_manager.remail_enter_key(keyPressed->shift, keyPressed->code);
				if (isRegisterState && password_rect_focused)
					gui_manager.rpassword_enter_key(keyPressed->shift, keyPressed->code);
				if (isRegisterState && password_c_rect_focused)
					gui_manager.rpassword_c_enter_key(keyPressed->shift, keyPressed->code);

				if (isChatlistState && search_rect_focused)
					gui_manager.search_enter_key(keyPressed->shift, keyPressed->code);

				if (chat_entered && new_msg_rect_focused)
					gui_manager.new_msg_enter_key(keyPressed->shift, keyPressed->code);

				if (keyPressed->code == sf::Keyboard::Key::Enter) {
					if (isLoginState) {
						LoginPageData lpdata = gui_manager.get_loginpage_data();

						if (client.load_user(lpdata.username, lpdata.password)) {
							isLoginState = false;
							isRegisterState = false;
							isChatlistState = true;
						}
					} else if (isRegisterState) {
						RegisterPageData rpdata = gui_manager.get_registerpage_data();

						if (rpdata.password != rpdata.password_conf) {
							std::cout << "ERROR: Passwords do not match!\n"; // Should render something (it may not be the right spot to check that)
						} else {
							if (client.load_user(rpdata.username, rpdata.email, rpdata.password, rpdata.password_conf)) {
								isLoginState = false;
								isRegisterState = false;
								isChatlistState = true;
							}
						}
					} else if (isChatlistState) {
						ChatListPageData clpdata = gui_manager.get_chatlistpage_data();

						if (search_rect_focused) {
							if (client.search_users(clpdata.search_term))
								isSearchState = true;
						}

						if (new_msg_rect_focused) {
							ChatPageData cpdata = gui_manager.get_chatpage_data(client.getUser().getUsername());
							client.send_message(cpdata.content, client.getUser().getChat(current_chat));
						}
					}
				}
			}
		}

		client.handle_request(gui_manager);

		main_window.clear({ MAIN_COLOR_R, MAIN_COLOR_G, MAIN_COLOR_B });

		if (isLoginState)
			gui_manager.render_login_page();
		else if (isRegisterState)
			gui_manager.render_register_page();
		else if (isChatlistState) {
			if (!stop_rendering_chat_rects && gui_manager.build_chat_rects(client.getUser())) // to prevent BIG trouble with memory usage
				stop_rendering_chat_rects = true;

			gui_manager.render_chatlist_page();

			if (chat_entered)
				gui_manager.render_chat_page();
			if (isSearchState)
				gui_manager.render_search_results();
		}


		main_window.display();
	}
}

