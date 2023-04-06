#pragma once

namespace game_framework {
	class Door {
	public:
		Door();
		void OnShow();
		void init(int x, int y,int COLOR);//0: BOY1:GIRL
		bool is_touched();
		int GetX();
		int GetY();
		int x, y;
		int color;
		CMovingBitmap Pic;
	};
}
