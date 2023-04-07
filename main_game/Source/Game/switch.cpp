#include "stdafx.h"
#include "../Core/Resource.h"
#include <mmsystem.h>
#include <ddraw.h>
#include "../Library/audio.h"
#include "../Library/gameutil.h"
#include "../Library/gamecore.h"
#include "switch.h"
#include  "pic_path.h"



namespace game_framework {


	Switch::Switch() {

	}
	void Switch::init(int coordinateX, int coordinateY, int COLOR)
	{
		x = coordinateX;
		y = coordinateY;
		body.left = x;
		body.top = y;
		color = COLOR;
		moving_status = 0;
		is_right = true; //預設為向右
		switch (color) {

		case 0:
			right.LoadBitmap( SWITCH_RIGHT, RGB(0, 0, 0));
			left.LoadBitmap(SWITCH_LEFT, RGB(0, 0, 0));
			break;
		case 2:
			//right.LoadBitmap(, RGB(0, 0, 0));
			//left.LoadBitmap(, RGB(0, 0, 0));
			break;

		}
		body.right = x + right.GetWidth();
		body.bottom = y + right.GetHeight();



	}
	void Switch::OnShow()
	{
		right.SetTopLeft(x, y);
		left.SetTopLeft(x, y);
		if (is_right) {
			
			right.ShowBitmap();

		}
		else  {
			
			left.ShowBitmap();
		}
	}

	void Switch::OnMove(const CRect & person_body) {


		if (person_body.top < body.top && person_body.bottom < body.bottom &&
			person_body.left > body.right-20 && person_body.left < body.right) {
			is_right = false;

		}
		else if (person_body.top < body.top && person_body.bottom > body.bottom &&
			person_body.right > body.left + 5 && person_body.right > body.left){
			

			is_right = true;


		}


	}




	/*int Switch::GetY()
		{
			return x;
		}
	int Switch::GetX()
		{
			return y;
		}
		*/
		




















}