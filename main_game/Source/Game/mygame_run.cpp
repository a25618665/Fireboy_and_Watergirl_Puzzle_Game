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
	AddFontResourceA(FONTS);

	level = 0;
	sub_phase = 0;
	blue_diamond_counter = 0;
	red_diamond_counter = 0;
	time_counter = 0;
	time_counter_start = 0;
	time_counter_flag = false;

	LoadMap();
	LoadSelectPage();
	LoadSubPhase();
	LoadLevel1();
	LoadLevel6();
	LoadLevel10();
	LoadLevel15();
	LoadLevel16();
	LoadLevel18();
	LoadLevel31();

	// 各關鑽石數量
	num_diamonds_each_level[0]["red_diamond"] = level1_red_diamond.size();
	num_diamonds_each_level[0]["blue_diamond"] = level1_blue_diamond.size();
	num_diamonds_each_level[5]["red_diamond"] = level6_red_diamond.size();
	num_diamonds_each_level[5]["blue_diamond"] = level6_blue_diamond.size();
	num_diamonds_each_level[30]["red_diamond"] = level31_red_diamond.size();
	num_diamonds_each_level[30]["blue_diamond"] = level31_blue_diamond.size();
	//num_diamonds_each_level[0]["g_diamond"] = .size();
}

void CGameStateRun::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if (level != 0 && sub_phase == 0)
	{
		// boy
		if (nChar == VK_RIGHT)			// 向右鍵
			boy[level - 1].SetMovingRight(true);
		else if (nChar == VK_LEFT)		// 向左鍵
			boy[level - 1].SetMovingLeft(true);

		if (nChar == VK_UP)				// 向上鍵
			boy[level - 1].Jump();

		// girl
		if (nChar == 0x44)				// D鍵
			girl[level - 1].SetMovingRight(true);
		else if (nChar == 0x41)			// A鍵
			girl[level - 1].SetMovingLeft(true);

		if (nChar == 0x57)				// W鍵
			girl[level - 1].Jump();

		// debug: 按下p鍵回到select page
		if (nChar == 0x50)
		{
			ResetL1();
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
					ifs << map[level - 1][k][j];
					ifs << endl;
				}
			}
			ifs.close();
		}
	}
}

void CGameStateRun::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if (level != 0 && sub_phase == 0)
	{
		// boy
		if (nChar == VK_RIGHT)			// 向右鍵
			boy[level - 1].SetMovingRight(false);
		else if (nChar == VK_LEFT)		// 向左鍵
			boy[level - 1].SetMovingLeft(false);

		// girl
		if (nChar == 0x44)				// D鍵
			girl[level - 1].SetMovingRight(false);
		else if (nChar == 0x41)			// A鍵
			girl[level - 1].SetMovingLeft(false);
	}
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
			ResetLevel(level);
			sub_phase = 0;
		}
		else if (is_sub1_back_clicked)
		{
			is_sub1_back_clicked = false;
			ResetLevel(level);
			level = 0;
			sub_phase = 0;
		}
	}
	else if (sub_phase == 2)
	{
		if (is_sub2_con_clicked)
		{
			is_sub2_con_clicked = false;
			ResetLevel(level);
			level = 0;
			sub_phase = 0;
		}
	}
	else if (sub_phase == 3)
	{
		if (is_sub3_retry_clicked)
		{
			is_sub3_retry_clicked = false;
			ResetLevel(level);
			sub_phase = 0;
		}
		else if (is_sub3_back_clicked)
		{
			is_sub3_back_clicked = false;
			ResetLevel(level);
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
		// 第一次進到關卡時重設時間
		if (!time_counter_flag)
		{
			time_counter_start = timeGetTime();
			time_counter_flag = true;
		}

		switch (level)
		{
		case 1:
			Level1OnMove();
			break;
		case 6:
			Level6OnMove();
			break;
		case 10:
			Level10OnMove();
			break;
		/*case 15:
			Level15OnMove();
			break;
		case 16:
			Level16OnMove();
			break;
		case 18:
			Level18OnMove();
			break;*/
		case 31:
			Level31OnMove();
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
			break;
		case 6:
			Level6OnShow();
			break;
		case 10:
			Level10OnShow();
			break;
		/*case 15:
			Level15OnShow();
			break;
		case 16:
			Level16OnShow();
			break;
		case 18:
			Level18OnShow();
			break;*/
		case 31:
			Level31OnShow();
			break;
		}
		break;
	}
}

void CGameStateRun::ResetLevel(int level)
{
	red_diamond_counter = 0;
	blue_diamond_counter = 0;

	time_counter = 0;
	time_counter_start = 0;
	time_counter_flag = false;

	switch (level)
	{
	case 1:
		ResetL1();
		break;
	case 6:
		ResetL6();
		break;
	case 31:
		ResetL31();
		break;
	}
}

void CGameStateRun::LoadMap()
{
	array<int, 3> a;
	a[0] = 0;
	a[1] = 5;
	a[2] = 30;
	for (int i: a)
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

	//for (int i = 0; i < int(map.size()); i++)
	//{
	//	
	//}
}

