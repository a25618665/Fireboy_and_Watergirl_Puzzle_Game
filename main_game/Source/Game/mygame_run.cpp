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
}

CGameStateRun::~CGameStateRun()
{
}

void CGameStateRun::OnBeginState()
{
}

void CGameStateRun::OnInit()  								// 遊戲的初值及圖形設定
{
	level = 0;
	sub_phase = 0;
	// person
	boy.Init("boy");
	girl.Init("girl");

	LoadMap();
	LoadSelectPage();
	LoadLevel1();


	LoadSubphase();
}

void CGameStateRun::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// boy
	if (nChar == VK_RIGHT)			// 向右鍵
		boy.SetMovingRight(true);
	else if (nChar == VK_LEFT)		// 向左鍵
		boy.SetMovingLeft(true);

	if (nChar == VK_UP)				// 向上鍵
		boy.Jump();

	// girl
	if (nChar == 0x44)				// D鍵
		girl.SetMovingRight(true);
	else if (nChar == 0x41)			// A鍵
		girl.SetMovingLeft(true);

	if (nChar == 0x57)				// W鍵
		girl.Jump();

	// debug: 按下p鍵回到select page
	if (nChar == 0x50)
	{
		boy.SetXY(35, 416);
		girl.SetXY(37, 356);
		sub_phase = 0;
		level = 0;
	}
}

void CGameStateRun::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// boy
	if (nChar == VK_RIGHT)			// 向右鍵
		boy.SetMovingRight(false);
	else if (nChar == VK_LEFT)		// 向左鍵
		boy.SetMovingLeft(false);

	// girl
	if (nChar == 0x44)				// D鍵
		girl.SetMovingRight(false);
	else if (nChar == 0x41)			// A鍵
		girl.SetMovingLeft(false);
}

void CGameStateRun::OnLButtonDown(UINT nFlags, CPoint point)  // 處理滑鼠的動作
{
	if (level == 0)
	{
		for (auto & diamond : select_page_diamond)
		{
			select_page_button_down = diamond.OnButtonDown(point);
			if (select_page_button_down != 0)
				break;
		}
	}
}

