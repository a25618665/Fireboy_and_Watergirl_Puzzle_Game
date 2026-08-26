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
	// Load字體
	AddFontResourceA(FONTS);

	level = 0;
	sub_phase = 0;
	blue_diamond_counter = 0;
	red_diamond_counter = 0;
	green_counter = 0;
	time_counter = 0;
	time_counter_start = 0;
	time_counter_flag = false;
	cheat_flag = false;

	LoadMap();
	ShowInitProgress(90, "Load Level");
	LoadSound();
	LoadSelectPage();
	LoadSubPhase(); 
	LoadLevel1();
	LoadLevel6();
	LoadLevel10();
	LoadLevel11();
	LoadLevel13();
	LoadLevel15();
	LoadLevel16();
	LoadLevel17();
	LoadLevel18();
	LoadLevel23();
	LoadLevel31();

	// 各關鑽石數量
	for (auto & map : num_diamonds_each_level)
		map["g_diamond"] = 0;

	num_diamonds_each_level[0]["red_diamond"] = level1_red_diamond.size();
	num_diamonds_each_level[0]["blue_diamond"] = level1_blue_diamond.size();
	num_diamonds_each_level[5]["red_diamond"] = level6_red_diamond.size();
	num_diamonds_each_level[5]["blue_diamond"] = level6_blue_diamond.size();
	num_diamonds_each_level[9]["red_diamond"] = level10_red_diamond.size();
	num_diamonds_each_level[9]["blue_diamond"] = level10_blue_diamond.size();
	num_diamonds_each_level[10]["g_diamond"] = 1;
	num_diamonds_each_level[12]["g_diamond"] = 1;
	num_diamonds_each_level[14]["red_diamond"] = level15_red_diamond.size();
	num_diamonds_each_level[14]["blue_diamond"] = level15_blue_diamond.size();
	num_diamonds_each_level[15]["red_diamond"] = level16_red_diamond.size();
	num_diamonds_each_level[15]["blue_diamond"] = level16_blue_diamond.size();
	num_diamonds_each_level[16]["g_diamond"] = 1;
	num_diamonds_each_level[17]["red_diamond"] = level18_red_diamond.size();
	num_diamonds_each_level[17]["blue_diamond"] = level18_blue_diamond.size();
	num_diamonds_each_level[22]["g_diamond"] = 1;
	num_diamonds_each_level[30]["red_diamond"] = level31_red_diamond.size();
	num_diamonds_each_level[30]["blue_diamond"] = level31_blue_diamond.size();
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
		{
			boy[level - 1].Jump();
			CAudio::Instance()->Play(A_JUMP_B);
		}

		// girl
		if (nChar == 0x44)				// D鍵
			girl[level - 1].SetMovingRight(true);
		else if (nChar == 0x41)			// A鍵
			girl[level - 1].SetMovingLeft(true);

		if (nChar == 0x57)				// W鍵
		{
			girl[level - 1].Jump();
			CAudio::Instance()->Play(A_JUMP_G);
		}

		// 按下M鍵回到關卡選擇頁面
		if (nChar == 0x4D)
		{
			CAudio::Instance()->Stop(A_PLAY);
			CAudio::Instance()->Play(A_PUSHER);
			CAudio::Instance()->Play(A_MENU, true);
			ResetLevel(level);
			sub_phase = 0;
			level = 0;
		}

		// 作弊按鈕: 按下N鍵，碰到水不會死
		if (nChar == 0x4E)
		{
			CAudio::Instance()->Play(A_PUSHER);
			cheat_flag = !cheat_flag;
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
		// main menu button
		if (menu_button_body.PtInRect(point))
		{
			CAudio::Instance()->Play(A_PUSHER);
			menu_button_down = true;
		}

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
		{
			CAudio::Instance()->Play(A_PUSHER);
			is_sub1_retry_clicked = true;
		}
		else if (sub1_back_body.PtInRect(point))
		{
			CAudio::Instance()->Play(A_PUSHER);
			is_sub1_back_clicked = true;
		}
	}
	else if (sub_phase == 2)
	{
		if (sub2_con_body.PtInRect(point))
		{
			CAudio::Instance()->Play(A_PUSHER);
			is_sub2_con_clicked = true;
		}
	}
	else if (sub_phase == 3)
	{
		if (sub3_retry_body.PtInRect(point))
		{
			CAudio::Instance()->Play(A_PUSHER);
			is_sub3_retry_clicked = true;
		}
		else if (sub3_back_body.PtInRect(point))
		{
			CAudio::Instance()->Play(A_PUSHER);
			is_sub3_back_clicked = true;
		}
	}
}

