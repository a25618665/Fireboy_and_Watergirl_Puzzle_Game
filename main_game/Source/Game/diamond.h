namespace game_framework {
	class Diamond {
	public:
		Diamond();
		void OnShow();
		void Touch();
		void init(int x, int y);
		int GetX();
		int GetY();
		CMovingBitmap DiamondPic;
	private:
		int x, y;
		bool isTouch;
		//CMovingBitmap diamondPic;
	};
}
