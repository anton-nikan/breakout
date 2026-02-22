#pragma once

#include <memory>
#include <string>
#include <cstdint>

namespace SDK {
	// An ID identifying a sprite added to the system.
	using SpriteID = uint32_t;

	// The available sprite types.
	enum class SpriteType {
		Ball,
		Brick,
		PaddleLeft,
		PaddleMid,
		PaddleRight
	};

	// An invalid sprite ID. Note that other IDs can also be invalid.
	constexpr SpriteID InvalidSprite = 0xffffffff;

	// An ID identifying a text added to the system.
	using TextID = uint32_t;

	// An invalid text ID. Note that other IDs can also be invalid.
	constexpr TextID InvalidText = 0xffffffff;

	// Method result for ID-based methods
	enum class Result {
		Success,
		InvalidID
	};

	// Main interface to the graphical portion of the SDK. A Window instance opens
	// a window and allows drawing.
	class Window {
	public:
		// Construct a window with a width, height and title. The values given
		// here cannot changed later.
		Window(uint32_t width, uint32_t height, const std::string& title);
		Window(Window&& moved);
		~Window();
		Window& operator= (Window&& rhs);

		// Check whether the window is still open. Once closed by the user,
		// it cannot be re-opened through the same Window instance.
		bool isOpen() const noexcept;

		// Draw a frame containing all valid sprites and texts.
		void draw() const noexcept;

		// Create a sprite. The type given here will remain, but the sprite can be
		// moved later using the moveSprite() method.
		SpriteID createSprite(SpriteType type, float x, float y);

		// Move a sprite. Passing in an invalid ID is a valid call, and will
		// result in an InvalidID return value. If the sprite is valid, it will be
		// moved and Success is returned.
		Result moveSprite(SpriteID sprite, float x, float y) noexcept;

		// Remove a sprite, if it is valid. This invalidates the ID.
		void removeSprite(SpriteID sprite) noexcept;

		// Check whether a given ID represents a valid sprite.
		bool isValidSprite(SpriteID sprite) const noexcept;

		// Returns the number of valid sprites.
		size_t countValidSprites() const noexcept;

		// Create a text. The size given here will remain, but the string can be
		// updated later using the updateText() method and the text can be moved
		// using the moveText method() method.
		TextID createText(const std::string& text, unsigned size, float x, float y);

		// Change a text string. Passing in an invalid ID is a valid call, and
		// will result in an InvalidID return value. If the text is valid, it will
		// be changed and Success is returned.
		Result updateText(TextID id, const std::string& text) noexcept;

		// Move a text. Passing in an invalid ID is a valid call, and will
		// result in an InvalidID return value. If the text is valid, it will be
		// moved and Success is returned.
		Result moveText(TextID id, float x, float y) noexcept;

		// Remove a text, if it is valid. This invalidates the ID.
		void removeText(TextID id) noexcept;

		// Check whether a given ID represents a valid text.
		bool isValidText(TextID text) const noexcept;

		// Returns the number of valid texts.
		size_t countValidTexts() const noexcept;

		// Get the time passed since the window was created.
		double getSecondsSinceCreation() const noexcept;

		// Draw a debug line for one frame.
		void debugDrawLine(float x1, float y1, float x2, float y2);
	private:
		struct Data;
		std::unique_ptr<Data> mData;
	};

}
