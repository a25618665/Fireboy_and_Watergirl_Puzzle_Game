#include "stdafx.h"
#include "../../Core/Resource.h"
#include <mmsystem.h>
#include <ddraw.h>
#include "../../Library/audio.h"
#include "../../Library/gameutil.h"
#include "../../Library/gamecore.h"
#include "../pic_path.h"
#include "person.h"
using namespace game_framework;

Person::Person()
{
}

Person::~Person()
{
	delete[] r_check_point;
	delete[] l_check_point;
}

void Person::Init(string type)
{
	if (type == "boy")
	{
		is_boy = true;

		img_left_offset[0] = 0;
		img_left_offset[1] = 11;
		img_right_offset[0] = -13;
		img_right_offset[1] = 11;

		body_offset.left = 7;
		body_offset.top = 12;
		body_offset.right = 17;
		body_offset.bottom = 39;
		//*******************************************
		r_check_point = new int[5][2] { {18, 12}, {18, 25}, {18, 37}, {18, 38}, {18, 39} };
		l_check_point = new int[5][2] { {6, 12}, {6, 25}, {6, 37}, {6, 38}, {6, 39} };
	}	
	else if (type == "girl")
	{
		is_boy = false;

		img_left_offset[0] = -1;
		img_left_offset[1] = -11;
		img_right_offset[0] = -19;
		img_right_offset[1] = -11;

		body_offset.left = 6;
		body_offset.top = 7;
		body_offset.right = 16;
		body_offset.bottom = 34;
		//
		r_check_point = new int[5][2]{ {17, 7}, {17, 20}, {17, 32}, {17, 33}, {17, 34} };
		l_check_point = new int[5][2]{ {5, 7}, {5, 20}, {5, 32}, {5, 33}, {5, 34} };
	}
		
	x = 0;
	y = 0;
	velocity = 0;
	is_jumping = false;
	is_on_the_ground = true;
	is_moving_left = false;
	is_moving_right = false;
	
	LoadImg();
}

void Person::SetXY(int x, int y)
{
	this->x = x;
	this->y = y;
}

void Person::SetMovingLeft(bool flag)
{
	is_moving_left = flag;
}

void Person::SetMovingRight(bool flag)
{
	is_moving_right = flag;
}

void Person::SetMap(int(*m)[640][480])
{
	map = m;
}

int Person::GetX()
{
	return x;
}

int Person::GetY()
{
	return y;
}

CRect Person::GetBody()
{
	CRect body;
	body.left = x + body_offset.left;
	body.top = y + body_offset.top;
	body.right = x + body_offset.right;
	body.bottom = y + body_offset.bottom;
	return body;
}

void Person::Jump()
{
	if (is_on_the_ground)
	{
		is_jumping = true;
		velocity = 9;
		is_on_the_ground = false;
	}	
}

