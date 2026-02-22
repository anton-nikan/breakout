#pragma once

#include <array>
#include "settings.h"

namespace levels {
	enum class BrickType : char { Empty,
		Single,
		Double,
		Solid };
	// clang-format off
	using GridDataType = std::array<BrickType, settings::GridWidth * settings::GridHeight>;
	const inline std::array<GridDataType, 1> GridData =
	{
		{
			{
				BrickType::Single,BrickType::Single,BrickType::Single,BrickType::Single,BrickType::Single,BrickType::Single,BrickType::Single,BrickType::Single,BrickType::Single,BrickType::Single,BrickType::Single,BrickType::Single,BrickType::Single,
				BrickType::Single,BrickType::Single,BrickType::Single,BrickType::Single,BrickType::Single,BrickType::Single,BrickType::Single,BrickType::Single,BrickType::Single,BrickType::Single,BrickType::Single,BrickType::Single,BrickType::Single,
				BrickType::Single,BrickType::Single,BrickType::Single,BrickType::Single,BrickType::Single,BrickType::Single,BrickType::Single,BrickType::Single,BrickType::Single,BrickType::Single,BrickType::Single,BrickType::Single,BrickType::Single,
				BrickType::Single,BrickType::Single,BrickType::Single,BrickType::Single,BrickType::Single,BrickType::Single,BrickType::Single,BrickType::Single,BrickType::Single,BrickType::Single,BrickType::Single,BrickType::Single,BrickType::Single,
				BrickType::Single,BrickType::Single,BrickType::Single,BrickType::Single,BrickType::Single,BrickType::Single,BrickType::Single,BrickType::Single,BrickType::Single,BrickType::Single,BrickType::Single,BrickType::Single,BrickType::Single,
				BrickType::Single,BrickType::Single,BrickType::Single,BrickType::Single,BrickType::Single,BrickType::Single,BrickType::Single,BrickType::Single,BrickType::Single,BrickType::Single,BrickType::Single,BrickType::Single,BrickType::Single,
			}
		}
	};
	// clang-format on
}
