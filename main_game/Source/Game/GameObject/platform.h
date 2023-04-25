#pragma once
#include "switch.h"
#include "button.h"
#include <array>

namespace game_framework {
	class Platform {
	public:
		Platform();			
		// Init: end: 移動完的座標(x或y), direction: 'U', 'D', 'L', 'R', color: 'Y', 'P'
		void Init(int x, int y, int end, char direction, char color, array<array<int, 480>, 640> *map);
		void Bind(Switch *ptr_switch);									
		void Bind(Button *ptr_button);
		void OnMove();
		void OnShow();
	private:
		void MovePlatform(char direction);

		int original_x, original_y, x, y, end;
		char direction;
		CMovingBitmap img;
		array<array<int, 480>, 640> *ptr_map;
		Switch *ptr_switch;
		Button *ptr_button;
		CRect body;
	};
}
