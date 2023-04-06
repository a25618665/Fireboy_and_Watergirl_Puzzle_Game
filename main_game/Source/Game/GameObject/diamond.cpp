#include "stdafx.h"
#include "../../Core/Resource.h"
#include <mmsystem.h>
#include <ddraw.h>
#include "../../Library/audio.h"
#include "../../Library/gameutil.h"
#include "../../Library/gamecore.h"
#include "diamond.h"

namespace game_framework {
	Diamond::Diamond()
	{
	}
	void Diamond::init(int coordinateX, int coordinateY)
	{
		x = coordinateX;
		y = coordinateY;
		
		body.left = x;
		body.top = y;
		body.right = x + 25;//暫定25
		body.bottom = y + 21;//暫定21

		is_showing = true;
	}

	void Diamond::OnMove(const CRect & person_body)
	{
		if (person_body.top < body.bottom && person_body.bottom > body.top &&
				person_body.left < body.right && person_body.right > body.left)
		{
			is_showing = false;
		}
	}

	void Diamond::OnShow()
	{
		if (is_showing)
		{
			DiamondPic.SetTopLeft(x, y);
			DiamondPic.ShowBitmap();
		}
	}
	/*bool Diamond::isTouch()
	{
		
	}*/
	int Diamond::GetX()
	{
		return x;
	}
	int Diamond::GetY()
	{
		return y;
	}

}