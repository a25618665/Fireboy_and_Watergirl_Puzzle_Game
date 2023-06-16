#pragma once

namespace game_framework {
	class Door {
	public:
		Door();
		void Init(int left_x, int top_y, char color);					//R:BOY B:GIRL
		bool OnMove(const CRect & boy_body, const CRect & girl_body);
		void OnShow();
	private:
		int x, y;
		int frame_counter, frame_size;
		char color;
		CRect body;
		CMovingBitmap Pic;
	};
}
