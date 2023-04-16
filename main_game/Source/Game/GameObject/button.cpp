#include "stdafx.h"
#include "../../Core/Resource.h"
#include <mmsystem.h>
#include <ddraw.h>
#include "../../Library/audio.h"
#include "../../Library/gameutil.h"
#include "../../Library/gamecore.h"
#include "button.h"
#include "../pic_path.h"

namespace game_framework {
	Button::Button()
	{
	}
	void Button::init(int coordinateX, int coordinateY)
	{
		x = coordinateX;
		y = coordinateY;
		body.left = x;
		body.top = y;
		Is_Click = false;
	
	}
	void Button::OnShow()
	{
		if (Is_Click == false) {
			buttonPic.SetTopLeft(x, y);
			buttonPic.ShowBitmap();
		}
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
		body.right = x + buttonPic.GetWidth();
		body.bottom = y + buttonPic.GetHeight();
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
	void Button::OnMove(const  CRect & person_body) {
		if (person_body.bottom > body.top && person_body.bottom < body.bottom &&
			person_body.left > body.left && person_body.right < body.right) {
			Is_Click = true;

		}
		else  {


			Is_Click = false;


		}


	}
}