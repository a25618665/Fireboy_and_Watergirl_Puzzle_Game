#include "stdafx.h"
#include "../../Core/Resource.h"
#include <mmsystem.h>
#include <ddraw.h>
#include "../../Library/audio.h"
#include "../../Library/gameutil.h"
#include "../../Library/gamecore.h"
#include "timer.h"
#include "../pic_path.h"


namespace game_framework {
	void timer_showtext::show(int time_counter ,  int blue_counter, int red_counter) {
		int  second = time_counter/1000 ;
		int  minute = (second / 10) / 6; //分鐘個位數
		show_as_text("TIME", 250, 142);
		show_as_text(second-(second/10)*10, 387, 142);
		show_as_text(second/10-((second/10)/6)*6, 372, 142);
		show_as_text(":", 362, 142);
		show_as_text((second/10)/6-(minute/10)*10, 342, 142);
		show_as_text(minute/10, 327, 142);
		show_as_text("X", 320, 180);
		show_as_text("X", 320, 212);
		show_as_text(blue_counter, 342, 180);
		show_as_text(red_counter, 342, 212);
	}

	void timer_showtext::show_in_the_game(int time_counter) {
		int tatal_second = time_counter / 1000;
		int second = tatal_second % 60;
		int minute = tatal_second / 60;

		string time_str = "";
		time_str += (minute >= 10 ? to_string(minute) : ("0" + to_string(minute))) +
					 ":" + (second >= 10 ? to_string(second) : ("0" + to_string(second)));
		show_as_text(time_str, 291, 0);
	}

	void timer_showtext::show_as_text(int  text , int x ,int y) {
		CDC *pDC = CDDraw::GetBackCDC();
		CTextDraw::ChangeFontLog(pDC, 20, "Cinzel", RGB(255, 218, 0), 800);
		CTextDraw::Print(pDC, x, y, std::to_string(text));
		CDDraw::ReleaseBackCDC();
		
	}

	void timer_showtext::show_as_text(string  text, int x, int y)
	{

		CDC *pDC = CDDraw::GetBackCDC();
		CTextDraw::ChangeFontLog(pDC, 20, "Cinzel", RGB(255, 218, 0), 800);
		CTextDraw::Print(pDC, x, y, text);
		CDDraw::ReleaseBackCDC();

	}

	void timer_showtext::ShowTime(int time, int x, int y)
	{
		int tatal_second = time / 1000;
		int second = tatal_second % 60;
		int minute = tatal_second / 60;

		string time_str = "";
		time_str += (minute >= 10 ? to_string(minute) : ("0" + to_string(minute))) +
			":" + (second >= 10 ? to_string(second) : ("0" + to_string(second)));

		CDC *pDC = CDDraw::GetBackCDC();
		CTextDraw::ChangeFontLog(pDC, 20, "Cinzel", RGB(255, 218, 0), 800);
		CTextDraw::Print(pDC, x, y, time_str);
		CDDraw::ReleaseBackCDC();
	}
}
