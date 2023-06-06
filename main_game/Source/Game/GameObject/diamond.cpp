#include "stdafx.h"
#include "../../Core/Resource.h"
#include <mmsystem.h>
#include <ddraw.h>
#include "../../Library/audio.h"
#include "../../Library/gameutil.h"
#include "../../Library/gamecore.h"
#include "../pic_path.h"
#include "diamond.h"


using namespace game_framework;

Diamond::Diamond()
{
}

Diamond::Diamond(string color)
{
	if (color == "blue")
	{
		img.LoadBitmapByString({ DIAMOND_BLUE_1, DIAMOND_BLUE_2 }, RGB(0, 0, 0));
		img.SetAnimation(400, FALSE);
	}
	else if (color == "red")
	{
		img.LoadBitmapByString({ DIAMOND_RED_1, DIAMOND_RED_2 }, RGB(0, 0, 0));
		img.SetAnimation(400, FALSE);
	}
	else if (color == "green")
	{
		img.LoadBitmapByString({DIAMOND_GREEN_1, DIAMOND_GREEN_2}, RGB(0, 0, 0));
		img.SetAnimation(400, FALSE);
	}
}

void Diamond::Init(int x, int y)
{
	this->x = x;
	this->y = y;
	img.SetTopLeft(x, y);

	body.left = x + 4;
	body.top = y + 2;
	body.right = x + 21;
	body.bottom = y + 17;
	 
	is_showing = true;
	is_recorded = false;
}

void Diamond::Reset()
{
	is_showing = true;
}

void Diamond::OnMove(const CRect& person_body, int &diamond_counter)
{
	if (is_showing)		// 還未被吃掉才判斷是否overlap
	{
		CRect temp_rect;
		bool is_overlap = temp_rect.IntersectRect(person_body, body);

		if (is_overlap) {
			is_showing = false;
			diamond_counter += 1;
		}
	}
}

void Diamond::OnMove(const CRect &boy_body, const CRect &girl_body, int &diamond_counter)
{
	if (is_showing)		// 還未被吃掉才判斷是否overlap
	{
		CRect temp_rect;
		bool boy_is_overlap = temp_rect.IntersectRect(boy_body, body);
		bool girl_is_overlap = temp_rect.IntersectRect(girl_body, body);

		if (boy_is_overlap || girl_is_overlap)
		{
			is_showing = false;
			diamond_counter += 1;
		}
	}
}

void Diamond::OnShow()
{
	if (is_showing)
		img.ShowBitmap();
}
