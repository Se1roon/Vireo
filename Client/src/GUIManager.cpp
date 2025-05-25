#include <iostream>
#include <optional>
#include <memory>
#include <cctype>
#include <string>

#include <SFML/Graphics.hpp>

#include "User.hpp"

#include "GUIManager.hpp"

#include "PHRectangle.hpp"
#include "PHText.hpp"


using namespace GUI;


static const std::unordered_map<sf::Keyboard::Key, std::string> skey_map = {
	{ sf::Keyboard::Key::A, "A"},
	{ sf::Keyboard::Key::B, "B"},
	{ sf::Keyboard::Key::C, "C"},
	{ sf::Keyboard::Key::D, "D"},
	{ sf::Keyboard::Key::E, "E"},
	{ sf::Keyboard::Key::F, "F"},
	{ sf::Keyboard::Key::G, "G"},
	{ sf::Keyboard::Key::H, "H"},
	{ sf::Keyboard::Key::I, "I"},
	{ sf::Keyboard::Key::J, "J"},
	{ sf::Keyboard::Key::K, "K"},
	{ sf::Keyboard::Key::L, "L"},
	{ sf::Keyboard::Key::M, "M"},
	{ sf::Keyboard::Key::N, "N"},
	{ sf::Keyboard::Key::O, "O"},
	{ sf::Keyboard::Key::P, "P"},
	{ sf::Keyboard::Key::Q, "Q"},
	{ sf::Keyboard::Key::R, "R"},
	{ sf::Keyboard::Key::S, "S"},
	{ sf::Keyboard::Key::T, "T"},
	{ sf::Keyboard::Key::U, "U"},
	{ sf::Keyboard::Key::V, "V"},
	{ sf::Keyboard::Key::W, "W"},
	{ sf::Keyboard::Key::X, "X"},
	{ sf::Keyboard::Key::Y, "Y"},
	{ sf::Keyboard::Key::Z, "Z"},
	{ sf::Keyboard::Key::Num0, "0" },
	{ sf::Keyboard::Key::Num1, "1" },
	{ sf::Keyboard::Key::Num2, "2" },
	{ sf::Keyboard::Key::Num3, "3" },
	{ sf::Keyboard::Key::Num4, "4" },
	{ sf::Keyboard::Key::Num5, "5" },
	{ sf::Keyboard::Key::Num6, "6" },
	{ sf::Keyboard::Key::Num7, "7" },
	{ sf::Keyboard::Key::Num8, "8" },
	{ sf::Keyboard::Key::Num9, "9" },
	{ sf::Keyboard::Key::Period, "." }
};


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
	if (loginpage_data.prompt->hasBeenClicked(mouse_position))
		return LPROMPT;
	
	if (loginpage_data.username_rect->hasBeenClicked(mouse_position))
		return LUSERNAME;
	
	if (loginpage_data.password_rect->hasBeenClicked(mouse_position))
		return LPASSWORD;

	return std::nullopt;
}

std::optional<RegisterPageAction> GUIManager::registerpage_action(sf::Vector2i mouse_position) {
	if (registerpage_data.prompt->hasBeenClicked(mouse_position))
		return RPROMPT;
	
	if (registerpage_data.username_rect->hasBeenClicked(mouse_position))
		return RUSERNAME;
	
	if (registerpage_data.password_rect->hasBeenClicked(mouse_position))
		return RPASSWORD;

	if (registerpage_data.email_rect->hasBeenClicked(mouse_position))
		return REMAIL;

	if (registerpage_data.password_conf_rect->hasBeenClicked(mouse_position))
		return RPASSWORD_C;

	return std::nullopt;
}


void GUIManager::lusername_enter_key(bool shift, sf::Keyboard::Key key) {
	std::string current_text = loginpage_data.username_rect->getPlaceholderText();
	if (key == sf::Keyboard::Key::Backspace && current_text.length() > 0)
		current_text.pop_back();
	else if (shift) {
		current_text.append(key_to_str(key));
	}
	else if (!shift)
		current_text.append(str_to_lower(key_to_str(key)));

	loginpage_data.username_rect->setPlaceholder(current_text);
}

