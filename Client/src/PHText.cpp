#include <memory>
#include <string>

#include <SFML/Graphics.hpp>

#include "PHText.hpp"


using namespace GUI;


PHText::PHText(sf::Font& font, std::string content, int characterSize = 48) {
	this->font = font;
	this->characterSize = characterSize;
	this->content = std::make_unique<sf::Text>(font, content, characterSize);

	this->content->setFillColor(color);
}


void PHText::setString(std::string s) {
	content->setString(s);
}

void PHText::setFont(sf::Font& font) {
	this->font = font;
	content->setFont(font);
}

void PHText::setCharSize(int charSize) {
	characterSize = charSize;
	content->setCharacterSize(charSize);
}

void PHText::setFgColor(sf::Color c) {
	color = c;
	content->setFillColor(c);
}

void PHText::setPosition(sf::Vector2f position) {
	sf::FloatRect bounds = content->getLocalBounds();
	content->setOrigin({
		bounds.position.x + bounds.size.x * 0.5f,
		bounds.position.y + bounds.size.y * 0.5f
	});

	content->setPosition(position);
}


std::string PHText::getString() {
	return content->getString();
}

sf::Vector2f PHText::getPosition() {
	return content->getPosition();
}

int PHText::getCharSize() {
	return characterSize;
}


bool PHText::hasBeenClicked(sf::Vector2i mouse_position) {
	sf::FloatRect bounds = content->getGlobalBounds();
	return (mouse_position.x >= bounds.position.x && mouse_position.x <= bounds.position.x + bounds.size.x &&
			mouse_position.y >= bounds.position.y && mouse_position.y <= bounds.position.y + bounds.size.y);
}


void PHText::render(sf::RenderWindow& window) {
	window.draw(*content);
}

