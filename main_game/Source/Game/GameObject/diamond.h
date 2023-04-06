namespace game_framework {
	class Diamond {
	public:
		Diamond();
		int color;
		void OnMove(const CRect & person_body);
		void OnShow();
		void Touch();
		void init(int x, int y);
		int GetX();
		int GetY();
		CMovingBitmap DiamondPic;
		bool isTouch();
	private:
		int x, y;
		CRect body;
		bool is_showing;
	};
}
