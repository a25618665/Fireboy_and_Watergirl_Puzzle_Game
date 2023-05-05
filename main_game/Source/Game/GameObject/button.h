#pragma once

namespace game_framework {
	class Button {
	public:
		Button();
		void Init(int x, int y, char color);
		bool IsTriggered();
		void Reset();
		void OnMove(const CRect & boy_body, const CRect & girl_body);
		void OnShow();
	private:
		int x, y;      // 座標x,y
		bool is_triggered;
		CMovingBitmap img_button;
		CRect body;
	};
}
