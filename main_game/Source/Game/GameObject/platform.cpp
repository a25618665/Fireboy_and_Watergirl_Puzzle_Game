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

void Platform::Init(int x, int y, int end, char direction, char color, array<array<int, 480>, 640> *map)
{
	this->original_x = x;
	this->original_y = y;
	this->x = x;
	this->y = y;
	this->end = end;
	this->direction = direction;
	ptr_map = map;
	ptr_switch = nullptr;
	ptr_button = nullptr;

	body.left = x + 2;
	body.top = y + 3;
	body.right = x + 57;
	body.bottom = y + 13;
	// 把地圖上platform的位置標成障礙物
	for (int i = 0; i < 56; i++)
	{
		for (int j = 0; j < 11; j++)
		{
			(*ptr_map)[body.left + i][body.top + j] = 1;
		}
	}

	switch (color)
	{
	case 'Y':
		img.LoadBitmap(YELLOW_PLAT, RGB(0, 0, 0));
		break;
	case 'P':
		img.LoadBitmap(PURPLE_PLAT, RGB(0, 0, 0));
		break;
	}
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
	if (ptr_switch)						// 當platform是bind switch時
	{
		switch (direction)
		{
		case 'U':
			if (ptr_switch->IsTriggered() && y > end)
			{
				y -= 2;
				MovePlatform('U');
				body.top -= 2;
				body.bottom -= 2;
			}
			else if (!ptr_switch->IsTriggered() && y < original_y)
			{
				y += 2;
				MovePlatform('D');
				body.top += 2;
				body.bottom += 2;
			}
			break;
		case 'D':
			if (ptr_switch->IsTriggered() && y < end)
			{
				y += 2;
				MovePlatform('D');
				body.top += 2;
				body.bottom += 2;
			}
			else if (!ptr_switch->IsTriggered() && y > original_y)
			{
				y -= 2;
				MovePlatform('U');
				body.top -= 2;
				body.bottom -= 2;
			}
			break;
		case 'L':
			if (ptr_switch->IsTriggered() && x > end)
			{
				x -= 2;
				MovePlatform('L');
				body.left -= 2;
				body.right -= 2;
			}
			else if (!ptr_switch->IsTriggered() && x < original_x)
			{
				x += 2;
				MovePlatform('R');
				body.left += 2;
				body.right += 2;
			}
			break;
		case 'R':
			if (ptr_switch->IsTriggered() && x < end)
			{
				x += 2;
				MovePlatform('R');
				body.left += 2;
				body.right += 2;
			}
			else if (!ptr_switch->IsTriggered() && x > original_x)
			{
				x -= 2;
				MovePlatform('L');
				body.left -= 2;
				body.right -= 2;
			}
			break;
		}
	}
	//else if (ptr_button)				// 當platform是bind button時
	//{
	//	
	//}
}

void Platform::OnShow()
{
	img.SetTopLeft(x, y);
	img.ShowBitmap();
}

void Platform::MovePlatform(char direction)			// 用此函式要注意要先用此函式再移動body
{
	if (direction == 'U' || direction == 'D')		// 上下移動
	{
		int y_start_fill_1, y_start_fill_0;
		switch (direction)
		{
		case 'U':
			y_start_fill_1 = body.top - 2;
			y_start_fill_0 = body.bottom - 1;
			break;
		case 'D':
			y_start_fill_1 = body.bottom + 1;
			y_start_fill_0 = body.top;
			break;
		}

		for (int i = 0; i < 56; i++)
		{
			for (int j = 0; j < 2; j++)
			{
				(*ptr_map)[body.left + i][y_start_fill_1 + j] = 1;
			}
			for (int j = 0; j < 2; j++)
			{
				(*ptr_map)[body.left + i][y_start_fill_0 + j] = 0;
			}
		}
	}
	else										// 左右移動
	{
		int x_start_fill_1, x_start_fill_0;
		switch (direction)
		{
		case 'L':
			x_start_fill_1 = body.left - 2;
			x_start_fill_0 = body.right - 1;
			break;
		case 'R':
			x_start_fill_1 = body.right + 1;
			x_start_fill_0 = body.left;
			break;
		}

		for (int i = 0; i < 2; i++)
		{
			for (int j = 0; j < 11; j++)
			{
				(*ptr_map)[x_start_fill_1 + i][body.top + j] = 1;
			}
			for (int j = 0; j < 11; j++)
			{
				(*ptr_map)[x_start_fill_0 + i][body.top + j] = 0;
			}
		}
	}
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
