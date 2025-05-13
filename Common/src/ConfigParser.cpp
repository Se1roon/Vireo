/// @file ConfigParser.cpp
/// @brief Implementation of the ConfigParser class.

#include <string>
#include <fstream>

#include "ConfigParser.hpp"
#include "ConfigParserException.hpp"


ConfigParser::ConfigParser(std::string config_file_path) : config_file(config_file_path) {}


std::string ConfigParser::get_value(std::string key) {
	std::ifstream file(config_file);
	if (!file.is_open())
		throw ConfigParserException("Unable to open config file. Does it exist?\n");

	std::string current_line;
	while (!file.eof()) {
		getline(file, current_line);

		int separator_i = current_line.find("|");
		if (key == current_line.substr(0, separator_i))
			return current_line.substr(separator_i + 1, current_line.length());
	}

	file.close();

	return ""; // Key doesn't exist! check for that
}

