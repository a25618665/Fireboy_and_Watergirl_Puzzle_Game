#include "stdafx.h"
#include "../../Core/Resource.h"
#include <mmsystem.h>
#include <ddraw.h>
#include "../../Library/audio.h"
#include "../../Library/gameutil.h"
#include "../../Library/gamecore.h"
#include "../pic_path.h"
#include "rock.h"


using namespace game_framework;

Rock::Rock()
{
}

void Rock::Init(int x, int y, array<array<int, 480>, 640> *m)
{
	init_x = x;
	init_y = y;
	this->x = x;
	this->y = y;
	horizontal_moving_distance_per_frame = 2;
	vertical_velocity = 0;
	vertical_acceleration = 1;
	ptr_map = m;
	body.SetRect(x + 2, y + 3, x + 28, y + 29);
	img_rock.LoadBitmapByString({ ROCK }, RGB(0, 0, 0));

	// 把地圖上rock的位置標成障礙物
	for (int i = 0; i < body.Width() + 1; i++)
	{
		for (int j = 0; j < body.Height() + 1; j++)
		{
			(*ptr_map)[body.left + i][body.top + j] = 1;
		}
	}
}

void Rock::Reset()
{
	if (init_x != x || init_y != y)
	{
		for (int i = 0; i < body.Width() + 1; i++)
		{
			for (int j = 0; j < body.Height() + 1; j++)
			{
				(*ptr_map)[body.left + i][body.top + j] = 0;
			}
		}

		x = init_x;
		y = init_y;
		vertical_velocity = 0;
		body.SetRect(x + 2, y + 3, x + 28, y + 29);

		for (int i = 0; i < body.Width() + 1; i++)
		{
			for (int j = 0; j < body.Height() + 1; j++)
			{
				(*ptr_map)[body.left + i][body.top + j] = 1;
			}
		}
	}
}

void Rock::OnMove(const CRect & boy_body, const CRect & girl_body)
{
	CRect temp_rect;
	CRect body_right(body.right + 1, body.top, body.right + 2, body.bottom);
	CRect body_left(body.left - 2, body.top, body.left - 1, body.bottom);
	bool is_left = temp_rect.IntersectRect(boy_body, body_right) || temp_rect.IntersectRect(girl_body, body_right);
	bool is_right = temp_rect.IntersectRect(boy_body, body_left) || temp_rect.IntersectRect(girl_body, body_left);

	// 檢查左右移動
	if (!is_left && is_right && RightSideIsClear())
	{
		int up = RightBottomSideIsClear();
		if (up)
		{
			vertical_velocity = 0;
			MoveRockOnMap('U', up);
			y -= up;
			body -= CPoint(0, up);
		}
		
		MoveRockOnMap('R', horizontal_moving_distance_per_frame);
		x += horizontal_moving_distance_per_frame;
		body += CPoint(horizontal_moving_distance_per_frame, 0);
	}
	else if (!is_right && is_left && LeftSideIsClear())
	{
		int up = LeftBottomSideIsClear();
		if (up)
		{
			vertical_velocity = 0;
			MoveRockOnMap('U', up);
			y -= up;
			body -= CPoint(0, up);
		}

		MoveRockOnMap('L', horizontal_moving_distance_per_frame);
		x -= horizontal_moving_distance_per_frame;
		body -= CPoint(horizontal_moving_distance_per_frame, 0);
	}

	// 檢查往下移動
	int check_distance = vertical_velocity + vertical_acceleration;
	int obstacle_distance = BottomSideIsClear(check_distance);					// 與下方障礙物距離
	if (obstacle_distance - 1)
	{
		MoveRockOnMap('D', obstacle_distance - 1);
		y += obstacle_distance - 1;
		vertical_velocity += vertical_acceleration;
		body += CPoint(0, obstacle_distance - 1);
	}
	else
		vertical_velocity = 0;
}

void Rock::OnShow()
{
	img_rock.SetTopLeft(x, y);
	img_rock.ShowBitmap();
}

void Rock::MoveRockOnMap(char direction, int distance)
{
	switch (direction)
	{
	case 'L':
		for (int i = 0; i < distance; i++)
		{
			for (int j = 0; j < body.Height() + 1; j++)
			{
				(*ptr_map)[body.right - i][body.top + j] = 0;
				(*ptr_map)[body.left - 1 - i][body.top + j] = 1;
			}
		}
		break;
	case 'R':
		for (int i = 0; i < distance; i++)
		{
			for (int j = 0; j < body.Height() + 1; j++)
			{
				(*ptr_map)[body.left + i][body.top + j] = 0;
				(*ptr_map)[body.right + 1 + i][body.top + j] = 1;
			}
		}
		break;
	case 'D':
		for (int i = 0; i < body.Width() + 1; i++)
		{
			for (int j = 0; j < distance; j++)
			{
				(*ptr_map)[body.left + i][body.top + j] = 0;
				(*ptr_map)[body.left + i][body.bottom + 1 + j] = 1;
			}
		}
		break;
	case 'U':
		for (int i = 0; i < body.Width() + 1; i++)
		{
			for (int j = 0; j < distance; j++)
			{
				(*ptr_map)[body.left + i][body.top - 1 - j] = 1;
				(*ptr_map)[body.left + i][body.bottom - j] = 0;
			}
		}
		break;
	}
}

bool Rock::RightSideIsClear()
{
	bool once = false;
	int up = 0;
	for (int i = 0; i < horizontal_moving_distance_per_frame; i++)
	{
		for (int j = 0; j < body.Height() - 1; j++)
		{
			if ((*ptr_map)[body.right + 1 + i][body.top + j])
				return false;
		}
		for (int k = 0; k < 2; k++)
		{
			if ((*ptr_map)[body.right + 1 + i][body.top + k] && !once)
			{
				once = true;
			}
		}
	}
	return true;
}

int Rock::RightBottomSideIsClear()
{
	for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j < horizontal_moving_distance_per_frame; j++)
		{
			if ((*ptr_map)[body.right + 1 + j][body.bottom - 1 + i])
				return 2 - i;
		}
	}
	return 0;
}

bool Rock::LeftSideIsClear()
{
	for (int i = 0; i < horizontal_moving_distance_per_frame; i++)
	{
		for (int j = 0; j < body.Height() - 1; j++)
		{
			if ((*ptr_map)[body.left - 1 - i][body.top + j])
				return false;
		}
	}
	return true;
}

int Rock::LeftBottomSideIsClear()
{
	for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j < horizontal_moving_distance_per_frame; j++)
		{
			if ((*ptr_map)[body.left - 1 - j][body.bottom - 1 + i])
				return 2 - i;
		}
	}
	return 0;
}

int Rock::BottomSideIsClear(int check_distance)
{
	for (int j = 0; j < check_distance; j++)
	{
		for (int i = 0; i < body.Width() + 1; i++)
		{
			if ((*ptr_map)[body.left + i][body.bottom + 1 + j])
				return j + 1;
		}
	}
	return check_distance + 1;
}