void CGameStateRun::OnLButtonUp(UINT nFlags, CPoint point)	// 處理滑鼠的動作
{
	if (level == 0) {
		if (select_page_button_down != 0)
		{
			select_page_diamond[select_page_button_down - 1].OnButtonUp();
			level = select_page_button_down;
		}
		else if (menu_button_down)
		{
			menu_button_down = false;
			GotoGameState(GAME_STATE_INIT);
		}
	}
	else if (sub_phase == 1)
	{
		if (is_sub1_retry_clicked)
		{
			CAudio::Instance()->Play(A_PLAY, true);
			is_sub1_retry_clicked = false;
			ResetLevel(level);
			sub_phase = 0;
		}
		else if (is_sub1_back_clicked)
		{
			CAudio::Instance()->Play(A_MENU, true);
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
			CAudio::Instance()->Play(A_MENU, true);
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
			CAudio::Instance()->Play(A_PLAY, true);
			is_sub3_retry_clicked = false;
			ResetLevel(level);
			sub_phase = 0;
		}
		else if (is_sub3_back_clicked)
		{
			CAudio::Instance()->Play(A_MENU, true);
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
		case 11:
			Level11OnMove();
			break;
		case 13:
			Level13OnMove();
			break;
		case 15:
			Level15OnMove();
			break;
		case 16:
			Level16OnMove();
			break;
		case 17:
			Level17OnMove();
			break;
		case 18:
			Level18OnMove();
			break;
		case 23:
			Level23OnMove();
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
		if (num_diamonds_each_level[level - 1]["g_diamond"])
			img_sub2_g_diamond.ShowBitmap();
		SubPhase2ShowText();
		break;
	case 3:
		img_sub2_bg.ShowBitmap();
		if (num_diamonds_each_level[level - 1]["g_diamond"])
			img_sub3_g_diamond.ShowBitmap();
		SubPhase3ShowText();
		break;
	case 0:
		switch (level)
		{
		case 0:
			select_page_bg.ShowBitmap();
			if (!menu_button_down)
				menu_button.ShowBitmap();

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
		case 11:
			Level11OnShow();
			break;
		case 13:
			Level13OnShow();
			break;
		case 15:
			Level15OnShow();
			break;
		case 16:
			Level16OnShow();
			break;
		case 17:
			Level17OnShow();
			break;
		case 18:
			Level18OnShow();
			break;
		case 23:
			Level23OnShow();
			break;
		case 31:
			Level31OnShow();
			break;
		}
		// cheat text show
		if (cheat_flag)
			CheatShowText();

		break;
	}
}

void CGameStateRun::ResetLevel(int level)
{
	red_diamond_counter = 0;
	blue_diamond_counter = 0;
	green_counter = 0;

	time_counter = 0;
	time_counter_start = 0;
	time_counter_flag = false;
	cheat_flag = false;

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
	case 11:
		ResetL11();
		break;
	case 13:
		ResetL13();
		break;
	case 15:
		ResetL15();
		break;
	case 16:
		ResetL16();
		break;
	case 17:
		ResetL17();
		break;
	case 18:
		ResetL18();
		break;
	case 23:
		ResetL23();
		break;
	case 31:
		ResetL31();
		break;
	}
}

void CGameStateRun::LoadMap()
{
	array<int, 11> level = { 1, 6, 10, 11, 13, 15, 16, 17, 18, 23, 31 };

	for (int i = 0; i < signed(level.size()); i++)
	{
		string s = "";
		ShowInitProgress(2 + i * 8, s + "Load Map " + std::to_string(level[i]));

		string map_path = "";
		ifstream ifs(map_path + MAP_TEMPLATE + to_string(level[i]) + ".map");
		for (int j = 0; j < 640; j++)
		{
			for (int k = 0; k < 480; k++)
			{
				ifs >> map[level[i] - 1][j][k];
			}
		}
		ifs.close();
	}
}

void CGameStateRun::LoadSound()
{
	CAudio* audio = CAudio::Instance();
	audio->Load(A_PUSHER, "Resources/sounds/Pusher.mp3");
	audio->Load(A_DIAMOND, "Resources/sounds/Diamond.mp3");
	audio->Load(A_JUMP_B, "Resources/sounds/Jump_boy.wav");
	audio->Load(A_JUMP_G, "Resources/sounds/Jump_girl.wav");
	audio->Load(A_PLAT, "Resources/sounds/Plat.wav");
	audio->Load(A_DIE, "Resources/sounds/Die.mp3");
	audio->Load(A_SWITCH, "Resources/sounds/Switch.mp3");
	audio->Load(A_MENU, "Resources/sounds/Menu.mp3");
	audio->Load(A_PLAY, "Resources/sounds/Play.mp3");
	audio->Load(A_FINISH, "Resources/sounds/Finish.mp3");
	audio->Load(A_FAIL, "Resources/sounds/Fail.mp3");
	audio->Load(A_DOOR_B, "Resources/sounds/Door.wav");
	audio->Load(A_DOOR_R, "Resources/sounds/Door.wav");
}

