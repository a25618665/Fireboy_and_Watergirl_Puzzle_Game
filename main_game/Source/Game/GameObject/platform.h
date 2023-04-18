#pragma once
#include "switch.h"
#include "button.h"

namespace game_framework {
	class Platform {
	public:
		Platform();														// ********
		void Init(int x, int y, int end, char direction, char color);	// 從這開始direction: 'U', 'D', 'L', 'R', color: 'Y', 'P'
		void Bind(Switch *ptr_switch);									// ********
		void Bind(Button *ptr_button);
		void OnMove();
		void OnShow();
		/*int GetX();
		int GetY();
		int GetColor();
		int GetStart_y();*/
	private:
		int x, y, end;
		char direction; //*************************************************colorconst
		bool isDown, isUp;//***********************************************
		CMovingBitmap img;
		Switch *ptr_switch;
		Button *ptr_button;
		CRect body;
	};
}
