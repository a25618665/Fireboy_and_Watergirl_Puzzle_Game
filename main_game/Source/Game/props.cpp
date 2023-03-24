#include "stdafx.h"
/*#include "../Core/Resource.h"
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
		for (int i = 0; i < 4; i++) {
			water_animation[i].LoadBitmapByString({ GREEN_WATER_1 ,GREEN_WATER_2 }, RGB(0, 0, 0));
			//if (i % 2 == 0) {
				//water_animation[i].SetFrameIndexOfBitmap(1);}
			water_animation[i].SetAnimation(200,FALSE);
			


		}




	}



}
void water::set_xy(int x, int y) {
	
	water_animation[0].SetTopLeft(x, y);
	water_animation[1].SetTopLeft(x +   (water_animation[0].GetWidth())-15, y);
	water_animation[2].SetTopLeft(x + 2*(water_animation[0].GetWidth())-15, y);
	water_animation[3].SetTopLeft(x + 3*(water_animation[0].GetWidth())-15, y);


	}




void water::set_water_type(int type_flag){
	water_type = type_flag;


}

int water::type_is(){
	return water_type;

}
bool water::is_touched( int math  ){

	if (math>water_animation[0].GetLeft) && (math< water_animation[3].GetLeft) 
		) {
		return true;

	else {return false;}}      */

