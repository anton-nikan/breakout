#pragma once

#include <functional>
#include <sdk/window.h>
#include "types.h"
#include "collision.h"

namespace game {
	class BallView {
		SDK::Window& mWindow;
		SDK::SpriteID mSprite = SDK::InvalidSprite;
	public:
		BallView(SDK::Window& wnd, Vector pos) : mWindow{ wnd } {
			mSprite = mWindow.createSprite(SDK::SpriteType::Ball,
				pos.x - 0.5f * settings::BallDiameter,
				pos.y - 0.5f * settings::BallDiameter);
		}

		BallView(SDK::Window& wnd) : BallView(wnd, Vector{ 0.0f, 0.0f }) { }

		~BallView() {
			mWindow.removeSprite(mSprite);
		}

		void updatePosition(Vector pos) {
			mWindow.moveSprite(mSprite, pos.x - 0.5f * settings::BallDiameter,
				pos.y - 0.5f * settings::BallDiameter);
		}
	};

	class StuckBallController {
		BallView& mView;
		using ValueExtractor = std::function<Vector()>;
		ValueExtractor mValueExtractor;
	public:
		StuckBallController(BallView& view, ValueExtractor f) :
				mView{ view }, mValueExtractor{ f } { }

		BallView& getView() {
			return mView;
		}

		const Vector getPosition() const {
			return mValueExtractor();
		}

		void update() {
			mView.updatePosition(mValueExtractor());
		}
	};

	class PlayingBallController {
		BallView& mView;
		Vector mPrevPosition;
		Vector mPosition;
	public:
		PlayingBallController(BallView& view, Vector pos, Vector velocity) :
				mView{ view }, mPosition{ pos } {
			mPrevPosition = { pos.x -
					static_cast<float>(velocity.x * settings::SimulationTimeStep),
				pos.y - static_cast<float>(velocity.y * settings::SimulationTimeStep) };
		}

		BallView& getView() {
			return mView;
		}

		const BallView& getView() const {
			return mView;
		}

		void update() {
			// Verlet integration
			auto currPosition = mPosition;
			mPosition = { 2.0f * mPosition.x - mPrevPosition.x,
				2.0f * mPosition.y - mPrevPosition.y };
			mPrevPosition = currPosition;

			mView.updatePosition(mPosition);
		}

		bool below(float y) const {
			return mPosition.y > y;
		}

		bool inside(Rect r) const {
			return ::inside(mPosition, r);
		}

		Segment getLastMotionPath() const {
			return { mPrevPosition, mPosition };
		}

		Segment reflect(Rect r) {
			Segment l{ mPrevPosition, mPosition };
			if (::reflect(l, r)) {
				mPrevPosition = l.origin;
				mPosition = l.ending;
			}

			return l;
		}

		// Special reflection, using normal as if bouncing off the circle around the
		// rect
		Segment reflectRound(Rect r) {
			Segment l{ mPrevPosition, mPosition };
			if (::reflectRound(l, r)) {
				mPrevPosition = l.origin;
				mPosition = l.ending;
			}

			return l;
		}
	};
}
