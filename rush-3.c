#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//#include <process.h>

#include "apps.h"



//	┏━━━━━━┓
//	┃・・・・・・┃
//	┃・・・・・・┃
//	┃・・・・・・
//	┃・・・・・・┃
//	┃・・・・・・┃
//	┃・・・・・・┃
//	┗━━━━━━┛

char rush_ban[8][8];	//	aaab bbbb
//	rush_ban[y][x]
//	aaa=1～4:車番号、5,6:赤い車
//	bbbbb=1からの通し番号

const char rush_car_pat[7][3][3]={
	{
		{0,0,0},	// dummy data
		{0,0,0},
		{0,0,0},
	},
	{
		{1,0,0},	// 車番号１
		{1,0,0},
		{0,0,0}
	},
	{
		{1,1,0},	// 車番号２
		{0,0,0},
		{0,0,0}
	},
	{
		{1,0,0},	// 車番号３
		{1,0,0},
		{1,0,0}
	},
	{
		{1,1,1},	// 車番号４
		{0,0,0},
		{0,0,0}
	},
	{
		{1,1,0},	// 車番号５ 赤い車
		{0,0,0},
		{0,0,0}
	},
	{
		{1,1,1},	// 車番号６ 赤い車
		{0,0,0},
		{0,0,0}
	}
};

