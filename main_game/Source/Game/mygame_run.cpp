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
	ShowInitProgress(90, "Load Level");
	LoadLevel1();
	LoadLevel6();
	LoadLevel10();
	LoadLevel15();
	LoadLevel16();
	LoadLevel18();
	LoadLevel31();
	ShowInitProgress(100, "Done");

	// 各關鑽石數量
	num_diamonds_each_level[0]["red_diamond"] = level1_red_diamond.size();
	num_diamonds_each_level[0]["blue_diamond"] = level1_blue_diamond.size();
	num_diamonds_each_level[5]["red_diamond"] = level6_red_diamond.size();
	num_diamonds_each_level[5]["blue_diamond"] = level6_blue_diamond.size();
	num_diamonds_each_level[9]["red_diamond"] = level10_red_diamond.size();
	num_diamonds_each_level[9]["blue_diamond"] = level10_blue_diamond.size();
	num_diamonds_each_level[14]["red_diamond"] = level15_red_diamond.size();
	num_diamonds_each_level[14]["blue_diamond"] = level15_blue_diamond.size();
	num_diamonds_each_level[15]["red_diamond"] = level16_red_diamond.size();
	num_diamonds_each_level[15]["blue_diamond"] = level16_blue_diamond.size();
	num_diamonds_each_level[17]["red_diamond"] = level18_red_diamond.size();
	num_diamonds_each_level[17]["blue_diamond"] = level18_blue_diamond.size();
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
			ResetLevel(level);
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
			select_page_diamond[level - 1].SetTime(time_counter);
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
		case 15:
			Level15OnMove();
			break;
		case 16:
			Level16OnMove();
			break;
		case 18:
			Level18OnMove();
			break;
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
		case 15:
			Level15OnShow();
			break;
		case 16:
			Level16OnShow();
			break;
		case 18:
			Level18OnShow();
			break;
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
	case 10:
		ResetL10();
		break;
	case 15:
		ResetL15();
		break;
	case 16:
		ResetL16();
		break;
	case 18:
		ResetL18();
		break;
	case 31:
		ResetL31();
		break;
	}
}

