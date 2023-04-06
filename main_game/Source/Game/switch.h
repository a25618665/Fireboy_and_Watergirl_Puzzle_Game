#pragma once
namespace game_framework {
	
	class Switch {
	public:
		Switch();
		void OnShow();
		void OnMove(const CRect & person_body);
		void init(int x, int y , int color);
		int GetX();
		int GetY();
		bool is_right;
		int x, y ,color;
		int moving_status; //1:left 0:right 
		CMovingBitmap right;
		CMovingBitmap left;
		CRect body;
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	}


















}