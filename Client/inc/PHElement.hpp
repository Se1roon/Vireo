#pragma once

#include <SFML/Graphics.hpp>


namespace GUI {
	class PHElement {
		public:
			virtual void render(sf::RenderWindow& window) = 0;
	};
};

