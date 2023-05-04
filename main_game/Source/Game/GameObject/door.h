#pragma once

namespace game_framework {
	class Door {
	public:
		Door();
		void OnShow();
		void init(int left_x, int top_y,int right_x,int bottom_y, char COLOR);//R:BOY B:GIRL
		int  OnMove(const CRect & boy_body, const CRect & girl_body);
		bool is_trigger = 0 ;
	private :
		int  length = 103 , width  = 59 ;
		char color;
		CRect body;
		CMovingBitmap Pic;
		int x, y;
	};
}
