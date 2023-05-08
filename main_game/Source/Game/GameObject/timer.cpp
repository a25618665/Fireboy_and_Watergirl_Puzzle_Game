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
		//second = second - minute * 60;
		timer_showtext::show_as_text("TIME", 250, 145);
		timer_showtext::show_as_text(second-(second/10)*10, 390, 145);
		timer_showtext::show_as_text(second/10-((second/10)/6)*6, 375, 145);
		timer_showtext::show_as_text(":", 365, 145);
		timer_showtext::show_as_text((second/10)/6-(minute/10)*10, 345, 145);
		timer_showtext::show_as_text(minute/10, 330, 145);
		timer_showtext::show_as_text("X", 320, 180);
		timer_showtext::show_as_text("X", 320, 220);
		timer_showtext::show_as_text(blue_counter, 345, 180);
		timer_showtext::show_as_text(red_counter, 345, 220);


		//

	}
	void timer_showtext::show_as_text(int  text , int x ,int y) {
		CDC *pDC = CDDraw::GetBackCDC();
		CTextDraw::ChangeFontLog(pDC, 21, "Trajan Pro", RGB(255, 192, 0), 800);
		CTextDraw::Print(pDC, x, y, std::to_string(text));
		CDDraw::ReleaseBackCDC();

	}
	void timer_showtext::show_as_text(string  text, int x, int y) {

		CDC *pDC = CDDraw::GetBackCDC();
		CTextDraw::ChangeFontLog(pDC, 21, "Trajan Pro", RGB(255, 192, 0), 800);
		CTextDraw::Print(pDC, x, y, text);
		CDDraw::ReleaseBackCDC();

	}












}