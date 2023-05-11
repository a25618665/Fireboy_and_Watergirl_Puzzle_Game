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
	
	blue_diamond_counter = 0;
	red_diamond_counter = 0;
	
	// person
	boy.Init("boy");
	girl.Init("girl");

	LoadMap();
	LoadSelectPage();
	LoadSubPhase();
	LoadLevel1();

	// 各關鑽石數量
	num_diamonds_each_level[0]["red_diamond"] = level1_red_diamond.size();
	num_diamonds_each_level[0]["blue_diamond"] = level1_blue_diamond.size();
	//num_diamonds_each_level[0]["g_diamond"] = .size();
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
		level1_rock.Reset();
		sub_phase = 0;
		level = 0;
	}
	// debug: 按下t鍵
	if (nChar == 0x54)
	{
		ofstream ifs("./test.map");
		for (int j = 0; j < 480; j++)
		{
			for (int k = 0; k < 640; k++)
			{
				ifs << map[0][k][j];
				ifs << endl;
			}
		}
		ifs.close();
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
	else if (sub_phase == 1)
	{
		if (sub1_retry_body.PtInRect(point))
			is_sub1_retry_clicked = true;
		else if (sub1_back_body.PtInRect(point))
			is_sub1_back_clicked = true;
	}
	else if (sub_phase == 2)
	{
		if (sub2_con_body.PtInRect(point))
			is_sub2_con_clicked = true;
	}
	else if (sub_phase == 3)
	{
		if (sub3_retry_body.PtInRect(point))
			is_sub3_retry_clicked = true;
		else if (sub3_back_body.PtInRect(point))
			is_sub3_back_clicked = true;
	}
}

