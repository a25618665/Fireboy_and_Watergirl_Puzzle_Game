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
	
	level1_door = new Door[2];
}   

CGameStateRun::~CGameStateRun()
{
	
	
	delete level1_door;
}

void CGameStateRun::OnBeginState()
{
}

void CGameStateRun::OnInit()  								// 遊戲的初值及圖形設定
{
	boy.Init("boy");
	boy.SetXY(35, 416);
	girl.Init("girl");
	girl.SetXY(37, 356);

	level = 0;
	sub_phase = 0;
	button_down = 0;

	LoadSelectPage();
	LoadMap(1);
	LoadLevel1();
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

	if (nChar == 0x50)   // debug: 按下p鍵回到select page
		level = 0;
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

	if (level == 0)
	{
		for (int i = 0; i < 2; i++)
		{
			button_down = select_page_diamond[i].OnButtonDown(x, y);
			if (button_down != 0)
				break;
		}
	}
}

void CGameStateRun::OnLButtonUp(UINT nFlags, CPoint point)	// 處理滑鼠的動作
{
	if (level == 0 && button_down != 0) {
		select_page_diamond[button_down - 1].OnButtonUp();
		level = button_down;
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
			for (int i = 0; i < 2; i++)
				select_page_diamond[i].OnShow();

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
		case 2:
			switch (sub_phase)
			{
			case 0:
				break;
			case 1:
				break;
			case 2:
				break;
			}
			break;
	}
}

void CGameStateRun::LoadSelectPage() 
{
	select_page_bg.LoadBitmapByString({ SELECT_PAGE_BG });
	select_page_bg.SetTopLeft(0, 0);
    
	select_page_diamond.fill( SelectPageDiamond() );
	select_page_diamond[0].Init(1, 302, 440, "blue");
	select_page_diamond[1].Init(2, 322, 386, "blue");
}

void CGameStateRun::LoadLevel1()
{
	//..
	boy.SetMap(&map[0]);
	girl.SetMap(&map[0]);

	level1_bg.LoadBitmapByString({ LEVEL_1_BG });
	level1_bg.SetTopLeft(0, 0);

	level1_red_diamond.assign(4, Diamond("red"));
	level1_blue_diamond.assign(4, Diamond("blue"));

	

	// switch init 
	level1_switch.assign(1, Switch());
	level1_switch[0].Init(156, 308, 'R', &map[0]);

	//button init 
	level1_purple_button.assign(2, Button());
	level1_purple_button[0].Init(171, 236, 'P');
	level1_purple_button[1].Init(475, 171, 'P');

	//platform init 
	level1_platform.assign(2, Platform());
	level1_platform[0].Init(26, 248, 296, 'D', 'Y', &map[0]);
	level1_platform[0].Bind(&level1_switch[0]);
	level1_platform[1].Init(553, 200, 248, 'D', 'P', &map[0]);

	vector<Button *> temp_button_ptr_vector;
	for (auto & button : level1_purple_button)
	{
		temp_button_ptr_vector.push_back(&button);
	}
	level1_platform[1].Bind(temp_button_ptr_vector);
	
	//door init 
	level1_door[0].init(511,5,0);
	level1_door[1].init(563,5,1);

	

	//diamond init 
	level1_red_diamond[0].Init(333, 423);
	level1_red_diamond[1].Init(129,213);
	level1_red_diamond[2].Init(174,38);
	level1_red_diamond[3].Init(299,73);
	level1_blue_diamond[0].Init(463, 421);
	level1_blue_diamond[1].Init(354, 222);
	level1_blue_diamond[2].Init(38, 73);
	level1_blue_diamond[3].Init(372, 63);

	
}

void CGameStateRun::Level1OnMove(const CRect& boy_body, const CRect& girl_body)
{
	level1_switch[0].OnMove(boy_body, girl_body);
	level1_purple_button[0].OnMove(boy_body, girl_body);
	level1_purple_button[1].OnMove(boy_body, girl_body);
	level1_platform[0].OnMove();
	level1_platform[1].OnMove();
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
	//..
	ifstream ifs( MAP_TEMPLATE );

	for (int i = 0; i < 640; i++) {
		for (int j = 0; j < 480; j++) {
			ifs >> map[0][i][j];
		}
	}

	ifs.close();
}