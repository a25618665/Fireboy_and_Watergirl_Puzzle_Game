#include "stdafx.h"
#include "../Core/Resource.h"
#include <mmsystem.h>
#include <ddraw.h>
#include "../Library/audio.h"
#include "../Library/gameutil.h"
#include "../Library/gamecore.h"
#include "mygame.h"

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

}

void CGameStateInit::OnLButtonDown(UINT nFlags, CPoint point)
{
	if ((point.x > 0) && (point.y > 0)) {
		
		background.SetFrameIndexOfBitmap(1);

}
	//GotoGameState(GAME_STATE_RUN);		// 切換至GAME_STATE_RUN
}

void CGameStateInit::OnShow()
{
	if (phase == 0)
		background.ShowBitmap();//載入圖片
	//button_of_play.ShowBitmap();//載入PLAY圖片
	
}

void CGameStateInit::load_background() {
	//指定底圖及設定高度
	background.LoadBitmapByString({ "../fireboy_icesister_resource/game_menu/intromenu/intro_use_word_removed.bmp","../fireboy_icesister_resource/level_map/background/images/499.bmp" });
	background.SetTopLeft(0, 0);
	//指定play圖片(去背)及設定高度
	button_of_play.LoadBitmapByString({ "../fireboy_icesister_resource/game_menu/button_of_play/buttons/DefineButton2_52_StartBtn/4.bmp" }, RGB(255, 255, 255));
	button_of_play.SetTopLeft(275, 235);
	phase = 0;
}

//void CGameStateInit::load_sound() {
	//指定背景音樂
    // CAudio::Instance()->Load(2, "../fireboy_icesister_resource/game_menu/soundtrack_menu/sounds/902_Menu_Sound.wav");
//}

void CGameStateInit::load_instruction() {
	instruction_manual.LoadBitmapByString({"../fireboy_icesister_resource/instruction_manual_in_manu/corner_of _backguormd/123.bmp" });
	instruction_manual.SetTopLeft(0, 0);
	instruction_manual.LoadBitmapByString({ "../fireboy_icesister_resource/instruction_manual_in_manu/frame_of_ins\119.bmp", "../fireboy_icesister_resource/instruction_manual_in_manu/frame_of_ins\122.bmp" });
	instruction_manual.SetTopLeft(0, 0);
	instruction_manual.LoadBitmapByString({});
	instruction_manual.SetTopLeft(0, 0);

}

