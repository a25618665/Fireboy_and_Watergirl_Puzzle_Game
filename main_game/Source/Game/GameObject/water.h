#pragma once

namespace game_framework {
	class Water {
	public:
		Water();
		void Init(int left_x, int top_y, int right_x, int bottom_y, char color);							// color: 'R', 'B', 'G'
		int OnMove(const CRect & boy_body, const CRect & girl_body);	// 死亡return 1, else return 0 
		void OnShow();
	private:
		int x, y;
		char color;
		CRect body;						// 判斷是否觸發的區域
	};
}