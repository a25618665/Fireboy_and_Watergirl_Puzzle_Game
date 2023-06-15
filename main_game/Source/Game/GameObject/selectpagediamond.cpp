#include "stdafx.h"
#include "../../Core/Resource.h"
#include <mmsystem.h>
#include <ddraw.h>
#include "../../Library/audio.h"
#include "../../Library/gameutil.h"
#include "../../Library/gamecore.h"
#include "../pic_path.h"
#include "selectpagediamond.h"


using namespace game_framework;

SelectPageDiamond::SelectPageDiamond()
{
	is_init = false;
}

void SelectPageDiamond::Init(int level, int x, int y, char color, int time_x, int time_y)
{
	this->level = level;
	time = 0;
	diamond_x = x;
	diamond_y = y;
	this->time_x = x + time_x;
	this->time_y = y + time_y;
	is_init = true;
	is_pass = false;

	// load img
	switch (color)
	{
	case 'B':
		img_diamond_unpass.LoadBitmapByString({UNPASS_BLUE_DIAMOND, UNPASS_BLUE_DIAMOND_CLICKED}, RGB(255, 204, 0));
		img_diamond_unpass.SetTopLeft(x, y);
		img_diamond.LoadBitmapByString({SELECT_PAGE_DIAMOND_BLUE, SELECT_PAGE_DIAMOND_BLUE_CLICKED}, RGB(255, 204, 0));
		img_diamond.SetTopLeft(x, y);
		break;
	case 'O':
		img_diamond_unpass.LoadBitmapByString({UNPASS_ORANGE_DIAMOND, UNPASS_ORANGE_DIAMOND_CLICKED}, RGB(255, 204, 0));
		img_diamond_unpass.SetTopLeft(x, y);
		img_diamond.LoadBitmapByString({SELECT_PAGE_DIAMOND_ORANGE, SELECT_PAGE_DIAMOND_ORANGE_CLICKED}, RGB(255, 204, 0));
		img_diamond.SetTopLeft(x, y);
		break;
	case 'G':
		img_diamond_unpass.LoadBitmapByString({UNPASS_GREEN_DIAMOND, UNPASS_GREEN_DIAMOND_CLICKED}, RGB(255, 204, 0));
		img_diamond_unpass.SetTopLeft(x, y);
		img_diamond.LoadBitmapByString({SELECT_PAGE_DIAMOND_GREEN, SELECT_PAGE_DIAMOND_GREEN_CLICKED}, RGB(255, 204, 0));
		img_diamond.SetTopLeft(x, y);
		break;
	}

	// 設定滑鼠按下區域
	body.SetRect(diamond_x, diamond_y, diamond_x + img_diamond.GetWidth(), diamond_y + img_diamond.GetHeight());
}

void SelectPageDiamond::SetTime(int total_second)
{
	if (time == 0 || time > total_second)
	{
		is_pass = true;
		time = total_second;
	}
}

int SelectPageDiamond::OnButtonDown(CPoint point)
{
	if (!is_init)
		return 0;
	else if (body.PtInRect(point))					// point是否在body裡
	{
		CAudio::Instance()->Play(A_PUSHER);
		img_diamond_unpass.SetFrameIndexOfBitmap(1);
		img_diamond.SetFrameIndexOfBitmap(1);	// 顯示按鈕被按下的圖片
		return level;
	}
	else
		return 0;
}

void SelectPageDiamond::OnButtonUp()
{
	if (is_init)
	{
		img_diamond_unpass.SetFrameIndexOfBitmap(0);
		img_diamond.SetFrameIndexOfBitmap(0);
		CAudio::Instance()->Stop(A_MENU);
		CAudio::Instance()->Play(A_PLAY, true);
	}
}

void SelectPageDiamond::OnShow()
{
	if (is_init)
	{
		if (is_pass)
		{
			img_diamond.ShowBitmap();
			timer_showtext::ShowTime(time, time_x, time_y);
		}
		else
		{
			img_diamond_unpass.ShowBitmap();
		}
	}
}
