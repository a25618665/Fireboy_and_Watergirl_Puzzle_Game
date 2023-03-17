#pragma once
#include<string>
namespace game_framework {
	class Person {
	public:
		Person();
		void init(string type);
		void loadImg();
		void setXY(int X, int Y);
		void setMovingLeft(bool flag);
		void setMovingRight(bool flag);
		int getX();
		int getY();
		void OnMove();
		void OnShow();

		bool is_boy;   //True: boy, False: girl
		int x, y;
		bool is_moving_left;
		bool is_moving_right;
		CMovingBitmap img_stop;
		CMovingBitmap img_left;
		CMovingBitmap img_right;
	};
}