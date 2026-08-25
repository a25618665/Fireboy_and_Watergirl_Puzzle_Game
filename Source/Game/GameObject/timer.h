#pragma once
#include <array>
#include <string>


namespace game_framework {
	class timer_showtext {
	public:
		static void show( int time_counter,int blue_counter ,int red_counter );
		static void show_in_the_game(int time_counter);
		static void show_as_text(int  text, int x, int y); //x,y分別為要擺放的位置text為內容
		static void show_as_text(string text, int x, int y); //x,y分別為要擺放的位置text為內容
		static void ShowTime(int time, int x, int y);
	};
}
