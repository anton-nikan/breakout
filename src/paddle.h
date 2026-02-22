#pragma once

#include <vector>
#include <algorithm>
#include <sdk/window.h>
#include <sdk/input.h>
#include "format.h"
#include "types.h"
#include "settings.h"

namespace game {
	class PaddleView {
		SDK::Window& mWindow;
		std::vector<SDK::SpriteID> mSprites;

		static float leftPosition(float x, float size) {
			return x - size * settings::PaddleMiddleWidth / 2 -
				settings::PaddleLeftWidth;
		}

		static float rightPosition(float x, float size) {
			return x + size * settings::PaddleMiddleWidth / 2;
		}

		static float midPosition(float x, float size, int idx) {
			return x - size * settings::PaddleMiddleWidth / 2 +
				settings::PaddleLeftWidth * idx;
		}
	public:
		PaddleView(SDK::Window& wnd, int size, Vector pos) : mWindow{ wnd } {
			mSprites.emplace_back(
				mWindow.createSprite(SDK::SpriteType::PaddleLeft, 0, 0));
			for (int i = 0; i < size; ++i) {
				mSprites.emplace_back(
					mWindow.createSprite(SDK::SpriteType::PaddleMid, 0, 0));
			}
			mSprites.emplace_back(
				mWindow.createSprite(SDK::SpriteType::PaddleRight, 0, 0));

			updatePosition(pos);
		}

		~PaddleView() {
			for (auto spriteID : mSprites) {
				mWindow.removeSprite(spriteID);
			}
		}

		int getSize() const {
			return static_cast<int>(mSprites.size()) - 2;
		}

		void updatePosition(Vector pos) {
			const float padSize = static_cast<float>(mSprites.size()) - 2;
			mWindow.moveSprite(mSprites.front(), leftPosition(pos.x, padSize), pos.y);
			mWindow.moveSprite(mSprites.back(), rightPosition(pos.x, padSize), pos.y);
			for (int i = 1; i < mSprites.size() - 1; ++i) {
				mWindow.moveSprite(mSprites[i], midPosition(pos.x, padSize, i - 1),
					pos.y);
			}
		}
	public:
		static int widthForSize(int size) {
			return settings::PaddleRightWidth + size * settings::PaddleMiddleWidth +
				settings::PaddleRightWidth;
		}
	};

	class PaddleViewController {
		PaddleView mView;
		Vector mPosition;
		float mSpeed = 0.0f;

		static float boundPosition(float x, float size) {
			const float leftBound = settings::GridLeft;
			const float rightBound = settings::GridLeft +
				settings::GridWidth * settings::BrickWidth;
			const float halfPaddleWidth = 0.5f *
				(size * settings::PaddleMiddleWidth + settings::PaddleLeftWidth +
					settings::PaddleRightWidth);
			x = std::max(x, leftBound + halfPaddleWidth);
			x = std::min(x, rightBound - halfPaddleWidth);

			return x;
		}
	public:
		PaddleViewController(SDK::Window& wnd, int size, Vector pos) :
				mView{ wnd, size, pos }, mPosition{ pos } { }

		void update() {
			constexpr double dt = settings::SimulationTimeStep;
			if (SDK::isButtonPressed(SDK::Button::Left)) {
				if (mSpeed > 0.0f) {
					mSpeed = 0.0f;
				}
				mSpeed -= static_cast<float>(dt * settings::PaddleAcceleration);
				mSpeed = std::max(mSpeed, -settings::PaddleMaxSpeed);
			}
			else if (SDK::isButtonPressed(SDK::Button::Right)) {
				if (mSpeed < 0.0f) {
					mSpeed = 0.0f;
				}
				mSpeed += static_cast<float>(dt * settings::PaddleAcceleration);
				mSpeed = std::min(mSpeed, settings::PaddleMaxSpeed);
			}
			else {
				mSpeed = 0.0f;
			}

			if (mSpeed != 0.0f) {
				const float padSize = static_cast<float>(mView.getSize());
				mPosition = { boundPosition(
												mPosition.x + static_cast<float>(mSpeed * dt), padSize),
					mPosition.y };
				mView.updatePosition(mPosition);
			}
		}

		const Vector& getPosition() const {
			return mPosition;
		}

		Rect getRect() const {
			const int padWidth = PaddleView::widthForSize(mView.getSize());
			return { .left = mPosition.x - 0.5f * padWidth,
				.top = mPosition.y,
				.right = mPosition.x + 0.5f * padWidth,
				.bottom = mPosition.y + settings::PaddleHeight };
		}
	};
}