#define C (char)
const char rush_ban_test1[8][8]={	//
	{20,         20,         20,         20,         20,         20,         20,      20},
	{20,       32+1,          0,     32*2+7,     32*2+7,    32*1+10,          0,      20},
	{20,       32+1,     32*2+4,     32*2+4,     32*3+8,    32*1+10,    32*3+12,      20},
	{20,          0,  C(32*5+5),  C(32*5+5),     32*3+8,    32*1+11,    32*3+12,       0},
	{20,     32*2+2,     32*2+2,     32*1+6,     32*3+8,    32*1+11,    32*3+12,      20},
	{20,          0,          0,     32*1+6,  C(32*4+9),  C(32*4+9),  C(32*4+9),      20},
	{20,     32*2+3,     32*2+3,          0,          0,          0,          0,      20},
	{20,         20,         20,         20,         20,         20,         20,      20},
};
const char rush_ban_test2[8][8]={	//
	{20,         20,         20,         20,         20,         20,         20,      20},
	{20,          0,     32*2+1,     32*2+1,          0,     32*1+4,     32*3+5,      20},
	{20,          0,     32*1+2,          0,          0,     32*1+4,     32*3+5,      20},
	{20,          0,     32*1+2,  C(32*5+3),  C(32*5+3),          0,     32*3+5,       0},
	{20,          0,          0,          0,     32*1+9,          0,          0,      20},
	{20,     32*1+6,     32*1+7,     32*1+8,     32*1+9,    32*2+10,    32*2+10,      20},
	{20,     32*1+6,     32*1+7,     32*1+8, C(32*4+11), C(32*4+11), C(32*4+11),      20},
	{20,         20,         20,         20,         20,         20,         20,      20},
};
const char rush_ban_test3[8][8]={	// 解いた
	{20,         20,         20,         20,        20,          20,         20,      20},
	{20,          0,          0,     32*3+1,         0,      32*2+2,     32*2+2,      20},
	{20,          0,          0,     32*3+1,         0,           0,          0,      20},
	{20,  C(32*5+3),  C(32*5+3),     32*3+1,         0,           0,          0,       0},
	{20,  C(32*4+4),  C(32*4+4),  C(32*4+4),         0,           0,     32*3+5,      20},
	{20,          0,          0,          0,         0,           0,     32*3+5,      20},
	{20,          0,          0,          0,         0,           0,     32*3+5,      20},
	{20,         20,         20,         20,        20,          20,         20,      20},
};
const char rush_ban_test4[8][8]={ // 解いた
	{20,         20,         20,         20,         20,         20,         20,      20},
	{20,  C(32*4+1),  C(32*4+1),  C(32*4+1),     32*1+2,     32*3+3,     32*1+4,      20},
	{20,          0,          0,          0,     32*1+2,     32*3+3,     32*1+4,      20},
	{20,  C(32*5+7),  C(32*5+7),    32*3+10,          0,     32*3+3,     32*1+5,       0},
	{20,     32*1+8,     32*1+9,    32*3+10,          0,          0,     32*1+5,      20},
	{20,     32*1+8,     32*1+9,    32*3+10,          0,     32*2+6,     32*2+6,      20},
	{20,          0,          0,          0,          0,          0,          0,      20},
	{20,         20,         20,         20,         20,         20,         20,      20},
};
const char rush_ban_test5[8][8]={	// 
	{20,         20,         20,         20,         20,         20,         20,      20},
	{20,     32*3+1,  C(32*4+2),  C(32*4+2),  C(32*4+2),          0,     32*3+6,      20},
	{20,     32*3+1,          0,          0,     32*1+4,     32*1+5,     32*3+6,      20},
	{20,     32*3+1,  C(32*5+3),  C(32*5+3),     32*1+4,     32*1+5,     32*3+6,       0},
	{20,          0,     32*1+7,     32*1+8,  C(32*4+9),  C(32*4+9),  C(32*4+9),      20},
	{20,          0,     32*1+7,     32*1+8,    32*1+10,          0,          0,      20},
	{20,          0,          0,          0,    32*1+10,          0,          0,      20},
	{20,         20,         20,         20,         20,         20,         20,      20},
};
const char rush_ban_test6[8][8]={	// 
	{20,         20,         20,         20,         20,         20,         20,      20},
	{20,     32*1+1,          0,          0,     32*3+4,     32*2+5,     32*2+5,      20},
	{20,     32*1+1,          0,     32*1+3,     32*3+4,          0,     32*3+7,      20},
	{20,  C(32*5+2),  C(32*5+2),     32*1+3,     32*3+4,     32*1+6,     32*3+7,       0},
	{20,          0,          0,          0,          0,     32*1+6,     32*3+7,      20},
	{20,     32*1+8,     32*1+9,    32*2+10,    32*2+10,          0,          0,      20},
	{20,     32*1+8,     32*1+9,          0, C(32*4+11), C(32*4+11), C(32*4+11),      20},
	{20,         20,         20,         20,         20,         20,         20,      20},
};
const char rush_ban_test7[8][8]={	// 
	{20,         20,         20,         20,         20,         20,         20,      20},
	{20,     32*3+1,     32*2+2,     32*2+2,          0,     32*3+5,     32*3+6,      20},
	{20,     32*3+1,     32*1+3,          0,          0,     32*3+5,     32*3+6,      20},
	{20,     32*3+1,     32*1+3,  C(32*5+4),  C(32*5+4),     32*3+5,     32*3+6,       0},
	{20,          0,          0,          0,     32*1+9,    32*2+11,    32*2+11,      20},
	{20,          0,          0,     32*1+8,     32*1+9,    32*2+12,    32*2+12,      20},
	{20,     32*2+7,     32*2+7,     32*1+8,    32*2+10,    32*2+10,          0,      20},
	{20,         20,         20,         20,         20,         20,         20,      20},
};
const char rush_ban_test8[8][8]={	// 
	{20,         20,         20,         20,         20,         20,         20,      20},
	{20,     32*1+1,          0,          0,     32*2+5,     32*2+5,     32*3+7,      20},
	{20,     32*1+1,     32*2+2,     32*2+2,     32*1+6,          0,     32*3+7,      20},
	{20,  C(32*5+3),  C(32*5+3),     32*1+4,     32*1+6,          0,     32*3+7,       0},
	{20,          0,          0,     32*1+4,  C(32*4+8),  C(32*4+8),  C(32*4+8),      20},
	{20,          0,          0,          0,    32*1+10,    32*2+11,    32*2+11,      20},
	{20,          0,     32*2+9,     32*2+9,    32*1+10,    32*2+12,    32*2+12,      20},
	{20,         20,         20,         20,         20,         20,         20,      20},
};
const char rush_ban_test9[8][8]={	// 
	{20,         20,         20,         20,         20,         20,         20,      20},
	{20,       32+1,          0,     32*2+5,     32*2+5,    32*3+11,          0,      20},
	{20,       32+1,          0,       32+6,       32+7,    32*3+11,    32*3+12,      20},
	{20,  C(32*5+2),  C(32*5+2),       32+6,       32+7,    32*3+11,    32*3+12,       0},
	{20,          0,       32+3,  C(32*4+8),  C(32*4+8),  C(32*4+8),    32*3+12,      20},
	{20,          0,       32+3,     32*2+9,     32*2+9,    32*2+10,    32*2+10,      20},
	{20,  C(32*4+4),  C(32*4+4),  C(32*4+4),          0,          0,          0,      20},
	{20,         20,         20,         20,         20,         20,         20,      20},
};
const char rush_ban_init_data[8][8]={
	{20,20,20,20,20,20,20,20},
	{20, 0, 0, 0, 0, 0, 0,20},
	{20, 0, 0, 0, 0, 0, 0,20},
	{20, 0, 0, 0, 0, 0, 0, 0},
	{20, 0, 0, 0, 0, 0, 0,20},
	{20, 0, 0, 0, 0, 0, 0,20},
	{20, 0, 0, 0, 0, 0, 0,20},
	{20,20,20,20,20,20,20,20},
};
const int rush_color[21]={
	0xffff,
	0xc618,0x53d7,0xcd7f,0x0e5f,0xb73d,0x1f36,0xeed3,0xbc8e,0xa254,0xca47,0xed00,0x4d84,
	RGB(255,200,100),RGB(128,255,200),RGB(220,100,255),RGB(230,230,180),RGB(180,230,230),RGB(230,180,230),RGB(0,0,255),
	0x0
};

