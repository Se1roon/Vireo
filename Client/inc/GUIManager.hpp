#pragma once

#include <memory>
#include <optional>
#include <vector>

#include <SFML/Graphics.hpp>

#include "Chat.hpp"
#include "User.hpp"

#include "PHElement.hpp"
#include "PHRectangle.hpp"
#include "PHText.hpp"

// Colors
#define MAIN_COLOR_R 230
#define MAIN_COLOR_G 230
#define MAIN_COLOR_B 230

#define ACCENT_COLOR_R 108
#define ACCENT_COLOR_G 0
#define ACCENT_COLOR_B 162

#define DIMM_COLOR_R 180
#define DIMM_COLOR_G 180
#define DIMM_COLOR_B 180

#define DIMM2_COLOR_R 140
#define DIMM2_COLOR_G 135
#define DIMM2_COLOR_B 135


namespace GUI {
	struct LoginPage {
		std::unique_ptr<PHText> title;
		std::unique_ptr<PHText> prompt;
		std::unique_ptr<PHRectangle> username_rect;
		std::unique_ptr<PHRectangle> password_rect;
	};

	struct LoginPageData {
		std::string username;
		std::string password;
	};

	enum LoginPageAction {
		LUSERNAME,
		LPASSWORD,
		LPROMPT
	};


	struct RegisterPage {
		std::unique_ptr<PHText> title;
		std::unique_ptr<PHText> prompt;
		std::unique_ptr<PHRectangle> username_rect;
		std::unique_ptr<PHRectangle> email_rect;
		std::unique_ptr<PHRectangle> password_rect;
		std::unique_ptr<PHRectangle> password_conf_rect;
	};

	struct RegisterPageData {
		std::string username;
		std::string email;
		std::string password;
		std::string password_conf;
	};

	enum RegisterPageAction {
		RUSERNAME,
		REMAIL,
		RPASSWORD,
		RPASSWORD_C,
		RPROMPT
	};


	struct ChatListPage {
		std::unique_ptr<PHRectangle> search_rect;
		std::unique_ptr<sf::RectangleShape> search_line;
		std::unique_ptr<sf::RectangleShape> chats_rect;
	};

	struct ChatListPageData {
		std::string search_term;
	};

	enum ChatListPageAction {
		CLSEARCH
	};


	class GUIManager {
		private:
			sf::RenderWindow& window;
			sf::Font& main_font;
			sf::Font& second_font;

			LoginPage loginpage_data;
			RegisterPage registerpage_data;
			ChatListPage chatlistpage_data; 

			LoginPage build_login_page();
			RegisterPage build_register_page();
			ChatListPage build_chatlist_page();

			std::string key_to_str(sf::Keyboard::Key key);
			std::string str_to_lower(std::string str);

		public:
			GUIManager(sf::RenderWindow& window, sf::Font& main_font, sf::Font& second_font);

			std::optional<LoginPageAction> loginpage_action(sf::Vector2i mouse_position);
			std::optional<RegisterPageAction> registerpage_action(sf::Vector2i mouse_position);
			std::optional<ChatListPageAction> chatlistpage_action(sf::Vector2i mouse_position);

			void lusername_enter_key(bool shift, sf::Keyboard::Key key);
			void lpassword_enter_key(bool shift, sf::Keyboard::Key key);

			void rusername_enter_key(bool shift, sf::Keyboard::Key key);
			void rpassword_enter_key(bool shift, sf::Keyboard::Key key);
			void remail_enter_key(bool shift, sf::Keyboard::Key key);
			void rpassword_c_enter_key(bool shift, sf::Keyboard::Key key);

			void search_enter_key(bool shift, sf::Keyboard::Key key);

			LoginPageData get_loginpage_data();
			RegisterPageData get_registerpage_data();
			ChatListPageData get_chatlistpage_data();

			void render_login_page();
			void render_register_page();
			void render_chatlist_page(User& user);
			void render_search_results(std::vector<std::string> usernames);
	};
};

