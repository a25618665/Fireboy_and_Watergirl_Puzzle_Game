#pragma once

namespace game_framework {
	class Diamond {
	public:
		Diamond(string color);
		void Init(int x, int y);
		void OnMove(const CRect& person_body);
		void OnShow();
	private:
		int x, y;
		bool is_showing;
		CMovingBitmap img;
		CRect body;
	};
}