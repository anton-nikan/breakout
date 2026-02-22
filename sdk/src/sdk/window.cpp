#include <sdk/window.h>

#include <SFML/Graphics.hpp>
#include <SFML/Graphics/PrimitiveType.hpp>

#include <cassert>
#include <vector>

namespace SDK {

	const uint32_t InvalidIndex = 0x00ffffff;

	using ResourceIndex = uint32_t;
	using MetaIndex = uint32_t;
	using Era = uint8_t;

	// A sprite/text ID is an index and an era
	// When the ID is valid (in use), it contains a resource index
	// When invalid, it can contain a meta index
	class InternalResourceID {
	public:
		explicit InternalResourceID(SpriteID id) : mID(id) { }
		explicit InternalResourceID(uint32_t index, Era era) : mID(index | (static_cast<uint32_t>(era) << 24)) { }

		uint32_t getIndex() const noexcept {
			return mID & 0x00ffffff;
		}

		Era getEra() const noexcept {
			return static_cast<Era>((mID & (0xff << 24)) >> 24);
		}

		SpriteID asID() const noexcept {
			return mID;
		}
	private:
		uint32_t mID;
	};

	template<typename Resource>
	class ResourceStorage {
	public:
		ResourceStorage() noexcept : mNext(0, 0) { }

		void findNext() noexcept {
			for (MetaIndex index = mNext.getIndex() + 1; index < mIndices.size(); ++index) {
				if (mIndices[index].getIndex() == InvalidIndex) {
					mNext = InternalResourceID(index, mIndices[index].getEra());
					return;
				}
			}
			mNext = InternalResourceID(static_cast<MetaIndex>(mIndices.size()), 0);
		}

		InternalResourceID create(Resource&& resource) {
			ResourceIndex index = static_cast<ResourceIndex>(mResources.size());
			InternalResourceID id = mNext;
			MetaIndex metaIndex = id.getIndex();

			mResources.emplace_back(std::make_pair(metaIndex, std::move(resource)));

			Era era = id.getEra();
			id = InternalResourceID(index, era);
			if (mIndices.size() == metaIndex) {
				mIndices.push_back(id);
			}
			else {
				mIndices[metaIndex] = id;
			}
			findNext();
			return InternalResourceID(metaIndex, era);
		}

		Resource* get(InternalResourceID id) noexcept {
			uint32_t index = id.getIndex();
			if (index >= mIndices.size()) {
				return nullptr;
			}

			Era era = id.getEra();
			const auto& metaEntry = mIndices[index];
			Era current = metaEntry.getEra();
			if (era != current) {
				return nullptr;
			}

			ResourceIndex resourceIndex = mIndices[index].getIndex();
			if (resourceIndex >= mResources.size()) {
				return nullptr;
			}

			return &mResources[metaEntry.getIndex()].second;
		}

		void remove(InternalResourceID id) noexcept {
			MetaIndex index = id.getIndex();
			if (index >= mIndices.size()) {
				return;
			}

			Era era = id.getEra();
			Era current = mIndices[index].getEra();
			if (era != current) {
				return;
			}

			if (index < mNext.getIndex()) {
				mNext = InternalResourceID(index, era + 1);
			}
			ResourceIndex resourceIndex = mIndices[index].getIndex();
			if (resourceIndex >= mResources.size()) {
				return;
			}

			if (resourceIndex != mResources.size() - 1) {
				mResources[resourceIndex] = std::move(mResources.back());
				MetaIndex movedMetaIndex = mResources[resourceIndex].first;
				InternalResourceID& movedID = mIndices[movedMetaIndex];
				movedID = InternalResourceID(resourceIndex, movedID.getEra());
				mResources[resourceIndex].first = movedMetaIndex;
			}
			mIndices[index] = InternalResourceID(InvalidIndex, era + 1);
			mResources.pop_back();
		}

		const std::vector<std::pair<MetaIndex, Resource>>& all() const noexcept {
			return mResources;
		}
	private:
		std::vector<std::pair<MetaIndex, Resource>> mResources;
		std::vector<InternalResourceID> mIndices;
		InternalResourceID mNext;
	};

	struct Window::Data {
		Data(uint32_t width, uint32_t height, const std::string& title) :
				window(sf::VideoMode({ width, height }), title) { }

