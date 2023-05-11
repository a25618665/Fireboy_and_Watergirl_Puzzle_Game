#pragma once
#include <array>
#include<string>
namespace game_framework {
	class timer_showtext {
	public:
		static void show( int time_counter,int blue_counter ,int red_counter );
		static void show_in_the_game(int time_counter);
		static void show_as_text(int  text, int x, int y); //x,y分別為要擺放的位置text為內容
		static void show_as_text(string text, int x, int y); //x,y分別為要擺放的位置text為內容
	private:
		//static int minute;
		//static int second; 
		//static int blue_diamond;
		//static int red_diamond; //用來決定顯示寶石數目及時間換算變數
		
								
	};
}
