#pragma once
#include "switch.h"
#include "button.h"
#include <array>
#include <vector>

namespace game_framework {
	class Platform {
	public:
		Platform();			
		// Init: end: 移動完的座標(x或y), direction: 'U', 'D', 'L', 'R', color: 'Y', 'P' ,'B', plat_dir: 'H', 'V'
		void Init(int x, int y, int end, char defult_direction, char color, array<array<int, 480>, 640> *map, 
				  char plat_dir = 'H', char len = 'S');
		void Bind(Switch *ptr_switch);
		void Bind(Button *ptr_button);							// 只有被一個buttton控制
		void Bind(const vector<Button *> & button_ptr_vector);	// 被兩個以上buttton控制
		void Reset();
		void OnMove();
		void OnShow();
	private:
		void PlatformOnMove(bool is_triggered);
		void MovePlatformOnMap(char direction);

		int original_x, original_y, x, y, end;
		char defult_direction;
		char plat_dir;
		char len;
		CMovingBitmap img;
		array<array<int, 480>, 640> *ptr_map;
		Switch *ptr_switch;
		vector<Button *> button_ptr_vector;
		CRect body;
	};
}
