#include "stdafx.h"
#include "../../Core/Resource.h"
#include <mmsystem.h>
#include <ddraw.h>
#include "../../Library/audio.h"
#include "../../Library/gameutil.h"
#include "../../Library/gamecore.h"
#include  "../pic_path.h"
#include "subphase1.h"


using namespace game_framework;

SubPhase1::SubPhase1()
{
}

void SubPhase1::Init()
{
	img_bg.LoadBitmapByString({ SUBPHASE_1 });
	img_bg.SetTopLeft(0, 0);

	img_retry_clicked.LoadBitmapByString({ SUBPHASE_1_RETRY });
	img_retry_clicked.SetTopLeft(230, 230);

	img_back_clicked.LoadBitmapByString({ SUBPHASE_1_BACK });
	img_back_clicked.SetTopLeft(260, 285);

	retry_body = img_retry_clicked.GetLocation();
	back_body = img_back_clicked.GetLocation();

	is_retry_clicked = false;
	is_back_clicked = false;
}

void SubPhase1::BindVarible(int * level, int * sub_phase)
{
	this->level = level;
	this->sub_phase = sub_phase;
}

void SubPhase1::OnLButtonDown()
{
	
}

void SubPhase1::OnLButtonUp()
{

}

void SubPhase1::OnMove()
{

}

void SubPhase1::OnShow()
{

}
