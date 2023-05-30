/*
 * mygame.h: 本檔案儲遊戲本身的class的interface
 * Copyright (C) 2002-2008 Woei-Kae Chen <wkc@csie.ntut.edu.tw>
 *
 * This file is part of game, a free game development framework for windows.
 *
 * game is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * game is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 *	 2004-03-02 V4.0
 *      1. Add CGameStateInit, CGameStateRun, and CGameStateOver to
 *         demonstrate the use of states.
 *   2005-09-13
 *      Rewrite the codes for CBall and CEraser.
 *   2005-09-20 V4.2Beta1.
 *   2005-09-29 V4.2Beta2.
 *   2006-02-08 V4.2
 *      1. Rename OnInitialUpdate() -> OnInit().
 *      2. Replace AUDIO_CANYON as AUDIO_NTUT.
 *      3. Add help bitmap to CGameStateRun.
 *   2006-09-09 V4.3
 *      1. Rename Move() and Show() as OnMove and OnShow() to emphasize that they are
 *         event driven.
 *   2008-02-15 V4.4
 *      1. Add namespace game_framework.
 *      2. Replace the demonstration of animation as a new bouncing ball.
 *      3. Use ShowInitProgress(percent) to display loading progress.
*/
#include "GameObject/person.h"
#include "GameObject/diamond.h"
#include "GameObject/button.h"
#include "GameObject/platform.h"
#include "GameObject/door.h"
#include "GameObject/switch.h"
#include "GameObject/selectpagediamond.h"
#include "GameObject/water.h"
#include "GameObject/timer.h"
#include "GameObject/rock.h"
#include <vector>
#include <array>
#include <string>
#include <map>


namespace game_framework {
	/////////////////////////////////////////////////////////////////////////////
	// Constants
	/////////////////////////////////////////////////////////////////////////////

	enum AUDIO_ID {				// 定義各種音效的編號
		AUDIO_DING,				// 0
		AUDIO_LAKE,				// 1
		AUDIO_NTUT				// 2
	};

	/////////////////////////////////////////////////////////////////////////////
	// 這個class為遊戲的遊戲開頭畫面物件
	// 每個Member function的Implementation都要弄懂
	/////////////////////////////////////////////////////////////////////////////


	class CGameStateInit : public CGameState {
	public:
		CGameStateInit(CGame *g);
		void OnInit();  								// 遊戲的初值及圖形設定
		void OnBeginState();                            // 設定每次重玩所需的變數
		void OnKeyDown(UINT, UINT, UINT)   ;
		void OnKeyUp(UINT, UINT, UINT); 				// 處理鍵盤Up的動作
		void OnLButtonDown(UINT nFlags, CPoint point);  // 處理滑鼠的動作
		void OnLButtonUp(UINT nFlags, CPoint point);
	protected:
		void OnShow();									// 顯示這個狀態的遊戲畫面
	private:
		void LoadBackground();							// 載入背景

		CMovingBitmap background;						// menu的背景圖片
		CMovingBitmap ins;								// 教學的背景圖片
		CMovingBitmap button_play;						// menu 的play button
		CMovingBitmap button_ins;						// menu 的 instruction
		CMovingBitmap button_ok_clicked;				// ins 的 ok按下的圖片
		int phase = 0;									// 在menu的第幾個畫面 0:首頁 1:教學頁面
		int button_flag = 0;							// 滑鼠按下哪個按鈕 1:play 2:instructions 3:OK
	};

	/////////////////////////////////////////////////////////////////////////////
	// 這個class為遊戲的遊戲執行物件，主要的遊戲程式都在這裡
	// 每個Member function的Implementation都要弄懂
	/////////////////////////////////////////////////////////////////////////////

	class CGameStateRun : public CGameState {
	public:
		CGameStateRun(CGame *g);
		~CGameStateRun();
		void OnBeginState();							// 設定每次重玩所需的變數
		void OnInit();  								// 遊戲的初值及圖形設定
		void OnKeyDown(UINT, UINT, UINT);
		void OnKeyUp(UINT, UINT, UINT);
		void OnLButtonDown(UINT nFlags, CPoint point);  // 處理滑鼠的動作
		void OnLButtonUp(UINT nFlags, CPoint point);	// 處理滑鼠的動作
		void OnMouseMove(UINT nFlags, CPoint point);	// 處理滑鼠的動作
		void OnRButtonDown(UINT nFlags, CPoint point);  // 處理滑鼠的動作
		void OnRButtonUp(UINT nFlags, CPoint point);	// 處理滑鼠的動作
	protected:
		void OnMove();									// 移動遊戲元素
		void OnShow();									// 顯示這個狀態的遊戲畫面
	private: 
		void ResetLevel(int level);
		void LoadMap();
		void LoadSelectPage();
		// sub phase
		void LoadSubPhase();
		void SubPhase2ShowText();
		void SubPhase3ShowText();
		// level 1
		void LoadLevel1(); 
		void Level1OnMove();
		void Level1OnShow();
		void ResetL1();
		// level 6
		void LoadLevel6();
		void Level6OnMove();
		void Level6OnShow();
		void ResetL6();
		// level 10
		void LoadLevel10();
		void Level10OnMove();
		void Level10OnShow();
		void ResetL10();
		// level 15
		void LoadLevel15();
		void Level15OnMove();
		void Level15OnShow();
		void ResetL15();
		// level 16
		void LoadLevel16();
		void Level16OnMove();
		void Level16OnShow();
		void ResetL16();
		// level 18
		void LoadLevel18();
		void Level18OnMove();
		void Level18OnShow();
		void ResetL18();
		// level 31
		void LoadLevel31();
		void Level31OnMove();
		void Level31OnShow();
		void ResetL31();
		
