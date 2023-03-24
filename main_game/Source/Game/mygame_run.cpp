#include "stdafx.h"
#include "../Core/Resource.h"
#include <mmsystem.h>
#include <ddraw.h>
#include "../Library/audio.h"
#include "../Library/gameutil.h"
#include "../Library/gamecore.h"
#include "mygame.h"
#include "pic_path.h"
using namespace game_framework;

/////////////////////////////////////////////////////////////////////////////
// 這個class為遊戲的遊戲執行物件，主要的遊戲程式都在這裡
/////////////////////////////////////////////////////////////////////////////

CGameStateRun::CGameStateRun(CGame *g) : CGameState(g)
{
	
}

CGameStateRun::~CGameStateRun()
{
	
}

void CGameStateRun::OnBeginState()
{
}

void CGameStateRun::OnInit()  								// 遊戲的初值及圖形設定
{
	
		load_background();
		boy.init("boy");
		boy.setXY(60, 400);

		girl.init("girl");
		girl.setXY(36, 400);
		
	
}

void CGameStateRun::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	const char VK_W = 0x57;
	const char VK_A = 0x41;
	const char VK_D = 0x44;
	
	//girl
	if (nChar == VK_D)
		girl.setMovingRight(true);
	else if (nChar == VK_A)
		girl.setMovingLeft(true);

	if (nChar == VK_W)
		girl.jump();

	//boy
	if (nChar == VK_RIGHT)     //向右鍵
		boy.setMovingRight(true);
	else if (nChar == VK_LEFT)  //向左鍵
		boy.setMovingLeft(true);

	if (nChar == VK_UP)
		boy.jump();
}

void CGameStateRun::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	const char VK_W = 0x57;
	const char VK_A = 0x41;
	const char VK_D = 0x44;
	
	if (nChar == VK_D) {
		girl.setMovingRight(false);
	}
	else if (nChar == VK_A) {
		girl.setMovingLeft(false);
	}

	if (nChar == VK_RIGHT) {     //向右鍵
		boy.setMovingRight(false);
	}
	else if (nChar == VK_LEFT) {  //向左鍵
		boy.setMovingLeft(false);
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
	}
}

void CGameStateRun::load_background() {
	level = 0;
	level_map.LoadBitmapByString({ LEVEL_BG });
	level_map.SetTopLeft(0, 0);
    
	d_1.LoadBitmapByString({ DIAMOND, DIAMOND_CLICKED },RGB(255, 204, 0));
    d_1.SetTopLeft(302,440);

	eachlevel_background.LoadBitmapByString({ LEVEL1_BAC });//第一張為第一關第二張為第二關
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