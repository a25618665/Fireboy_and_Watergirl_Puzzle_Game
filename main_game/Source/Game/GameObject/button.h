#pragma once
namespace game_framework {
	class Button {
	public:
		Button();
		void OnShow();
		void LoadBitmap(int color);//1:purple2:YELLOW3:GREEN
		void init(int x, int y);
		int GetX();
		int GetY();
		int GetColor();
		CMovingBitmap buttonPic;
		bool IsTouch();
		int colorindex;
	private:
		int x, y;      //座標x,y
		bool IsDown, IsUp;
		
	};
}

