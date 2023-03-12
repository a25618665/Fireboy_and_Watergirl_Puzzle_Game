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

void CGameStateRun::OnMove()							// 移動遊戲元素
{
	
}

void CGameStateRun::OnInit()  								// 遊戲的初值及圖形設定
{
	load_background();
}

void CGameStateRun::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	
}

void CGameStateRun::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	
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

void CGameStateRun::OnShow()
{
	if (phase_run == 0) {
		level_map.ShowBitmap();
		d_1.ShowBitmap();
    }
}


void CGameStateRun::load_background() {
	phase_run = 0;
	level_map.LoadBitmapByString({ LEVEL_BG });
	level_map.SetTopLeft(0, 0);
    
	d_1.LoadBitmapByString({ DIAMOND, DIAMOND_CLICKED },RGB(255, 204, 0));
    d_1.SetTopLeft(302,440);
}
