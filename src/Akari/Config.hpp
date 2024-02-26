
#pragma once

#include <vshlib.hpp>

class Config
{
public:
	struct
	{
		struct
		{
			paf::vec2 position{ 940.0, 110.0 };
			float width = 640.0;
			float optionSize = 21.0;
			float optionSpacing = 2.5;
		} layout;
		struct
		{
			paf::vec4 outline{ 3.0, 95.0, 33.0, 255.0 };
			paf::vec4 text{ 255.0, 255.0, 255.0, 255.0 };
			bool outlineFade{};
			bool textFade{};
		} color;
	} menu;
};

extern Config g_Config;