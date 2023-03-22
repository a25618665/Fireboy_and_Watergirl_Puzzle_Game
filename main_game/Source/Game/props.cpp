#include "stdafx.h"
#include "../Core/Resource.h"
#include <mmsystem.h>
#include <ddraw.h>
#include "../Library/audio.h"
#include "../Library/gameutil.h"
#include "../Library/gamecore.h"
#include "pic_path.h"
#include "props.h"
using namespace game_framework;


water::water(){

}
void water::load() {
	if (water_type == 0) {
		




	}
	else if (water_type == 1) {



	}
	else {
		for (int i = 0; i++; i < (sizeof(water_animation) / sizeof(CMovingBitmap))) {
			water_animation[i].LoadBitmapByString({ GREEN_WATER_1 ,GREEN_WATER_2 }, RGB(0, 0, 0));



		}




	}



}
void water::set_xy(int x, int y) {
	
	water_animation[0].SetTopLeft(x, y);
	water_animation[1].SetTopLeft(x +   (water_animation[0].GetWidth()), y);
	water_animation[2].SetTopLeft(x + 2*(water_animation[0].GetWidth()), y);
	water_animation[3].SetTopLeft(x + 3*(water_animation[0].GetWidth()), y);


	}




void water::set_water_type(int type_flag){
	water_type = type_flag;


}

void water::type_is(){
	return water_type;

}
bool is_touched();
int get_x();
int get_y();


