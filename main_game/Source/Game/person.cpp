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

		img_left.LoadBitmapByString({ BOY_LEFT_1, BOY_LEFT_2, BOY_LEFT_3 }, RGB(0, 0, 0));
		img_left.SetAnimation(20, false);
		img_left.SetTopLeft(0, 0);

		img_right.LoadBitmapByString({ BOY_RIGHT_1, BOY_RIGHT_2, BOY_RIGHT_3, BOY_RIGHT_4 }, RGB(0, 0, 0));
		img_right.SetAnimation(20, false);
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
	x = X;
	y = Y;
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

void Person::OnMove()
{
	if (is_moving_left) {
		x -= 5;
	}
	else if (is_moving_right) {
		x += 5;
	}
}
void Person::OnShow()
{
	if (is_moving_left) {
		img_left.SetTopLeft(x, y);
		img_left.ShowBitmap();
	}
	else if (is_moving_right) {
		img_right.SetTopLeft(x, y);
		img_right.ShowBitmap();
	}
	else {
		img_stop.SetTopLeft(x, y);
		img_stop.ShowBitmap();
	}
}