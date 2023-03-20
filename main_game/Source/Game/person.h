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
		void jump();
		void OnMove();
		void OnShow();

		bool is_boy;   //True: boy, False: girl
		int x, y;
		int jump_iterator;
		const int jump_velocity[33] = { 8, 8, 7, 7, 6, 6, 5, 5, 4, 4, 3, 3, 2, 2, 1, 1, 0, 
			                           -1, -1, -2, -2, -3, -3, -4, -4, -5, -5, -6, -6, -7, -7, -8, -8};
		bool is_jumping;
		bool is_moving_left;
		bool is_moving_right;
		CMovingBitmap img_stop;
		CMovingBitmap img_left;
		CMovingBitmap img_right;
	};
}