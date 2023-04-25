#pragma once

namespace game_framework {
	class Button {
	public:
		Button();
		void Init(int x, int y, char color);
		void OnMove(const CRect & boy_body, const CRect & girl_body);
		void OnShow();
		/*int GetX();
		int GetY();
		int GetColor();*/
		//int colorindex;bool IsDown, IsUp;
	private:
		int x, y;      // 座標x,y
		bool is_clicked;
		CMovingBitmap img_button;
		CRect body;
	};
}
