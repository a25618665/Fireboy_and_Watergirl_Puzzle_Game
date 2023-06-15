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

void Platform::Init(int x, int y, int end, char defult_direction, char color, array<array<int, 480>, 640> *map, 
					char plat_dir, char len)
{
	this->original_x = x;
	this->original_y = y;
	this->x = x;
	this->y = y;
	this->end = end;
	this->defult_direction = defult_direction;
	this->plat_dir = plat_dir;
	this->len = len;
	ptr_map = map;
	ptr_switch = nullptr;
	is_moving = false;

	if (plat_dir == 'H')
	{
		body.SetRect(x + 2, y + 2, x + 60, y + 13);

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
	else if (plat_dir == 'V')
	{
		if (len == 'S')
			body.SetRect(x + 0, y + 1, x + 13, y + 45);
		else if (len == 'L')
			body.SetRect(x + 2, y + 2, x + 13, y + 60);

		switch (color)
		{
		case 'Y':
			if (len == 'S')
				img.LoadBitmap(YELLOW_PLAT_V, RGB(0, 0, 0));
			else if (len == 'L')
				img.LoadBitmap(YELLOW_PLAT_V_L, RGB(0, 0, 0));
			break;
		case 'P':
			img.LoadBitmap(PURPLE_PLAT_V, RGB(0, 0, 0));
			break;
		case 'B':
			img.LoadBitmap(BLUE_PLAT_V, RGB(0, 0, 0));
			break;
		case 'G':
			img.LoadBitmap(GREEN_PLAT_V, RGB(0, 0, 0));
			break;
		case 'O':
			img.LoadBitmap(ORANGE_PLAT_V, RGB(0, 0, 0));
			break;
		case 'W':
			if (len == 'S')
				img.LoadBitmap(WHITE_PLAT_V, RGB(0, 0, 0));
			else if (len == 'L')
				img.LoadBitmap(WHITE_PLAT_V_L, RGB(0, 0, 0));
			break;
		}
	}
	
	// 把地圖上platform的位置標成障礙物
	for (int i = 0; i < body.Width() + 1; i++)
	{
		for (int j = 0; j < body.Height() + 1; j++)
		{
			if (body.left + i >= 0 && body.top + j >= 0 && (*ptr_map)[body.left + i][body.top + j] == 0)
				(*ptr_map)[body.left + i][body.top + j] = 2;
		}
	}
}

void Platform::Bind(Switch *ptr_switch)
{
	this->ptr_switch = ptr_switch;
}

void Platform::Bind(Button *ptr_button)
{
	this->button_ptr_vector.push_back(ptr_button);
}

void Platform::Bind(const vector<Button *> & button_ptr_vector)
{
	this->button_ptr_vector = button_ptr_vector;
}

void Platform::Reset()
{
	if (x != original_x || y != original_y)
	{
		// 把現在的區域設成0
		for (int i = 0; i < body.Width() + 1; i++)
		{
			for (int j = 0; j < body.Height() + 1; j++)
			{
				if (body.left + i >= 0 && body.top + j >= 0 && (*ptr_map)[body.left + i][body.top + j] == 2)
					(*ptr_map)[body.left + i][body.top + j] = 0;
			}
		}

		x = original_x;
		y = original_y;
		if (plat_dir == 'H')
			body.SetRect(x + 2, y + 2, x + 60, y + 13);
		else if (len == 'S')
			body.SetRect(x + 0, y + 1, x + 13, y + 45);
		else
			body.SetRect(x + 2, y + 2, x + 13, y + 60);
		
		for (int i = 0; i < body.Width() + 1; i++)
		{
			for (int j = 0; j < body.Height() + 1; j++)
			{
				if (body.left + i >= 0 && body.top + j >= 0 && (*ptr_map)[body.left + i][body.top + j] == 0)
					(*ptr_map)[body.left + i][body.top + j] = 2;
			}
		}
	}
}

void Platform::OnMove()
{
	// is_moving
	bool old_is_moving = is_moving;
	switch (defult_direction)
	{
	case 'L':
		is_moving = (x > end && x < original_x) ? true : false;
		break;
	case 'R':
		is_moving = (x > original_x && x < end) ? true : false;
		break;
	case 'U':
		is_moving = (y > end && y < original_y) ? true : false;
		break;
	case 'D':
		is_moving = (y > original_y && y < end) ? true : false;
		break;
	}
	// sound
	if (!old_is_moving && is_moving)
		CAudio::Instance()->Play(A_PLAT, true);
	else if (old_is_moving && !is_moving)
		CAudio::Instance()->Stop(A_PLAT);

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
		int y_start_fill_2, y_start_fill_0;
		switch (direction)
		{
		case 'U':
			y_start_fill_2 = body.top - 2;
			y_start_fill_0 = body.bottom - 1;
			break;
		case 'D':
			y_start_fill_2 = body.bottom + 1;
			y_start_fill_0 = body.top;
			break;
		}

		for (int i = 0; i < body.Width() + 1; i++)
		{
			for (int j = 0; j < 2; j++)
			{
				if (body.left + i >= 0 && y_start_fill_2 + j >= 0 && (*ptr_map)[body.left + i][y_start_fill_2 + j] == 0)
					(*ptr_map)[body.left + i][y_start_fill_2 + j] = 2;
			}
			for (int j = 0; j < 2; j++)
			{
				if (body.left + i >= 0 && y_start_fill_0 + j >= 0 && (*ptr_map)[body.left + i][y_start_fill_0 + j] == 2)
					(*ptr_map)[body.left + i][y_start_fill_0 + j] = 0;
			}
		}
	}
	else										// 左右移動
	{
		int x_start_fill_2, x_start_fill_0;
		switch (direction)
		{
		case 'L':
			x_start_fill_2 = body.left - 2;
			x_start_fill_0 = body.right - 1;
			break;
		case 'R':
			x_start_fill_2 = body.right + 1;
			x_start_fill_0 = body.left;
			break;
		}

		for (int i = 0; i < 2; i++)
		{
			for (int j = 0; j < body.Height() + 1; j++)
			{
				if (x_start_fill_2 + i >= 0 && body.top + j >= 0 && (*ptr_map)[x_start_fill_2 + i][body.top + j] == 0)
					(*ptr_map)[x_start_fill_2 + i][body.top + j] = 2;
			}
			for (int j = 0; j < body.Height() + 1; j++)
			{
				if (x_start_fill_0 + i >= 0 && body.top + j >= 0 && (*ptr_map)[x_start_fill_0 + i][body.top + j] == 2)
					(*ptr_map)[x_start_fill_0 + i][body.top + j] = 0;
			}
		}
	}
}