void GUIManager::lpassword_enter_key(bool shift, sf::Keyboard::Key key) {
	std::string current_text = loginpage_data.password_rect->getPlaceholderText();
	if (key == sf::Keyboard::Key::Backspace && current_text.length() > 0)
		current_text.pop_back();
	else if (shift) {
		current_text.append(key_to_str(key));
	}
	else if (!shift)
		current_text.append(str_to_lower(key_to_str(key)));

	loginpage_data.password_rect->setPlaceholder(current_text);
}

void GUIManager::rusername_enter_key(bool shift, sf::Keyboard::Key key) {
	std::string current_text = registerpage_data.username_rect->getPlaceholderText();
	if (key == sf::Keyboard::Key::Backspace && current_text.length() > 0)
		current_text.pop_back();
	else if (shift) {
		current_text.append(key_to_str(key));
	}
	else if (!shift)
		current_text.append(str_to_lower(key_to_str(key)));

	registerpage_data.username_rect->setPlaceholder(current_text);
}

void GUIManager::rpassword_enter_key(bool shift, sf::Keyboard::Key key) {
	std::string current_text = registerpage_data.password_rect->getPlaceholderText();
	if (key == sf::Keyboard::Key::Backspace && current_text.length() > 0)
		current_text.pop_back();
	else if (shift) {
		current_text.append(key_to_str(key));
	}
	else if (!shift)
		current_text.append(str_to_lower(key_to_str(key)));

	registerpage_data.password_rect->setPlaceholder(current_text);
}

void GUIManager::remail_enter_key(bool shift, sf::Keyboard::Key key) {
	std::string current_text = registerpage_data.email_rect->getPlaceholderText();
	if (key == sf::Keyboard::Key::Backspace && current_text.length() > 0)
		current_text.pop_back();
	else if (shift) {
		if (key == sf::Keyboard::Key::Num2) current_text.append("@");
		else current_text.append(key_to_str(key));
	}
	else if (!shift)
		current_text.append(str_to_lower(key_to_str(key)));

	registerpage_data.email_rect->setPlaceholder(current_text);
}

void GUIManager::rpassword_c_enter_key(bool shift, sf::Keyboard::Key key) {
	std::string current_text = registerpage_data.password_conf_rect->getPlaceholderText();
	if (key == sf::Keyboard::Key::Backspace && current_text.length() > 0)
		current_text.pop_back();
	else if (shift) {
		current_text.append(key_to_str(key));
	}
	else if (!shift)
		current_text.append(str_to_lower(key_to_str(key)));

	registerpage_data.password_conf_rect->setPlaceholder(current_text);
}


LoginPageData GUIManager::get_loginpage_data() {
	LoginPageData lpdata;

	lpdata.username = loginpage_data.username_rect->getPlaceholderText();
	lpdata.password = loginpage_data.password_rect->getPlaceholderText();

	return lpdata;
}

RegisterPageData GUIManager::get_registerpage_data() {
	RegisterPageData rpdata;

	rpdata.username = registerpage_data.username_rect->getPlaceholderText();
	rpdata.email= registerpage_data.email_rect->getPlaceholderText();
	rpdata.password = registerpage_data.password_rect->getPlaceholderText();
	rpdata.password_conf = registerpage_data.password_conf_rect->getPlaceholderText();

	return rpdata;
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

void GUIManager::render_chatlist_page(User& user) {
	sf::Text username(main_font);

	username.setString(user.getUsername());
	username.setCharacterSize(36);
	username.setFillColor({ ACCENT_COLOR_R, ACCENT_COLOR_G, ACCENT_COLOR_B });
	username.setPosition({ 100.f, 200.f });

	window.draw(username);
}


std::string GUIManager::key_to_str(sf::Keyboard::Key key) {
	auto k_it = skey_map.find(key);

	return (k_it != skey_map.end() ? k_it->second : "");
}

std::string GUIManager::str_to_lower(std::string str) {
	if (!str.empty())
		str[0] = static_cast<char>( std::tolower(static_cast<unsigned char>(str[0])) );
	return str;
}

