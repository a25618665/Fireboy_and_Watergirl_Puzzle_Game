#include "stdafx.h"
#include "../../Core/Resource.h"
#include <mmsystem.h>
#include <ddraw.h>
#include "../../Library/audio.h"
#include "../../Library/gameutil.h"
#include "../../Library/gamecore.h"
#include  "../pic_path.h"
#include "switch.h"


using namespace game_framework;

Switch::Switch()
{
}

void Switch::Init(int x, int y, char defult_direction, array<array<int, 480>, 640> *map)
{
	this->x = x;
	this->y = y;
	is_triggered = false;
	ptr_map = map;

	body_right.left = x + 35;
	body_right.top = y + 5;
	body_right.right = x + 36;
	body_right.bottom = y + 8;

	body_left.left = x + 3;
	body_left.top = y + 5;
	body_left.right = x + 4;
	body_left.bottom = y + 8; 
	
	switch (defult_direction)
	{
	case 'R':
		is_right = true;
		// 地圖上一開始搖桿沒有標成障礙物，所以在此要把搖桿向右時在地圖上的區域標成1
		for (int i = 0; i < 16; i++)
		{
			for (int j = 0; j < 18; j++)
			{
				if (right_obstacle_aria[i][j])
					(*ptr_map)[x + 19 + i][y + 2 + j] = 1;
			}
		}
		break;
	case 'L':
		is_right = false;
		// 地圖上一開始搖桿沒有標成障礙物，所以在此要把搖桿向左時在地圖上的區域標成1
		for (int i = 0; i < 16; i++)
		{
			for (int j = 0; j < 18; j++)
			{
				if (left_obstacle_aria[i][j])
					(*ptr_map)[x + 5 + i][y + 4 + j] = 1;
			}
		}
		break;
	}
	
	/*switch (color) {

	case 0:*/
		img_right.LoadBitmap(SWITCH_RIGHT, RGB(0, 0, 0));
		img_right.SetTopLeft(x, y);
		img_left.LoadBitmap(SWITCH_LEFT, RGB(0, 0, 0));
		img_left.SetTopLeft(x, y);
	//	break;
	//case 2:
	//	//right.LoadBitmap(, RGB(0, 0, 0));
	//	//left.LoadBitmap(, RGB(0, 0, 0));
	//	break;

	//}
}

bool Switch::IsTriggered()
{
	return is_triggered;
}

void Switch::OnMove(const CRect & boy_body, const CRect & girl_body)
{
	if (is_right)
	{
		CRect temp_rect;
		bool boy_is_overlap = temp_rect.IntersectRect(boy_body, body_right);
		bool girl_is_overlap = temp_rect.IntersectRect(girl_body, body_right);

		if (boy_is_overlap || girl_is_overlap)
			Right2Left();
	}
	else
	{
		CRect temp_rect;
		bool boy_is_overlap = temp_rect.IntersectRect(boy_body, body_left);
		bool girl_is_overlap = temp_rect.IntersectRect(girl_body, body_left);

		if (boy_is_overlap || girl_is_overlap)
			Left2Right();
	}
}

void Switch::OnShow()
{
	if (is_right)
		img_right.ShowBitmap();
	else
		img_left.ShowBitmap();
}

void Switch::Right2Left()
{
	for (int i = 0; i < 16; i++)
	{
		for (int j = 0; j < 18; j++)
		{
			if (right_obstacle_aria[i][j])
				(*ptr_map)[x + 19 + i][y + 2 + j] = 0;
		}
	}

	for (int i = 0; i < 16; i++)
	{
		for (int j = 0; j < 18; j++)
		{
			if (left_obstacle_aria[i][j])
				(*ptr_map)[x + 5 + i][y + 4 + j] = 1;
		}
	}
	is_right = false;
	is_triggered = !is_triggered;
}

void Switch::Left2Right()
{
	for (int i = 0; i < 16; i++)
	{
		for (int j = 0; j < 18; j++)
		{
			if (left_obstacle_aria[i][j])
				(*ptr_map)[x + 5 + i][y + 4 + j] = 0;
		}
	}

	for (int i = 0; i < 16; i++)
	{
		for (int j = 0; j < 18; j++)
		{
			if (right_obstacle_aria[i][j])
				(*ptr_map)[x + 19 + i][y + 2 + j] = 1;
		}
	}
	is_right = true;
	is_triggered = !is_triggered;
}
