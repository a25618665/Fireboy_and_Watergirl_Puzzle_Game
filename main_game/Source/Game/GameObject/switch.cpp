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

void Switch::Init(int x, int y, string defult_direction, array<array<int, 480>, 640> *map)
{
	this->x = x;
	this->y = y;

	body_right.left = x + 35;
	body_right.top = y + 5;
	body_right.right = x + 36;
	body_right.bottom = y + 8;

	body_left.left = x + 3;
	body_left.top = y + 5;
	body_left.right = x + 4;
	body_left.bottom = y + 8; 
	
	if (defult_direction == "right")
	{
		is_right = true;
		for (int i = 0; i < 16; i++)
		{
			for (int j = 0; j < 18; j++)
			{
				if (right_obstacle_aria[i][j])
					(*ptr_map)[x + 19 + i][y + 2 + j] = 1;
			}
		}
	}
	else
	{
		is_right = false;
		for (int i = 0; i < 16; i++)
		{
			for (int j = 0; j < 18; j++)
			{
				if (left_obstacle_aria[i][j])
					(*ptr_map)[x + 5 + i][y + 4 + j] = 1;
			}
		}
	}

	ptr_map = map;

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
}
