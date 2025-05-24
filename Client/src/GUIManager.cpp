#include <optional>

#include <SFML/Graphics.hpp>

#include "GUIManager.hpp"

#include "PHRectangle.hpp"
#include "PHText.hpp"


using namespace GUI;


// Maybe later add a vector of fonts
GUIManager::GUIManager(sf::RenderWindow& window, sf::Font& main_font, sf::Font& second_font) :
	window(window), main_font(main_font), second_font(second_font) {

	loginpage_data = build_login_page();
	registerpage_data = build_register_page();
}


LoginPage GUIManager::build_login_page() {
	LoginPage lpdata;

	lpdata.title = std::make_unique<PHText>(second_font, "Vireo", 72);
	lpdata.prompt = std::make_unique<PHText>(main_font, "Click here to register", 24);
	lpdata.username_rect = std::make_unique<PHRectangle>(main_font, "Username", 48);
	lpdata.password_rect = std::make_unique<PHRectangle>(main_font, "Password", 48);

	lpdata.title->setFgColor({ ACCENT_COLOR_R, ACCENT_COLOR_G, ACCENT_COLOR_B });
	lpdata.title->setPosition({ window.getSize().x * .5f, 90});

	lpdata.username_rect->setSize({ 500.f, 88.f });
	lpdata.username_rect->setPosition({ window.getSize().x * .5f - lpdata.username_rect->getSize().x * .5f, 336.f });

	lpdata.password_rect->setSize({ 500.f, 88.f });
	lpdata.password_rect->setPosition({ window.getSize().x * .5f - lpdata.password_rect->getSize().x * .5f, 474.f });

	lpdata.prompt->setFgColor({ DIMM_COLOR_R, DIMM_COLOR_R, DIMM_COLOR_B });
	lpdata.prompt->setPosition({ 
			lpdata.password_rect->getPosition().x + lpdata.password_rect->getSize().x * .5f,
			lpdata.password_rect->getPosition().y + lpdata.password_rect->getSize().y + 50
	});
	
	return lpdata;
}

RegisterPage GUIManager::build_register_page() {
	RegisterPage rpdata;

	rpdata.title = std::make_unique<PHText>(second_font, "Vireo", 72);
	rpdata.prompt = std::make_unique<PHText>(main_font, "Click here to login", 24);
	rpdata.username_rect = std::make_unique<PHRectangle>(main_font, "Username", 48);
	rpdata.password_rect = std::make_unique<PHRectangle>(main_font, "Password", 48);
	rpdata.email_rect = std::make_unique<PHRectangle>(main_font, "Email", 48);
	rpdata.password_conf_rect = std::make_unique<PHRectangle>(main_font, "Confirm password", 48);

	rpdata.title->setFgColor({ ACCENT_COLOR_R, ACCENT_COLOR_G, ACCENT_COLOR_B });
	rpdata.title->setPosition({ window.getSize().x * .5f, 90});

	rpdata.username_rect->setSize({ 500.f, 88.f });
	rpdata.username_rect->setPosition({ window.getSize().x * .5f - rpdata.username_rect->getSize().x * .5f, 205.f });

	rpdata.email_rect->setSize({ 500.f, 88.f });
	rpdata.email_rect->setPosition({ window.getSize().x * .5f - rpdata.email_rect->getSize().x * .5f, 318.f });

	rpdata.password_rect->setSize({ 500.f, 88.f });
	rpdata.password_rect->setPosition({ window.getSize().x * .5f - rpdata.password_rect->getSize().x * .5f, 432.f });

	rpdata.password_conf_rect->setSize({ 500.f, 88.f });
	rpdata.password_conf_rect->setPosition({ window.getSize().x * .5f - rpdata.password_conf_rect->getSize().x * .5f, 545.f });

	rpdata.prompt->setFgColor({ DIMM_COLOR_R, DIMM_COLOR_R, DIMM_COLOR_B });
	rpdata.prompt->setPosition({ 
			rpdata.password_conf_rect->getPosition().x + rpdata.password_conf_rect->getSize().x * .5f,
			rpdata.password_conf_rect->getPosition().y + rpdata.password_conf_rect->getSize().y + 50
	});

	return rpdata;
}


std::optional<LoginPageAction> GUIManager::loginpage_action(sf::Vector2i mouse_position) {
	// Prompt has been clicked
	if (loginpage_data.prompt->hasBeenClicked(mouse_position))
		return PROMPT;
	
	// Username rect has been clicked
	if (loginpage_data.username_rect->hasBeenClicked(mouse_position))
		return USERNAME;
	
	// Password rect has been clicked
	if (loginpage_data.password_rect->hasBeenClicked(mouse_position))
		return PASSWORD;

	return std::nullopt;
}


void GUIManager::render_login_page() {
	loginpage_data.title->render(window);
	loginpage_data.username_rect->render(window);
	loginpage_data.password_rect->render(window);
	loginpage_data.prompt->render(window);
}

void GUIManager::render_register_page() {
	registerpage_data.title->render(window);
	registerpage_data.username_rect->render(window);
	registerpage_data.email_rect->render(window);
	registerpage_data.password_rect->render(window);
	registerpage_data.password_conf_rect->render(window);
	registerpage_data.prompt->render(window);
}

