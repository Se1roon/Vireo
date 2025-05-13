/// @file ConfigParser.hpp
/// @brief Declaration of the ConfigParser class.

#pragma once

#include <string>

/**
 * @class ConfigParser
 * @brief Responsible for getting information from the config files (key|value\n format).
 */
class ConfigParser {
	private:
		std::string config_file;

	public:
		ConfigParser(std::string config_file_path);

		std::string get_value(std::string key);
};