		// test
		CPoint t;
		int test_int = 0;

		// whole game varables
		int level;                                      // 0為遊戲關卡選擇1為第一關依此類推
		int sub_phase;                                  // 遊戲現在在哪個狀態 0:正在玩，1:死亡，2:破關，3:寶石沒吃完
		int blue_diamond_counter, red_diamond_counter;
		int time_counter, time_counter_start;			//start紀錄進入每個關卡的初始時間 counter 紀錄總共遊玩時間
		bool time_counter_flag;							//控制儲存進time變數
		array<Person, 32> boy;
		array<Person, 32> girl;
		array<array<array<int, 480>, 640>, 32> map;		//每關的地圖陣列
		array<std::map<string, int>, 32> num_diamonds_each_level;
		CDC* pDC;										//字體顯示

		// sub phase 1
		bool is_sub1_retry_clicked;
		bool is_sub1_back_clicked;
		CRect sub1_retry_body;
		CRect sub1_back_body;
		CMovingBitmap img_sub1_bg;
		CMovingBitmap img_sub1_retry_clicked;
		CMovingBitmap img_sub1_back_clicked;

		// sub phase2
		bool is_sub2_con_clicked;
		CRect sub2_con_body;
		CMovingBitmap img_sub2_bg;
		CMovingBitmap img_sub2_con_clicked;

		// sub phase 3
		bool is_sub3_retry_clicked;
		bool is_sub3_back_clicked;
		CRect sub3_retry_body;
		CRect sub3_back_body;

		// select page
		int select_page_button_down;					// 紀錄在select page哪關正在被按下
		CMovingBitmap select_page_bg;
		array<SelectPageDiamond, 32> select_page_diamond;

		// level 1
		CMovingBitmap level1_bg;
		array<Diamond, 4> level1_red_diamond;
		array<Diamond, 4> level1_blue_diamond;
		array<Switch, 1>  level1_switch;
		array<Button, 2> level1_button;
		array<Platform, 2> level1_platform;
		array<Door, 2> level1_door;
		array<Water, 3> level1_water;
		Rock level1_rock;

		//level 6
		CMovingBitmap level6_bg;
		array<Diamond, 8> level6_red_diamond;
		array<Diamond, 8> level6_blue_diamond;
		array<Water, 8> level6_water;
		array<Door, 2> level6_door;

		//level 10
		CMovingBitmap level10_bg;
		array<Diamond, 6> level10_red_diamond;
		array<Diamond, 6> level10_blue_diamond;
		array<Switch, 2>  level10_switch;
		array<Platform, 2> level10_platform;
		array<Door, 2> level10_door;
		array<Water, 10> level10_water;

		//level 15
		/*CMovingBitmap level15_bg;
		array<Diamond, 8> level15_red_diamond;
		array<Diamond, 8> level15_blue_diamond;
		array<Switch, 4>  level15_switch;
		array<Button, 4> level15_button;
		array<Platform, 9> level15_platform;
		array<Door, 2> level15_door;*/

		//level 16
		//CMovingBitmap level16_bg;
		
		// level 18
		CMovingBitmap level18_bg;
		array<Diamond, 5> level18_red_diamond;
		array<Diamond, 5> level18_blue_diamond;
		array<Switch, 2>  level18_switch;
		array<Button, 2> level18_button;
		array<Platform, 1> level18_platform;
		array<Door, 2> level18_door;
		array<Water, 7> level18_water;

		// level 31
	    CMovingBitmap level31_bg;
		array<Diamond, 7> level31_red_diamond;
		array<Diamond, 7> level31_blue_diamond;
		array<Switch, 2>  level31_switch;
		//array<Button, 2> level31_button;
		array<Platform, 2> level31_platform;
		array<Door, 2> level31_door;
		array<Water, 10> level31_water;
	};

	/////////////////////////////////////////////////////////////////////////////
	// 這個class為遊戲的結束狀態(Game Over)
	// 每個Member function的Implementation都要弄懂
	/////////////////////////////////////////////////////////////////////////////

	class CGameStateOver : public CGameState {
	public:
		CGameStateOver(CGame *g);
		void OnBeginState();							// 設定每次重玩所需的變數
		void OnInit();
	protected:
		void OnMove();									// 移動遊戲元素
		void OnShow();									// 顯示這個狀態的遊戲畫面
	private:
	};
}
