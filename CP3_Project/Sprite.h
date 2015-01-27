#pragma once

#include "allegro5\allegro.h"

namespace Display
{
	struct Sprite
	{
		int maxFrame;
		int curFrame;
		int curRow;
		int frameCount;
		int frameDelay;
		int frameWidth;
		int frameHeight;
		int animationColumns;

		ALLEGRO_BITMAP *image;
	};
}