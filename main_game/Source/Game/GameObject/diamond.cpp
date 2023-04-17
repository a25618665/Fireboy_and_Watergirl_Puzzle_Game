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
}

void Diamond::OnMove(const CRect& person_body)
{
	if (is_showing)		// 還未被吃掉才判斷是否overlap
	{
		CRect rect;
		bool is_overlap = rect.IntersectRect(person_body, body);

		if (is_overlap)
			is_showing = false;
	}
}

void Diamond::OnShow()
{
	if (is_showing)
		img.ShowBitmap();
}