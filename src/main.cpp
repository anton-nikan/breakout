#include <cassert>
#include <string>

#include <sdk/window.h>
#include <sdk/input.h>

#include "settings.h"
#include "levels.h"
#include "player.h"
#include "session.h"

int main() {
	using namespace game;

	auto playerProgress = player::Progress{};	 // likely to be loaded from save
																						 // file
	SDK::Window window(settings::WindowWidth, settings::WindowHeight, "Craftout");
	while (window.isOpen()) {
		Session session{ window, levels::GridData[playerProgress.mLevel] };

		Session::UpdateResult sessionResult = Session::UpdateResult::GameContinues;
		double lastTime = window.getSecondsSinceCreation();
		while (window.isOpen() &&
			sessionResult == Session::UpdateResult::GameContinues) {
			const double nowTime = window.getSecondsSinceCreation();

			// Locked step simulation
			while (nowTime - lastTime >= settings::SimulationTimeStep) {
				sessionResult = session.update();
				switch (sessionResult) {
					case Session::UpdateResult::GameOver:
						playerProgress.mLevel = 0;
						break;
					case Session::UpdateResult::GameWon:
						++playerProgress.mLevel;
						if (playerProgress.mLevel >= levels::GridData.size()) {
							playerProgress.mLevel = 0;
						}
						break;
					case Session::UpdateResult::GameContinues:
						break;
				}

				if (sessionResult != Session::UpdateResult::GameContinues) {
					break;
				}

				lastTime += settings::SimulationTimeStep;
			}

			window.draw();
		}

		// Session outcome for player to see
		const std::string outcomeText =
			sessionResult == Session::UpdateResult::GameOver ? "Game Over"
																											 : "Level Completed";
		SDK::TextID outcomeTextID = window.createText(outcomeText, 20,
			0.5f * (settings::WindowWidth - 12 * outcomeText.size()),
			0.5f * settings::WindowHeight);
		while (window.isOpen()) {
			window.draw();
			if (SDK::isButtonPressed(SDK::Button::Fire)) {
				break;
			}
		}
		window.removeText(outcomeTextID);
	}

	assert(window.countValidSprites() == 0);
	assert(window.countValidTexts() == 0);

	return 0;
}
