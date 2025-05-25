#include <string>
#include <SFML/Graphics.hpp>

#include "PHRectangle.hpp"


using namespace GUI;


PHRectangle::PHRectangle(sf::Font& font, std::string placeholder, int charSize) {
	this->font = font;
	this->charSize = charSize;

	this->placeholder = std::make_unique<sf::Text>(font, placeholder, charSize);
	this->placeholder->setFillColor(fg_color);

	rect.setPosition(position);
	rect.setSize(size);
	rect.setFillColor(bg_color);
	rect.setOutlineColor(outline_color);
	rect.setOutlineThickness(outline_thickness);

	// Literal numbers here are paddings
	this->placeholder->setPosition({ position.x + 5, position.y + 5});
}


void PHRectangle::setPlaceholder(std::string placeholder) {
	this->placeholder->setString(placeholder);
}

void PHRectangle::setCharSize(int charSize) {
	this->charSize = charSize;
	placeholder->setCharacterSize(charSize);
}

void PHRectangle::setFont(sf::Font& font) {
	this->font = font;
	placeholder->setFont(font);
}

void PHRectangle::setBgColor(sf::Color bg_color) {
	this->bg_color = bg_color;
	rect.setFillColor(bg_color);
}

void PHRectangle::setFgColor(sf::Color fg_color) {
	this->fg_color = fg_color;
	placeholder->setFillColor(fg_color);
}

void PHRectangle::setOutlineColor(sf::Color outline_color) {
	this->outline_color = outline_color;
	rect.setOutlineColor(outline_color);
}

void PHRectangle::setOutlineThickness(int thickness) {
	outline_thickness = thickness;
	rect.setOutlineThickness(thickness);
}

void PHRectangle::setPosition(sf::Vector2f position) {
	this->position = position;
	rect.setPosition(position);
	placeholder->setPosition({ position.x + 5, position.y + 5 });
}

void PHRectangle::setSize(sf::Vector2f size) {
	this->size = size;
	rect.setSize(size);
}


std::string PHRectangle::getPlaceholderText() {
	return placeholder->getString();
}

sf::Vector2f PHRectangle::getPosition() {
	return position;
}

sf::Vector2f PHRectangle::getSize() {
	return size;
}


bool PHRectangle::hasBeenClicked(sf::Vector2i& mousePosition) {
	return (mousePosition.x >= position.x && mousePosition.x <= position.x + size.x &&
			mousePosition.y >= position.y && mousePosition.y <= position.y + size.y);
}


void PHRectangle::render(sf::RenderWindow& window) {
	window.draw(rect);
	window.draw(*placeholder);
}

