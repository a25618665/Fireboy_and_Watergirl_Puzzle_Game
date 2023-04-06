#pragma once
namespace game_framework {
	class PlatForm {
	public:
		PlatForm();
		void OnShow();
		void init(int x, int y , int  color );//1:purple2:yellow3:green
		int GetX();
		int GetY();
		int GetColor();
		int GetStart_x();
		int GetStart_y();
		CMovingBitmap Pic;
		int x, y, color;    
		 int x_start, y_start;
		bool isDown, isUp;
	};
}

