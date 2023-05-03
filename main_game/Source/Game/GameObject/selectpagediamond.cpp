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
}

void SelectPageDiamond::Init(int level, int x, int y, string color) {
	//..
	this->level = level;
	time = 0;//暫定
	diamond_x = x;
	diamond_y = y;
	time_x = x + 30;//暫定
	time_y = y;//暫定

	// load img
	if (color == "blue") {
		img_diamond.LoadBitmapByString({ SELECT_PAGE_DIAMOND_BLUE, SELECT_PAGE_DIAMOND_BLUE_CLICKED }, RGB(255, 204, 0));
		img_diamond.SetTopLeft(x, y);
	}
	else if (color == "orange")
	{
		img_diamond.LoadBitmapByString({ SELECT_PAGE_DIAMOND_ORANGE, SELECT_PAGE_DIAMOND_ORANGE_CLICKED }, RGB(255, 204, 0));
		img_diamond.SetTopLeft(x, y);
	}
	else if (color == "green")
	{
		img_diamond.LoadBitmapByString({ SELECT_PAGE_DIAMOND_GREEN, SELECT_PAGE_DIAMOND_GREEN_CLICKED }, RGB(255, 204, 0));
		img_diamond.SetTopLeft(x, y);
	}

	// 設定滑鼠按下區域
	body.SetRect(diamond_x, diamond_x + img_diamond.GetWidth(), diamond_y, diamond_y + img_diamond.GetHeight());
}

void SelectPageDiamond::SetTime(int time)
{
	//.
	if (time >= 0)
		this->time = time;
}

int SelectPageDiamond::OnButtonDown(CPoint point)
{
	if (body.PtInRect(point))					// point是否在body裡
	{
		img_diamond.SetFrameIndexOfBitmap(1);	// 顯示按鈕被按下的圖片
		return level;
	}
	else
		return 0;
}

void SelectPageDiamond::OnButtonUp()
{
	img_diamond.SetFrameIndexOfBitmap(0);
}

void SelectPageDiamond::OnShow()
{
	img_diamond.ShowBitmap();
}
