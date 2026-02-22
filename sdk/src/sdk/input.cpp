#include <sdk/input.h>
#include <SFML/Window/Keyboard.hpp>

namespace SDK {
	bool isButtonPressed(Button button) {
		sf::Keyboard::Key keys[3] = {
			sf::Keyboard::Key::A,
			sf::Keyboard::Key::D,
			sf::Keyboard::Key::Space
		};

		return sf::Keyboard::isKeyPressed(keys[static_cast<int>(button)]);
	}
}