void Person::OnMove()
{
	if (is_moving_left)
	{
		int i, j;
		int d1 = 6;
		for (j = 0; j < 3; j++) {
			for (i = 0; i < 6; i++) {
				if ((*map)[x + l_check_point[j][0] - i][y + l_check_point[j][1]])
					break;
			}
			d1 = i < d1 ? i : d1;
		}

		x -= d1;

		for (j = 4; j > 2; j--) {
			for (i = 0; i < 6; i++) {
				if ((*map)[x + l_check_point[j][0] - i][y + l_check_point[j][1]])
					y -= 1;
				break;
			}
		}
	}
	else if (is_moving_right)
	{
		int i, j;
		int d1 = 6;
		for (j = 0; j < 3; j++) {
			for (i = 0; i < 6; i++) {
				if ((*map)[x + r_check_point[j][0] + i][y + r_check_point[j][1]])
					break;
			}
			d1 = i < d1 ? i : d1;
		}

		x += d1;

		for(j = 4; j > 2; j--) {
			for (i = 0; i < 6; i++) {
				if ((*map)[x + r_check_point[j][0] + i][y + r_check_point[j][1]])
					y -= 1;
					break;
			}
		}
	}

	if (is_jumping)
	{
		if (velocity > 0)
		{
			int i;
			for (i = 0; i < velocity; i++) {
				if ((*map)[x + 7][y + 11 - i] || (*map)[x + 17][y + 11 - i])
					break;
			}

			if (i == velocity)
			{
				y -= i;
				velocity -= 1;
			}
			else
			{
				y -= i;
				velocity = -velocity;
			}
		}
		else
		{
			int i;
			for (i = 0; i > velocity; i--) {
				if ((*map)[x + 7][y + 40 - i] || (*map)[x + 17][y + 40 - i])
					break;
			}

			if (i == velocity)
			{
				y -= i;
				velocity -= 1;
			}
			else
			{
				y -= i;
				is_jumping = false;
				is_on_the_ground = true;
			}
		}
	}
	else
	{
		if ((*map)[x + 7][y + 40 + 1] && !(*map)[x + 12][y + 40 + 1] && !(*map)[x + 17][y + 40 + 1])
		{
			y -= -1;
			velocity = -2;
			is_jumping = true;

			int i;
			for (i = 1; i < 5; i++) {
				if (!(*map)[x + 7 + i][y + 40 + 1])
					break;
			}
			x += i;
		}
		else if (!(*map)[x + 7][y + 40 + 1] && !(*map)[x + 12][y + 40 + 1] && (*map)[x + 17][y + 40 + 1])
		{
			y -= -1;
			velocity = -2;
			is_jumping = true;

			int i;
			for (i = 1; i < 5; i++) {
				if (!(*map)[x + 17 - i][y + 40 + 1])
					break;
			}
			x -= i;
		}
		else if (!(*map)[x + 7][y + 40 + 1] && !(*map)[x + 12][y + 40 + 1] && !(*map)[x + 17][y + 40 + 1])
		{
			y -= -1;
			velocity = -2;
			is_jumping = true;
		}
	}
}

void Person::OnShow()
{
	if (is_moving_left) {
		img_left.SetTopLeft(x + img_left_offset[0], y + img_left_offset[1]);
		img_left.ShowBitmap();
	}
	else if (is_moving_right) {
		img_right.SetTopLeft(x + img_right_offset[0], y + img_right_offset[1]);
		img_right.ShowBitmap();
	}
	else {
		img_stop.SetTopLeft(x, y);
		img_stop.ShowBitmap();
	}
}

void Person::LoadImg()
{
	if (is_boy)
	{
		img_stop.LoadBitmapByString({ BOY_STOP }, RGB(0, 0, 0));
		img_stop.SetTopLeft(0, 0);

		img_left.LoadBitmapByString({ BOY_LEFT_1, BOY_LEFT_2, BOY_LEFT_3, BOY_LEFT_4 }, RGB(0, 0, 0));
		img_left.SetAnimation(80, false);
		img_left.SetTopLeft(0, 0);

		img_right.LoadBitmapByString({ BOY_RIGHT_1, BOY_RIGHT_2, BOY_RIGHT_3, BOY_RIGHT_4 }, RGB(0, 0, 0));
		img_right.SetAnimation(80, false);
		img_left.SetTopLeft(0, 0);
	}
	else
	{
		img_stop.LoadBitmapByString({ GIRL_STOP }, RGB(0, 0, 0));
		img_stop.SetTopLeft(0, 0);

		img_left.LoadBitmapByString({ GIRL_LEFT_1, GIRL_LEFT_2, GIRL_LEFT_3, GIRL_LEFT_4 }, RGB(0, 0, 0));
		img_left.SetAnimation(80, false);
		img_left.SetTopLeft(0, 0);

		img_right.LoadBitmapByString({ GIRL_RIGHT_1, GIRL_RIGHT_2, GIRL_RIGHT_3, GIRL_RIGHT_4 }, RGB(0, 0, 0));
		img_right.SetAnimation(80, false);
		img_left.SetTopLeft(0, 0);
	}
}
