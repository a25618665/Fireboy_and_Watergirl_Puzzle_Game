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

void Person::init(string type) 
{
	if (type == "boy")
		is_boy = true;
	else if (type == "girl")
		is_boy = false;

	x = 0;
	y = 0;
	velocity = 0;
	is_jumping = false;
	is_on_the_ground = true;
	is_moving_left = false;
	is_moving_right = false;
	loadImg();

	body_offset.left = 7;
	body_offset.top = 11;
	body_offset.right = 17;
	body_offset.bottom = 39;
}

void Person::loadImg()
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

		img_left.LoadBitmapByString({ GIRL_LEFT_1, GIRL_LEFT_2, GIRL_LEFT_3 }, RGB(0, 0, 0));
		img_left.SetAnimation(20, false);
		img_left.SetTopLeft(0, 0);

		img_right.LoadBitmapByString({ GIRL_RIGHT_1, GIRL_RIGHT_2, GIRL_RIGHT_3 }, RGB(0, 0, 0));
		img_right.SetAnimation(20, false);
		img_left.SetTopLeft(0, 0);
	}
}

void Person::setXY(int X, int Y)
{
	x = x = X;
	y = y = Y;
}

void Person::setMovingLeft(bool flag)
{
	is_moving_left = flag;
}

void Person::setMovingRight(bool flag)
{
	is_moving_right = flag;
}

int Person::getX()
{
	return x;
}

int Person::getY()
{
	return y;
}

CRect Person::GetBody()
{
	CRect c;
	c.left = x + body_offset.left;
	c.top = y + body_offset.top;
	c.right = x + body_offset.right;
	c.bottom = y + body_offset.bottom;
	return c;
}

void Person::jump()
{
	if (is_on_the_ground)
	{
		is_jumping = true;
		velocity = 10;
		is_on_the_ground = false;
	}	
}

void Person::setMap(int(*m)[640][480])
{
	ptr_map = m;
}

void Person::OnMove()
{
	if (is_moving_left)
	{
		int i, j;
		int d1 = 6;
		for (j = 0; j < 3; j++) {
			for (i = 0; i < 6; i++) {
				if ((*ptr_map)[x + l_check_point[j][0] - i][y + l_check_point[j][1]])
					break;
			}
			d1 = i < d1 ? i : d1;
		}

		x -= d1;

		for (j = 4; j > 2; j--) {
			for (i = 0; i < 6; i++) {
				if ((*ptr_map)[x + l_check_point[j][0] - i][y + l_check_point[j][1]])
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
				if ((*ptr_map)[x + r_check_point[j][0] + i][y + r_check_point[j][1]])
					break;
			}
			d1 = i < d1 ? i : d1;
		}

		x += d1;

		for(j = 4; j > 2; j--) {
			for (i = 0; i < 6; i++) {
				if ((*ptr_map)[x + r_check_point[j][0] + i][y + r_check_point[j][1]])
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
				if ((*ptr_map)[x + 7][y + 11 - i] || (*ptr_map)[x + 17][y + 11 - i])
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
				if ((*ptr_map)[x + 7][y + 40 - i] || (*ptr_map)[x + 17][y + 40 - i])
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
		if ((*ptr_map)[x + 7][y + 40 + 1] && !(*ptr_map)[x + 12][y + 40 + 1] && !(*ptr_map)[x + 17][y + 40 + 1])
		{
			y -= -1;
			velocity = -2;
			is_jumping = true;

			int i;
			for (i = 1; i < 5; i++) {
				if (!(*ptr_map)[x + 7 + i][y + 40 + 1])
					break;
			}
			x += i;
		}
		else if (!(*ptr_map)[x + 7][y + 40 + 1] && !(*ptr_map)[x + 12][y + 40 + 1] && (*ptr_map)[x + 17][y + 40 + 1])
		{
			y -= -1;
			velocity = -2;
			is_jumping = true;

			int i;
			for (i = 1; i < 5; i++) {
				if (!(*ptr_map)[x + 17 - i][y + 40 + 1])
					break;
			}
			x -= i;
		}
		else if (!(*ptr_map)[x + 7][y + 40 + 1] && !(*ptr_map)[x + 12][y + 40 + 1] && !(*ptr_map)[x + 17][y + 40 + 1])
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
		img_left.SetTopLeft(x, y+11);
		img_left.ShowBitmap();
	}
	else if (is_moving_right) {
		img_right.SetTopLeft(x-13, y+11);
		img_right.ShowBitmap();
	}
	else {
		img_stop.SetTopLeft(x, y);
		img_stop.ShowBitmap();
	}
}
