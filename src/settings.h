#pragma once

namespace settings {
	constexpr double SimulationTimeStep = 0.016;
	constexpr int GridWidth = 13;
	constexpr int GridHeight = 9;
	constexpr int BrickWidth = 34;
	constexpr int BrickHeight = 18;
	constexpr int WindowWidth = 640;
	constexpr int WindowHeight = 360;
	constexpr int GridLeft = (WindowWidth - BrickWidth * GridWidth) / 2;
	constexpr int GridTop = (WindowHeight / 3 - BrickHeight * GridHeight / 2);
	constexpr int GridRight = WindowWidth - GridLeft;
	constexpr int GridBottom = WindowHeight - GridTop;
	constexpr int GameNumLives = 3;
	constexpr int GameScorePerBrick = 100;
	constexpr int PaddleSizeNormal = 5;
	constexpr int PaddleLeftWidth = 8;
	constexpr int PaddleRightWidth = 8;
	constexpr int PaddleMiddleWidth = 8;
	constexpr int PaddleHeight = 16;
	constexpr int PaddleRoundStartX = WindowWidth / 2;
	constexpr int PaddleRoundStartY = WindowHeight - PaddleHeight - 10;
	constexpr float PaddleAcceleration = 6000.0f;
	constexpr float PaddleMaxSpeed = 3000.0f;
	constexpr int ScoreX = 10;
	constexpr int ScoreY = 8;
	constexpr int LifePaddleY = 8;
	constexpr int LifePaddleGap = 6;
	constexpr int BallDiameter = 8;
	constexpr int BallRadius = BallDiameter / 2;
	constexpr float BallSpeedNormal = 300.0f;
	constexpr int GameAreaLeft = GridLeft;
	constexpr int GameAreaRight = GridRight;
	constexpr int GameAreaTop = GridTop;
	constexpr int GameAreaBottom = PaddleRoundStartY + PaddleHeight + 5;
};
