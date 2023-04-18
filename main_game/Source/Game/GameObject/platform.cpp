#include "stdafx.h"
#include "../../Core/Resource.h"
#include <mmsystem.h>
#include <ddraw.h>
#include "../../Library/audio.h"
#include "../../Library/gameutil.h"
#include "../../Library/gamecore.h"
#include  "../pic_path.h"
#include "platform.h"


using namespace game_framework;

Platform::Platform()
{
}

void Platform::Init(int x, int y, int end, char direction, char color)
{
	this->x = x;
	this->y = y;
	this->end = end;
	this->direction = direction;
	ptr_switch = nullptr;
	ptr_button = nullptr;

	body.left = x + 2;
	body.top = y + 3;
	body.right = x + 57;
	body.bottom = y + 13;

	switch (color)
	{
	case 'Y':
		img.LoadBitmap(YELLOW_PLAT, RGB(0, 0, 0));
		break;
	case 'P':
		img.LoadBitmap(PURPLE_PLAT, RGB(0, 0, 0));
		break;
	}
	img.SetTopLeft(x, y);
}

void Platform::Bind(Switch *ptr_switch)
{
	this->ptr_switch = ptr_switch;
}

void Platform::Bind(Button *ptr_button)
{
	this->ptr_button = ptr_button;
}

void Platform::OnMove()
{
	if (ptr_switch)			// 當platform是bind switch時
	{

		y = y + 2;




	}

	if (switch_add.is_right == false && y >= y_end) // 當platform是bind button時
	{
		y = y - 2;
	}
}

void Platform::OnShow()
{
	img.ShowBitmap();
}

/*int PlatForm::GetX()
{
	return x;
}
int PlatForm::GetY()
{
	return y;
}
int PlatForm::GetColor()
{
	return color;
}

int PlatForm::GetStart_y()
{
	return y_start;
}*/
