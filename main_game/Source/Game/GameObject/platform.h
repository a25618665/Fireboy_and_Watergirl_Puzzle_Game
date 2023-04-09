#pragma once
#include "switch.h"
namespace game_framework {
	class PlatForm {
	public:
		PlatForm();
		void OnShow();
		void OnMove(const  Switch  &switch_add);
		void init(int x, int y ,int Y_END , int  color );//1:purple2:yellow3:green
		int GetX();
		int GetY();
		int GetColor();
		int GetStart_y();
		CMovingBitmap Pic;
		int x, y, color;    
		 int y_start  ;
		 int  y_end ;
		bool isDown, isUp;
	};
}

