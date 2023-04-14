#pragma once

namespace game_framework {
	class Switch {
	public:
		Switch();
		void Init(int x, int y, int color);
		void OnShow();
		void OnMove(const CRect & person_body);
		
		int GetX();
		int GetY();
	private:
		bool is_right;
		int x, y ,color;/// color 0: yellow 
		int moving_status; //1:left 0:right 
		CMovingBitmap right;
		CMovingBitmap left;
		CRect body;
	};
}