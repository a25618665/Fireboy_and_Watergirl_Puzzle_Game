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
// 這個class為遊戲的遊戲開頭畫面物件
/////////////////////////////////////////////////////////////////////////////

CGameStateInit::CGameStateInit(CGame *g) : CGameState(g)
{
}

void CGameStateInit::OnInit()
{
	//
	// 當圖很多時，OnInit載入所有的圖要花很多時間。為避免玩遊戲的人
	//     等的不耐煩，遊戲會出現「Loading ...」，顯示Loading的進度。
	//
	ShowInitProgress(0, "Start Initialize...");	// 一開始的loading進度為0%
	load_background(); //設定背景
	//CAudio::Instance()->Load(2, "../fireboy_icesister_resource/game_menu/soundtrack_menu/sounds/5.mp3");
	//CAudio::Instance()->Load(2, "../ireboy_icesister_resource/game_menu/soundtrack_menu/sounds/902_Menu_Sound.mp3");
	ShowInitProgress(60, "Initializing");	// 一開始的loading進度為0%
	Sleep(200);				// 放慢，以便看清楚進度，實際遊戲請刪除此Sleep
	//
	// 此OnInit動作會接到CGameStaterRun::OnInit()，所以進度還沒到100%
	//
	//CAudio::Instance()->Play(2, TRUE);//撥放背景音樂
}

void CGameStateInit::OnBeginState()
{
}

void CGameStateInit::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) {

}

void CGameStateInit::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
<<<<<<< HEAD
	

	
=======

>>>>>>> eb896a25f392a06db40e5ca2cc29106955e6b1c6
}

void CGameStateInit::OnLButtonDown(UINT nFlags, CPoint point)
{
<<<<<<< HEAD
	if ((point.x > 0) && (point.y > 0&&phase ==0 )) {
		background.SetFrameIndexOfBitmap(1);
		start_in_levelmap.ShowBitmap();
	}

	    
	
	
	//}
	//GotoGameState(GAME_STATE_RUN);		// 切換至GAME_STATE_RUN
=======
	//左鍵按下時滑鼠的座標
	int x = point.x;
	int y = point.y;
	
	if (phase == 0) {
		if (x >= 272 && x <= 372 && y >= 238 && y <= 273) {   //判斷是否為play按鈕
			button_flag = 1;
			button_play.SetFrameIndexOfBitmap(1);
		}
		else if (x >= 201 && x <= 441 && y >= 322 && y <= 353) {   //判斷是否為instructions按鈕
			button_flag = 2;
			button_ins.SetFrameIndexOfBitmap(1);
		}
	}
	else if (phase == 1) {
		if (x >= 299 && x <= 348 && y >= 383 && y <= 410) {   //判斷是否為ok按鈕
			button_flag = 3;
		}
	}
}

void CGameStateInit::OnLButtonUp(UINT nFlags, CPoint point)
{
	if (button_flag == 1) {   //判斷是否為play按鈕
		button_flag = 0;
		button_play.SetFrameIndexOfBitmap(0);
		//GotoGameState(GAME_STATE_RUN);
	}
	else if (button_flag == 2) {   //判斷是否為instructions按鈕
		button_flag = 0;
		button_ins.SetFrameIndexOfBitmap(0);
		phase = 1;
	}
	else if (button_flag == 3) {   //判斷是否為ok按鈕
		button_flag = 0;
		phase = 0;
	}
>>>>>>> eb896a25f392a06db40e5ca2cc29106955e6b1c6
}

void CGameStateInit::OnShow()
{
	if (phase == 0) {
		background.ShowBitmap();//載入圖片
		button_play.ShowBitmap();
		button_ins.ShowBitmap();
	}
	else if (phase == 1) {
		ins.ShowBitmap();
		if (button_flag == 3)
			button_ok_clicked.ShowBitmap();
	}
}

void CGameStateInit::load_background() {
	background.LoadBitmapByString({ MENU_BG });
	background.SetTopLeft(0, 0);

	ins.LoadBitmapByString({ INS });
	ins.SetTopLeft(0, 0);

	button_play.LoadBitmapByString({ BUTTON_PLAY, BUTTON_PLAY_CLICKED });
	button_play.SetTopLeft(272, 238);
	
	button_ins.LoadBitmapByString({ BUTTON_INS, BUTTON_INS_CLICKED });
	button_ins.SetTopLeft(201, 322);

	button_ok_clicked.LoadBitmapByString({ BUTTON_OK_CLICKED });
	button_ok_clicked.SetTopLeft(299, 383);
}

//void CGameStateInit::load_sound() {
	//指定背景音樂
    // CAudio::Instance()->Load(2, "../fireboy_icesister_resource/game_menu/soundtrack_menu/sounds/902_Menu_Sound.wav");
//}
