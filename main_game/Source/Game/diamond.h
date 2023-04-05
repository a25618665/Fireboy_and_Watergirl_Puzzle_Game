namespace game_framework {
	class Diamond {
	public:
		Diamond();
		int color;
		void OnShow();
		void Touch();
		void init(int x, int y);
		int GetX();
		int GetY();
		CMovingBitmap DiamondPic;
		bool isTouch();
	private:
		int x, y;
	};
}
