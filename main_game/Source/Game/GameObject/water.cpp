#include "stdafx.h"
#include "../../Core/Resource.h"
#include <mmsystem.h>
#include <ddraw.h>
#include "../../Library/audio.h"
#include "../../Library/gameutil.h"
#include "../../Library/gamecore.h"
#include  "../pic_path.h"
#include "water.h"


using namespace game_framework;

Water::Water()
{
}

void Water::Init(int left_x, int top_y, int right_x, int bottom_y, char color)
{
	x = left_x;
	y = top_y;
	this->color = color;

	body.SetRect(left_x, top_y, right_x, bottom_y);
}

int Water::OnMove(const CRect & boy_body, const CRect & girl_body)
{
	CRect temp_rect;
	bool boy_is_overlap = temp_rect.IntersectRect(boy_body, body);
	bool girl_is_overlap = temp_rect.IntersectRect(girl_body, body);

	switch (color)
	{
	case 'R':
		if (girl_is_overlap)
			return 1;
		break;
	case 'B':
		if (boy_is_overlap)
			return 1;
		break;
	case 'G':
		if (boy_is_overlap || girl_is_overlap)
			return 1;
		break;
	}
	return 0;
}