void CGameStateRun::OnLButtonUp(UINT nFlags, CPoint point)	// 處理滑鼠的動作
{
	if (level == 0 && select_page_button_down != 0) {
		select_page_diamond[select_page_button_down - 1].OnButtonUp();
		level = select_page_button_down;
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

void CGameStateRun::OnMove()					// 移動遊戲元素
{
	if (level != 0 && sub_phase == 0)			// 在所有關卡遊玩頁面
	{
		// 每關都有的元素
		boy.OnMove();
		girl.OnMove();

		CRect boy_body = boy.GetBody();
		CRect girl_body = girl.GetBody();
		switch (level)
		{
		case 1:
			Level1OnMove(boy_body, girl_body);
			break;
		case 2:
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
			for (auto & diamond : select_page_diamond)
				diamond.OnShow();
			//CDC *pDC = CDDraw::GetBackCDC();
			
			break;
		case 1:
			switch (sub_phase)
			{
			case 0:
				
				Level1OnShow();
				if (c_time_counter == 0) {
					time_counter = timeGetTime(); 
					c_time_counter = c_time_counter + 1;
				}
				break;
			case 1:
				
				SubPhaseOnShow();
				break;
			case 2:
				SubPhaseOnShow();
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

void CGameStateRun::LoadMap()
{
	for (int i = 0; i < int(map.size()); i++)
	{
		string map_path = "";
		ifstream ifs(map_path + MAP_TEMPLATE + to_string(i + 1) + ".map");
		for (int j = 0; j < 640; j++)
		{
			for (int k = 0; k < 480; k++)
			{
				ifs >> map[i][j][k];
			}
		}
		ifs.close();
	}
}

void CGameStateRun::LoadSelectPage() 
{
	select_page_button_down = 0;
	
	select_page_bg.LoadBitmapByString({ SELECT_PAGE_BG });
	select_page_bg.SetTopLeft(0, 0);
    
	select_page_diamond.fill( SelectPageDiamond() );
	select_page_diamond[0].Init(1, 302, 440, 'B');
	select_page_diamond[1].Init(2, 322, 386, 'B');
}



void CGameStateRun::LoadLevel1()
{   //subphase init
	blue_diamond_counter = 0;
	red_diamond_counter = 0;
	time_counter = 0;
	c_time_counter = 0;
	// person
	boy.SetXY(35,416);
	girl.SetXY(35, 416);
	boy.SetMap(&map[0]);
	girl.SetMap(&map[0]);

	// background
	level1_bg.LoadBitmapByString({ LEVEL_1_BG });
	level1_bg.SetTopLeft(0, 0);
	

	// diamond 
	level1_red_diamond.fill(Diamond("red"));
	level1_red_diamond[0].Init(333, 423);
	level1_red_diamond[1].Init(129, 213);
	level1_red_diamond[2].Init(174, 38);
	level1_red_diamond[3].Init(299, 73);

	level1_blue_diamond.fill(Diamond("blue"));
	level1_blue_diamond[0].Init(463, 421);
	level1_blue_diamond[1].Init(354, 222);
	level1_blue_diamond[2].Init(38, 73);
	level1_blue_diamond[3].Init(372, 63);

	// switch init 
	level1_switch.fill(Switch());
	level1_switch[0].Init(156, 308, 'R', &map[0]);

	// button init 
	level1_button.fill(Button());
	level1_button[0].Init(171, 236, 'P');
	level1_button[1].Init(475, 171, 'P');

	// platform init 
	level1_platform.fill(Platform());
	level1_platform[0].Init(26, 248, 296, 'D', 'Y', &map[0]);
	level1_platform[0].Bind(&level1_switch[0]);
	level1_platform[1].Init(553, 200, 248, 'D', 'P', &map[0]);

	vector<Button *> temp_button_ptr_vector;
	for (auto & button : level1_button)
		temp_button_ptr_vector.push_back(&button);

	level1_platform[1].Bind(temp_button_ptr_vector);

	// door init 
	level1_door.fill(Door());
	level1_door[0].init(500,25,510,100,'R');
	level1_door[1].init(551,25,570,100,'B');

	// water
	level1_water.fill(Water());
	level1_water[0].Init(302, 455, 369, 462, 'R');
	level1_water[1].Init(428, 455, 498, 462, 'B');
	level1_water[2].Init(396, 359, 467, 366, 'G');
}


void CGameStateRun::LoadSubphase() {
	subphse_bg.LoadBitmapByString({ SUBPHASE });
	subphse_bg.SetTopLeft(0, 0);

	red.LoadBitmapByString({ DIAMOND_RED_1 },RGB(0,0,0));
	red.SetTopLeft(280,180);

	blue.LoadBitmapByString({ DIAMOND_BLUE_1 }, RGB(0, 0, 0));
	blue.SetTopLeft(280, 215);


}
void CGameStateRun::SubPhaseOnShow() {
	subphse_bg.ShowBitmap();
	red.ShowBitmap();
	blue.ShowBitmap();
	if (c_time_counter == 1) {
		
		time_counter = timeGetTime() - time_counter;
		c_time_counter = 0;
	}

	timer_showtext::show(time_counter,blue_diamond_counter,red_diamond_counter);
	//blue_diamond_counter = 0;
	//red_diamond_counter = 0;
}

void CGameStateRun::Level1OnMove(const CRect& boy_body, const CRect& girl_body)
{
	// diamond
	for (auto & diamond : level1_red_diamond) {
		diamond.OnMove(boy_body,red_diamond_counter);
	}
	for (auto & diamond : level1_blue_diamond)
		diamond.OnMove(girl_body,blue_diamond_counter);
	
	// switch
	for (auto & s : level1_switch)
		s.OnMove(boy_body, girl_body);

	// button
	for (auto & button : level1_button)
		button.OnMove(boy_body, girl_body);

	// platform
	for (auto & platform : level1_platform)
		platform.OnMove();

	// water
	for (auto & water : level1_water)
	{
		sub_phase = water.OnMove(boy_body, girl_body);
		if (sub_phase)
			break;
	}
	door_counter = 0;
	for (auto & door : level1_door)
	{
	
		door_counter = door_counter +  door.OnMove(boy_body, girl_body);
         		
		if (door_counter == 2) {
			sub_phase = 1;
			break;
		}
		
		
	}
}

void CGameStateRun::Level1OnShow()
{
	// background
	level1_bg.ShowBitmap();
	//timer紀錄開始時間
	//if (c_time_counter == 0) {
		
	// diamond
	for (auto & diamond : level1_red_diamond)
		diamond.OnShow();
	for (auto & diamond : level1_blue_diamond)
		diamond.OnShow();

	// switch
	for (auto & s : level1_switch)
		s.OnShow();

	// button
	for (auto & button : level1_button)
		button.OnShow();

	// platform
	for (auto & platform : level1_platform)
		platform.OnShow();

	// door
	for (auto & door : level1_door)
		door.OnShow();
	
	// person
	boy.OnShow();
	girl.OnShow();

	// water
	for (auto & water : level1_water)
		water.OnShow();
}
