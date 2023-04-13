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
	
	level1_purple_button = new Button[2];
	level1_platform = new PlatForm[2];
	level1_door = new Door[2];
	level1_switch = new Switch[1];
}   

CGameStateRun::~CGameStateRun()
{
	
	delete level1_purple_button;
	delete level1_door;
	delete level1_platform;
	delete level1_switch;
}

void CGameStateRun::OnBeginState()
{
}

void CGameStateRun::OnInit()  								// 遊戲的初值及圖形設定
{
	LoadSelectPage();
	LoadLevel1();

	boy.Init("boy");
	boy.SetXY(35, 416);
	girl.Init("girl");
	girl.SetXY(37, 356);

	//..
	LoadMap(1);
	boy.SetMap(&map);
	girl.SetMap(&map);

	diamond_flag = 0;
	level = 0;
	sub_phase = 0;
}

void CGameStateRun::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	//..
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
	if (level != 0 && sub_phase == 0) //關卡選擇、死亡和破關畫面不用OnMove
	{
		//每關都有的元素
		boy.OnMove();
		girl.OnMove();

		//判斷diamond是否被人物吃掉
		CRect boy_body = boy.GetBody();
		CRect girl_body = girl.GetBody();
		for (int i = 0; i < 4; i++)
		{
			level1_red_diamond[i].OnMove(boy_body);
			level1_blue_diamond[i].OnMove(girl_body);
		}

		switch (level)
		{
			case 1:
				Level1OnMove(boy_body, girl_body);
				break;
			default:
				break;
		}
	}
}

void CGameStateRun::OnShow()
{
	switch (level)
	{
		case 0:
			select_page_bg.ShowBitmap();
			d_1.ShowBitmap();
			break;
		case 1:
			switch (sub_phase)
			{
			case 0:
				Level1OnShow();
				break;
			case 1:
				break;
			case 2:
				break;
			}
			break;
		/*case 2:
			switch (sub_phase)
			{
				case 0:
					break;
				case 1:
					break;
				case 2:
					break;
			}
			break;*/
	}
}

void CGameStateRun::LoadSelectPage() 
{
	select_page_bg.LoadBitmapByString({ SELECT_PAGE_BG });
	select_page_bg.SetTopLeft(0, 0);
    
	d_1.LoadBitmapByString({ DIAMOND, DIAMOND_CLICKED },RGB(255, 204, 0));
    d_1.SetTopLeft(302,440);
}

void CGameStateRun::LoadLevel1()
{
	//..
	level1_bg.LoadBitmapByString({ LEVEL_1_BG });
	level1_bg.SetTopLeft(0, 0);

	level1_red_diamond.assign(4, Diamond("red"));
	level1_blue_diamond.assign(4, Diamond("blue"));

	for (int i = 0; i < 2; i++) {
		level1_purple_button[i].buttonPic.LoadBitmapByString({ PURPLE_BUTTON }, RGB(0, 0, 0));
	}
	//platform init 
	level1_platform[0].init(24,320,245,2);
	level1_platform[1].init(555,255,185,1);
	//door init 
	level1_door[0].init(511,5,0);
	level1_door[1].init(563,5,1);

	//button init 
	level1_purple_button[0].init(165,235);
	level1_purple_button[1].init(478,175);

	//diamond init 
	level1_red_diamond[0].Init(333, 423);
	level1_red_diamond[1].Init(129,213);
	level1_red_diamond[2].Init(174,38);
	level1_red_diamond[3].Init(299,73);
	level1_blue_diamond[0].Init(463, 421);
	level1_blue_diamond[1].Init(354, 222);
	level1_blue_diamond[2].Init(38, 73);
	level1_blue_diamond[3].Init(372, 63);

	// switch init 
	level1_switch[0].init(163,302,0);
}

void CGameStateRun::Level1OnMove(const CRect& boy_body, const CRect& girl_body)
{
	level1_switch[0].OnMove(boy_body);
	level1_platform[0].OnMove(level1_switch[0]);
	level1_platform[1].OnMove(level1_purple_button[0]);
	level1_platform[1].OnMove(level1_purple_button[1]);
}

void CGameStateRun::Level1OnShow()
{
	//..
	level1_bg.ShowBitmap();
	boy.OnShow();
	girl.OnShow();

	for (int i = 0; i < 4; i++) {//顯示所有寶石
		
		level1_red_diamond[i].OnShow();
		level1_blue_diamond[i].OnShow();
		
	}
	for ( int i = 0; i < 2; i++) {//顯示所有按鈕
		level1_purple_button[i].OnShow();

	}
	for (int i = 0; i < 2; i++) {//顯示所有升降平台
		level1_platform[i].OnShow();

	}
	level1_door[0].OnShow();
	level1_door[1].OnShow();//顯示door


	level1_switch[0].OnShow();//顯示switch
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