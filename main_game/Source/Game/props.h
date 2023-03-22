
namespace game_framework {
    class gadget{
	public:
		bool is_touched();
		int get_x();
		int get_y();


    };
    class water  {
	public:
		water();
		CMovingBitmap water_animation[4];
		void load();//先決定watertype再根據不同的類型load不同動畫
		void set_xy(int x,int y );//設置最左邊第一張動畫位置
		void set_water_type( int type_flag) ;
		int  water_type;//0 : blue 1:red 2:green 
		void type_is();
		bool is_touched();
		int get_x();
		int get_y();

};

}