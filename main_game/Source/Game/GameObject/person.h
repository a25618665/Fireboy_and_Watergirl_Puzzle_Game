#pragma once
#include <array>

namespace game_framework {
	class Person {
	public:
		Person();
		~Person();
		void Init(int x, int y, string gender);
		void SetMovingLeft(bool flag);
		void SetMovingRight(bool flag);
		void SetMap(array<array<int, 480>, 640> *m);
		void Reset();
		int GetX();
		int GetY();
		CRect GetBody();
		void Jump();
		void OnMove();
		void OnShow();
	private:
		void LoadImg();

		bool is_boy;   //True: boy, False: girl
		bool is_jumping;
		bool is_on_the_ground;
		bool is_moving_left;
		bool is_moving_right;
		int init_x, init_y; 
		int x, y;
		int velocity;
		array<array<int, 480>, 640> *map;
		int img_left_offset[2];
		int img_right_offset[2];
		CRect body_offset;
		CMovingBitmap img_stop;
		CMovingBitmap img_left;
		CMovingBitmap img_right;

		//
		int (*r_check_point)[2] = nullptr;
		int (*l_check_point)[2] = nullptr;
	};
}