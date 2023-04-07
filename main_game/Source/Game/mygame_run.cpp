#include "stdafx.h"
#include "../Core/Resource.h"
#include <mmsystem.h>
#include <ddraw.h>
#include "../Library/audio.h"
#include "../Library/gameutil.h"
#include "../Library/gamecore.h"
#include "mygame.h"
#include "pic_path.h"
#include <fstream>
using namespace game_framework;

/////////////////////////////////////////////////////////////////////////////
// 這個class為遊戲的遊戲執行物件，主要的遊戲程式都在這裡
/////////////////////////////////////////////////////////////////////////////

CGameStateRun::CGameStateRun(CGame *g) : CGameState(g)
{
	level1_red = new Diamond[4];
	level1_blue = new Diamond[4];
	level1_purple_button = new Button[2];
	level1_platform = new PlatForm[2];
	level1_door = new Door[2];
}   

CGameStateRun::~CGameStateRun()
{
	delete level1_red;
	delete level1_blue;
	delete level1_purple_button;
	delete level1_door;
	delete level1_platform;

}

void CGameStateRun::OnBeginState()
{
}

void CGameStateRun::OnInit()  								// 遊戲的初值及圖形設定
{
	load_background();
	load_level1();
	boy.Init("boy");
	boy.SetXY(35, 416);
	girl.Init("girl");
	girl.SetXY(37, 356);

	

	
	LoadMap(1);
	boy.SetMap(&map);
	girl.SetMap(&map);
}

void CGameStateRun::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	const char VK_W = 0x57;
	const char VK_A = 0x41;
	const char VK_D = 0x44;
	
	//girl
	if (nChar == VK_D)
		girl.SetMovingRight(true);
	else if (nChar == VK_A)
		girl.SetMovingLeft(true);

	if (nChar == VK_W)
		girl.Jump();

	//boy
	if (nChar == VK_RIGHT)     //向右鍵
		boy.SetMovingRight(true);
	else if (nChar == VK_LEFT)  //向左鍵
		boy.SetMovingLeft(true);

	if (nChar == VK_UP)
		boy.Jump();
}

void CGameStateRun::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	const char VK_W = 0x57;
	const char VK_A = 0x41;
	const char VK_D = 0x44;
	
	if (nChar == VK_D) {
		girl.SetMovingRight(false);
	}
	else if (nChar == VK_A) {
		girl.SetMovingLeft(false);
	}

	if (nChar == VK_RIGHT) {     //向右鍵
		boy.SetMovingRight(false);
	}
	else if (nChar == VK_LEFT) {  //向左鍵
		boy.SetMovingLeft(false);
	}
}

void CGameStateRun::OnLButtonDown(UINT nFlags, CPoint point)  // 處理滑鼠的動作
{
	//左鍵按下時滑鼠的座標
	int x = point.x;
	int y = point.y;

	if (x >= 302 && x <= 326 && y >= 440 && y <= 468) {
		diamond_flag = 1;
		d_1.SetFrameIndexOfBitmap(1);
		
	}
}

void CGameStateRun::OnLButtonUp(UINT nFlags, CPoint point)	// 處理滑鼠的動作
{
	if (diamond_flag == 1) {
		diamond_flag = 0;
		d_1.SetFrameIndexOfBitmap(0);
		level = 1;
	}
}

void CGameStateRun::OnMouseMove(UINT nFlags, CPoint point)	// 處理滑鼠的動作
{
}

void CGameStateRun::OnRButtonDown(UINT nFlags, CPoint point)  // 處理滑鼠的動作
{
}

void CGameStateRun::OnRButtonUp(UINT nFlags, CPoint point)	// 處理滑鼠的動作
{
}

void CGameStateRun::OnMove()							// 移動遊戲元素
{
	boy.OnMove();
	girl.OnMove();

	CRect boy_body = boy.GetBody();
	CRect girl_body = girl.GetBody();
	for (int i = 0; i < 4; i++) {
		level1_red[i].OnMove(boy_body);
		level1_blue[i].OnMove(girl_body);
	}
	
	//girl.OnMove();
}

