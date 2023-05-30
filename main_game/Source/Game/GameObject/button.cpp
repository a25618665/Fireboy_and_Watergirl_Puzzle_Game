#include "stdafx.h"
#include "../../Core/Resource.h"
#include <mmsystem.h>
#include <ddraw.h>
#include "../../Library/audio.h"
#include "../../Library/gameutil.h"
#include "../../Library/gamecore.h"
#include "../pic_path.h"
#include "button.h"


using namespace game_framework;

Button::Button()
{
}

void Button::Init(int x, int y, char color)
{
	this->x = x;
	this->y = y;
	is_triggered = false;

	switch (color)
	{
	case 'P':
		img_button.LoadBitmap(PURPLE_BUTTON, RGB(0, 0, 0));
		break;
	case 'Y':
		img_button.LoadBitmap(YELLOW_BUTTON, RGB(0, 0, 0)); 
		break;
	case 'G':
		img_button.LoadBitmap(GREEN_BUTTON, RGB(0, 0, 0)); 
		break;
	case 'W':
		img_button.LoadBitmap(WHITE_BUTTON, RGB(0, 0, 0));
		break;
	case 'B':
		img_button.LoadBitmap(BLUE_BUTTON, RGB(0, 0, 0));
		break;
	case 'O':
		img_button.LoadBitmap(ORANGE_BUTTON, RGB(0, 0, 0));
		break;
	}
	img_button.SetTopLeft(x, y);

	body.left = x;
	body.top = y;
	body.right = x + img_button.GetWidth();
	body.bottom = y + img_button.GetHeight();
}

bool Button::IsTriggered()
{
	return is_triggered;
}

void Button::Reset()
{
	is_triggered = false;
}

void Button::OnMove(const CRect & boy_body, const CRect & girl_body)
{
	CRect temp_rect;
	bool boy_is_overlap = temp_rect.IntersectRect(boy_body, body);
	bool girl_is_overlap = temp_rect.IntersectRect(girl_body, body);

	if (boy_is_overlap || girl_is_overlap)
		is_triggered = true;
	else
		is_triggered = false;
}

void Button::OnShow()
{
	if (!is_triggered)
		img_button.ShowBitmap();
}
