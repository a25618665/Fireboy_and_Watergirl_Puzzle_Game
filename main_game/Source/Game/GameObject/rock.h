#pragma once
#include <array>

namespace game_framework {
	class Rock {
	public:
		Rock();
		void Init(int x, int y, array<array<int, 480>, 640> *m);
		void Reset();
		void OnMove(const CRect & boy_body, const CRect & girl_body);
		void OnShow();
	private:
		void MoveRockOnMap(char direction, int distance);
		bool RightSideIsClear();
		bool LeftSideIsClear();
		int BottomSideIsClear(int check_distance);	// return岩石底部與下方check_distance距離內障礙物的距離，無障礙物則return check_distance + 1

		int init_x, init_y, x, y;
		int horizontal_moving_distance_per_frame;
		int vertical_velocity;
		int vertical_acceleration;
		array<array<int, 480>, 640> *ptr_map;
		CRect body;
		CMovingBitmap img_rock;
	};
}