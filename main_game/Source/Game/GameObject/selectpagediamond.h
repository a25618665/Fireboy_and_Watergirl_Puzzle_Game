#pragma once
#include "timer.h"

namespace game_framework {
	class SelectPageDiamond {
	public:
		SelectPageDiamond();
		void Init(int level, int x, int y, char color, int time_x = 41, int time_y = 11);	// x, y 是寶石的位置
		void SetTime(int total_second);
		// 傳入滑鼠按下時的座標，判斷是否按下此關，是的話回傳level，不是的話回傳0
		int OnButtonDown(CPoint point);
		void OnButtonUp();
		void OnShow();
	private:
		int level;							// 代表哪一關
		int time;							// 毫秒
		int diamond_x, diamond_y;			// 選擇關卡頁面的寶石位置
		int time_x, time_y;					// 顯示時間的相對位置
		bool is_init;
		bool is_pass;						// 是否破關
		CMovingBitmap img_diamond_unpass;
		CMovingBitmap img_diamond;			// 選擇關卡頁面的寶石
		CRect body;							// 判斷寶石是否被按下的區域
	};
}