void CGameStateRun::LoadSelectPage() 
{
	select_page_button_down = 0;
	
	select_page_bg.LoadBitmapByString({ SELECT_PAGE_BG });
	select_page_bg.SetTopLeft(0, 0);
    
	select_page_diamond.fill( SelectPageDiamond() );
	select_page_diamond[0].Init(1, 302, 440, 'B');
	// select_page_diamond[1].Init(2, 322, 386, 'B');
	select_page_diamond[5].Init(6, 145, 274, 'O');
	select_page_diamond[9].Init(10, 85, 252, 'B');
	select_page_diamond[14].Init(15, 242, 144, 'B');
	select_page_diamond[15].Init(16, 188, 96, 'O');
	select_page_diamond[17].Init(18, 317, 121, 'B');
	select_page_diamond[30].Init(31, 485, 279, 'B');
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
	sub3_retry_body.SetRect(254, 302, 407, 322);
	sub3_back_body.SetRect(237, 337, 424, 356);
	is_sub3_retry_clicked = false;
	is_sub3_back_clicked = false;
}

void CGameStateRun::SubPhase2ShowText()
{
	if (time_counter_flag)
	{
		time_counter = timeGetTime() - time_counter_start;
		time_counter_flag = false;
	}

	int tatal_second = time_counter / 1000;
	int second = tatal_second % 60;
	int minute = tatal_second / 60;

	string time_str = "";
	time_str += (minute >= 10 ? to_string(minute) : ("0" + to_string(minute))) +
				 ":" + (second >= 10 ? to_string(second) : ("0" + to_string(second)));

	CDC *pDC = CDDraw::GetBackCDC();
	CTextDraw::ChangeFontLog(pDC, 20, "Trajan Pro", RGB(255, 218, 0), 800);

	CTextDraw::Print(pDC, 250, 142, "TIME   " + time_str);
	CTextDraw::Print(pDC, 320, 180, "X " + to_string(blue_diamond_counter) + "/" + to_string(num_diamonds_each_level[level - 1]["blue_diamond"]));
	CTextDraw::Print(pDC, 320, 212, "X " + to_string(red_diamond_counter) + "/" + to_string(num_diamonds_each_level[level - 1]["red_diamond"]));

	if (!is_sub2_con_clicked)
		CTextDraw::Print(pDC, 260, 326, "CONTINUE");

	CDDraw::ReleaseBackCDC();
}

