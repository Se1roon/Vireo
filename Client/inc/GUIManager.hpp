#pragma once

#include <memory>
#include <optional>

#include <SFML/Graphics.hpp>

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

	enum LoginPageAction {
		USERNAME,
		PASSWORD,
		PROMPT
	};

	struct RegisterPage {
		std::unique_ptr<PHText> title;
		std::unique_ptr<PHText> prompt;
		std::unique_ptr<PHRectangle> username_rect;
		std::unique_ptr<PHRectangle> email_rect;
		std::unique_ptr<PHRectangle> password_rect;
		std::unique_ptr<PHRectangle> password_conf_rect;
	};


	class GUIManager {
		private:
			sf::RenderWindow& window;
			sf::Font& main_font;
			sf::Font& second_font;

			LoginPage loginpage_data;
			RegisterPage registerpage_data;

			LoginPage build_login_page();
			RegisterPage build_register_page();

		public:
			GUIManager(sf::RenderWindow& window, sf::Font& main_font, sf::Font& second_font);

			std::optional<LoginPageAction> loginpage_action(sf::Vector2i mouse_position);

			void render_login_page();
			void render_register_page();
	};
};