char *rush_msg1, *rush_msg2, *rush_msg3, *rush_msg4;
/********************************************************************************/
/*		rush_ban_init															*/
/********************************************************************************/
void rush_ban_init()
{
	memcpy(rush_ban, rush_ban_init_data, sizeof(rush_ban));
}
/********************************************************************************/
/*		rush_koma_disp															*/
/*		x,y で指定した位置に正方形を１個表示する、色指定もできる				*/
/********************************************************************************/
void rush_koma_disp(int x, int y, int color)
{
	lcd_clearA(80+20*x, 20*y, 80+19+20*x, 19+20*y, color);
}
/********************************************************************************/
/*		rush_disp_ban															*/
/*		盤を表示する															*/
/********************************************************************************/
void rush_disp_ban()
{
	int x, y, num, car;
	
	for(y=0; y<8; y++){
		for(x=0; x<8; x++){
			car = (rush_ban[y][x] >> 5) & 7;
			num = rush_ban[y][x] & 0x1f;
			if(car <= 4){
				rush_koma_disp(x, y, rush_color[num]);
			}
			else{					// 赤い車
				rush_koma_disp(x, y, RGB(255,0,0));	// 赤
			}
		}
	}
	for(y=1; y<7; y++){
		for(x=0; x<7; x++){
			if(rush_ban[y][x] != rush_ban[y][x+1])
				lcd_clearA(80+19+20*x, 20*y, 80+21+20*x, 19+20*y, BLACK);
		}
	}
	for(x=1; x<7; x++){
		for(y=0; y<7; y++){
			if(rush_ban[y][x] != rush_ban[y+1][x])
				lcd_clearA(80+20*x, 19+20*y, 80+19+20*x, 21+20*y, BLACK);
		}
	}
}
/********************************************************************************/
/*		rush_display															*/
/********************************************************************************/
void rush_display(int color)
{
	lcd_clear(WHITE);
	rush_disp_ban();
	text_color = color;
	if(rush_msg1)
		lcd_disp_str_x1(0,176,rush_msg1);
	text_color = BLACK;
	if(rush_msg2)
		lcd_disp_str_x1(0,192,rush_msg2);
	if(rush_msg3)
		lcd_disp_str_x1(0,208,rush_msg3);
	if(rush_msg4)
		lcd_disp_str_x1(0,224,rush_msg4);
}
/********************************************************************************/
/*		rush_disp_car															*/
/*		車を１台表示する														*/
/*		car: 1～6 の車番号														*/
/*		str: 全角１文字															*/
/********************************************************************************/
void rush_disp_car(int x, int y, int car, int tooshi)
{
	int x1, y1, car1;
	
	for(y1=0; y1<3; y1++){
		for(x1=0; x1<3; x1++){
			if(car <= 4){
				if(rush_car_pat[car][y1][x1]){
					rush_koma_disp(x+x1, y+y1, rush_color[tooshi]);
				}
			}
			else{					// 赤い車
				car1 = car==5 ? 2 : 4;
				if(rush_car_pat[car1][y1][x1]){
					rush_koma_disp(x+x1, y+y1, RGB(255,0,0));	// 背景色　赤
				}
			}
		}
	}
}
/********************************************************************************/
/*		rush_put_car_test														*/
/*		return 1: 置ける、0:置けない											*/
/********************************************************************************/
int rush_put_car_test(char ban[8][8], int x, int y, int car_numb)
{
	int x1, y1;
	
	for(y1=0; y1<3; y1++){
		for(x1=0; x1<3; x1++){
			if(ban[y+y1][x+x1] && rush_car_pat[car_numb][y1][x1])
				return 0;				// 置けない
		}
	}
	return 1;							// 置ける
}
/********************************************************************************/
/*		rush_put_car															*/
/*		車を盤に配置する														*/
/********************************************************************************/
void rush_put_car()
{
	int sw, color, car_numb, koma_numb, tooshi_bango, x, y, x1, y1, right, bottom;
	
	lcd_clear(WHITE);						// クリアスクリーン
	car_numb = x = y = 1;
	tooshi_bango = 1;
	rush_ban_init();
	for(;;){

		//		PRG1
		//		car_numb を作る
		//		tooshi_bango は変更しない
		//		抜けるときの car_numb を枠外に表示しておく

//		text_color = BRUE;
//		lcd_disp_str_x1(0,176,"       ｸﾙﾏ ｦ ｾﾝﾀｸｽﾙ     ");
//		text_color = BLACK;
		rush_msg1 = "       ｸﾙﾏ ｦ ｾﾝﾀｸｽﾙ";
		color = BRUE;
		
		for(;;){
//			rush_disp_ban();
//			lcd_clearA(80+20*9, 20*1, 80+19+20*11, 19+20*4, WHITE);	// クリア
//			rush_disp_car(9,1,car_numb, 20);						// 設置すべき車種表示
//
//			lcd_disp_str_x1(0,192,"sw1: ｸﾙﾏ ｾﾝﾀｸ           ");
//			lcd_disp_str_x1(0,208,"sw2: ｹｯﾃｲ               ");
//			lcd_disp_str_x1(0,224,"sw3: ｾｯﾃｲ ｼｭｳﾘｮｳ        ");

			rush_msg2 = "sw1: ｸﾙﾏ ｾﾝﾀｸ";
			rush_msg3 = "sw2: ｹｯﾃｲ";
			rush_msg4 = "sw3: ｾｯﾃｲ ｼｭｳﾘｮｳ";
			rush_display(color);
			lcd_clearA(80+20*9, 20*1, 80+19+20*11, 19+20*4, WHITE);	// クリア
			rush_disp_car(9,1,car_numb, 20);						// 設置すべき車種表示
			
			sw = sw_updownup();
			rush_msg1 = "       ｸﾙﾏ ｦ ｾﾝﾀｸｽﾙ";
			color = BRUE;
			
			lcd_disp_str_x1(0,176,"                       ");
			if(sw==1){						// 車選択
				car_numb++;
				if(car_numb > 6)
					car_numb = 1;
				lcd_clearA(80+20*9, 20*1, 80+19+20*11, 19+20*4, WHITE);
				rush_disp_car(9,1,car_numb, tooshi_bango);
			}
			else if(sw==2){					// 決定
				lcd_clearA(80+20*9, 20*1, 80+19+20*11, 19+20*4, WHITE);
				rush_disp_car(9,1,car_numb, tooshi_bango);
				break;
			}
			else if(sw==4)					// 配置終了
				return;
		}
		
		//		PRG2
		//		car_numb を移動した後設置する
		//		car_numb はこの中では更新しない
		//		car_numb の枠外表示は変更しない
		//		盤上に car_numb を表示してからキー入力する
		//		右移動した移動して(x,y の更新)一つ前に戻るかそれとも
		//		決定入力処理で設置できるときは設置した後 通し番号をインクリメントして盤表示
		//		新しい通し番号での car_numb の表示は PRG1 の頭で実行する
		
//		text_color = BRUE;
//		lcd_disp_str_x1(0,176,"         ﾁｭｳｼｬ ｽﾙ     ");
//		text_color = BLACK;
		
		rush_msg1 = "         ﾁｭｳｼｬ ｽﾙ";
		color = BRUE;
		
		x = y = 1;
		for(;;){
			if(car_numb==1 || car_numb==3)
				right = 6;
			else if(car_numb==2 || car_numb==5)
				right = 5;
			else
				right = 4;
			if(car_numb==2 || car_numb==4 || car_numb==5 || car_numb==6)
				bottom = 6;
			else if(car_numb==1)
				bottom = 5;
			else
				bottom = 4;
//			lcd_disp_str_x1(0,192,"sw1: ﾐｷﾞ ｲﾄﾞｳ           ");
//			lcd_disp_str_x1(0,208,"sw2: ｼﾀ  ｲﾄﾞｳ           ");
//			lcd_disp_str_x1(0,224,"sw3: ﾁｭｳｼｬ              ");
//			rush_disp_ban();			// 盤表示

			rush_msg2 = "sw1: ﾐｷﾞ ｲﾄﾞｳ";
			rush_msg3 = "sw2: ｼﾀ  ｲﾄﾞｳ";
			rush_msg4 = "sw3: ﾁｭｳｼｬ";
			rush_display(color);
			rush_disp_car(x, y, car_numb, tooshi_bango);

			sw = sw_updownup();
			rush_msg1 = "         ﾁｭｳｼｬ ｽﾙ";
			color = BRUE;

			lcd_disp_str_x1(0,176,"                        ");
			if(sw==1){						// 右移動
				x++;
				if(x > right)
					x = 1;
			}
			else if(sw==2){					// 下移動
				y++;
				if(y > bottom)
					y = 1;
			}
			else if(sw==4){					// 決定
				if(rush_put_car_test(rush_ban, x, y, car_numb)){	// 配置できる
					koma_numb = (car_numb << 5) + tooshi_bango;
					tooshi_bango++;
					for(y1=0; y1<3; y1++){				// 配置する
						for(x1=0; x1<3; x1++){
							if(rush_car_pat[car_numb][y1][x1]){
								rush_ban[y+y1][x+x1] = koma_numb;
							}
						}
					}
					rush_disp_ban();
					break;
				}
				else{					// 配置できない
//					text_color = RED;
//					lcd_disp_str_x1(0,176,"ﾁｭｳｼｬ ﾃﾞｷﾅｲ       ");
//					text_color = BLACK;

					rush_msg1 = "       ﾁｭｳｼｬ ﾃﾞｷﾅｲ";
					color = RED;
				}
			}
		}
	}
}
/********************************************************************************/
/*		rush_move_test															*/
/*		車を設置できるかどうかテストする										*/
/*		return 1:OK, 0:NG														*/
/********************************************************************************/
int rush_move_test(int x, int y, int car_numb)
{
	int x1, y1;
	
	for(y1=0; y1<3; y1++){
		for(x1=0; x1<3; x1++){
			if(rush_car_pat[car_numb][y1][x1] && rush_ban[y+y1][x+x1])
				return 0;
		}
	}
	return 1;								// 設置出来る
}
/********************************************************************************/
/*		rush_make_param															*/
/*		x,y 位置の駒から car_numb と tooshi_bango を作る						*/
/********************************************************************************/
void rush_make_param(int x, int y, int *p_car_numb, int *p_tooshi_bango)
{
	*p_car_numb = (rush_ban[y][x] >> 5) & 7;
	*p_tooshi_bango = rush_ban[y][x] & 0x1f;
}
/********************************************************************************/
/*		rush_move_car															*/
/*		車を移動して遊ぶ														*/
/********************************************************************************/
void rush_move_car()
{
	int color, sw, i, koma, x, y, x1, y1, x_save, y_save, car_numb, tooshi_bango;
	
	lcd_clear(WHITE);
	for(;;){
		/***	移動すべき車を指定する									***/
		/*		駒カーソル位置の車から car_numb とtooshi_bango を作る	*/
		/*		x,y から駒カーソル表示									*/
		/*		sw1 と sw2 で駒カーソルを右と下に移動する				*/
		/*		sw3 で駒カーソル位置が車かどうか調べる					*/
		/*		車ならば car_numb と tooshi_bango を作りループを抜ける	*/
		/*		車でなければエラーメッセージを出してループ先頭に戻る	*/
		x = y = 1;
//		text_color = BRUE;
//		lcd_disp_str_x1(0,176,"ｸﾙﾏ ｼﾃｲ                   ");
//		text_color = BLACK;

		rush_msg1 = "          ｸﾙﾏ ｼﾃｲ";
		color = BRUE;
		
		for(;;){
			rush_make_param(x, y, &car_numb, &tooshi_bango);	// カーソル位置の車から car_numb とtooshi_bango を作る
//			rush_disp_ban();
//			lcd_clearA(80+5+20*x, 5+20*y, 80+14+20*x, 14+20*y, BLACK);

//			lcd_disp_str_x1(0,192,"sw1: ﾐｷﾞ ｲﾄﾞｳ           ");
//			lcd_disp_str_x1(0,208,"sw2: ｼﾀ  ｲﾄﾞｳ           ");
//			lcd_disp_str_x1(0,224,"sw3: ｸﾙﾏ ｹｯﾃｲ           ");

			rush_msg2 = "sw1: ﾐｷﾞ ｲﾄﾞｳ";
			rush_msg3 = "sw2: ｼﾀ  ｲﾄﾞｳ";
			rush_msg4 = "sw3: ｸﾙﾏ ｹｯﾃｲ";
			rush_display(color);
			lcd_clearA(80+5+20*x, 5+20*y, 80+14+20*x, 14+20*y, BLACK);
			
			sw = sw_updownup();
			rush_msg1 = "          ｸﾙﾏ ｼﾃｲ";
			color = BRUE;

			lcd_disp_str_x1(0,176,"                       ");
			if(sw==1){							// カーソル右移動
				x++;
				if(x > 6)
					x = 1;
			}
			else if(sw==2){						// カーソル下移動
				y++;
				if(y > 6)
					y = 1;
			}
			else if(sw==4){						// 決定
				if(rush_ban[y][x]){				// 車なら
					rush_make_param(x, y, &car_numb, & tooshi_bango);	// カーソル位置の車から car_numb とtooshi_bango を作る
					break;
				}
				else{
//					text_color = RED;
//					lcd_disp_str_x1(0,176,"ｸﾙﾏ ﾃﾞﾊｱﾘﾏｾﾝ            ");
//					text_color = BLACK;

					rush_msg1 = "      ｸﾙﾏ ﾃﾞﾊｱﾘﾏｾﾝ";
					color = RED;
				}
			}
		}
		/***	車を移動して sw3 で設置して終局処理を行う							***/
		/*		car_numb とtooshi_bango は作られている								*/
		/*		初めに盤から指定車を削除しておく									*/
		/*		x,y を車の左上に変更する											*/
		/*		x,y,car_numb 及び tooshi_bango から車カーソルを表示する				*/
		/*		sw1 と sw2 で車カーソルを前進後退させる（表示もいるよ）				*/
		/*		sw3 で車を設置しようとする											*/
		/*			設置出来るかどうかテストして出来るなら							*/
		/*				設置する													*/
		/*				パズル完成ならメッセージを出して sw 入力で return			*/
		/*				パズル完成でなければプログラム先頭に戻る					*/
		/*			設置出来なければエラーメッセージを出してプログラム先頭に戻る	*/
		koma = (unsigned char)rush_ban[y][x];
		while(koma==(unsigned char)rush_ban[y][x-1]){
			x--;
		}
		while(koma==(unsigned char)rush_ban[y-1][x]){
			y--;
		}
		for(i=0; i<sizeof(rush_ban); i++){
			if(koma==((unsigned char*)rush_ban)[i])
				((unsigned char*)rush_ban)[i] = 0;
		}
		
//		text_color = BRUE;
//		lcd_disp_str_x1(0,176,"ﾃｲｼ                   ");
//		text_color = BLACK;
		
		rush_msg1 = "         ﾃｲｼ ｼｮﾘ";
		color = BRUE;
		for(;;){
//			rush_disp_ban();
//			rush_disp_car(x, y, car_numb, tooshi_bango);	//	x,y,car_numb 及び tooshi_bango から車カーソルを表示する
//			
//			lcd_disp_str_x1(0,192,"sw1: ｾﾞﾝｼﾝ              ");
//			lcd_disp_str_x1(0,208,"sw2: ｺｳﾀｲ               ");
//			lcd_disp_str_x1(0,224,"sw3: ﾃｲｼ                ");

			rush_msg2 = "sw1: ｾﾞﾝｼﾝ";
			rush_msg3 = "sw2: ｺｳﾀｲ";
			rush_msg4 = "sw3: ﾃｲｼ";
			rush_display(color);
			rush_disp_car(x, y, car_numb, tooshi_bango);	//	x,y,car_numb 及び tooshi_bango から車カーソルを表示する
			
			sw = sw_updownup();
			rush_msg1 = "         ﾃｲｼ ｼｮﾘ";
			text_color = BRUE;

			lcd_disp_str_x1(0,176,"                       ");
			if(sw==1){						// 前進
				if(car_numb==1 || car_numb==3){	// 上下
					y_save = y;
					y++;
					if(rush_move_test(x, y, car_numb)==0)
						y = y_save;
				}
				else{							// 左右
					x_save = x;
					x++;
					if(rush_move_test(x, y, car_numb)==0)
						x = x_save;
				}
			}
			else if(sw==2){						// 後退
				if(car_numb==1 || car_numb==3){	// 上下
					y_save = y;
					y--;
					if(rush_move_test(x, y, car_numb)==0)
						y = y_save;
				}
				else{							// 左右
					x_save = x;
					x--;
					if(rush_move_test(x, y, car_numb)==0)
						x = x_save;
				}
			}
			else if(sw==4){						// 決定
				//	設置できるかどうかのテストは
				//
				//	ここで盤に車が置けるかどうかのテストをやるのみ
				//	設置出来るなら設置して完了テストしてからプログラムトップに戻る
				//	設置出来ないならエラーメッセージを出してプログラムトップに戻る
				if(rush_put_car_test(rush_ban, x, y, car_numb)){	// 配置できる
					for(y1=0; y1<3; y1++){				// 配置する
						for(x1=0; x1<3; x1++){
							if(rush_car_pat[car_numb][y1][x1]){
								rush_ban[y+y1][x+x1] = (car_numb<<5) + tooshi_bango;
							}
						}
					}
					rush_disp_ban();
					int tmp = (rush_ban[3][7] >> 5) & 0x7;
					if(tmp==5 || tmp==6){			// パズル完成
//						lcd_clear(WHITE);
//						rush_disp_ban();
//						lcd_disp_str_x1(0,224,"                                ");
						rush_msg1 = rush_msg2 = rush_msg3 = rush_msg4 = 0;
						rush_display(color);
						lcd_disp_str_x2(0,176,"ｶﾝｾｲｼﾏｼﾀ");
						sw_updownup();		// sw wait
						return;
					}
					break;
				}
				else{							// 配置できない
//					text_color = RED;
//					lcd_disp_str_x1(0,176,"internal error            ");
//					text_color = BLACK;

					rush_msg1 = "internal error";
					break;
				}
			}
		}
	}
}
/********************************************************************************/
/*		main																	*/
/********************************************************************************/
const struct menu rush_main_menu[]={
	{80, 1, "ﾃﾆｭﾘｮｸ"},
	{100,2, "ﾚｲﾀﾞｲ"},
	{0,0,0},
};
const struct menu rush_file_menu[]={
	{60, 1, "ﾚｲﾀﾞｲ 1"},
	{80, 2, "ﾚｲﾀﾞｲ 2"},
	{100,3, "ﾚｲﾀﾞｲ 3"},
	{120,4, "ﾚｲﾀﾞｲ 4"},
	{140,5, "ﾚｲﾀﾞｲ 5"},
	{160,6, "ﾚｲﾀﾞｲ 6"},
	{180,7, "ﾚｲﾀﾞｲ 7"},
	{200,8, "ﾚｲﾀﾞｲ 8"},
	{220,9, "ﾚｲﾀﾞｲ 9"},
	{0,0,0},
};

