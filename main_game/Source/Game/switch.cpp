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
		body.right = x + right.GetWidth;
		body.bottom = y + right.GetHeight;
		color = COLOR;
		moving_status = 0;
		is_right = true; //預設為向右
		switch (color) {

		case 1:
			right.LoadBitmap(, RGB(0, 0, 0));
			left.LoadBitmap(, RGB(0, 0, 0));
			break;
		case 2:
			//right.LoadBitmap(, RGB(0, 0, 0));
			//left.LoadBitmap(, RGB(0, 0, 0));
			break;

		}



	}
	void Switch::OnShow()
	{
		if (is_right) {
			right.SetTopLeft(x, y);
			right.ShowBitmap();

		}
		else  {
			left.SetTopLeft(x, y);
			left.ShowBitmap();
		}
	}

	void Switch::OnMove(const CRect & person_body) {


		if (person_body.top < body.bottom && person_body.bottom > body.top &&
			person_body.left < body.right && person_body.right > body.left) {

		}


	}




	int Switch::GetY()
		{
			return x;
		}
	int Switch::GetX()
		{
			return y;
		}

		




















}*/