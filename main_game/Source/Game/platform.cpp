#include "stdafx.h"
#include "../Core/Resource.h"
#include <mmsystem.h>
#include <ddraw.h>
#include "../Library/audio.h"
#include "../Library/gameutil.h"
#include "../Library/gamecore.h"
#include "platform.h"
#include  "pic_path.h"



namespace game_framework {
	PlatForm::PlatForm()
	{
	}
	void PlatForm::init(int X, int Y, int  COLOR)
	{
		x = X;
		y = Y;
		color = COLOR;
			x_start = X;
		y_start = Y;
		switch (color) {

		case 1:
			Pic.LoadBitmap(PURPLE_PLAT, RGB(0, 0, 0));
			break;
		case 2:
			Pic.LoadBitmap(YELLOW_PLAT, RGB(0, 0, 0));
			break;

		}
	}
	void PlatForm::OnShow()
	{
		Pic.SetTopLeft(x, y);
		Pic.ShowBitmap();
	}
	int PlatForm::GetX()
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
	int PlatForm::GetStart_x()
	{
		return x_start;
	}
	int PlatForm::GetStart_y()
	{
		return y_start;
	}
}