void CGameStateRun::LoadMap()
{
	array<int, 7> a;
	a[0] = 0;
	a[1] = 5;
	a[2] = 30;
	a[3] = 9;
	a[4] = 14;
	a[5] = 15;
	a[6] = 17;
	int t = 0;
	for (int i: a)
	{
		string s = "";
		ShowInitProgress(25 + t * 10, s + "Load Map " + std::to_string(t + 1));
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
		t++;
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

// Level 1
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
	level1_rock.Init(320, 121, &map[0]);

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
	for (auto & water : level1_water)
	{
		sub_phase = water.OnMove(boy_body, girl_body);
		if (sub_phase)
			break;
	}

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

// Level 6
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

// Level 10
void CGameStateRun::LoadLevel10()
{

	boy[9].Init(278, 49, "boy");
	girl[9].Init(300, 49, "girl");
	boy[9].SetMap(&map[9]);
	girl[9].SetMap(&map[9]);


	// background
	level10_bg.LoadBitmapByString({LEVEL_10_BG});
	level10_bg.SetTopLeft(0, 0);


	//diamond
	level10_red_diamond.fill(Diamond("red"));
	level10_red_diamond[0].Init(154, 43);
	level10_red_diamond[1].Init(396, 90);
	level10_red_diamond[2].Init(588, 58);
	level10_red_diamond[3].Init(381, 187);
	level10_red_diamond[4].Init(217, 251);
	level10_red_diamond[5].Init(217, 360);

	level10_blue_diamond.fill(Diamond("blue"));
	level10_blue_diamond[0].Init(233, 93);
	level10_blue_diamond[1].Init(491, 123);
	level10_blue_diamond[2].Init(123, 172);
	level10_blue_diamond[3].Init(557, 220);
	level10_blue_diamond[4].Init(427, 300);
	level10_blue_diamond[5].Init(475, 397);

	// switch
	level10_switch.fill(Switch());
	level10_switch[0].Init(564, 87, 'R', &map[9], 'Y');
	level10_switch[1].Init(161, 184, 'L', &map[9], 'B');


	// platform
	level10_platform.fill(Platform());
	level10_platform[0].Init(200, 30, 90, 'D', 'Y', &map[9]);
	level10_platform[0].Bind(&level10_switch[0]);

	level10_platform[1].Init(522, 155, 570, 'R', 'B', &map[9]);
	level10_platform[1].Bind(&level10_switch[1]);


	//door
	level10_door.fill(Door());
	level10_door[0].Init(580, 283, 'R');
	level10_door[1].Init(26, 363, 'B');

	// water
	level10_water.fill(Water());
	level10_water[0].Init(104, 70, 179, 78, 'R');
	level10_water[1].Init(346, 119, 189, 126, 'R');
	level10_water[2].Init(409, 118, 435, 126, 'R');
	level10_water[3].Init(139, 121, 242, 128, 'B');
	level10_water[4].Init(316, 233, 339, 238, 'B');
	level10_water[5].Init(526, 249, 578, 255, 'B');
	level10_water[6].Init(156, 280, 195, 285, 'B');
	level10_water[7].Init(106, 280, 130, 287, 'B');
	level10_water[8].Init(235, 280, 180, 289, 'B');
	level10_water[9].Init(74, 440, 613, 450, 'G');

}

void CGameStateRun::Level10OnMove()
{



	boy[9].OnMove();
	girl[9].OnMove();

	CRect boy_body = boy[9].GetBody();
	CRect girl_body = girl[9].GetBody();





	// diamond
	for (auto & diamond : level10_red_diamond)
		diamond.OnMove(boy_body, red_diamond_counter);

	for (auto & diamond : level10_blue_diamond)
		diamond.OnMove(girl_body, blue_diamond_counter);

	// switch
	for (auto & s : level10_switch)
		s.OnMove(boy_body, girl_body);


	// platform
	for (auto & platform : level10_platform)
		platform.OnMove();

	// water
	for (auto & water : level10_water)
	{
		sub_phase = water.OnMove(boy_body, girl_body);
		if (sub_phase)
			break;
	}

	// door
	bool door0_is_triggered = level10_door[0].OnMove(boy_body, girl_body);
	bool door1_is_triggered = level10_door[1].OnMove(boy_body, girl_body);
	if (door0_is_triggered && door1_is_triggered)
	{
		if (red_diamond_counter == num_diamonds_each_level[9]["red_diamond"] &&
			blue_diamond_counter == num_diamonds_each_level[9]["blue_diamond"])	// 破關
			sub_phase = 2;
		else				// 寶石沒吃完
			sub_phase = 3;
	}
}

void CGameStateRun::Level10OnShow()
{










	// background
	level10_bg.ShowBitmap();

	// timer
	time_counter = timeGetTime() - time_counter_start;
	timer_showtext::show_in_the_game(time_counter);

	// diamond
	for (auto & diamond : level10_red_diamond)
		diamond.OnShow();
	for (auto & diamond : level10_blue_diamond)
		diamond.OnShow();

	// switch
	for (auto & s : level10_switch)
		s.OnShow();

	// button
	/*for (auto & button : level31_button)
		button.OnShow();*/

		// platform
	for (auto & platform : level10_platform)
		platform.OnShow();

	// door
	for (auto & door : level10_door)
		door.OnShow();

	// person
	boy[9].OnShow();
	girl[9].OnShow();










}

void CGameStateRun::ResetL10()
{
	boy[9].Reset();
	girl[9].Reset();

	// diamond
	for (auto & diamond : level10_red_diamond)
		diamond.Reset();

	for (auto & diamond : level10_blue_diamond)
		diamond.Reset();

	// switch
	for (auto & s : level10_switch)
		s.Reset();

	// platform
	for (auto & platform : level10_platform)
		platform.Reset();
}

// Level 15
void CGameStateRun::LoadLevel15()
{
	// person
	boy[14].Init(500, 360, "boy");
	girl[14].Init(455, 360, "girl");
	boy[14].SetMap(&map[14]);
	girl[14].SetMap(&map[14]);

	// background
	level15_bg.LoadBitmapByString({LEVEL_15_BG});
	level15_bg.SetTopLeft(0, 0);

	//diamond
	level15_red_diamond.fill(Diamond("red"));
	level15_red_diamond[0].Init(185, 40);
	level15_red_diamond[1].Init(520, 42);
	level15_red_diamond[2].Init(200, 107);
	level15_red_diamond[3].Init(107, 170);
	level15_red_diamond[4].Init(170, 234);
	level15_red_diamond[5].Init(520, 234);
	level15_red_diamond[6].Init(425, 300);
	level15_red_diamond[7].Init(458, 427);

	level15_blue_diamond.fill(Diamond("blue"));
	level15_blue_diamond[0].Init(218, 40);
	level15_blue_diamond[1].Init(570, 42);
	level15_blue_diamond[2].Init(458, 107);
	level15_blue_diamond[3].Init(425, 170);
	level15_blue_diamond[4].Init(425, 234);
	level15_blue_diamond[5].Init(475, 300);
	level15_blue_diamond[6].Init(140, 363);
	level15_blue_diamond[7].Init(140, 428);

	// switch
	level15_switch.fill(Switch());
	level15_switch[0].Init(197, 182, 'R', &map[14], 'Y');
	level15_switch[1].Init(96, 378, 'R', &map[14], 'W');
	level15_switch[2].Init(96, 440, 'R', &map[14], 'G');
	level15_switch[3].Init(560, 439, 'L', &map[14], 'P');

	// button 
	level15_button.fill(Button());
	level15_button[0].Init(466, 61, 'P');
	level15_button[1].Init(482, 250, 'Y');
	level15_button[2].Init(50, 315, 'B');
	level15_button[3].Init(273, 315, 'Y');

	// platform
	level15_platform.fill(Platform());
	level15_platform[0].Init(330, 73, 265, 'L', 'Y', &map[14]);
	level15_platform[0].Bind(&level15_switch[0]);

	level15_platform[1].Init(530, 138, 595, 'R', 'P', &map[14]);
	vector<Button *> temp_button_ptr_vector;
	for (auto & button : level15_button)
		temp_button_ptr_vector.push_back(&button);

	level15_platform[1].Bind(temp_button_ptr_vector);
	level15_platform[2].Init(257, 200, 330, 'R', 'O', &map[14]);
	level15_platform[2].Bind(temp_button_ptr_vector);
	level15_platform[3].Init(315, 220, 270, 'U', 'B', &map[14]);
	level15_platform[3].Bind(temp_button_ptr_vector);
	level15_platform[4].Init(48, 265, 110, 'R', 'O', &map[14]);
	level15_platform[4].Bind(temp_button_ptr_vector);
	level15_platform[5].Init(313, 282, 273, 'U', 'G', &map[14]);
	level15_platform[5].Bind(&level15_switch[2]);
	level15_platform[6].Init(530, 330, 585, 'R', 'W', &map[14]);
	level15_platform[6].Bind(&level15_switch[1]);
	level15_platform[7].Init(310, 410, 250, 'L', 'P', &map[14]);
	level15_platform[8].Init(370, 410, 310, 'L', 'P', &map[14]);
	level15_platform[7].Bind(&level15_switch[3]);
	level15_platform[8].Bind(&level15_switch[3]);



	/*level31_platform[1].Init(423, 455, 409, 'D', 'P', &map[30]);
	vector<Button *> temp_button_ptr_vector;
	for (auto & button : level31_button)
		temp_button_ptr_vector.push_back(&button);
	level31_platform[1].Bind(temp_button_ptr_vector);*/

	//door
	level15_door.fill(Door());
	level15_door[0].Init(40, 25, 'R');
	level15_door[1].Init(110, 25, 'B');



}

void CGameStateRun::Level15OnMove()
{
	boy[14].OnMove();
	girl[14].OnMove();

	CRect boy_body = boy[14].GetBody();
	CRect girl_body = girl[14].GetBody();

	// diamond
	for (auto & diamond : level15_red_diamond)
		diamond.OnMove(boy_body, red_diamond_counter);

	for (auto & diamond : level15_blue_diamond)
		diamond.OnMove(girl_body, blue_diamond_counter);

	// switch
	for (auto & s : level15_switch)
		s.OnMove(boy_body, girl_body);

	// button
	for (auto & button : level15_button)
		button.OnMove(boy_body, girl_body);

	// platform
	for (auto & platform : level15_platform)
		platform.OnMove();



	// door
	bool door0_is_triggered = level15_door[0].OnMove(boy_body, girl_body);
	bool door1_is_triggered = level15_door[1].OnMove(boy_body, girl_body);
	if (door0_is_triggered && door1_is_triggered)
	{
		if (red_diamond_counter == num_diamonds_each_level[14]["red_diamond"] &&
			blue_diamond_counter == num_diamonds_each_level[14]["blue_diamond"])	// 破關
			sub_phase = 2;
		else				// 寶石沒吃完
			sub_phase = 3;
	}
}

void CGameStateRun::Level15OnShow()
{
	// background
	level15_bg.ShowBitmap();

	// timer
	time_counter = timeGetTime() - time_counter_start;
	timer_showtext::show_in_the_game(time_counter);

	// diamond
	for (auto & diamond : level15_red_diamond)
		diamond.OnShow();
	for (auto & diamond : level15_blue_diamond)
		diamond.OnShow();

	// switch
	for (auto & s : level15_switch)
		s.OnShow();

	// button
	for (auto & button : level15_button)
		button.OnShow();
	// platform
	for (auto & platform : level15_platform)
		platform.OnShow();

	// door
	for (auto & door : level15_door)
		door.OnShow();

	// person
	boy[14].OnShow();
	girl[14].OnShow();
}

void CGameStateRun::ResetL15()
{
	boy[14].Reset();
	girl[14].Reset();

	// diamond
	for (auto & diamond : level15_red_diamond)
		diamond.Reset();

	for (auto & diamond : level15_blue_diamond)
		diamond.Reset();

	// switch
	for (auto & s : level15_switch)
		s.Reset();

	// button
	for (auto & button : level15_button)
		button.Reset();

	// platform
	for (auto & platform : level15_platform)
		platform.Reset();
}

// Level 16
void CGameStateRun::LoadLevel16()
{
	// person
	boy[15].Init(535, 418, "boy");
	girl[15].Init(535, 359, "girl");
	boy[15].SetMap(&map[15]);
	girl[15].SetMap(&map[15]);

	// background
	level16_bg.LoadBitmapByString({LEVEL_16_BG});
	level16_bg.SetTopLeft(0, 0);

	//diamond
	level16_red_diamond.fill(Diamond("red"));
	level16_red_diamond[0].Init(265, 40);
	level16_red_diamond[1].Init(538, 40);
	level16_red_diamond[2].Init(395, 150);
	level16_red_diamond[3].Init(282, 263);
	level16_red_diamond[4].Init(265, 330);
	level16_red_diamond[5].Init(135, 390);

	level16_blue_diamond.fill(Diamond("blue"));
	level16_blue_diamond[0].Init(170, 87);
	level16_blue_diamond[1].Init(282, 119);
	level16_blue_diamond[2].Init(187, 183);
	level16_blue_diamond[3].Init(427, 264);
	level16_blue_diamond[4].Init(140, 330);
	level16_blue_diamond[5].Init(329, 408);

	// switch
	level16_switch.fill(Switch());
	level16_switch[0].Init(350, 290, 'L', &map[15], 'B');
	level16_switch[1].Init(252, 276, 'L', &map[15], 'Y');



	// platform
	level16_platform.fill(Platform());
	level16_platform[0].Init(185, 297, 350, 'D', 'B', &map[15]);
	level16_platform[0].Bind(&level16_switch[0]);

	level16_platform[1].Init(185, 380, 430, 'R', 'Y', &map[15]);
	level16_platform[1].Bind(&level16_switch[1]);

	/*level31_platform[1].Init(423, 455, 409, 'D', 'P', &map[30]);
	vector<Button *> temp_button_ptr_vector;
	for (auto & button : level31_button)
		temp_button_ptr_vector.push_back(&button);
	level31_platform[1].Bind(temp_button_ptr_vector);*/

	//door
	level16_door.fill(Door());
	level16_door[0].Init(570, 348, 'R');
	level16_door[1].Init(570, 402, 'B');

	// water
	/*level16_water.fill(Water());
	level16_water[0].Init(175, 358, 236, 365, 'R');
	level16_water[1].Init(175, 438, 177, 445, 'B');*/


}

void CGameStateRun::Level16OnMove()
{
	boy[15].OnMove();
	girl[15].OnMove();

	CRect boy_body = boy[15].GetBody();
	CRect girl_body = girl[15].GetBody();

	// diamond
	for (auto & diamond : level16_red_diamond)
		diamond.OnMove(boy_body, red_diamond_counter);

	for (auto & diamond : level16_blue_diamond)
		diamond.OnMove(girl_body, blue_diamond_counter);

	// switch
	for (auto & s : level31_switch)
		s.OnMove(boy_body, girl_body);


	// platform
	for (auto & platform : level16_platform)
		platform.OnMove();

	// water
	/*for (auto & water : level16_water)
	{
		sub_phase = water.OnMove(boy_body, girl_body);
		if (sub_phase)
			break;
	}*/

	// door
	bool door0_is_triggered = level16_door[0].OnMove(boy_body, girl_body);
	bool door1_is_triggered = level16_door[1].OnMove(boy_body, girl_body);
	if (door0_is_triggered && door1_is_triggered)
	{
		if (red_diamond_counter == num_diamonds_each_level[15]["red_diamond"] &&
			blue_diamond_counter == num_diamonds_each_level[15]["blue_diamond"])	// 破關
			sub_phase = 2;
		else				// 寶石沒吃完
			sub_phase = 3;
	}
}

void CGameStateRun::Level16OnShow()
{
	// background
	level16_bg.ShowBitmap();

	// timer
	time_counter = timeGetTime() - time_counter_start;
	timer_showtext::show_in_the_game(time_counter);

	// diamond
	for (auto & diamond : level16_red_diamond)
		diamond.OnShow();
	for (auto & diamond : level16_blue_diamond)
		diamond.OnShow();

	// switch
	for (auto & s : level16_switch)
		s.OnShow();



	// platform
	for (auto & platform : level16_platform)
		platform.OnShow();

	// door
	for (auto & door : level16_door)
		door.OnShow();

	// person
	boy[15].OnShow();
	girl[15].OnShow();
}

void CGameStateRun::ResetL16()
{
	boy[15].Reset();
	girl[15].Reset();

	// diamond
	for (auto & diamond : level16_red_diamond)
		diamond.Reset();

	for (auto & diamond : level16_blue_diamond)
		diamond.Reset();

	// switch
	for (auto & s : level16_switch)
		s.Reset();



	// platform
	for (auto & platform : level16_platform)
		platform.Reset();
}

// Level 18
void CGameStateRun::LoadLevel18()
{
	// person
	boy[17].Init(292, 173, "boy");
	girl[17].Init(324, 179, "girl");
	boy[17].SetMap(&map[17]);
	girl[17].SetMap(&map[17]);

	// background
	level18_bg.LoadBitmapByString({LEVEL_18_BG});
	level18_bg.SetTopLeft(0, 0);

	// diamond 
	level18_red_diamond.fill(Diamond("red"));
	level18_red_diamond[0].Init(67, 215);
	level18_red_diamond[1].Init(290, 39);
	level18_red_diamond[2].Init(500, 36);
	level18_red_diamond[3].Init(515, 229);
	level18_red_diamond[4].Init(113, 340);

	level18_blue_diamond.fill(Diamond("blue"));
	level18_blue_diamond[0].Init(82, 68);
	level18_blue_diamond[1].Init(116, 215);
	level18_blue_diamond[2].Init(548, 309);
	level18_blue_diamond[3].Init(325, 407);
	level18_blue_diamond[4].Init(435, 407);

	// switch init 
	level18_switch.fill(Switch());
	level18_switch[0].Init(222, 101, 'L', &map[17], 'G');
	level18_switch[1].Init(174, 342, 'R', &map[17], 'W');

	// button init 
	level18_button.fill(Button());
	level18_button[0].Init(43, 43, 'W');
	level18_button[1].Init(443, 348, 'B');

	// platform init 
	level18_platform.fill(Platform());
	level18_platform[0].Init(278, 25, 65, 'D', 'W', &map[17]);
	level18_platform[1].Init(456, 183, 137, 'U', 'G', &map[17], 'V');
	level18_platform[2].Init(103, 391, 327, 'U', 'B', &map[17]);
	level18_platform[3].Init(232, 310, 252, 'U', 'W', &map[17], 'V');
	level18_platform[4].Init(359, 310, 250, 'U', 'W', &map[17], 'V');

	level18_platform[0].Bind(&level18_button[0]);
	level18_platform[1].Bind(&level18_switch[0]);
	level18_platform[2].Bind(&level18_button[1]);
	level18_platform[3].Bind(&level18_switch[1]);
	level18_platform[4].Bind(&level18_switch[1]);

	// door init 
	level18_door.fill(Door());
	level18_door[0].Init(551, 403, 'R');
	level18_door[1].Init(175, 402, 'B');

	// water
	level18_water.fill(Water());
	level18_water[0].Init(93, 168, 177, 174, 'B');
	level18_water[1].Init(251, 71, 356, 78, 'G');
	level18_water[2].Init(444, 167, 547, 174, 'R');
	level18_water[3].Init(236, 295, 275, 302, 'R');
	level18_water[4].Init(364, 296, 403, 302, 'B');
	level18_water[5].Init(507, 263, 548, 270, 'R');
	level18_water[6].Init(524, 344, 579, 350, 'B');
}

void CGameStateRun::Level18OnMove()
{
	boy[17].OnMove();
	girl[17].OnMove();

	CRect boy_body = boy[17].GetBody();
	CRect girl_body = girl[17].GetBody();

	// diamond
	for (auto & diamond : level18_red_diamond)
		diamond.OnMove(boy_body, red_diamond_counter);

	for (auto & diamond : level18_blue_diamond)
		diamond.OnMove(girl_body, blue_diamond_counter);

	// switch
	for (auto & s : level18_switch)
		s.OnMove(boy_body, girl_body);

	// button
	for (auto & button : level18_button)
		button.OnMove(boy_body, girl_body);

	// platform
	for (auto & platform : level18_platform)
		platform.OnMove();

	// water
	for (auto & water : level18_water)
	{
		sub_phase = water.OnMove(boy_body, girl_body);
		if (sub_phase)
			break;
	}

	// door
	bool door0_is_triggered = level18_door[0].OnMove(boy_body, girl_body);
	bool door1_is_triggered = level18_door[1].OnMove(boy_body, girl_body);
	if (door0_is_triggered && door1_is_triggered)
	{
		if (red_diamond_counter == num_diamonds_each_level[0]["red_diamond"] &&
			blue_diamond_counter == num_diamonds_each_level[0]["blue_diamond"])	// 破關
			sub_phase = 2;
		else				// 寶石沒吃完
			sub_phase = 3;
	}
}

void CGameStateRun::Level18OnShow()
{
	// background
	level18_bg.ShowBitmap();

	// timer
	time_counter = timeGetTime() - time_counter_start;
	timer_showtext::show_in_the_game(time_counter);

	// diamond
	for (auto & diamond : level18_red_diamond)
		diamond.OnShow();
	for (auto & diamond : level18_blue_diamond)
		diamond.OnShow();

	// switch
	for (auto & s : level18_switch)
		s.OnShow();

	// button
	for (auto & button : level18_button)
		button.OnShow();

	// platform
	for (auto & platform : level18_platform)
		platform.OnShow();

	// door
	for (auto & door : level18_door)
		door.OnShow();

	// person
	boy[17].OnShow();
	girl[17].OnShow();
}

void CGameStateRun::ResetL18()
{
	boy[17].Reset();
	girl[17].Reset();

	// diamond
	for (auto & diamond : level18_red_diamond)
		diamond.Reset();

	for (auto & diamond : level18_blue_diamond)
		diamond.Reset();

	// switch
	for (auto & s : level18_switch)
		s.Reset();

	// button
	for (auto & button : level18_button)
		button.Reset();

	// platform
	for (auto & platform : level18_platform)
		platform.Reset();
}

// Level 31
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
	level31_switch[1].Init(491, 370, 'R', &map[30], 'O');

	// platform
	level31_platform.fill(Platform());
	level31_platform[0].Init(265, 280, 222, 'L', 'B', &map[30]);
	level31_platform[0].Bind(&level31_switch[0]);

	level31_platform[1].Init(344, 360, 389, 'R', 'O', &map[30]);
	level31_platform[1].Bind(&level31_switch[1]);

	// door
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

	// platform
	for (auto & platform : level31_platform)
		platform.Reset();
}