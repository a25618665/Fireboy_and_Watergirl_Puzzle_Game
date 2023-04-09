#pragma once
namespace game_framework {
	class Button {
	public:
		Button();
		void OnShow();
		void OnMove(const CRect & person_body);
		void LoadBitmap(int color);//1:purple2:YELLOW3:GREEN
		void init(int x, int y);
		int GetX();
		int GetY();
		int GetColor();
		CMovingBitmap buttonPic;
		bool Is_Click;
		int colorindex;
		CRect body;
	private:
		int x, y;      //座標x,y
		bool IsDown, IsUp;
		
	};
}