void rush_main()
{
	int cmd;
	
	rush_ban_init();						// 盤初期化
	lcd_clear(WHITE);
//	lcd_disp_str_x1(0,0, "sw1: select");
//	lcd_disp_str_x1(0,20,"sw2: set");
//	lcd_disp_str_x1(0,40,"sw3: exit");
	cmd = menu_select(rush_main_menu);

	if(cmd==1){								// manual input
		rush_put_car();						// 車を配置する
		rush_move_car();					// 車を移動して遊ぶ
	}
	else if(cmd==2){						// file input
		lcd_clear(WHITE);
//		lcd_disp_str_x1(0,0, "sw1: select");
//		lcd_disp_str_x1(0,20,"sw2: set");
//		lcd_disp_str_x1(0,40,"sw3: exit");
		cmd = menu_select(rush_file_menu);
		
		switch(cmd){
			case 1:
				memcpy(rush_ban, rush_ban_test1, sizeof(rush_ban));
				break;
			case 2:
				memcpy(rush_ban, rush_ban_test2, sizeof(rush_ban));
				break;
			case 3:
				memcpy(rush_ban, rush_ban_test3, sizeof(rush_ban));
				break;
			case 4:
				memcpy(rush_ban, rush_ban_test4, sizeof(rush_ban));
				break;
			case 5:
				memcpy(rush_ban, rush_ban_test5, sizeof(rush_ban));
				break;
			case 6:
				memcpy(rush_ban, rush_ban_test6, sizeof(rush_ban));
				break;
			case 7:
				memcpy(rush_ban, rush_ban_test7, sizeof(rush_ban));
				break;
			case 8:
				memcpy(rush_ban, rush_ban_test8, sizeof(rush_ban));
				break;
			case 9:
				memcpy(rush_ban, rush_ban_test9, sizeof(rush_ban));
				break;
			default:						// exit
				return;
		}
		rush_move_car();					// 車を移動して遊ぶ
	}
}
