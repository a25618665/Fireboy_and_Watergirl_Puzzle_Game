#include "stdafx.h"
#include "../../Core/Resource.h"
#include <mmsystem.h>
#include <ddraw.h>
#include "../../Library/audio.h"
#include "../../Library/gameutil.h"
#include "../../Library/gamecore.h"
#include "platform.h"
#include  "../pic_path.h"
#include "switch.h"


namespace game_framework {
	PlatForm::PlatForm()
	{
	}
	void PlatForm::init(int X, int Y,int Y_END, int  COLOR)
	{
		x = X;
		y = Y;
		y_end = Y_END;
		color = COLOR;
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
	
	int PlatForm::GetStart_y()
	{
		return y_start;
	}
	void PlatForm::OnMove(const  Switch  &switch_add) {
		if (switch_add.is_right == true && y<=y_start) {

			y = y + 2;




		}
		else if (switch_add.is_right == false && y >= y_end) {
			y = y - 2;
		}


	}
	void PlatForm::OnMove(const Button &button) {

		if (button.Is_Click == true&&y>=y_end) {

			y = y - 2;
		}

	}
}



