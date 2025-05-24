#pragma once

#include <memory>
#include <string>

#include <SFML/Graphics.hpp>

#include "PHElement.hpp"


namespace GUI {
	class PHText : public PHElement {
		private:
			std::unique_ptr<sf::Text> content;
			sf::Font font; 
			sf::Color color = { 140, 135, 145 };
			int characterSize;

		public:
			PHText(sf::Font& font, std::string content, int characterSize);

			void setString(std::string s);
			void setFont(sf::Font& font);
			void setCharSize(int characterSize);
			void setFgColor(sf::Color c);
			void setPosition(sf::Vector2f position);

			std::string getString();
			sf::Vector2f getPosition();
			int getCharSize();

			bool hasBeenClicked(sf::Vector2i mouse_position);

			void render(sf::RenderWindow& window) override;
	};
};