void CGameStateRun::SubPhase3ShowText()
{
	if (time_counter_flag)
	{
		time_counter = timeGetTime() - time_counter_start;
		time_counter_flag = false;
	}

	int tatal_second = time_counter / 1000;
	int second = tatal_second % 60;
	int minute = tatal_second / 60;

	string time_str = "";
	time_str += (minute >= 10 ? to_string(minute) : ("0" + to_string(minute))) +
		":" + (second >= 10 ? to_string(second) : ("0" + to_string(second)));

	CDC *pDC = CDDraw::GetBackCDC();
	CTextDraw::ChangeFontLog(pDC, 20, "Trajan Pro", RGB(255, 218, 0), 800);

	CTextDraw::Print(pDC, 250, 142, "TIME   " + time_str);
	CTextDraw::Print(pDC, 320, 180, "X " + to_string(blue_diamond_counter) + "/" + to_string(num_diamonds_each_level[level - 1]["blue_diamond"]));
	CTextDraw::Print(pDC, 320, 212, "X " + to_string(red_diamond_counter) + "/" + to_string(num_diamonds_each_level[level - 1]["red_diamond"]));
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
	// person
	boy[0].Init(35, 416, "boy");
	girl[0].Init(37, 356, "girl");
	boy[0].SetMap(&map[0]);
	girl[0].SetMap(&map[0]);

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
	level1_rock.Init(220, 250, &map[0]);

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

void CGameStateRun::Level1OnMove()
{
	boy[0].OnMove();
	girl[0].OnMove();

	CRect boy_body = boy[0].GetBody();
	CRect girl_body = girl[0].GetBody();

	// diamond
	for (auto & diamond : level1_red_diamond)
		diamond.OnMove(boy_body, red_diamond_counter);
	
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
	/*for (auto & water : level1_water)
	{
		sub_phase = water.OnMove(boy_body, girl_body);
		if (sub_phase)
			break;
	}*/

	// door
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

	// timer
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

	// door
	for (auto & door : level1_door)
		door.OnShow();

	// rock
	level1_rock.OnShow();
	
	// person
	boy[0].OnShow();
	girl[0].OnShow();
}

void CGameStateRun::ResetL1()
{
	boy[0].Reset();
	girl[0].Reset();

	// diamond
	for (auto & diamond : level1_red_diamond)
		diamond.Reset();

	for (auto & diamond : level1_blue_diamond)
		diamond.Reset();

	// switch
	for (auto & s : level1_switch)
		s.Reset();

	// button
	for (auto & button : level1_button)
		button.Reset();

	// platform
	for (auto & platform : level1_platform)
		platform.Reset();

	// rock
	level1_rock.Reset();
}

void CGameStateRun::LoadLevel6()
{
	// person
	boy[5].Init(563, 42, "boy");
	girl[5].Init(56, 240, "girl");
	boy[5].SetMap(&map[5]);
	girl[5].SetMap(&map[5]);

	// background
	level6_bg.LoadBitmapByString({LEVEL_6_BG});
	level6_bg.SetTopLeft(0, 0);

	// diamond 
	level6_red_diamond.fill(Diamond("red"));
	level6_red_diamond[0].Init(86, 106);
	level6_red_diamond[1].Init(70, 170);
	level6_red_diamond[2].Init(167, 42);
	level6_red_diamond[3].Init(230, 90);
	level6_red_diamond[4].Init(423, 90);
	level6_red_diamond[5].Init(325, 90);
	level6_red_diamond[6].Init(263, 154);
	level6_red_diamond[7].Init(407, 170);




	level6_blue_diamond.fill(Diamond("blue"));
	level6_blue_diamond[0].Init(199, 280);
	level6_blue_diamond[1].Init(296, 280);
	level6_blue_diamond[2].Init(392, 280);
	level6_blue_diamond[3].Init(457, 234);
	level6_blue_diamond[4].Init(535, 299);
	level6_blue_diamond[5].Init(550, 360);
	level6_blue_diamond[6].Init(359, 347);
	level6_blue_diamond[7].Init(215, 362);

	
	

	

	

	// door init 
	level6_door.fill(Door());
	level6_door[0].Init(500, 200, 'R');
	level6_door[1].Init(94, 404, 'B');

	// water
	level6_water.fill(Water());
	level6_water[0].Init(250, 120, 305, 127, 'B');
	level6_water[1].Init(345, 120, 403, 127, 'B');
	level6_water[2].Init(443, 120, 496, 127, 'B');
	level6_water[3].Init(250, 200, 291, 206, 'B');
	level6_water[4].Init(140, 310, 194, 320, 'R');
	level6_water[5].Init(235, 310, 290, 320, 'R');
	level6_water[6].Init(330, 310, 385, 320, 'R');
	level6_water[7].Init(348, 390, 386, 400, 'R');
}

void CGameStateRun::Level6OnMove()
{
	boy[5].OnMove();
	girl[5].OnMove();

	CRect boy_body = boy[5].GetBody();
	CRect girl_body = girl[5].GetBody();

	// diamond
	for (auto & diamond : level6_red_diamond)
		diamond.OnMove(boy_body, red_diamond_counter);

	for (auto & diamond : level6_blue_diamond)
		diamond.OnMove(girl_body, blue_diamond_counter);


	// water
	for (auto & water : level6_water)
	{
		sub_phase = water.OnMove(boy_body, girl_body);
		if (sub_phase)
			break;
	}

	// door
	bool door0_is_triggered = level6_door[0].OnMove(boy_body, girl_body);
	bool door1_is_triggered = level6_door[1].OnMove(boy_body, girl_body);
	if (door0_is_triggered && door1_is_triggered)
	{
		if (red_diamond_counter == num_diamonds_each_level[5]["red_diamond"] &&
			blue_diamond_counter == num_diamonds_each_level[5]["blue_diamond"])	// 破關
			sub_phase = 2;
		else				// 寶石沒吃完
			sub_phase = 3;
	}
}

void CGameStateRun::Level6OnShow()
{
	// background
	level6_bg.ShowBitmap();

	// timer
	time_counter = timeGetTime() - time_counter_start;
	timer_showtext::show_in_the_game(time_counter);

	// diamond
	for (auto & diamond : level6_red_diamond)
		diamond.OnShow();
	for (auto & diamond : level6_blue_diamond)
		diamond.OnShow();


	// door
	for (auto & door : level6_door)
		door.OnShow();

	
	// person
	boy[5].OnShow();
	girl[5].OnShow();
}

void CGameStateRun::ResetL6()
{
	boy[5].Reset();
	girl[5].Reset();

	// diamond
	for (auto & diamond : level6_red_diamond)
		diamond.Reset();

	for (auto & diamond : level6_blue_diamond)
		diamond.Reset();
}

void CGameStateRun::LoadLevel10()
{
	//// person
	//boy[30].Init(41, 34, "boy");
	//girl[30].Init(516, 34, "girl");
	//boy[30].SetMap(&map[30]);
	//girl[30].SetMap(&map[30]);

	// background
	level10_bg.LoadBitmapByString({LEVEL_10_BG});
	level10_bg.SetTopLeft(0, 0);

	////diamond
	//level31_red_diamond.fill(Diamond("red"));
	//level31_red_diamond[0].Init(147, 38);
	//level31_red_diamond[1].Init(50, 120);
	//level31_red_diamond[2].Init(275, 185);
	//level31_red_diamond[3].Init(129, 218);
	//level31_red_diamond[4].Init(360, 197);
	//level31_red_diamond[5].Init(144, 348);
	//level31_red_diamond[6].Init(177, 408);

	//level31_blue_diamond.fill(Diamond("blue"));
	//level31_blue_diamond[0].Init(453, 119);
	//level31_blue_diamond[1].Init(334, 231);
	//level31_blue_diamond[2].Init(420, 212);
	//level31_blue_diamond[3].Init(357, 342);
	//level31_blue_diamond[4].Init(489, 348);
	//level31_blue_diamond[5].Init(224, 346);
	//level31_blue_diamond[6].Init(210, 408);

	//// switch
	//level31_switch.fill(Switch());
	//level31_switch[0].Init(43, 242, 'R', &map[30], 'B');
	//level31_switch[1].Init(491, 370, 'R', &map[30], 'R');

	//// button 
	///*level31_button.fill(Button());
	//level31_button[0].Init(308, 443, 'W');
	//level31_button[1].Init(579, 443, 'W');*/

	//// platform
	//level31_platform.fill(Platform());
	//level31_platform[0].Init(265, 280, 222, 'L', 'B', &map[30]);
	//level31_platform[0].Bind(&level31_switch[0]);

	//level31_platform[1].Init(344, 360, 389, 'R', 'O', &map[30]);
	//level31_platform[1].Bind(&level31_switch[1]);

	///*level31_platform[1].Init(423, 455, 409, 'D', 'P', &map[30]);
	//vector<Button *> temp_button_ptr_vector;
	//for (auto & button : level31_button)
	//	temp_button_ptr_vector.push_back(&button);
	//level31_platform[1].Bind(temp_button_ptr_vector);*/

	////door
	//level31_door.fill(Door());
	//level31_door[0].Init(471, 402, 'R');
	//level31_door[1].Init(519, 402, 'B');

	//// water
	//level31_water.fill(Water());
	//level31_water[0].Init(90, 165, 162, 175, 'R');
	//level31_water[1].Init(121, 375, 177, 385, 'R');
	//level31_water[2].Init(426, 327, 449, 335, 'R');
	//level31_water[3].Init(586, 295, 610, 302, 'R');
	//level31_water[4].Init(123, 264, 163, 272, 'B');
	//level31_water[5].Init(206, 264, 226, 270, 'B');
	//level31_water[6].Init(221, 377, 271, 385, 'B');
	//level31_water[7].Init(525, 200, 575, 207, 'B');
	//level31_water[8].Init(155, 437, 180, 445, 'G');
	//level31_water[9].Init(235, 437, 260, 447, 'G');

}

void CGameStateRun::Level10OnMove()
{
	//boy[30].OnMove();
	//girl[30].OnMove();

	//CRect boy_body = boy[30].GetBody();
	//CRect girl_body = girl[30].GetBody();

	//// diamond
	//for (auto & diamond : level31_red_diamond)
	//	diamond.OnMove(boy_body, red_diamond_counter);

	//for (auto & diamond : level31_blue_diamond)
	//	diamond.OnMove(girl_body, blue_diamond_counter);

	//// switch
	//for (auto & s : level31_switch)
	//	s.OnMove(boy_body, girl_body);

	//// button
	///*for (auto & button : level31_button)
	//	button.OnMove(boy_body, girl_body);*/

	//	// platform
	//for (auto & platform : level31_platform)
	//	platform.OnMove();

	//// water
	//for (auto & water : level31_water)
	//{
	//	sub_phase = water.OnMove(boy_body, girl_body);
	//	if (sub_phase)
	//		break;
	//}

	//// door
	//bool door0_is_triggered = level31_door[0].OnMove(boy_body, girl_body);
	//bool door1_is_triggered = level31_door[1].OnMove(boy_body, girl_body);
	//if (door0_is_triggered && door1_is_triggered)
	//{
	//	if (red_diamond_counter == num_diamonds_each_level[30]["red_diamond"] &&
	//		blue_diamond_counter == num_diamonds_each_level[30]["blue_diamond"])	// 破關
	//		sub_phase = 2;
	//	else				// 寶石沒吃完
	//		sub_phase = 3;
	//}
}

void CGameStateRun::Level10OnShow()
{
	// background
	level10_bg.ShowBitmap();

	// timer
	time_counter = timeGetTime() - time_counter_start;
	timer_showtext::show_in_the_game(time_counter);

	//// diamond
	//for (auto & diamond : level31_red_diamond)
	//	diamond.OnShow();
	//for (auto & diamond : level31_blue_diamond)
	//	diamond.OnShow();

	//// switch
	//for (auto & s : level31_switch)
	//	s.OnShow();

	//// button
	///*for (auto & button : level31_button)
	//	button.OnShow();*/

	//	// platform
	//for (auto & platform : level31_platform)
	//	platform.OnShow();

	//// door
	//for (auto & door : level31_door)
	//	door.OnShow();

	//// person
	//boy[30].OnShow();
	//girl[30].OnShow();
}

void CGameStateRun::ResetL10()
{
	//boy[30].Reset();
	//girl[30].Reset();

	//// diamond
	//for (auto & diamond : level31_red_diamond)
	//	diamond.Reset();

	//for (auto & diamond : level31_blue_diamond)
	//	diamond.Reset();

	//// switch
	//for (auto & s : level31_switch)
	//	s.Reset();

	//// button
	///*for (auto & button : level31_button)
	//	button.Reset();*/

	//	// platform
	//for (auto & platform : level31_platform)
	//	platform.Reset();
}

void CGameStateRun::LoadLevel15()
{
	// person
	boy[30].Init(41, 34, "boy");
	girl[30].Init(516, 34, "girl");
	boy[30].SetMap(&map[30]);
	girl[30].SetMap(&map[30]);

	// background
	level15_bg.LoadBitmapByString({LEVEL_15_BG});
	level15_bg.SetTopLeft(0, 0);

	//diamond
	level31_red_diamond.fill(Diamond("red"));
	level31_red_diamond[0].Init(147, 38);
	level31_red_diamond[1].Init(50, 120);
	level31_red_diamond[2].Init(275, 185);
	level31_red_diamond[3].Init(129, 218);
	level31_red_diamond[4].Init(360, 197);
	level31_red_diamond[5].Init(144, 348);
	level31_red_diamond[6].Init(177, 408);

	level31_blue_diamond.fill(Diamond("blue"));
	level31_blue_diamond[0].Init(453, 119);
	level31_blue_diamond[1].Init(334, 231);
	level31_blue_diamond[2].Init(420, 212);
	level31_blue_diamond[3].Init(357, 342);
	level31_blue_diamond[4].Init(489, 348);
	level31_blue_diamond[5].Init(224, 346);
	level31_blue_diamond[6].Init(210, 408);

	// switch
	level31_switch.fill(Switch());
	level31_switch[0].Init(43, 242, 'R', &map[30], 'B');
	level31_switch[1].Init(491, 370, 'R', &map[30], 'R');

	// button 
	/*level31_button.fill(Button());
	level31_button[0].Init(308, 443, 'W');
	level31_button[1].Init(579, 443, 'W');*/

	// platform
	level31_platform.fill(Platform());
	level31_platform[0].Init(265, 280, 222, 'L', 'B', &map[30]);
	level31_platform[0].Bind(&level31_switch[0]);

	level31_platform[1].Init(344, 360, 389, 'R', 'O', &map[30]);
	level31_platform[1].Bind(&level31_switch[1]);

	/*level31_platform[1].Init(423, 455, 409, 'D', 'P', &map[30]);
	vector<Button *> temp_button_ptr_vector;
	for (auto & button : level31_button)
		temp_button_ptr_vector.push_back(&button);
	level31_platform[1].Bind(temp_button_ptr_vector);*/

	//door
	level31_door.fill(Door());
	level31_door[0].Init(471, 402, 'R');
	level31_door[1].Init(519, 402, 'B');

	// water
	level31_water.fill(Water());
	level31_water[0].Init(90, 165, 162, 175, 'R');
	level31_water[1].Init(121, 375, 177, 385, 'R');
	level31_water[2].Init(426, 327, 449, 335, 'R');
	level31_water[3].Init(586, 295, 610, 302, 'R');
	level31_water[4].Init(123, 264, 163, 272, 'B');
	level31_water[5].Init(206, 264, 226, 270, 'B');
	level31_water[6].Init(221, 377, 271, 385, 'B');
	level31_water[7].Init(525, 200, 575, 207, 'B');
	level31_water[8].Init(155, 437, 180, 445, 'G');
	level31_water[9].Init(235, 437, 260, 447, 'G');

}

//void CGameStateRun::Level15OnMove()
//{
//	boy[30].OnMove();
//	girl[30].OnMove();
//
//	CRect boy_body = boy[30].GetBody();
//	CRect girl_body = girl[30].GetBody();
//
//	// diamond
//	for (auto & diamond : level31_red_diamond)
//		diamond.OnMove(boy_body, red_diamond_counter);
//
//	for (auto & diamond : level31_blue_diamond)
//		diamond.OnMove(girl_body, blue_diamond_counter);
//
//	// switch
//	for (auto & s : level31_switch)
//		s.OnMove(boy_body, girl_body);
//
//	// button
//	/*for (auto & button : level31_button)
//		button.OnMove(boy_body, girl_body);*/
//
//		// platform
//	for (auto & platform : level31_platform)
//		platform.OnMove();
//
//	// water
//	for (auto & water : level31_water)
//	{
//		sub_phase = water.OnMove(boy_body, girl_body);
//		if (sub_phase)
//			break;
//	}
//
//	// door
//	bool door0_is_triggered = level31_door[0].OnMove(boy_body, girl_body);
//	bool door1_is_triggered = level31_door[1].OnMove(boy_body, girl_body);
//	if (door0_is_triggered && door1_is_triggered)
//	{
//		if (red_diamond_counter == num_diamonds_each_level[30]["red_diamond"] &&
//			blue_diamond_counter == num_diamonds_each_level[30]["blue_diamond"])	// 破關
//			sub_phase = 2;
//		else				// 寶石沒吃完
//			sub_phase = 3;
//	}
//}
//
//void CGameStateRun::Level15OnShow()
//{
//	// background
//	level31_bg.ShowBitmap();
//
//	// timer
//	time_counter = timeGetTime() - time_counter_start;
//	timer_showtext::show_in_the_game(time_counter);
//
//	// diamond
//	for (auto & diamond : level31_red_diamond)
//		diamond.OnShow();
//	for (auto & diamond : level31_blue_diamond)
//		diamond.OnShow();
//
//	// switch
//	for (auto & s : level31_switch)
//		s.OnShow();
//
//	// button
//	/*for (auto & button : level31_button)
//		button.OnShow();*/
//
//		// platform
//	for (auto & platform : level31_platform)
//		platform.OnShow();
//
//	// door
//	for (auto & door : level31_door)
//		door.OnShow();
//
//	// person
//	boy[30].OnShow();
//	girl[30].OnShow();
//}
//
//void CGameStateRun::ResetL15()
//{
//	boy[30].Reset();
//	girl[30].Reset();
//
//	// diamond
//	for (auto & diamond : level31_red_diamond)
//		diamond.Reset();
//
//	for (auto & diamond : level31_blue_diamond)
//		diamond.Reset();
//
//	// switch
//	for (auto & s : level31_switch)
//		s.Reset();
//
//	// button
//	/*for (auto & button : level31_button)
//		button.Reset();*/
//
//		// platform
//	for (auto & platform : level31_platform)
//		platform.Reset();
//}

void CGameStateRun::LoadLevel16()
{
	// person
	boy[30].Init(41, 34, "boy");
	girl[30].Init(516, 34, "girl");
	boy[30].SetMap(&map[30]);
	girl[30].SetMap(&map[30]);

	// background
	level16_bg.LoadBitmapByString({LEVEL_16_BG});
	level16_bg.SetTopLeft(0, 0);

	//diamond
	level31_red_diamond.fill(Diamond("red"));
	level31_red_diamond[0].Init(147, 38);
	level31_red_diamond[1].Init(50, 120);
	level31_red_diamond[2].Init(275, 185);
	level31_red_diamond[3].Init(129, 218);
	level31_red_diamond[4].Init(360, 197);
	level31_red_diamond[5].Init(144, 348);
	level31_red_diamond[6].Init(177, 408);

	level31_blue_diamond.fill(Diamond("blue"));
	level31_blue_diamond[0].Init(453, 119);
	level31_blue_diamond[1].Init(334, 231);
	level31_blue_diamond[2].Init(420, 212);
	level31_blue_diamond[3].Init(357, 342);
	level31_blue_diamond[4].Init(489, 348);
	level31_blue_diamond[5].Init(224, 346);
	level31_blue_diamond[6].Init(210, 408);

	// switch
	level31_switch.fill(Switch());
	level31_switch[0].Init(43, 242, 'R', &map[30], 'B');
	level31_switch[1].Init(491, 370, 'R', &map[30], 'R');

	// button 
	/*level31_button.fill(Button());
	level31_button[0].Init(308, 443, 'W');
	level31_button[1].Init(579, 443, 'W');*/

	// platform
	level31_platform.fill(Platform());
	level31_platform[0].Init(265, 280, 222, 'L', 'B', &map[30]);
	level31_platform[0].Bind(&level31_switch[0]);

	level31_platform[1].Init(344, 360, 389, 'R', 'O', &map[30]);
	level31_platform[1].Bind(&level31_switch[1]);

	/*level31_platform[1].Init(423, 455, 409, 'D', 'P', &map[30]);
	vector<Button *> temp_button_ptr_vector;
	for (auto & button : level31_button)
		temp_button_ptr_vector.push_back(&button);
	level31_platform[1].Bind(temp_button_ptr_vector);*/

	//door
	level31_door.fill(Door());
	level31_door[0].Init(471, 402, 'R');
	level31_door[1].Init(519, 402, 'B');

	// water
	level31_water.fill(Water());
	level31_water[0].Init(90, 165, 162, 175, 'R');
	level31_water[1].Init(121, 375, 177, 385, 'R');
	level31_water[2].Init(426, 327, 449, 335, 'R');
	level31_water[3].Init(586, 295, 610, 302, 'R');
	level31_water[4].Init(123, 264, 163, 272, 'B');
	level31_water[5].Init(206, 264, 226, 270, 'B');
	level31_water[6].Init(221, 377, 271, 385, 'B');
	level31_water[7].Init(525, 200, 575, 207, 'B');
	level31_water[8].Init(155, 437, 180, 445, 'G');
	level31_water[9].Init(235, 437, 260, 447, 'G');

}

//void CGameStateRun::Level16OnMove()
//{
//	boy[30].OnMove();
//	girl[30].OnMove();
//
//	CRect boy_body = boy[30].GetBody();
//	CRect girl_body = girl[30].GetBody();
//
//	// diamond
//	for (auto & diamond : level31_red_diamond)
//		diamond.OnMove(boy_body, red_diamond_counter);
//
//	for (auto & diamond : level31_blue_diamond)
//		diamond.OnMove(girl_body, blue_diamond_counter);
//
//	// switch
//	for (auto & s : level31_switch)
//		s.OnMove(boy_body, girl_body);
//
//	// button
//	/*for (auto & button : level31_button)
//		button.OnMove(boy_body, girl_body);*/
//
//		// platform
//	for (auto & platform : level31_platform)
//		platform.OnMove();
//
//	// water
//	for (auto & water : level31_water)
//	{
//		sub_phase = water.OnMove(boy_body, girl_body);
//		if (sub_phase)
//			break;
//	}
//
//	// door
//	bool door0_is_triggered = level31_door[0].OnMove(boy_body, girl_body);
//	bool door1_is_triggered = level31_door[1].OnMove(boy_body, girl_body);
//	if (door0_is_triggered && door1_is_triggered)
//	{
//		if (red_diamond_counter == num_diamonds_each_level[30]["red_diamond"] &&
//			blue_diamond_counter == num_diamonds_each_level[30]["blue_diamond"])	// 破關
//			sub_phase = 2;
//		else				// 寶石沒吃完
//			sub_phase = 3;
//	}
//}
//
//void CGameStateRun::Level16OnShow()
//{
//	// background
//	level31_bg.ShowBitmap();
//
//	// timer
//	time_counter = timeGetTime() - time_counter_start;
//	timer_showtext::show_in_the_game(time_counter);
//
//	// diamond
//	for (auto & diamond : level31_red_diamond)
//		diamond.OnShow();
//	for (auto & diamond : level31_blue_diamond)
//		diamond.OnShow();
//
//	// switch
//	for (auto & s : level31_switch)
//		s.OnShow();
//
//	// button
//	/*for (auto & button : level31_button)
//		button.OnShow();*/
//
//		// platform
//	for (auto & platform : level31_platform)
//		platform.OnShow();
//
//	// door
//	for (auto & door : level31_door)
//		door.OnShow();
//
//	// person
//	boy[30].OnShow();
//	girl[30].OnShow();
//}
//
//void CGameStateRun::ResetL16()
//{
//	boy[30].Reset();
//	girl[30].Reset();
//
//	// diamond
//	for (auto & diamond : level31_red_diamond)
//		diamond.Reset();
//
//	for (auto & diamond : level31_blue_diamond)
//		diamond.Reset();
//
//	// switch
//	for (auto & s : level31_switch)
//		s.Reset();
//
//	// button
//	/*for (auto & button : level31_button)
//		button.Reset();*/
//
//		// platform
//	for (auto & platform : level31_platform)
//		platform.Reset();
//}

void CGameStateRun::LoadLevel18()
{
	// person
	boy[30].Init(41, 34, "boy");
	girl[30].Init(516, 34, "girl");
	boy[30].SetMap(&map[30]);
	girl[30].SetMap(&map[30]);

	// background
	level18_bg.LoadBitmapByString({LEVEL_18_BG});
	level18_bg.SetTopLeft(0, 0);

	//diamond
	level31_red_diamond.fill(Diamond("red"));
	level31_red_diamond[0].Init(147, 38);
	level31_red_diamond[1].Init(50, 120);
	level31_red_diamond[2].Init(275, 185);
	level31_red_diamond[3].Init(129, 218);
	level31_red_diamond[4].Init(360, 197);
	level31_red_diamond[5].Init(144, 348);
	level31_red_diamond[6].Init(177, 408);

	level31_blue_diamond.fill(Diamond("blue"));
	level31_blue_diamond[0].Init(453, 119);
	level31_blue_diamond[1].Init(334, 231);
	level31_blue_diamond[2].Init(420, 212);
	level31_blue_diamond[3].Init(357, 342);
	level31_blue_diamond[4].Init(489, 348);
	level31_blue_diamond[5].Init(224, 346);
	level31_blue_diamond[6].Init(210, 408);

	// switch
	level31_switch.fill(Switch());
	level31_switch[0].Init(43, 242, 'R', &map[30], 'B');
	level31_switch[1].Init(491, 370, 'R', &map[30], 'R');

	// button 
	/*level31_button.fill(Button());
	level31_button[0].Init(308, 443, 'W');
	level31_button[1].Init(579, 443, 'W');*/

	// platform
	level31_platform.fill(Platform());
	level31_platform[0].Init(265, 280, 222, 'L', 'B', &map[30]);
	level31_platform[0].Bind(&level31_switch[0]);

	level31_platform[1].Init(344, 360, 389, 'R', 'O', &map[30]);
	level31_platform[1].Bind(&level31_switch[1]);

	/*level31_platform[1].Init(423, 455, 409, 'D', 'P', &map[30]);
	vector<Button *> temp_button_ptr_vector;
	for (auto & button : level31_button)
		temp_button_ptr_vector.push_back(&button);
	level31_platform[1].Bind(temp_button_ptr_vector);*/

	//door
	level31_door.fill(Door());
	level31_door[0].Init(471, 402, 'R');
	level31_door[1].Init(519, 402, 'B');

	// water
	level31_water.fill(Water());
	level31_water[0].Init(90, 165, 162, 175, 'R');
	level31_water[1].Init(121, 375, 177, 385, 'R');
	level31_water[2].Init(426, 327, 449, 335, 'R');
	level31_water[3].Init(586, 295, 610, 302, 'R');
	level31_water[4].Init(123, 264, 163, 272, 'B');
	level31_water[5].Init(206, 264, 226, 270, 'B');
	level31_water[6].Init(221, 377, 271, 385, 'B');
	level31_water[7].Init(525, 200, 575, 207, 'B');
	level31_water[8].Init(155, 437, 180, 445, 'G');
	level31_water[9].Init(235, 437, 260, 447, 'G');

}

//void CGameStateRun::Level18OnMove()
//{
//	boy[30].OnMove();
//	girl[30].OnMove();
//
//	CRect boy_body = boy[30].GetBody();
//	CRect girl_body = girl[30].GetBody();
//
//	// diamond
//	for (auto & diamond : level31_red_diamond)
//		diamond.OnMove(boy_body, red_diamond_counter);
//
//	for (auto & diamond : level31_blue_diamond)
//		diamond.OnMove(girl_body, blue_diamond_counter);
//
//	// switch
//	for (auto & s : level31_switch)
//		s.OnMove(boy_body, girl_body);
//
//	// button
//	/*for (auto & button : level31_button)
//		button.OnMove(boy_body, girl_body);*/
//
//		// platform
//	for (auto & platform : level31_platform)
//		platform.OnMove();
//
//	// water
//	for (auto & water : level31_water)
//	{
//		sub_phase = water.OnMove(boy_body, girl_body);
//		if (sub_phase)
//			break;
//	}
//
//	// door
//	bool door0_is_triggered = level31_door[0].OnMove(boy_body, girl_body);
//	bool door1_is_triggered = level31_door[1].OnMove(boy_body, girl_body);
//	if (door0_is_triggered && door1_is_triggered)
//	{
//		if (red_diamond_counter == num_diamonds_each_level[30]["red_diamond"] &&
//			blue_diamond_counter == num_diamonds_each_level[30]["blue_diamond"])	// 破關
//			sub_phase = 2;
//		else				// 寶石沒吃完
//			sub_phase = 3;
//	}
//}
//
//void CGameStateRun::Level18OnShow()
//{
//	// background
//	level31_bg.ShowBitmap();
//
//	// timer
//	time_counter = timeGetTime() - time_counter_start;
//	timer_showtext::show_in_the_game(time_counter);
//
//	// diamond
//	for (auto & diamond : level31_red_diamond)
//		diamond.OnShow();
//	for (auto & diamond : level31_blue_diamond)
//		diamond.OnShow();
//
//	// switch
//	for (auto & s : level31_switch)
//		s.OnShow();
//
//	// button
//	/*for (auto & button : level31_button)
//		button.OnShow();*/
//
//		// platform
//	for (auto & platform : level31_platform)
//		platform.OnShow();
//
//	// door
//	for (auto & door : level31_door)
//		door.OnShow();
//
//	// person
//	boy[30].OnShow();
//	girl[30].OnShow();
//}
//
//void CGameStateRun::ResetL18()
//{
//	boy[30].Reset();
//	girl[30].Reset();
//
//	// diamond
//	for (auto & diamond : level31_red_diamond)
//		diamond.Reset();
//
//	for (auto & diamond : level31_blue_diamond)
//		diamond.Reset();
//
//	// switch
//	for (auto & s : level31_switch)
//		s.Reset();
//
//	// button
//	/*for (auto & button : level31_button)
//		button.Reset();*/
//
//		// platform
//	for (auto & platform : level31_platform)
//		platform.Reset();
//}

void CGameStateRun::LoadLevel31()
{
	// person
	boy[30].Init(41, 34, "boy");
	girl[30].Init(516, 34, "girl");
	boy[30].SetMap(&map[30]);
	girl[30].SetMap(&map[30]);

	// background
	level31_bg.LoadBitmapByString({ LEVEL_31_BG });
	level31_bg.SetTopLeft(0, 0);

	//diamond
	level31_red_diamond.fill(Diamond("red"));
	level31_red_diamond[0].Init(147, 38);
	level31_red_diamond[1].Init(50, 120);
	level31_red_diamond[2].Init(275, 185);
	level31_red_diamond[3].Init(129, 218);
	level31_red_diamond[4].Init(360, 197);
	level31_red_diamond[5].Init(144, 348);
	level31_red_diamond[6].Init(177, 408);

	level31_blue_diamond.fill(Diamond("blue"));
	level31_blue_diamond[0].Init(453, 119);
	level31_blue_diamond[1].Init(334, 231);
	level31_blue_diamond[2].Init(420, 212);
	level31_blue_diamond[3].Init(357, 342);
	level31_blue_diamond[4].Init(489, 348);
	level31_blue_diamond[5].Init(224, 346);
	level31_blue_diamond[6].Init(210, 408);

	// switch
	level31_switch.fill(Switch());
	level31_switch[0].Init(43, 242, 'R', &map[30], 'B');
	level31_switch[1].Init(491, 370, 'R', &map[30], 'R');

	// button 
	/*level31_button.fill(Button());
	level31_button[0].Init(308, 443, 'W');
	level31_button[1].Init(579, 443, 'W');*/

	// platform
	level31_platform.fill(Platform());
	level31_platform[0].Init(265, 280, 222, 'L', 'B', &map[30]);
	level31_platform[0].Bind(&level31_switch[0]);

	level31_platform[1].Init(344, 360, 389, 'R', 'O', &map[30]);
	level31_platform[1].Bind(&level31_switch[1]);

	/*level31_platform[1].Init(423, 455, 409, 'D', 'P', &map[30]);
	vector<Button *> temp_button_ptr_vector;
	for (auto & button : level31_button)
		temp_button_ptr_vector.push_back(&button);
	level31_platform[1].Bind(temp_button_ptr_vector);*/

	//door
	level31_door.fill(Door());
	level31_door[0].Init(471, 402, 'R');
	level31_door[1].Init(519, 402, 'B');

	// water
	level31_water.fill(Water());
	level31_water[0].Init(90, 165, 162, 175, 'R');
	level31_water[1].Init(121, 375, 177, 385, 'R');
	level31_water[2].Init(426, 327, 449, 335, 'R');
	level31_water[3].Init(586, 295, 610, 302, 'R');
	level31_water[4].Init(123, 264, 163, 272, 'B');
	level31_water[5].Init(206, 264, 226, 270, 'B');
	level31_water[6].Init(221, 377, 271, 385, 'B');
	level31_water[7].Init(525, 200, 575, 207, 'B');
	level31_water[8].Init(155, 437, 180, 445, 'G');
	level31_water[9].Init(235, 437, 260, 447, 'G');

}

void CGameStateRun::Level31OnMove()
{
	boy[30].OnMove();
	girl[30].OnMove();

	CRect boy_body = boy[30].GetBody();
	CRect girl_body = girl[30].GetBody();

	// diamond
	for (auto & diamond : level31_red_diamond)
		diamond.OnMove(boy_body, red_diamond_counter);

	for (auto & diamond : level31_blue_diamond)
		diamond.OnMove(girl_body, blue_diamond_counter);

	// switch
	for (auto & s : level31_switch)
		s.OnMove(boy_body, girl_body);

	// button
	/*for (auto & button : level31_button)
		button.OnMove(boy_body, girl_body);*/

	// platform
	for (auto & platform : level31_platform)
		platform.OnMove();

	// water
	for (auto & water : level31_water)
	{
		sub_phase = water.OnMove(boy_body, girl_body);
		if (sub_phase)
			break;
	}

	// door
	bool door0_is_triggered = level31_door[0].OnMove(boy_body, girl_body);
	bool door1_is_triggered = level31_door[1].OnMove(boy_body, girl_body);
	if (door0_is_triggered && door1_is_triggered)
	{
		if (red_diamond_counter == num_diamonds_each_level[30]["red_diamond"] &&
			blue_diamond_counter == num_diamonds_each_level[30]["blue_diamond"])	// 破關
			sub_phase = 2;
		else				// 寶石沒吃完
			sub_phase = 3;
	}
}

void CGameStateRun::Level31OnShow()
{
	// background
	level31_bg.ShowBitmap();

	// timer
	time_counter = timeGetTime() - time_counter_start;
	timer_showtext::show_in_the_game(time_counter);

	// diamond
	for (auto & diamond : level31_red_diamond)
		diamond.OnShow();
	for (auto & diamond : level31_blue_diamond)
		diamond.OnShow();

	// switch
	for (auto & s : level31_switch)
		s.OnShow();

	// button
	/*for (auto & button : level31_button)
		button.OnShow();*/

	// platform
	for (auto & platform : level31_platform)
		platform.OnShow();

	// door
	for (auto & door : level31_door)
		door.OnShow();

	// person
	boy[30].OnShow();
	girl[30].OnShow();
}

void CGameStateRun::ResetL31()
{
	boy[30].Reset();
	girl[30].Reset();

	// diamond
	for (auto & diamond : level31_red_diamond)
		diamond.Reset();

	for (auto & diamond : level31_blue_diamond)
		diamond.Reset();

	// switch
	for (auto & s : level31_switch)
		s.Reset();

	// button
	/*for (auto & button : level31_button)
		button.Reset();*/

	// platform
	for (auto & platform : level31_platform)
		platform.Reset();
}
