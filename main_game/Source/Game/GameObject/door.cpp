#include "stdafx.h"
#include "../../Core/Resource.h"
#include <mmsystem.h>
#include <ddraw.h>
#include "../../Library/audio.h"
#include "../../Library/gameutil.h"
#include "../../Library/gamecore.h"
#include "../pic_path.h"
#include "door.h"


using namespace game_framework;

Door::Door()
{
}
	
void Door::Init(int left_x, int top_y, char color)
{
	this->x = left_x;
	this->y = top_y;
	this->color = color;
	body.SetRect(x + 23, y + 41, x + 25, y + 81);

	switch (color)
	{
	case 'R':
		Pic.LoadBitmap({"Resources/game_run/door/door_red/sprites/DefineSprite_159_FinishBoy/1.bmp",
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
						"Resources/game_run/door/door_red/sprites/DefineSprite_159_FinishBoy/22.bmp"}, RGB(0, 0, 0));
		break;
	case 'B':
		Pic.LoadBitmap({"Resources/game_run/door/door_girl/1.bmp",
						"Resources/game_run/door/door_girl/3.bmp",
						"Resources/game_run/door/door_girl/4.bmp",
						"Resources/game_run/door/door_girl/5.bmp",
						"Resources/game_run/door/door_girl/7.bmp",
						"Resources/game_run/door/door_girl/11.bmp",
						"Resources/game_run/door/door_girl/13.bmp",
						"Resources/game_run/door/door_girl/15.bmp",
						"Resources/game_run/door/door_girl/17.bmp",
						"Resources/game_run/door/door_girl/19.bmp",
						"Resources/game_run/door/door_girl/20.bmp",
						"Resources/game_run/door/door_girl/22.bmp"}, RGB(0, 0, 0));
		break;
	}
	Pic.SetTopLeft(x, y);

	frame_counter = 0;
	frame_size = Pic.GetFrameSizeOfBitmap();
}

bool Door::OnMove(const CRect & boy_body, const CRect & girl_body)
{
	CRect temp_rect;
	bool boy_is_overlap = temp_rect.IntersectRect(boy_body, body);
	bool girl_is_overlap = temp_rect.IntersectRect(girl_body, body);

	switch (color)
	{
	case 'R':
		if (boy_is_overlap)
		{
			if (frame_counter == (frame_size - 1))
				return true;
			frame_counter += 1;
		}
		else
		{
			frame_counter = 0;
		}
		break;
	case 'B':
		if (girl_is_overlap)
		{
			if (frame_counter == (frame_size - 1))
				return true;
			frame_counter += 1;
		}
		else
		{
			frame_counter = 0;
		}
		break;
	}
	return false;
}

void Door::OnShow()
{
	Pic.SetFrameIndexOfBitmap(frame_counter);
	Pic.ShowBitmap();
}

	/*int Door::OnMove(const CRect & boy_body, const CRect & girl_body) {

		CRect temp_rect;
		bool boy_is_overlap = temp_rect.IntersectRect(boy_body, body);
		bool girl_is_overlap = temp_rect.IntersectRect(girl_body, body);

		switch (color)
		{
		case 'R':
			if (boy_is_overlap) {
				Pic.ToggleAnimation();
				return 1;
			}
			break;
		case 'B':
			if (girl_is_overlap) {
				Pic.ToggleAnimation();
				return 1;
			}
			break;
			
		}

		return 0;

	}
	void Door::OnShow()
	{
		Pic.SetTopLeft(x, y);
		Pic.SetAnimation(200, true);
		Pic.ShowBitmap();
	}
	
	
}*/