		sf::RenderWindow window;
		sf::Clock clock;
		ResourceStorage<sf::Sprite> sprites;
		ResourceStorage<sf::Text> texts;
		std::vector<sf::Vector2f> debugLines;
		sf::Texture textures[5];
		sf::Font font;
	};

	Window::Window(uint32_t width, uint32_t height, const std::string& title) :
			mData(std::make_unique<Data>(width, height, title)) {
		bool loaded = mData->textures[0].loadFromFile("resources/ball.png");
		assert(loaded);
		loaded = mData->textures[1].loadFromFile("resources/brick.png");
		assert(loaded);
		loaded = mData->textures[2].loadFromFile("resources/paddle_left.png");
		assert(loaded);
		loaded = mData->textures[3].loadFromFile("resources/paddle_mid.png");
		assert(loaded);
		loaded = mData->textures[4].loadFromFile("resources/paddle_right.png");
		assert(loaded);
		loaded = mData->font.openFromFile("resources/VictorMono-Bold.ttf");
		assert(loaded);
	}

	Window::Window(Window&& moved) = default;
	Window::~Window() = default;
	Window& Window::operator= (Window&& rhs) = default;

	bool Window::isOpen() const noexcept {
		return mData->window.isOpen();
	}

	SpriteID Window::createSprite(SpriteType type, float x, float y) {
		sf::Sprite sprite(mData->textures[static_cast<int>(type)]);
		sprite.setPosition({ x, y });
		return mData->sprites.create(std::move(sprite)).asID();
	}

	Result Window::moveSprite(SpriteID id, float x, float y) noexcept {
		sf::Sprite* sprite = mData->sprites.get(InternalResourceID(id));
		if (!sprite) {
			return Result::InvalidID;
		}

		sprite->setPosition({ x, y });
		return Result::Success;
	}

	void Window::removeSprite(SpriteID sprite) noexcept {
		mData->sprites.remove(InternalResourceID(sprite));
	}

	bool Window::isValidSprite(SpriteID sprite) const noexcept {
		InternalResourceID id(sprite);
		return mData->sprites.get(id) != nullptr;
	}

	size_t Window::countValidSprites() const noexcept {
		return mData->sprites.all().size();
	}

	TextID Window::createText(const std::string& text, unsigned size, float x, float y) {
		sf::Text element(mData->font, text, size);
		element.setPosition({ x, y });
		return mData->texts.create(std::move(element)).asID();
	}

	Result Window::updateText(TextID id, const std::string& string) noexcept {
		sf::Text* text = mData->texts.get(InternalResourceID(id));
		if (!text) {
			return Result::InvalidID;
		}
		text->setString(string);
		return Result::Success;
	}

	Result Window::moveText(TextID id, float x, float y) noexcept {
		sf::Text* text = mData->texts.get(InternalResourceID(id));
		if (!text) {
			return Result::InvalidID;
		}
		text->setPosition({ x, y });
		return Result::Success;
	}

	void Window::removeText(TextID id) noexcept {
		mData->texts.remove(InternalResourceID(id));
	}

	bool Window::isValidText(TextID text) const noexcept {
		return mData->texts.get(InternalResourceID(text)) != nullptr;
	}

	size_t Window::countValidTexts() const noexcept {
		return mData->texts.all().size();
	}

	void Window::debugDrawLine(float x1, float y1, float x2, float y2) {
		mData->debugLines.push_back(sf::Vector2f(x1, y1));
		mData->debugLines.push_back(sf::Vector2f(x2, y2));
	}

	void Window::draw() const noexcept {
		while (auto event = mData->window.pollEvent()) {
			// "close requested" event: we close the window
			if (event->is<sf::Event::Closed>()) {
				mData->window.close();
			}
		}

		mData->window.clear();
		for (auto&& entry : mData->sprites.all()) {
			mData->window.draw(entry.second);
		}
		for (auto&& entry : mData->texts.all()) {
			mData->window.draw(entry.second);
		}
		for (size_t i = 0; i < mData->debugLines.size(); i += 2) {
			sf::Vertex line[] = {
				sf::Vertex(mData->debugLines[i]),
				sf::Vertex(mData->debugLines[i + 1])
			};
			mData->window.draw(line, 2, sf::PrimitiveType::Lines);
		}
		mData->debugLines.clear();
		mData->window.display();
	}

	double Window::getSecondsSinceCreation() const noexcept {
		sf::Time time = mData->clock.getElapsedTime();
		return time.asSeconds();
	}

}
