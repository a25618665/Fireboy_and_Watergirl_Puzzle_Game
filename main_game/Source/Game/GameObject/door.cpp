#include "stdafx.h"
#include "../../Core/Resource.h"
#include <mmsystem.h>
#include <ddraw.h>
#include "../../Library/audio.h"
#include "../../Library/gameutil.h"
#include "../../Library/gamecore.h"
#include "door.h"
#include "../pic_path.h"











namespace game_framework {
	Door::Door()
	{
	}
	void Door::init(int coordinateX, int coordinateY, int COLOR)
	{
		x = coordinateX;
		y = coordinateY;
		color = COLOR;
		switch (COLOR) {

		case 0:
			Pic.LoadBitmap({ "Resources/game_run/door/door_red/sprites/DefineSprite_159_FinishBoy/1.bmp",
				"Resources/game_run/door/door_red/sprites/DefineSprite_159_FinishBoy/3.bmp",
				"Resources/game_run/door/door_red/sprites/DefineSprite_159_FinishBoy/4.bmp",
				"Resources/game_run/door/door_red/sprites/DefineSprite_159_FinishBoy/5.bmp",
				"Resources/game_run/door/door_red/sprites/DefineSprite_159_FinishBoy/7.bmp",
				"Resources/game_run/door/door_red/sprites/DefineSprite_159_FinishBoy/11.bmp",
				"Resources/game_run/door/door_red/sprites/DefineSprite_159_FinishBoy/13.bmp",
				"Resources/game_run/door/door_red/sprites/DefineSprite_159_FinishBoy/15.bmp",
				"Resources/game_run/door/door_red/sprites/DefineSprite_159_FinishBoy/17.bmp",
				"Resources/game_run/door/door_red/sprites/DefineSprite_159_FinishBoy/19.bmp",
				"Resources/game_run/door/door_red/sprites/DefineSprite_159_FinishBoy/20.bmp",
				"Resources/game_run/door/door_red/sprites/DefineSprite_159_FinishBoy/22.bmp"

				}, RGB(0, 0, 0));
			Pic.SetAnimation(200, true);
			break;
		case 1:
			Pic.LoadBitmap({ "Resources/game_run/door/door_girl/1.bmp",
				"Resources/game_run/door/door_girl/3.bmp" ,
				"Resources/game_run/door/door_girl/4.bmp" ,
				"Resources/game_run/door/door_girl/5.bmp" ,
				"Resources/game_run/door/door_girl/7.bmp" ,
				"Resources/game_run/door/door_girl/11.bmp" ,
				"Resources/game_run/door/door_girl/13.bmp" ,
				"Resources/game_run/door/door_girl/15.bmp" ,
				"Resources/game_run/door/door_girl/17.bmp" ,
				"Resources/game_run/door/door_girl/19.bmp" ,
				"Resources/game_run/door/door_girl/20.bmp" ,
				"Resources/game_run/door/door_girl/22.bmp",
				}, RGB(0, 0, 0));
			Pic.SetAnimation(200, true);
			break;
		}
	}

	void Door::OnShow()
	{
		Pic.SetTopLeft(x, y);
		Pic.ShowBitmap();
	}
	int Door::GetX()
	{
		return x;
	}
	int Door::GetY()
	{
		return y;
	}
}