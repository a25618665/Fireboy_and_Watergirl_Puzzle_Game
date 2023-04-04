#include "stdafx.h"
#include "../Core/Resource.h"
#include <mmsystem.h>
#include <ddraw.h>
#include "../Library/audio.h"
#include "../Library/gameutil.h"
#include "../Library/gamecore.h"
#include "pic_path.h"
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
	jump_iterator = 0;
	is_jumping = false;
	is_moving_left = false;
	is_moving_right = false;
	loadImg();
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

void Person::jump()
{
	if (!is_jumping)
	{
		is_jumping = true;
		jump_iterator = 0;
	}	
}

void Person::setMap(int (*m)[480][640])
{
	ptr_map = m;
}

void Person::OnMove()
{
	if (is_moving_left) 
	{
		int i;
		for (i = 0; i < 6; i++) {
			if ((*ptr_map)[y + 39][x + 6 - i])
				break;
		}
		x -= i;
	}
	else if (is_moving_right) 
	{
		int i;
		for (i = 0; i < 6; i++) {
			if ((*ptr_map)[y + 39][x + 18 + i])
				break;
		}
		x += i;
	}
	
	if (is_jumping)
	{
		/*bool is_falling = true;
		if (jump_iterator < 11)
			is_falling = false;

		int disdance = jump_velocity[jump_iterator];
		y -= disdance;
		jump_iterator++;
		if (jump_iterator > 21)
			is_jumping = false;

		
		檢查頂部是否會碰到
		if (!is_falling)
		{
			int i;
			for (i = 0; i < disdance; i++) {
				if ((*ptr_map)[y + 11 - i][x + 7] || (*ptr_map)[y + 11 - i][x + 17])
					break;
			}

			if (i == disdance)
			{
				y -= i;
				jump_iterator++;
			}
			else
			{
				y -= i;
				jump_iterator = 21 - jump_iterator;
			}
		}
		else
		{
			int i;
			for (i = 0; i < disdance; i++) {
				if ((*ptr_map)[y + 40 - i][x + 7] || (*ptr_map)[y + 40 - i][x + 17])
					break;
			}

			if (i == disdance)
			{
				y -= i;
				jump_iterator++;
				if (jump_iterator > 21)
					is_jumping = false;
			}
			else
			{
				y -= i;
				is_jumping = false;
			}
		}*/
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