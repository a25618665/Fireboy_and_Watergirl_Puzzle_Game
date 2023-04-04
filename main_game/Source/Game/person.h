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
		void setMap(int (*ptr_map)[640][480]);
		void OnMove();
		void OnShow();

		bool is_boy;   //True: boy, False: girl
		int x, y;
		int velocity;
		bool is_jumping;
		bool is_on_the_ground;
		bool is_moving_left;
		bool is_moving_right;
		CMovingBitmap img_stop;
		CMovingBitmap img_left;
		CMovingBitmap img_right;
		int (*ptr_map)[640][480];
		const int r_check_point[5][2] = { {18, 12}, {18, 25}, {18, 37}, {18, 38}, {18, 39} };
		const int l_check_point[5][2] = { {6, 12}, {6, 25}, {6, 37}, {6, 38}, {6, 39} };
	};
}