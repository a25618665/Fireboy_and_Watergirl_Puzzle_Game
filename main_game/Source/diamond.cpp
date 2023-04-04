#include "stdafx.h"
#include "Resource.h"
#include <mmsystem.h>
#include <ddraw.h>
#include "../Library/audio.h"
#include "../Library/gameutil.h"
#include "../Library/gamecore.h"
#include "../game/diamond.h"

namespace game_framework {
	Diamond::Diamond()
	{
	}
	void Diamond::init(int coordinateX, int coordinateY)
	{
		x = coordinateX;
		y = coordinateY;
		isTouch = false;
	}
	void Diamond::OnShow()
	{
		DiamondPic.SetTopLeft(x, y);
		if (isTouch == false) DiamondPic.ShowBitmap();
	}
	void Diamond::Touch()
	{
		isTouch = true;
	}
	int Diamond::GetX()
	{
		return x;
	}
	int Diamond::GetY()
	{
		return y;
	}

}