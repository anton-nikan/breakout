#pragma once

#include <vector>
#include <memory>
#include <sdk/window.h>
#include "format.h"
#include "types.h"
#include "settings.h"
#include "levels.h"
#include "grid.h"
#include "round.h"

namespace game {
	class Session {
		SDK::Window& mWindow;

		Grid mGrid;
		std::unique_ptr<Round> mRoundPtr;
		std::vector<PaddleView> mLivesList;
		int mGameScore = 0;

		SDK::TextID mScoreText = SDK::InvalidText;

		std::unique_ptr<Round> makeNewRound() {
			return std::make_unique<Round>(mWindow, mGrid,
				[this](levels::BrickType bt) {
				mGameScore += settings::GameScorePerBrick;
			});
		}
	public:
		Session(SDK::Window& wnd, const levels::GridDataType& level) :
				mWindow{ wnd }, mGrid{ wnd, level } {
			mScoreText = mWindow.createText("Score: 000000", 20, settings::ScoreX,
				settings::ScoreY);
			mRoundPtr = makeNewRound();

			const float normalPaddleWidth = static_cast<float>(
				PaddleView::widthForSize(settings::PaddleSizeNormal));
			for (int i = 0; i < settings::GameNumLives; ++i) {
				mLivesList.emplace_back(mWindow, settings::PaddleSizeNormal,
					Vector{ settings::WindowWidth -
							(i + 0.5f) * (settings::LifePaddleGap + normalPaddleWidth),
						settings::LifePaddleY });
			}
		}

		~Session() {
			mWindow.removeText(mScoreText);
		}

		enum class UpdateResult { GameContinues,
			GameOver,
			GameWon };
		UpdateResult update() {
			auto roundResult = mRoundPtr->update();
			mWindow.updateText(mScoreText, std::format("Score: {:0>6}", mGameScore));

			switch (roundResult) {
				case Round::UpdateResult::Completed:
					return UpdateResult::GameWon;
				case Round::UpdateResult::Failed:
					if (mLivesList.empty()) {
						return UpdateResult::GameOver;
					}

					mLivesList.pop_back();
					mRoundPtr = makeNewRound();
					[[fallthrough]];
				case Round::UpdateResult::Continue:
					return UpdateResult::GameContinues;
			}

			// For whatever reason msvc complains here about not all control paths
			// returning a value
			return UpdateResult::GameContinues;
		}
	};
}