void CGameStateRun::LoadSelectPage() 
{
	select_page_button_down = 0;
	menu_button_down = false;
	
	select_page_bg.LoadBitmapByString({NEW_SELECT_PAGE_BG});
	select_page_bg.SetTopLeft(0, 0);

	menu_button.LoadBitmapByString({MENU_BUTTON});
	menu_button.SetTopLeft(0, 420);

	menu_button_body = menu_button.GetLocation();

	select_page_diamond.fill(SelectPageDiamond());
	select_page_diamond[0].Init(1, 168, 188, 'B');
	select_page_diamond[5].Init(6, 139, 304, 'O');
	select_page_diamond[9].Init(10, 233, 161, 'B');
	select_page_diamond[10].Init(11, 273, 319, 'G');
	select_page_diamond[12].Init(13, 350, 285, 'G', 35, -24);
	select_page_diamond[14].Init(15, 296, 128, 'B', 38, -15);
	select_page_diamond[15].Init(16, 206, 286, 'O', 32, -16);
	select_page_diamond[16].Init(17, 426, 319, 'G');
	select_page_diamond[17].Init(18, 379, 156, 'B');
	select_page_diamond[22].Init(23, 492, 291, 'G');
	select_page_diamond[30].Init(31, 442, 128, 'B');
    
	/*select_page_diamond.fill( SelectPageDiamond() );
	select_page_diamond[0].Init(1, 302, 440, 'B');
	select_page_diamond[5].Init(6, 145, 274, 'O', 2, 34);
	select_page_diamond[9].Init(10, 85, 252, 'B', -24, 35);
	select_page_diamond[10].Init(11, 23, 246, 'G', -13, -30);
	select_page_diamond[12].Init(13, 26, 153, 'G', 47, -7);
	select_page_diamond[14].Init(15, 242, 144, 'B', -67, 19);
	select_page_diamond[15].Init(16, 188, 96, 'O', 35, -18);
	select_page_diamond[16].Init(17, 101, 71, 'G', 47, -18);
	select_page_diamond[17].Init(18, 317, 121, 'B', 36, -2);
	select_page_diamond[22].Init(23, 505, 55, 'G');
	select_page_diamond[30].Init(31, 485, 279, 'B', 35, -9);*/
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

	img_sub2_g_diamond.LoadBitmapByString({ GREEN_PASS });
	img_sub2_g_diamond.SetTopLeft(250, 170);

	img_sub2_con_clicked.LoadBitmapByString({ SUBPHASE_2_CONTINUE });
	img_sub2_con_clicked.SetTopLeft(263, 379);

	sub2_con_body.SetRect(263, 328, 397, 348);
	is_sub2_con_clicked = false;

	// sub phase 3
	img_sub3_g_diamond.LoadBitmapByString({ GREEN_FAIL });
	img_sub3_g_diamond.SetTopLeft(250, 170);

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
	CTextDraw::ChangeFontLog(pDC, 20, "Cinzel", RGB(255, 218, 0), 800);

	CTextDraw::Print(pDC, 250, 142, "TIME   " + time_str);

	if (!num_diamonds_each_level[level - 1]["g_diamond"])
	{
		CTextDraw::Print(pDC, 320, 180, "X " + to_string(blue_diamond_counter) + "/" + to_string(num_diamonds_each_level[level - 1]["blue_diamond"]));
		CTextDraw::Print(pDC, 320, 212, "X " + to_string(red_diamond_counter) + "/" + to_string(num_diamonds_each_level[level - 1]["red_diamond"]));
	}

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
	CTextDraw::ChangeFontLog(pDC, 20, "Cinzel", RGB(255, 218, 0), 800);

	CTextDraw::Print(pDC, 250, 142, "TIME   " + time_str);

	if (!num_diamonds_each_level[level - 1]["g_diamond"])
	{
		CTextDraw::Print(pDC, 320, 180, "X " + to_string(blue_diamond_counter) + "/" + to_string(num_diamonds_each_level[level - 1]["blue_diamond"]));
		CTextDraw::Print(pDC, 320, 212, "X " + to_string(red_diamond_counter) + "/" + to_string(num_diamonds_each_level[level - 1]["red_diamond"]));
	}

	CTextDraw::Print(pDC, 300, 257, "FAIL");

	if (!is_sub3_retry_clicked)
		CTextDraw::Print(pDC, 254, 300, "RETRY LEVEL");

	if (!is_sub3_back_clicked)
		CTextDraw::Print(pDC, 237, 334, "BACK TO MENU");

	CDDraw::ReleaseBackCDC();
}

