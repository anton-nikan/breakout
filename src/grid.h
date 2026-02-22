#pragma once

#include <vector>
#include <optional>
#include <algorithm>
#include <sdk/window.h>
#include "types.h"
#include "settings.h"
#include "levels.h"
#include "collision.h"

namespace game {
	struct Brick {
		Rect bounds;
		levels::BrickType type = levels::BrickType::Empty;
		SDK::SpriteID spriteID = SDK::InvalidSprite;

		bool operator== (const Brick& b) const {
			return b.type == type && b.bounds == bounds && b.spriteID == spriteID;
		}
	};

	class Grid {
		SDK::Window& mWindow;
		std::vector<Brick> mBricks;

		static SDK::SpriteType brickTypeToSpriteType(levels::BrickType bt) {
			// TODO: add more sprites for each BrickType
			return SDK::SpriteType::Brick;
		}

		static Point indexToGrid(int index, int gridWidth) {
			return { index / gridWidth, index % gridWidth };
		}
	public:
		Grid(SDK::Window& wnd, const levels::GridDataType& level) : mWindow{ wnd } {
			for (int index = 0; index < level.size(); ++index) {
				levels::BrickType bt = level[index];
				if (bt == levels::BrickType::Empty) {
					continue;
				}

				const auto [i, j] = indexToGrid(index, settings::GridWidth);
				const float x = static_cast<float>(
					settings::GridLeft + settings::BrickWidth * j);
				const float y = static_cast<float>(
					settings::GridTop + settings::BrickHeight * i);

				mBricks.emplace_back(Brick{
					Rect{ x, y, x + settings::BrickWidth, y + settings::BrickHeight },
					bt,
					mWindow.createSprite(brickTypeToSpriteType(bt), x, y)
				});
			}
		}

		~Grid() {
			for (const auto& brick : mBricks) {
				mWindow.removeSprite(brick.spriteID);
			}
		}

		bool empty() const {
			return mBricks.empty();
		}

		void removeBrick(const Brick& brick) {
			auto it = std::ranges::find(mBricks, brick);
			if (it != std::end(mBricks)) {
				mWindow.removeSprite(it->spriteID);
				mBricks.erase(it);
			}
		}

		std::optional<Brick> findBrick(Vector p) const {
			auto it = std::ranges::find_if(mBricks,
				[&](const Brick& b) { return inside(p, b.bounds); });
			if (it != std::end(mBricks)) {
				return *it;
			}
			return std::nullopt;
		}
	};
}
