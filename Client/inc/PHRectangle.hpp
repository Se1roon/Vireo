#pragma once

#include <string>

#include <SFML/Graphics.hpp>


namespace GUI {
	class PHRectangle {
		private:
			sf::RectangleShape rect;
			std::unique_ptr<sf::Text> placeholder;
			sf::Font font;

			int charSize = 24;

			sf::Vector2f position = { 0, 0 };
			sf::Vector2f size = { 100, 100 };

			sf::Color bg_color = { 180, 180, 180 };
			sf::Color fg_color = { 140, 135, 145 };
			sf::Color outline_color = { 108, 0, 162 };

			int outline_thickness = 4;

		public:
			PHRectangle(sf::Font& font, std::string placeholder, int charSize);
			//PHRectangle(sf::Font& font, std::string placeholder, sf::Color bg_color, sf::Color fg_color, sf::Color outline_color, int outline_thickness);

			void setPlaceholder(std::string placeholder);
			void setCharSize(int charSize);
			void setFont(sf::Font& font);
			void setBgColor(sf::Color bg_color);
			void setFgColor(sf::Color fg_color);
			void setOutlineColor(sf::Color outline_color);
			void setOutlineThickness(int thickness);
			void setPosition(sf::Vector2f position);
			void setSize(sf::Vector2f size);

			sf::Vector2f getPosition();
			sf::Vector2f getSize();
			
			bool hasBeenClicked(sf::Vector2i& mousePosition);

			void render(sf::RenderWindow& window);
	};
};

