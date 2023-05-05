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

void Platform::Init(int x, int y, int end, char defult_direction, char color, array<array<int, 480>, 640> *map)
{
	this->original_x = x;
	this->original_y = y;
	this->x = x;
	this->y = y;
	this->end = end;
	this->defult_direction = defult_direction;
	ptr_map = map;
	ptr_switch = nullptr;

	body.SetRect(x + 2, y + 2, x + 60, y + 13);
	// 把地圖上platform的位置標成障礙物
	for (int i = 0; i < body.Width(); i++)
	{
		for (int j = 0; j < body.Height(); j++)
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
	case 'B':
		img.LoadBitmap(BLUE_PLAT, RGB(0, 0, 0));
		break;
	case 'G':
		img.LoadBitmap(GREEN_PLAT, RGB(0, 0, 0));
		break;
	case 'O':
		img.LoadBitmap(ORANGE_PLAT, RGB(0, 0, 0));
		break;
	case 'W':
		img.LoadBitmap(WHITE_PLAT, RGB(0, 0, 0));
		break;
	}
}

void Platform::Bind(Switch *ptr_switch)
{
	this->ptr_switch = ptr_switch;
}

void Platform::Bind(const vector<Button *> & button_ptr_vector)
{
	this->button_ptr_vector = button_ptr_vector;
}

void Platform::Reset()
{
	if (x != original_x || y != original_y)
	{
		for (int i = 0; i < body.Width(); i++)
		{
			for (int j = 0; j < body.Height(); j++)
			{
				(*ptr_map)[body.left + i][body.top + j] = 0;
			}
		}

		x = original_x;
		y = original_y;
		body.SetRect(x + 2, y + 2, x + 60, y + 13);
		for (int i = 0; i < body.Width(); i++)
		{
			for (int j = 0; j < body.Height(); j++)
			{
				(*ptr_map)[body.left + i][body.top + j] = 1;
			}
		}
	}
}

void Platform::OnMove()
{
	if (ptr_switch)							// 當platform是bind switch時
	{
		PlatformOnMove( ptr_switch->IsTriggered() );
	}
	else if (button_ptr_vector.size())		// 當platform是bind button時
	{
		bool is_triggered = false;
		for (auto & button_ptr : button_ptr_vector)
		{
			is_triggered = is_triggered || button_ptr->IsTriggered();
		}
		PlatformOnMove(is_triggered);
	}
}

void Platform::OnShow()
{
	img.SetTopLeft(x, y);
	img.ShowBitmap();
}

void Platform::PlatformOnMove(bool is_triggered)
{
	switch (defult_direction)
	{
	case 'U':
		if (is_triggered && y > end)
		{
			y -= 2;
			MovePlatformOnMap('U');
			body -= CPoint(0, 2);
		}
		else if (!is_triggered && y < original_y)
		{
			y += 2;
			MovePlatformOnMap('D');
			body += CPoint(0, 2);
		}
		break;
	case 'D':
		if (is_triggered && y < end)
		{
			y += 2;
			MovePlatformOnMap('D');
			body += CPoint(0, 2);
		}
		else if (!is_triggered && y > original_y)
		{
			y -= 2;
			MovePlatformOnMap('U');
			body -= CPoint(0, 2);
		}
		break;
	case 'L':
		if (is_triggered && x > end)
		{
			x -= 2;
			MovePlatformOnMap('L');
			body -= CPoint(2, 0);
		}
		else if (!is_triggered && x < original_x)
		{
			x += 2;
			MovePlatformOnMap('R');
			body += CPoint(2, 0);
		}
		break;
	case 'R':
		if (is_triggered && x < end)
		{
			x += 2;
			MovePlatformOnMap('R');
			body += CPoint(2, 0);
		}
		else if (!is_triggered && x > original_x)
		{
			x -= 2;
			MovePlatformOnMap('L');
			body -= CPoint(2, 0);
		}
		break;
	}
}

void Platform::MovePlatformOnMap(char direction)	// 用此函式要注意要先用此函式再移動body 
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

		for (int i = 0; i < body.Width(); i++)
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
			for (int j = 0; j < body.Height(); j++)
			{
				(*ptr_map)[x_start_fill_1 + i][body.top + j] = 1;
			}
			for (int j = 0; j < body.Height(); j++)
			{
				(*ptr_map)[x_start_fill_0 + i][body.top + j] = 0;
			}
		}
	}
}
