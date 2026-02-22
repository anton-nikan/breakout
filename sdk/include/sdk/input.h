#pragma once

namespace SDK {
	enum class Button {
		Left,
		Right,
		Fire
	};

	// Check whether a button is currently pressed.
	bool isButtonPressed(Button button);
}
