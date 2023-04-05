#include "stdafx.h"
#include "../Core/Resource.h"
#include <mmsystem.h>
#include <ddraw.h>
#include "../Library/audio.h"
#include "../Library/gameutil.h"
#include "../Library/gamecore.h"
#include "../Game/button.h"
#include "pic_path.h"

namespace game_framework {
	Button::Button()
	{
	}
	void Button::init(int coordinateX, int coordinateY)
	{
		x = coordinateX;
		y = coordinateY;
	
	}
	void Button::OnShow()
	{
		buttonPic.SetTopLeft(x, y);
		buttonPic.ShowBitmap();
	}
	void Button::LoadBitmap(int color)//1:purple2:yellow3:green
	{
		colorindex = color;
		switch (colorindex) {
		case 1:
			buttonPic.LoadBitmap(PURPLE_BUTTON, RGB(0, 0, 0));
			break;
		case 2:
			//buttonPic.LoadBitmap(YELLOW_BUTTON, RGB(0, 0, 0)); 
			break;
		case 3:
			//buttonPic.LoadBitmap(IDB_GREENBOTTON, RGB(0, 0, 0)); 
			break;
		default:
			break;
		}
	}
	
	int Button::GetX()
	{
		return x;
	}
	int Button::GetY()
	{
		return y;
	}
	int Button::GetColor()
	{
		return colorindex;
	}
}