void CGameStateRun::OnLButtonUp(UINT nFlags, CPoint point)	// 處理滑鼠的動作
{
	if (level == 0 && select_page_button_down != 0) {
		select_page_diamond[select_page_button_down - 1].OnButtonUp();
		level = select_page_button_down;
	}
	else if (sub_phase == 1)
	{
		if (is_sub1_retry_clicked)
		{
			is_sub1_retry_clicked = false;
			// reset 暫時
			boy.SetXY(35, 416);
			girl.SetXY(37, 356);
			sub_phase = 0;
		}
		else if (is_sub1_back_clicked)
		{
			is_sub1_back_clicked = false;
			// reset 暫時
			boy.SetXY(35, 416);
			girl.SetXY(37, 356);
			level = 0;
			sub_phase = 0;
		}
	}
	else if (sub_phase == 2)
	{
		if (is_sub2_con_clicked)
		{
			is_sub2_con_clicked = false;
			// reset 暫時
			boy.SetXY(35, 416);
			girl.SetXY(37, 356);
			level = 0;
			sub_phase = 0;
		}
	}
	else if (sub_phase == 3)
	{
		if (is_sub3_retry_clicked)
		{
			is_sub3_retry_clicked = false;
			// reset 暫時
			boy.SetXY(35, 416);
			girl.SetXY(37, 356);
			sub_phase = 0;
		}
		else if (is_sub3_back_clicked)
		{
			is_sub3_back_clicked = false;
			// reset 暫時
			boy.SetXY(35, 416);
			girl.SetXY(37, 356);
			level = 0;
			sub_phase = 0;
		}
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
	switch (sub_phase)
	{
	case 1:
		img_sub1_bg.ShowBitmap();
		if (is_sub1_retry_clicked)
			img_sub1_retry_clicked.ShowBitmap();
		if (is_sub1_back_clicked)
			img_sub1_back_clicked.ShowBitmap();
		break;
	case 2:
		img_sub2_bg.ShowBitmap();
		SubPhase2ShowText();
		break;
	case 3:
		img_sub2_bg.ShowBitmap();
		SubPhase3ShowText();
		break;
	case 0:
		switch (level)
		{
		case 0:
			select_page_bg.ShowBitmap();
			for (auto & diamond : select_page_diamond)
				diamond.OnShow();
			break;
		case 1:
			Level1OnShow();
			// 暫
			if (c_time_counter == 0)
			{
				time_counter_start = timeGetTime();
				c_time_counter = c_time_counter + 1;
			}
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

void CGameStateRun::LoadSubPhase()
{
	// sub phase 1
	img_sub1_bg.LoadBitmapByString({ SUBPHASE_1 });
	img_sub1_bg.SetTopLeft(0, 0);

	img_sub1_retry_clicked.LoadBitmapByString({ SUBPHASE_1_RETRY });
	img_sub1_retry_clicked.SetTopLeft(230, 230);

	img_sub1_back_clicked.LoadBitmapByString({ SUBPHASE_1_BACK });
	img_sub1_back_clicked.SetTopLeft(260, 285);

	sub1_retry_body = img_sub1_retry_clicked.GetLocation();
	sub1_back_body = img_sub1_back_clicked.GetLocation();

	is_sub1_retry_clicked = false;
	is_sub1_back_clicked = false;

	// sub phase 2
	img_sub2_bg.LoadBitmapByString({ SUBPHASE_2 });
	img_sub2_bg.SetTopLeft(0, 0);

	img_sub2_con_clicked.LoadBitmapByString({ SUBPHASE_2_CONTINUE });
	img_sub2_con_clicked.SetTopLeft(263, 379);

	sub2_con_body.SetRect(263, 328, 397, 348);
	is_sub2_con_clicked = false;

	// sub phase 3
	sub3_retry_body.SetRect(25, 302, 178, 321);
	sub3_back_body.SetRect(237, 337, 424, 356);
	is_sub3_retry_clicked = false;
	is_sub3_back_clicked = false;
}

void CGameStateRun::SubPhase2ShowText()
{
	if (c_time_counter == 1)
	{
		time_counter = timeGetTime() - time_counter_start;
		c_time_counter = 0;
	}

	timer_showtext::show(time_counter, blue_diamond_counter, red_diamond_counter);

	if (!is_sub2_con_clicked)
	{
		CDC *pDC = CDDraw::GetBackCDC();
		CTextDraw::ChangeFontLog(pDC, 20, "Trajan Pro", RGB(255, 218, 0), 800);
		CTextDraw::Print(pDC, 260, 326, "CONTINUE");
		CDDraw::ReleaseBackCDC();
	}
}

void CGameStateRun::SubPhase3ShowText()
{
	CDC *pDC = CDDraw::GetBackCDC();
	CTextDraw::ChangeFontLog(pDC, 20, "Trajan Pro", RGB(255, 218, 0), 800);


	CTextDraw::Print(pDC, 300, 257, "FAIL");

	// 暫時
	if (!is_sub3_retry_clicked)
		CTextDraw::Print(pDC, 254, 300, "RETRY LEVEL");

	if (!is_sub3_back_clicked)
		CTextDraw::Print(pDC, 237, 334, "BACK TO MENU");

	CDDraw::ReleaseBackCDC();
}

void CGameStateRun::LoadLevel1()
{   
	time_counter = 0;
	c_time_counter = 0;
	// person
	/*boy.SetXY(35, 416);
	girl.SetXY(37, 356);*/
	boy.SetXY(470, 130);
	girl.SetXY(500, 130);
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
	level1_switch[0].Init(156, 308, 'R', &map[0],'Y');

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

	// rock
	level1_rock.Init(320, 121, &map[0]);
	// level1_rock.Init(388, 233, &map[0]);

	// door init 
	level1_door.fill(Door());
	level1_door[0].Init(500, 52, 'R');
	level1_door[1].Init(551, 52, 'B');

	// water
	level1_water.fill(Water());
	level1_water[0].Init(302, 455, 369, 462, 'R');
	level1_water[1].Init(428, 455, 498, 462, 'B');
	level1_water[2].Init(396, 359, 467, 366, 'G');
}

void CGameStateRun::Level1OnMove(const CRect& boy_body, const CRect& girl_body)
{
	// diamond
	for (auto & diamond : level1_red_diamond) {
		diamond.OnMove(boy_body, red_diamond_counter);
	}
	for (auto & diamond : level1_blue_diamond)
		diamond.OnMove(girl_body, blue_diamond_counter);
	
	// switch
	for (auto & s : level1_switch)
		s.OnMove(boy_body, girl_body);

	// button
	for (auto & button : level1_button)
		button.OnMove(boy_body, girl_body);

	// platform
	for (auto & platform : level1_platform)
		platform.OnMove();

	// rock
	level1_rock.OnMove(boy_body, girl_body);

	// water
	for (auto & water : level1_water)
	{
		sub_phase = water.OnMove(boy_body, girl_body);
		if (sub_phase)
			break;
	}

	// door
	/*door_counter = 0;
	for (auto & door : level1_door)
		door_counter +=  door.OnMove(boy_body, girl_body);*/

	bool door0_is_triggered = level1_door[0].OnMove(boy_body, girl_body);
	bool door1_is_triggered = level1_door[1].OnMove(boy_body, girl_body);
	if (door0_is_triggered && door1_is_triggered)
	{
		if (red_diamond_counter == num_diamonds_each_level[0]["red_diamond"] &&
			blue_diamond_counter == num_diamonds_each_level[0]["blue_diamond"])	// 破關
			sub_phase = 2;
		else				// 寶石沒吃完
			sub_phase = 3;
	}
}

void CGameStateRun::Level1OnShow()
{
	// background
	level1_bg.ShowBitmap();

	// 
	time_counter = timeGetTime() - time_counter_start;
	timer_showtext::show_in_the_game(time_counter);

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

	// rock
	level1_rock.OnShow();

	// door
	for (auto & door : level1_door)
		door.OnShow();
	
	// person
	boy.OnShow();
	girl.OnShow();
}

void CGameStateRun::LoadLevel2()
{


	time_counter = 0;
	c_time_counter = 0;







	//diamond
	level2_red_diamond.fill(Diamond("red"));
	level2_red_diamond[0].Init(147, 38);
	level2_red_diamond[1].Init(50, 120);
	level2_red_diamond[2].Init(275, 185);
	level2_red_diamond[3].Init(129, 218);
	level2_red_diamond[4].Init(360, 197);
	level2_red_diamond[5].Init(144, 348);
	level2_red_diamond[6].Init(177, 408);

	level2_blue_diamond.fill(Diamond("blue"));
	level2_blue_diamond[0].Init(453, 119);
	level2_blue_diamond[1].Init(334, 231);
	level2_blue_diamond[2].Init(420, 212);
	level2_blue_diamond[3].Init(357, 342);
	level2_blue_diamond[4].Init(489, 348);
	level2_blue_diamond[5].Init(224, 346);
	level2_blue_diamond[6].Init(210, 408);


	// platform init


	// button init 
	level2_button.fill(Button());
	level2_button[0].Init(305, 445, 'W');
	level2_button[1].Init(582, 445, 'W');




	//door
	// door init 
	level2_door.fill(Door());
	level2_door[0].Init(500, 25, 'R');
	level2_door[1].Init(551, 25, 'B');





}