void CGameStateRun::OnShow()
{
	if (level == 0) {
		level_map.ShowBitmap();
		d_1.ShowBitmap();
    }
	else if (level == 1) {
		//eachlevel_background.SetFrameIndexOfBitmap(level-1);
		eachlevel_background.ShowBitmap();
		boy.OnShow();
		girl.OnShow();
		level1_onshow();

	}
}

void CGameStateRun::load_background() {
	level = 0;
	level_map.LoadBitmapByString({ LEVEL_BG });
	level_map.SetTopLeft(0, 0);
    
	d_1.LoadBitmapByString({ DIAMOND, DIAMOND_CLICKED },RGB(255, 204, 0));
    d_1.SetTopLeft(302,440);

	eachlevel_background.LoadBitmapByString({ TEMP_BG });
	eachlevel_background.SetTopLeft(0, 0);
}

//void CGameStateRun::load_char() {
//	fire_boy.LoadBitmapByString({ firestop }, RGB(0, 0, 0));
//	//fire_boy.SetAnimation(40, false);
//	fire_boy.SetTopLeft(36,400);
//	fire_right.LoadBitmapByString({ fireright1,
//		fireright2,
//		fireright3,
//		fireright4,}, RGB(0, 0, 0));
//	fire_right.SetTopLeft(25,405);
//	fire_right.SetAnimation(20, false);
//	fire_move = 0;
//	fire_left.LoadBitmapByString({ fireleft1,
//		fireleft2,
//		fireleft3,
//		}, RGB(0, 0, 0));
//	fire_left.SetTopLeft(25, 405);
//	fire_left.SetAnimation(20, false);
//	water_sis.LoadBitmapByString({ waterstop }, RGB(0, 0, 0));
//	water_sis.SetTopLeft(36, 395);
//	water_move = 0;
//	water_right.LoadBitmapByString({ waterright1,
//		waterright2,
//		waterright3}, RGB(0, 0, 0));
//	water_right.SetTopLeft(25, 405);
//	water_left.LoadBitmapByString({ watwerleft1,
//		watwerleft2,
//		watwerleft3}, RGB(0, 0, 0));
//	water_left.SetTopLeft(25, 405);
//
//
//}
void CGameStateRun::load_level1() {

	for (int i = 0; i < 4; i++) {
		level1_red[i].DiamondPic.LoadBitmapByString({RED_1,RED_2}, RGB(0, 0, 0));
		level1_red[i].DiamondPic.SetAnimation(400, FALSE);
		level1_blue[i].DiamondPic.LoadBitmapByString({ BLUE_1,BLUE_2 }, RGB(0, 0, 0));
		level1_blue[i].DiamondPic.SetAnimation(400, FALSE);
	    

	}
	for (int i = 0; i < 2; i++) {
		level1_purple_button[i].buttonPic.LoadBitmapByString({ PURPLE_BUTTON }, RGB(0, 0, 0));
	}
	//platform init 
	level1_platform[0].init(24,244,2);
	level1_platform[1].init(562,198,1);
	//door init 
	level1_door[0].init(511,51,0);
	level1_door[1].init(563,51,1);

	//button init 
	level1_purple_button[0].init(165,229);
	level1_purple_button[1].init(478,165);

	//diamond init 
	level1_red[0].init(339, 425);
	level1_red[1].init(131,215);
	level1_red[2].init(176,40);
	level1_red[3].init(301,75);
	level1_blue[0].init(465, 423);
	level1_blue[1].init(356, 224);
	level1_blue[2].init(40, 75);
	level1_blue[3].init(374, 65);




}
void CGameStateRun::level1_onshow() {



	for (int i = 0; i < 4; i++) {//顯示所有寶石
		
		level1_red[i].OnShow();
		level1_blue[i].OnShow();
		
	}
	for ( int i = 0; i < 2; i++) {//顯示所有按鈕
		level1_purple_button[i].OnShow();

	}
	for (int i = 0; i < 2; i++) {//顯示所有升降平台
		level1_platform[i].OnShow();

	}

}

void CGameStateRun::LoadMap(int level)
{
	ifstream ifs( MAP_TEMPLATE );

	for (int i = 0; i < 640; i++) {
		for (int j = 0; j < 480; j++) {
			ifs >> map[i][j];
		}
	}

	ifs.close();
}