void CGameStateRun::CheatShowText()
{
	CDC *pDC = CDDraw::GetBackCDC();
	CTextDraw::ChangeFontLog(pDC, 20, "Cinzel", RGB(255, 218, 0), 800);
	CTextDraw::Print(pDC, 100, 1, "Cheat Mode");
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
	if (!cheat_flag)
	{
		for (auto & water : level1_water)
		{
			sub_phase = water.OnMove(boy_body, girl_body);
			if (sub_phase)
				break;
		}
	}

	// door
	bool door0_is_triggered = level1_door[0].OnMove(boy_body, girl_body);
	bool door1_is_triggered = level1_door[1].OnMove(boy_body, girl_body);
	if (door0_is_triggered && door1_is_triggered)
	{
		if (red_diamond_counter == num_diamonds_each_level[0]["red_diamond"] &&
			blue_diamond_counter == num_diamonds_each_level[0]["blue_diamond"])	// 破關
		{
			CAudio::Instance()->Stop(A_PLAY);
			CAudio::Instance()->Play(A_FINISH);
			sub_phase = 2;
		}
		else				// 寶石沒吃完
		{
			CAudio::Instance()->Stop(A_PLAY);
			CAudio::Instance()->Play(A_FAIL);
			sub_phase = 3;
		}
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
	if (!cheat_flag)
	{
		for (auto & water : level6_water)
		{
			sub_phase = water.OnMove(boy_body, girl_body);
			if (sub_phase)
				break;
		}
	}

	// door
	bool door0_is_triggered = level6_door[0].OnMove(boy_body, girl_body);
	bool door1_is_triggered = level6_door[1].OnMove(boy_body, girl_body);
	if (door0_is_triggered && door1_is_triggered)
	{
		if (red_diamond_counter == num_diamonds_each_level[5]["red_diamond"] &&
			blue_diamond_counter == num_diamonds_each_level[5]["blue_diamond"])	// 破關
		{
			CAudio::Instance()->Stop(A_PLAY);
			CAudio::Instance()->Play(A_FINISH);
			sub_phase = 2;
		}
		else				// 寶石沒吃完
		{
			CAudio::Instance()->Stop(A_PLAY);
			CAudio::Instance()->Play(A_FAIL);
			sub_phase = 3;
		}
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
	level10_red_diamond[0].Init(150, 40);
	level10_red_diamond[1].Init(391, 85);
	level10_red_diamond[2].Init(583, 53);
	level10_red_diamond[3].Init(375, 183);
	level10_red_diamond[4].Init(212, 246);
	level10_red_diamond[5].Init(212, 355);

	level10_blue_diamond.fill(Diamond("blue"));
	level10_blue_diamond[0].Init(227, 87);
	level10_blue_diamond[1].Init(486, 120);
	level10_blue_diamond[2].Init(117, 170);
	level10_blue_diamond[3].Init(552, 218);
	level10_blue_diamond[4].Init(422, 295);
	level10_blue_diamond[5].Init(470, 392);

	// switch
	level10_switch.fill(Switch());
	level10_switch[0].Init(558, 85, 'R', &map[9], 'Y');
	level10_switch[1].Init(155, 182, 'L', &map[9], 'B');

	// platform
	level10_platform.fill(Platform());
	level10_platform[0].Init(200, 30, 90, 'D', 'Y', &map[9],'V');
	level10_platform[0].Bind(&level10_switch[0]);

	level10_platform[1].Init(522, 155, 568, 'R', 'B', &map[9],'H');
	level10_platform[1].Bind(&level10_switch[1]);

	//door
	level10_door.fill(Door());
	level10_door[0].Init(575, 275, 'R');
	level10_door[1].Init(22, 354, 'B');

	// water
	level10_water.fill(Water());
	level10_water[0].Init(104, 70, 179, 78, 'R');
	level10_water[1].Init(346, 119, 387, 127, 'R');
	level10_water[2].Init(409, 118, 435, 126, 'R');
	level10_water[3].Init(139, 121, 241, 127, 'B');
	level10_water[4].Init(316, 233, 339, 238, 'B');
	level10_water[5].Init(526, 249, 578, 255, 'B');
	level10_water[6].Init(156, 280, 195, 285, 'B');
	level10_water[7].Init(106, 280, 130, 287, 'B');
	level10_water[8].Init(235, 280, 274, 286, 'B');
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
	if (!cheat_flag)
	{
		for (auto & water : level10_water)
		{
			sub_phase = water.OnMove(boy_body, girl_body);
			if (sub_phase)
				break;
		}
	}

	// door
	bool door0_is_triggered = level10_door[0].OnMove(boy_body, girl_body);
	bool door1_is_triggered = level10_door[1].OnMove(boy_body, girl_body);
	if (door0_is_triggered && door1_is_triggered)
	{
		if (red_diamond_counter == num_diamonds_each_level[9]["red_diamond"] &&
			blue_diamond_counter == num_diamonds_each_level[9]["blue_diamond"])	// 破關
		{
			CAudio::Instance()->Stop(A_PLAY);
			CAudio::Instance()->Play(A_FINISH);
			sub_phase = 2;
		}
		else				// 寶石沒吃完
		{
			CAudio::Instance()->Stop(A_PLAY);
			CAudio::Instance()->Play(A_FAIL);
			sub_phase = 3;
		}
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

//level 11
void CGameStateRun::LoadLevel11()
{
	// person
	boy[10].Init(490, 417, "boy");
	girl[10].Init(295, 422, "girl");
	boy[10].SetMap(&map[10]);
	girl[10].SetMap(&map[10]);

	// background
	level11_bg.LoadBitmapByString({ LEVEL_11_BG });
	level11_bg.SetTopLeft(0, 0);

	// diamond 
	level11_green_diamond.fill(Diamond("green"));
	level11_green_diamond[0].Init(70, 315);

	// switch init 
	level11_switch.fill(Switch());
	level11_switch[0].Init(389, 325, 'R', &map[10], 'B');
	level11_switch[1].Init(40, 435, 'R', &map[10], 'G');
	
	// button init 
	level11_button.fill(Button());
	level11_button[0].Init(251, 267, 'P');
	
	// platform init
	level11_platform.fill(Platform());
	level11_platform[0].Init(442, 247, 394, 'L', 'P', &map[10]);
	level11_platform[0].Bind(&level11_button[0]);
	level11_platform[1].Init(73, 246, 202, 'U', 'G', &map[10], 'V');
	level11_platform[1].Bind(&level11_switch[1]);
	level11_platform[2].Init(440, 374, 422, 'D', 'B', &map[10], 'V');
	level11_platform[2].Bind(&level11_switch[0]);
	
	// water
	level11_water.fill(Water());
	level11_water[0].Init(308, 279, 358, 285, 'B');
	level11_water[1].Init(175, 358, 230, 367, 'B');
	level11_water[2].Init(432, 340, 460, 349, 'R');
	level11_water[3].Init(110, 436, 170, 445, 'R');

	// door init 
	level11_door.fill(Door());
	level11_door[0].Init(551, 403, 'B');
	level11_door[1].Init(214, 403, 'R');
}

void CGameStateRun::Level11OnMove()
{
	boy[10].OnMove();
	girl[10].OnMove();

	CRect boy_body = boy[10].GetBody();
	CRect girl_body = girl[10].GetBody();

	// diamond
	for (auto & diamond : level11_green_diamond)
		diamond.OnMove(boy_body, girl_body, green_counter);

	// switch
	for (auto & s : level11_switch)
		s.OnMove(boy_body, girl_body);

	// button
	for (auto & button : level11_button)
		button.OnMove(boy_body, girl_body);

	// platform
	for (auto & platform : level11_platform)
		platform.OnMove();

	// water
	if (!cheat_flag)
	{
		for (auto & water : level11_water)
		{
			sub_phase = water.OnMove(boy_body, girl_body);
			if (sub_phase)
				break;
		}
	}

	// door
	bool door0_is_triggered = level11_door[0].OnMove(boy_body, girl_body);
	bool door1_is_triggered = level11_door[1].OnMove(boy_body, girl_body);
	if (door0_is_triggered && door1_is_triggered)
	{
		if (green_counter == num_diamonds_each_level[10]["g_diamond"])	// 破關
		{
			CAudio::Instance()->Stop(A_PLAY);
			CAudio::Instance()->Play(A_FINISH);
			sub_phase = 2;
		}
		else				// 寶石沒吃完
		{
			CAudio::Instance()->Stop(A_PLAY);
			CAudio::Instance()->Play(A_FAIL);
			sub_phase = 3;
		}
	}
}

void CGameStateRun::Level11OnShow()
{
	// background
	level11_bg.ShowBitmap();

	// timer
	time_counter = timeGetTime() - time_counter_start;
	timer_showtext::show_in_the_game(time_counter);

	// diamond
	for (auto & diamond : level11_green_diamond)
		diamond.OnShow();

	// switch
	for (auto & s : level11_switch)
		s.OnShow();

	// button
	for (auto & button : level11_button)
		button.OnShow();

	// platform
	for (auto & platform : level11_platform)
		platform.OnShow();

	// door
	for (auto & door : level11_door)
		door.OnShow();

	// person
	boy[10].OnShow();
	girl[10].OnShow();
}

void CGameStateRun::ResetL11()
{
	boy[10].Reset();
	girl[10].Reset();

	// diamond
	for (auto & diamond : level11_green_diamond)
		diamond.Reset();

	// switch
	for (auto & s : level11_switch)
		s.Reset();

	// button
	for (auto & button : level11_button)
		button.Reset();

	// platform
	for (auto & platform : level11_platform)
		platform.Reset();
}

// Level 13
void CGameStateRun::LoadLevel13()
{
	// person
	boy[12].Init(52, 269, "boy");
	girl[12].Init(84, 275, "girl");
	boy[12].SetMap(&map[12]);
	girl[12].SetMap(&map[12]);

	// background
	level13_bg.LoadBitmapByString({LEVEL_13_BG});
	level13_bg.SetTopLeft(0, 0);

	// diamond 
	level13_green_diamond.fill(Diamond("green"));
	level13_green_diamond[0].Init(355, 42);

	// switch init 
	level13_switch.fill(Switch());
	level13_switch[0].Init(254, 373, 'R', &map[12], 'W');
	level13_switch[1].Init(45, 437, 'R', &map[12], 'B');
	level13_switch[2].Init(173, 437, 'R', &map[12], 'O');
	level13_switch[3].Init(429, 437, 'R', &map[12], 'G');
	level13_switch[4].Init(557, 437, 'R', &map[12], 'P');

	// button init 
	level13_button.fill(Button());
	level13_button[0].Init(259, 125, 'Y');
	level13_button[1].Init(403, 380, 'Y');

	// platform init 
	vector<Button *> temp_button_ptr_vector;
	for (auto & button : level13_button)
		temp_button_ptr_vector.push_back(&button);

	level13_platform.fill(Platform());
	level13_platform[0].Init(41, 56, 72, 'R', 'B', &map[12]);
	level13_platform[0].Bind(&level13_switch[1]);
	level13_platform[1].Init(153, 56, 121, 'L', 'O', &map[12]);
	level13_platform[1].Bind(&level13_switch[2]);
	level13_platform[2].Init(247, 88, 293, 'R', 'W', &map[12]);
	level13_platform[2].Bind(&level13_switch[0]);
	level13_platform[3].Init(424, 56, 456, 'R', 'G', &map[12]);
	level13_platform[3].Bind(&level13_switch[3]);
	level13_platform[4].Init(537, 56, 506, 'L', 'P', &map[12]);
	level13_platform[4].Bind(&level13_switch[4]);
	level13_platform[5].Init(439, 327, 264, 'U', 'Y', &map[12], 'V', 'L');
	level13_platform[5].Bind(temp_button_ptr_vector);

	// rock
	level13_rock.fill(Rock()); 
	level13_rock[0].Init(41, 26, &map[12]);
	level13_rock[1].Init(183, 27, &map[12]);
	level13_rock[2].Init(423, 26, &map[12]);
	level13_rock[3].Init(568, 27, &map[12]);

	// door init 
	level13_door.fill(Door());
	level13_door[0].Init(31, 339, 'R');
	level13_door[1].Init(79, 338, 'B');
}

void CGameStateRun::Level13OnMove()
{
	boy[12].OnMove();
	girl[12].OnMove();

	CRect boy_body = boy[12].GetBody();
	CRect girl_body = girl[12].GetBody();

	// diamond
	for (auto & diamond : level13_green_diamond)
		diamond.OnMove(boy_body, girl_body, green_counter);

	// switch
	for (auto & s : level13_switch)
		s.OnMove(boy_body, girl_body);

	// button
	for (auto & button : level13_button)
		button.OnMove(boy_body, girl_body);

	// platform
	for (auto & platform : level13_platform)
		platform.OnMove();

	// rock
	for (auto & rock : level13_rock)
		rock.OnMove(boy_body, girl_body);

	// door
	bool door0_is_triggered = level13_door[0].OnMove(boy_body, girl_body);
	bool door1_is_triggered = level13_door[1].OnMove(boy_body, girl_body);
	if (door0_is_triggered && door1_is_triggered)
	{
		if (green_counter == num_diamonds_each_level[12]["g_diamond"] )	// 破關
		{
			CAudio::Instance()->Stop(A_PLAY);
			CAudio::Instance()->Play(A_FINISH);
			sub_phase = 2;
		}
		else				// 寶石沒吃完
		{
			CAudio::Instance()->Stop(A_PLAY);
			CAudio::Instance()->Play(A_FAIL);
			sub_phase = 3;
		}
	}
}

void CGameStateRun::Level13OnShow()
{
	// background
	level13_bg.ShowBitmap();

	// timer
	time_counter = timeGetTime() - time_counter_start;
	timer_showtext::show_in_the_game(time_counter);

	// diamond
	for (auto & diamond : level13_green_diamond)
		diamond.OnShow();

	// switch
	for (auto & s : level13_switch)
		s.OnShow();

	// button
	for (auto & button : level13_button)
		button.OnShow();

	// platform
	for (auto & platform : level13_platform)
		platform.OnShow();

	// door
	for (auto & door : level13_door)
		door.OnShow();

	// rock
	for (auto & rock : level13_rock)
		rock.OnShow();

	// person
	boy[12].OnShow();
	girl[12].OnShow();
}

void CGameStateRun::ResetL13()
{
	boy[12].Reset();
	girl[12].Reset();

	// diamond
	for (auto & diamond : level13_green_diamond)
		diamond.Reset();

	// switch
	for (auto & s : level13_switch)
		s.Reset();

	// button
	for (auto & button : level13_button)
		button.Reset();

	// platform
	for (auto & platform : level13_platform)
		platform.Reset();

	// rock
	for (auto & rock : level13_rock)
		rock.Reset();
}

// Level 15
void CGameStateRun::LoadLevel15()
{
	// person
	boy[14].Init(500, 349, "boy");
	girl[14].Init(452, 355, "girl");
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
	level15_switch[0].Init(189, 180, 'R', &map[14], 'Y');
	level15_switch[1].Init(61, 373, 'R', &map[14], 'W');
	level15_switch[2].Init(61, 436, 'R', &map[14], 'G');
	level15_switch[3].Init(558, 437, 'L', &map[14], 'P');

	// button 
	level15_button.fill(Button());
	level15_button[0].Init(466, 61, 'P');
	level15_button[1].Init(482, 250, 'O');
	level15_button[2].Init(50, 315, 'B');
	level15_button[3].Init(273, 315, 'O');

	// platform
	vector<Button *> temp_button_ptr_vector;
	temp_button_ptr_vector.push_back(&level15_button[1]);
	temp_button_ptr_vector.push_back(&level15_button[3]);

	level15_platform.fill(Platform());
	level15_platform[0].Init(330, 73, 265, 'L', 'Y', &map[14], 'H');
	level15_platform[0].Bind(&level15_switch[0]);
	level15_platform[1].Init(530, 136, 461, 'L', 'P', &map[14], 'H');
	level15_platform[1].Bind(&level15_button[0]);
	level15_platform[2].Init(257, 200, 191, 'L', 'O', &map[14],'H');
	level15_platform[2].Bind(temp_button_ptr_vector);
	level15_platform[3].Init(312, 220, 170, 'U', 'B', &map[14],'V');
	level15_platform[3].Bind(&level15_button[2]);
	level15_platform[4].Init(48, 265, 110, 'R', 'O', &map[14],'H');
	level15_platform[4].Bind(temp_button_ptr_vector);
	level15_platform[5].Init(312, 282, 328, 'D', 'G', &map[14],'V');
	level15_platform[5].Bind(&level15_switch[2]);
	level15_platform[6].Init(530, 330, 466, 'L', 'W', &map[14],'H');
	level15_platform[6].Bind(&level15_switch[1]);
	level15_platform[7].Init(250, 407, 314, 'R', 'P', &map[14],'V');
	level15_platform[7].Bind(&level15_switch[3]);
	level15_platform[8].Init(314, 409, 379, 'R', 'P', &map[14],'V');
	level15_platform[8].Bind(&level15_switch[3]);

	//door
	level15_door.fill(Door());
	level15_door[0].Init(40, 20, 'R');
	level15_door[1].Init(105, 20, 'B');
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
		{
			CAudio::Instance()->Stop(A_PLAY);
			CAudio::Instance()->Play(A_FINISH);
			sub_phase = 2;
		}
		else				// 寶石沒吃完
		{
			CAudio::Instance()->Stop(A_PLAY);
			CAudio::Instance()->Play(A_FAIL);
			sub_phase = 3;
		}
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
	level16_switch[0].Init(349, 291, 'L', &map[15], 'B');
	level16_switch[1].Init(252, 276, 'L', &map[15], 'Y');

	// platform
	level16_platform.fill(Platform());
	level16_platform[0].Init(185, 297, 350, 'D', 'B', &map[15]);
	level16_platform[0].Bind(&level16_switch[0]);

	level16_platform[1].Init(180, 375, 430, 'D', 'Y', &map[15]);
	level16_platform[1].Bind(&level16_switch[1]);

	//door
	level16_door.fill(Door());
	level16_door[0].Init(567, 335, 'R');
	level16_door[1].Init(566, 403, 'B');

	// water
	level16_water.fill(Water());
	level16_water[0].Init(175, 358, 243, 365, 'B');
	level16_water[1].Init(175, 438, 243, 445, 'R');
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
	for (auto & s : level16_switch)
		s.OnMove(boy_body, girl_body);

	// platform
	for (auto & platform : level16_platform)
		platform.OnMove();

	// water
	if (!cheat_flag)
	{
		for (auto & water : level16_water)
		{
			sub_phase = water.OnMove(boy_body, girl_body);
			if (sub_phase)
				break;
		}
	}

	// door
	bool door0_is_triggered = level16_door[0].OnMove(boy_body, girl_body);
	bool door1_is_triggered = level16_door[1].OnMove(boy_body, girl_body);
	if (door0_is_triggered && door1_is_triggered)
	{
		if (red_diamond_counter == num_diamonds_each_level[15]["red_diamond"] &&
			blue_diamond_counter == num_diamonds_each_level[15]["blue_diamond"])	// 破關
		{
			CAudio::Instance()->Stop(A_PLAY);
			CAudio::Instance()->Play(A_FINISH);
			sub_phase = 2;
		}
		else				// 寶石沒吃完
		{
			CAudio::Instance()->Stop(A_PLAY);
			CAudio::Instance()->Play(A_FAIL);
			sub_phase = 3;
		}
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

// Level 17
void CGameStateRun::LoadLevel17()
{
	// person
	boy[16].Init(68, 413, "boy");
	girl[16].Init(516, 35, "girl");
	boy[16].SetMap(&map[16]);
	girl[16].SetMap(&map[16]);

	// background
	level17_bg.LoadBitmapByString({ LEVEL_17_BG });
	level17_bg.SetTopLeft(0, 0);

	// diamond 
	level17_green_diamond.fill(Diamond("green"));
	level17_green_diamond[0].Init(32, 108);

	// switch init 
	level17_switch.fill(Switch());
	level17_switch[0].Init(430, 85, 'R', &map[16], 'W');
	level17_switch[1].Init(144, 116, 'R', &map[16], 'B');
	level17_switch[2].Init(556, 311, 'L', &map[16], 'O');
	level17_switch[3].Init(360, 65, 'R', &map[16], 'G');
	level17_switch[4].Init(285, 87, 'R', &map[16], 'P');

	// platform init
	level17_platform.fill(Platform());
	level17_platform[0].Init(422, 201, 359, 'L', 'W', &map[16]);
	level17_platform[0].Bind(&level17_switch[0]);
	level17_platform[1].Init(361, 331, 294, 'L', 'W', &map[16]);
	level17_platform[1].Bind(&level17_switch[0]);
	level17_platform[2].Init(200, 260, 200, 'U', 'W', &map[16]);
	level17_platform[2].Bind(&level17_switch[0]);
	level17_platform[3].Init(200, 194, 139, 'U', 'W', &map[16]);
	level17_platform[3].Bind(&level17_switch[0]);
	level17_platform[4].Init(74, 136, 195, 'D', 'G', &map[16]);
	level17_platform[4].Bind(&level17_switch[3]);
	level17_platform[5].Init(246, 282, 180, 'L', 'G', &map[16],'V');
	level17_platform[5].Bind(&level17_switch[3]);
	level17_platform[6].Init(487, 323, 266, 'U', 'G', &map[16]);
	level17_platform[6].Bind(&level17_switch[3]);
	level17_platform[7].Init(474, 215, 338, 'L', 'P', &map[16],'V');
	level17_platform[7].Bind(&level17_switch[4]);
	level17_platform[8].Init(86, 394, 155, 'R', 'P', &map[16]);
	level17_platform[8].Bind(&level17_switch[4]);
	level17_platform[9].Init(567, 138, 520, 'L', 'O', &map[16]);
	level17_platform[9].Bind(&level17_switch[2]);
	level17_platform[10].Init(247, 24, 73, 'D', 'B', &map[16],'V');
	level17_platform[10].Bind(&level17_switch[1]);

	// door init 
	level17_door.fill(Door());
	level17_door[0].Init(43, 18, 'R');
	level17_door[1].Init(90, 18, 'B');
}

void CGameStateRun::Level17OnMove()
{
	boy[16].OnMove();
	girl[16].OnMove();

	CRect boy_body = boy[16].GetBody();
	CRect girl_body = girl[16].GetBody();

	// diamond
	for (auto & diamond : level17_green_diamond)
		diamond.OnMove(boy_body, girl_body, green_counter);

	// switch
	for (auto & s : level17_switch)
		s.OnMove(boy_body, girl_body);

	// platform
	for (auto & platform : level17_platform)
		platform.OnMove();

	// door
	bool door0_is_triggered = level17_door[0].OnMove(boy_body, girl_body);
	bool door1_is_triggered = level17_door[1].OnMove(boy_body, girl_body);
	if (door0_is_triggered && door1_is_triggered)
	{
		if (green_counter == num_diamonds_each_level[16]["g_diamond"])	// 破關
		{
			CAudio::Instance()->Stop(A_PLAY);
			CAudio::Instance()->Play(A_FINISH);
			sub_phase = 2;
		}
		else				// 寶石沒吃完
		{
			CAudio::Instance()->Stop(A_PLAY);
			CAudio::Instance()->Play(A_FAIL);
			sub_phase = 3;
		}
	}
}

void CGameStateRun::Level17OnShow()
{
	// background
	level17_bg.ShowBitmap();

	// timer
	time_counter = timeGetTime() - time_counter_start;
	timer_showtext::show_in_the_game(time_counter);

	// diamond
	for (auto & diamond : level17_green_diamond)
		diamond.OnShow();

	// switch
	for (auto & s : level17_switch)
		s.OnShow();

	// platform
	for (auto & platform : level17_platform)
		platform.OnShow();

	// door
	for (auto & door : level17_door)
		door.OnShow();

	// person
	boy[16].OnShow();
	girl[16].OnShow();
}

void CGameStateRun::ResetL17()
{
	boy[16].Reset();
	girl[16].Reset();

	// diamond
	for (auto & diamond : level17_green_diamond)
		diamond.Reset();

	// switch
	for (auto & s : level17_switch)
		s.Reset();

	// platform
	for (auto & platform : level17_platform)
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
	level18_platform[1].Init(457, 199, 153, 'U', 'G', &map[17], 'V');
	level18_platform[2].Init(103, 391, 327, 'U', 'B', &map[17]);
	level18_platform[3].Init(232, 310, 252, 'U', 'W', &map[17], 'V', 'L');
	level18_platform[4].Init(359, 310, 250, 'U', 'W', &map[17], 'V', 'L');

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
	if (!cheat_flag)
	{
		for (auto & water : level18_water)
		{
			sub_phase = water.OnMove(boy_body, girl_body);
			if (sub_phase)
				break;
		}
	}

	// door
	bool door0_is_triggered = level18_door[0].OnMove(boy_body, girl_body);
	bool door1_is_triggered = level18_door[1].OnMove(boy_body, girl_body);
	if (door0_is_triggered && door1_is_triggered)
	{
		if (red_diamond_counter == num_diamonds_each_level[17]["red_diamond"] &&
			blue_diamond_counter == num_diamonds_each_level[17]["blue_diamond"])	// 破關
		{
			CAudio::Instance()->Stop(A_PLAY);
			CAudio::Instance()->Play(A_FINISH);
			sub_phase = 2;
		}
		else				// 寶石沒吃完
		{
			CAudio::Instance()->Stop(A_PLAY);
			CAudio::Instance()->Play(A_FAIL);
			sub_phase = 3;
		}
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

// Level 23
void CGameStateRun::LoadLevel23()
{
	// person
	boy[22].Init(100, 28, "boy");
	girl[22].Init(132, 34, "girl");
	boy[22].SetMap(&map[22]);
	girl[22].SetMap(&map[22]);

	// background
	level23_bg.LoadBitmapByString({ LEVEL_23_BG });
	level23_bg.SetTopLeft(0, 0);

	// diamond 
	level23_green_diamond.fill(Diamond("green"));
	level23_green_diamond[0].Init(208, 424);

	// switch init 
	level23_switch.fill(Switch());
	level23_switch[0].Init(317, 213, 'L', &map[22], 'W');
	
	// button init 
	level23_button.fill(Button());
	level23_button[0].Init(100, 268, 'P');
	level23_button[1].Init(275, 443, 'G');

	// platform init
	level23_platform.fill(Platform());
	level23_platform[0].Init(550, 346, 395, 'D', 'W', &map[22],'V');
	level23_platform[0].Bind(&level23_switch[0]);
	level23_platform[1].Init(313, 250, 190, 'U', 'G', &map[22],'V');
	level23_platform[1].Bind(&level23_button[1]);
	level23_platform[2].Init(263, 409, 203, 'L', 'P', &map[22]);
	level23_platform[2].Bind(&level23_button[0]);
	
	// water
	level23_water.fill(Water());
	level23_water[0].Init(492, 88, 546, 94, 'B');
	level23_water[1].Init(541, 184, 578, 190, 'B');
	level23_water[2].Init(460, 168, 499, 175, 'B');
	level23_water[3].Init(78, 215, 112, 225, 'B');
	level23_water[4].Init(43, 423, 147, 430, 'B');
	level23_water[5].Init(258, 150, 378, 160, 'G');
	level23_water[6].Init(396, 439, 481, 446, 'G');
	level23_water[7].Init(413, 247, 433, 255, 'R');

	// door init 
	level23_door.fill(Door());
	level23_door[0].Init(513, 210, 'R');
	level23_door[1].Init(567, 408, 'B');
}

void CGameStateRun::Level23OnMove()
{
	boy[22].OnMove();
	girl[22].OnMove();

	CRect boy_body = boy[22].GetBody();
	CRect girl_body = girl[22].GetBody();

	// diamond
	for (auto & diamond : level23_green_diamond)
		diamond.OnMove(boy_body, girl_body, green_counter);

	// switch
	for (auto & s : level23_switch)
		s.OnMove(boy_body, girl_body);

	// button
	for (auto & button : level23_button)
		button.OnMove(boy_body, girl_body);

	// platform
	for (auto & platform : level23_platform)
		platform.OnMove();

	// water
	if (!cheat_flag)
	{
		for (auto & water : level23_water)
		{
			sub_phase = water.OnMove(boy_body, girl_body);
			if (sub_phase)
				break;
		}
	}

	// door
	bool door0_is_triggered = level23_door[0].OnMove(boy_body, girl_body);
	bool door1_is_triggered = level23_door[1].OnMove(boy_body, girl_body);
	if (door0_is_triggered && door1_is_triggered)
	{
		if (green_counter == num_diamonds_each_level[22]["g_diamond"])	// 破關
		{
			CAudio::Instance()->Stop(A_PLAY);
			CAudio::Instance()->Play(A_FINISH);
			sub_phase = 2;
		}
		else				// 寶石沒吃完
		{
			CAudio::Instance()->Stop(A_PLAY);
			CAudio::Instance()->Play(A_FAIL);
			sub_phase = 3;
		}
	}
}

void CGameStateRun::Level23OnShow()
{
	// background
	level23_bg.ShowBitmap();

	// timer
	time_counter = timeGetTime() - time_counter_start;
	timer_showtext::show_in_the_game(time_counter);

	// diamond
	for (auto & diamond : level23_green_diamond)
		diamond.OnShow();

	// switch
	for (auto & s : level23_switch)
		s.OnShow();

	// button
	for (auto & button : level23_button)
		button.OnShow();

	// platform
	for (auto & platform : level23_platform)
		platform.OnShow();

	// door
	for (auto & door : level23_door)
		door.OnShow();

	// person
	boy[22].OnShow();
	girl[22].OnShow();
}

void CGameStateRun::ResetL23()
{
	boy[22].Reset();
	girl[22].Reset();

	// diamond
	for (auto & diamond : level23_green_diamond)
		diamond.Reset();

	// switch
	for (auto & s : level23_switch)
		s.Reset();

	// button
	for (auto & button : level23_button)
		button.Reset();

	// platform
	for (auto & platform : level23_platform)
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
	if (!cheat_flag)
	{
		for (auto & water : level31_water)
		{
			sub_phase = water.OnMove(boy_body, girl_body);
			if (sub_phase)
				break;
		}
	}

	// door
	bool door0_is_triggered = level31_door[0].OnMove(boy_body, girl_body);
	bool door1_is_triggered = level31_door[1].OnMove(boy_body, girl_body);
	if (door0_is_triggered && door1_is_triggered)
	{
		if (red_diamond_counter == num_diamonds_each_level[30]["red_diamond"] &&
			blue_diamond_counter == num_diamonds_each_level[30]["blue_diamond"])	// 破關
		{
			CAudio::Instance()->Stop(A_PLAY);
			CAudio::Instance()->Play(A_FINISH);
			sub_phase = 2;
		}
		else				// 寶石沒吃完
		{
			CAudio::Instance()->Stop(A_PLAY);
			CAudio::Instance()->Play(A_FAIL);
			sub_phase = 3;
		}
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
