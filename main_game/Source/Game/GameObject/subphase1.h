#pragma once

namespace game_framework {
	class SubPhase1 {
	public:
		SubPhase1();
		void Init();
		void BindVarible(int * level, int * sub_phase);
		void OnLButtonDown();
		void OnLButtonUp();
		void OnMove();
		void OnShow();
	private:
		int * level, * sub_phase;
		bool is_retry_clicked;
		bool is_back_clicked;
		CMovingBitmap img_bg;
		CMovingBitmap img_retry_clicked;
		CMovingBitmap img_back_clicked;
		CRect retry_body;
		CRect back_body;
	};
}
