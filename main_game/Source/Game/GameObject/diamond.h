#pragma once

namespace game_framework {
	class Diamond {
	public:
		Diamond();
		Diamond(string color);
		void Init(int x, int y);
		void Reset();
		void OnMove(const CRect& person_body ,int &diamond_counter);	// 傳入person的body判斷是否吃到鑽石
		void OnShow();
	private:
		int x, y;
		bool is_showing;						// 鑽石被吃掉後為false
		CMovingBitmap img;
		CRect body;
		bool is_recorded ;
 	};
}