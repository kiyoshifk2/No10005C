#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//#include <process.h>

#include "apps.h"



//	����������������
//	���E�E�E�E�E�E��
//	���E�E�E�E�E�E��
//	���E�E�E�E�E�E
//	���E�E�E�E�E�E��
//	���E�E�E�E�E�E��
//	���E�E�E�E�E�E��
//	����������������

char rush_ban[8][8];	//	aaab bbbb
//	rush_ban[y][x]
//	aaa=1�`4:�Ԕԍ��A5,6:�Ԃ���
//	bbbbb=1����̒ʂ��ԍ�

const char rush_car_pat[7][3][3]={
	{
		{0,0,0},	// dummy data
		{0,0,0},
		{0,0,0},
	},
	{
		{1,0,0},	// �Ԕԍ��P
		{1,0,0},
		{0,0,0}
	},
	{
		{1,1,0},	// �Ԕԍ��Q
		{0,0,0},
		{0,0,0}
	},
	{
		{1,0,0},	// �Ԕԍ��R
		{1,0,0},
		{1,0,0}
	},
	{
		{1,1,1},	// �Ԕԍ��S
		{0,0,0},
		{0,0,0}
	},
	{
		{1,1,0},	// �Ԕԍ��T �Ԃ���
		{0,0,0},
		{0,0,0}
	},
	{
		{1,1,1},	// �Ԕԍ��U �Ԃ���
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
const char rush_ban_test3[8][8]={	// ������
	{20,         20,         20,         20,        20,          20,         20,      20},
	{20,          0,          0,     32*3+1,         0,      32*2+2,     32*2+2,      20},
	{20,          0,          0,     32*3+1,         0,           0,          0,      20},
	{20,  C(32*5+3),  C(32*5+3),     32*3+1,         0,           0,          0,       0},
	{20,  C(32*4+4),  C(32*4+4),  C(32*4+4),         0,           0,     32*3+5,      20},
	{20,          0,          0,          0,         0,           0,     32*3+5,      20},
	{20,          0,          0,          0,         0,           0,     32*3+5,      20},
	{20,         20,         20,         20,        20,          20,         20,      20},
};
const char rush_ban_test4[8][8]={ // ������
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
/*		x,y �Ŏw�肵���ʒu�ɐ����`���P�\������A�F�w����ł���				*/
/********************************************************************************/
void rush_koma_disp(int x, int y, int color)
{
	lcd_clearA(80+20*x, 20*y, 80+19+20*x, 19+20*y, color);
}
/********************************************************************************/
/*		rush_disp_ban															*/
/*		�Ղ�\������															*/
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
			else{					// �Ԃ���
				rush_koma_disp(x, y, RGB(255,0,0));	// ��
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
/*		�Ԃ��P��\������														*/
/*		car: 1�`6 �̎Ԕԍ�														*/
/*		str: �S�p�P����															*/
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
			else{					// �Ԃ���
				car1 = car==5 ? 2 : 4;
				if(rush_car_pat[car1][y1][x1]){
					rush_koma_disp(x+x1, y+y1, RGB(255,0,0));	// �w�i�F�@��
				}
			}
		}
	}
}
/********************************************************************************/
/*		rush_put_car_test														*/
/*		return 1: �u����A0:�u���Ȃ�											*/
/********************************************************************************/
int rush_put_car_test(char ban[8][8], int x, int y, int car_numb)
{
	int x1, y1;
	
	for(y1=0; y1<3; y1++){
		for(x1=0; x1<3; x1++){
			if(ban[y+y1][x+x1] && rush_car_pat[car_numb][y1][x1])
				return 0;				// �u���Ȃ�
		}
	}
	return 1;							// �u����
}
/********************************************************************************/
/*		rush_put_car															*/
/*		�Ԃ�Ղɔz�u����														*/
/********************************************************************************/
void rush_put_car()
{
	int sw, color, car_numb, koma_numb, tooshi_bango, x, y, x1, y1, right, bottom;
	
	lcd_clear(WHITE);						// �N���A�X�N���[��
	car_numb = x = y = 1;
	tooshi_bango = 1;
	rush_ban_init();
	for(;;){

		//		PRG1
		//		car_numb �����
		//		tooshi_bango �͕ύX���Ȃ�
		//		������Ƃ��� car_numb ��g�O�ɕ\�����Ă���

//		text_color = BRUE;
//		lcd_disp_str_x1(0,176,"       ��� � ������     ");
//		text_color = BLACK;
		rush_msg1 = "       ��� � ������";
		color = BRUE;
		
		for(;;){
//			rush_disp_ban();
//			lcd_clearA(80+20*9, 20*1, 80+19+20*11, 19+20*4, WHITE);	// �N���A
//			rush_disp_car(9,1,car_numb, 20);						// �ݒu���ׂ��Ԏ�\��
//
//			lcd_disp_str_x1(0,192,"sw1: ��� ����           ");
//			lcd_disp_str_x1(0,208,"sw2: ��ò               ");
//			lcd_disp_str_x1(0,224,"sw3: ��ò ���خ�        ");

			rush_msg2 = "sw1: ��� ����";
			rush_msg3 = "sw2: ��ò";
			rush_msg4 = "sw3: ��ò ���خ�";
			rush_display(color);
			lcd_clearA(80+20*9, 20*1, 80+19+20*11, 19+20*4, WHITE);	// �N���A
			rush_disp_car(9,1,car_numb, 20);						// �ݒu���ׂ��Ԏ�\��
			
			sw = sw_updownup();
			rush_msg1 = "       ��� � ������";
			color = BRUE;
			
			lcd_disp_str_x1(0,176,"                       ");
			if(sw==1){						// �ԑI��
				car_numb++;
				if(car_numb > 6)
					car_numb = 1;
				lcd_clearA(80+20*9, 20*1, 80+19+20*11, 19+20*4, WHITE);
				rush_disp_car(9,1,car_numb, tooshi_bango);
			}
			else if(sw==2){					// ����
				lcd_clearA(80+20*9, 20*1, 80+19+20*11, 19+20*4, WHITE);
				rush_disp_car(9,1,car_numb, tooshi_bango);
				break;
			}
			else if(sw==4)					// �z�u�I��
				return;
		}
		
		//		PRG2
		//		car_numb ���ړ�������ݒu����
		//		car_numb �͂��̒��ł͍X�V���Ȃ�
		//		car_numb �̘g�O�\���͕ύX���Ȃ�
		//		�Տ�� car_numb ��\�����Ă���L�[���͂���
		//		�E�ړ������ړ�����(x,y �̍X�V)��O�ɖ߂邩����Ƃ�
		//		������͏����Őݒu�ł���Ƃ��͐ݒu������ �ʂ��ԍ����C���N�������g���ĔՕ\��
		//		�V�����ʂ��ԍ��ł� car_numb �̕\���� PRG1 �̓��Ŏ��s����
		
//		text_color = BRUE;
//		lcd_disp_str_x1(0,176,"         ����� ��     ");
//		text_color = BLACK;
		
		rush_msg1 = "         ����� ��";
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
//			lcd_disp_str_x1(0,192,"sw1: з� ��޳           ");
//			lcd_disp_str_x1(0,208,"sw2: ��  ��޳           ");
//			lcd_disp_str_x1(0,224,"sw3: �����              ");
//			rush_disp_ban();			// �Օ\��

			rush_msg2 = "sw1: з� ��޳";
			rush_msg3 = "sw2: ��  ��޳";
			rush_msg4 = "sw3: �����";
			rush_display(color);
			rush_disp_car(x, y, car_numb, tooshi_bango);

			sw = sw_updownup();
			rush_msg1 = "         ����� ��";
			color = BRUE;

			lcd_disp_str_x1(0,176,"                        ");
			if(sw==1){						// �E�ړ�
				x++;
				if(x > right)
					x = 1;
			}
			else if(sw==2){					// ���ړ�
				y++;
				if(y > bottom)
					y = 1;
			}
			else if(sw==4){					// ����
				if(rush_put_car_test(rush_ban, x, y, car_numb)){	// �z�u�ł���
					koma_numb = (car_numb << 5) + tooshi_bango;
					tooshi_bango++;
					for(y1=0; y1<3; y1++){				// �z�u����
						for(x1=0; x1<3; x1++){
							if(rush_car_pat[car_numb][y1][x1]){
								rush_ban[y+y1][x+x1] = koma_numb;
							}
						}
					}
					rush_disp_ban();
					break;
				}
				else{					// �z�u�ł��Ȃ�
//					text_color = RED;
//					lcd_disp_str_x1(0,176,"����� �޷Ų       ");
//					text_color = BLACK;

					rush_msg1 = "       ����� �޷Ų";
					color = RED;
				}
			}
		}
	}
}
/********************************************************************************/
/*		rush_move_test															*/
/*		�Ԃ�ݒu�ł��邩�ǂ����e�X�g����										*/
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
	return 1;								// �ݒu�o����
}
/********************************************************************************/
/*		rush_make_param															*/
/*		x,y �ʒu�̋�� car_numb �� tooshi_bango �����						*/
/********************************************************************************/
void rush_make_param(int x, int y, int *p_car_numb, int *p_tooshi_bango)
{
	*p_car_numb = (rush_ban[y][x] >> 5) & 7;
	*p_tooshi_bango = rush_ban[y][x] & 0x1f;
}
/********************************************************************************/
/*		rush_move_car															*/
/*		�Ԃ��ړ����ėV��														*/
/********************************************************************************/
void rush_move_car()
{
	int color, sw, i, koma, x, y, x1, y1, x_save, y_save, car_numb, tooshi_bango;
	
	lcd_clear(WHITE);
	for(;;){
		/***	�ړ����ׂ��Ԃ��w�肷��									***/
		/*		��J�[�\���ʒu�̎Ԃ��� car_numb ��tooshi_bango �����	*/
		/*		x,y �����J�[�\���\��									*/
		/*		sw1 �� sw2 �ŋ�J�[�\�����E�Ɖ��Ɉړ�����				*/
		/*		sw3 �ŋ�J�[�\���ʒu���Ԃ��ǂ������ׂ�					*/
		/*		�ԂȂ�� car_numb �� tooshi_bango ����胋�[�v�𔲂���	*/
		/*		�ԂłȂ���΃G���[���b�Z�[�W���o���ă��[�v�擪�ɖ߂�	*/
		x = y = 1;
//		text_color = BRUE;
//		lcd_disp_str_x1(0,176,"��� �ò                   ");
//		text_color = BLACK;

		rush_msg1 = "          ��� �ò";
		color = BRUE;
		
		for(;;){
			rush_make_param(x, y, &car_numb, &tooshi_bango);	// �J�[�\���ʒu�̎Ԃ��� car_numb ��tooshi_bango �����
//			rush_disp_ban();
//			lcd_clearA(80+5+20*x, 5+20*y, 80+14+20*x, 14+20*y, BLACK);

//			lcd_disp_str_x1(0,192,"sw1: з� ��޳           ");
//			lcd_disp_str_x1(0,208,"sw2: ��  ��޳           ");
//			lcd_disp_str_x1(0,224,"sw3: ��� ��ò           ");

			rush_msg2 = "sw1: з� ��޳";
			rush_msg3 = "sw2: ��  ��޳";
			rush_msg4 = "sw3: ��� ��ò";
			rush_display(color);
			lcd_clearA(80+5+20*x, 5+20*y, 80+14+20*x, 14+20*y, BLACK);
			
			sw = sw_updownup();
			rush_msg1 = "          ��� �ò";
			color = BRUE;

			lcd_disp_str_x1(0,176,"                       ");
			if(sw==1){							// �J�[�\���E�ړ�
				x++;
				if(x > 6)
					x = 1;
			}
			else if(sw==2){						// �J�[�\�����ړ�
				y++;
				if(y > 6)
					y = 1;
			}
			else if(sw==4){						// ����
				if(rush_ban[y][x]){				// �ԂȂ�
					rush_make_param(x, y, &car_numb, & tooshi_bango);	// �J�[�\���ʒu�̎Ԃ��� car_numb ��tooshi_bango �����
					break;
				}
				else{
//					text_color = RED;
//					lcd_disp_str_x1(0,176,"��� ��ʱ�Ͼ�            ");
//					text_color = BLACK;

					rush_msg1 = "      ��� ��ʱ�Ͼ�";
					color = RED;
				}
			}
		}
		/***	�Ԃ��ړ����� sw3 �Őݒu���ďI�Ǐ������s��							***/
		/*		car_numb ��tooshi_bango �͍���Ă���								*/
		/*		���߂ɔՂ���w��Ԃ��폜���Ă���									*/
		/*		x,y ���Ԃ̍���ɕύX����											*/
		/*		x,y,car_numb �y�� tooshi_bango ����ԃJ�[�\����\������				*/
		/*		sw1 �� sw2 �ŎԃJ�[�\����O�i��ނ�����i�\���������j				*/
		/*		sw3 �ŎԂ�ݒu���悤�Ƃ���											*/
		/*			�ݒu�o���邩�ǂ����e�X�g���ďo����Ȃ�							*/
		/*				�ݒu����													*/
		/*				�p�Y�������Ȃ烁�b�Z�[�W���o���� sw ���͂� return			*/
		/*				�p�Y�������łȂ���΃v���O�����擪�ɖ߂�					*/
		/*			�ݒu�o���Ȃ���΃G���[���b�Z�[�W���o���ăv���O�����擪�ɖ߂�	*/
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
//		lcd_disp_str_x1(0,176,"ò�                   ");
//		text_color = BLACK;
		
		rush_msg1 = "         ò� ���";
		color = BRUE;
		for(;;){
//			rush_disp_ban();
//			rush_disp_car(x, y, car_numb, tooshi_bango);	//	x,y,car_numb �y�� tooshi_bango ����ԃJ�[�\����\������
//			
//			lcd_disp_str_x1(0,192,"sw1: ��ݼ�              ");
//			lcd_disp_str_x1(0,208,"sw2: ����               ");
//			lcd_disp_str_x1(0,224,"sw3: ò�                ");

			rush_msg2 = "sw1: ��ݼ�";
			rush_msg3 = "sw2: ����";
			rush_msg4 = "sw3: ò�";
			rush_display(color);
			rush_disp_car(x, y, car_numb, tooshi_bango);	//	x,y,car_numb �y�� tooshi_bango ����ԃJ�[�\����\������
			
			sw = sw_updownup();
			rush_msg1 = "         ò� ���";
			text_color = BRUE;

			lcd_disp_str_x1(0,176,"                       ");
			if(sw==1){						// �O�i
				if(car_numb==1 || car_numb==3){	// �㉺
					y_save = y;
					y++;
					if(rush_move_test(x, y, car_numb)==0)
						y = y_save;
				}
				else{							// ���E
					x_save = x;
					x++;
					if(rush_move_test(x, y, car_numb)==0)
						x = x_save;
				}
			}
			else if(sw==2){						// ���
				if(car_numb==1 || car_numb==3){	// �㉺
					y_save = y;
					y--;
					if(rush_move_test(x, y, car_numb)==0)
						y = y_save;
				}
				else{							// ���E
					x_save = x;
					x--;
					if(rush_move_test(x, y, car_numb)==0)
						x = x_save;
				}
			}
			else if(sw==4){						// ����
				//	�ݒu�ł��邩�ǂ����̃e�X�g��
				//
				//	�����ŔՂɎԂ��u���邩�ǂ����̃e�X�g�����̂�
				//	�ݒu�o����Ȃ�ݒu���Ċ����e�X�g���Ă���v���O�����g�b�v�ɖ߂�
				//	�ݒu�o���Ȃ��Ȃ�G���[���b�Z�[�W���o���ăv���O�����g�b�v�ɖ߂�
				if(rush_put_car_test(rush_ban, x, y, car_numb)){	// �z�u�ł���
					for(y1=0; y1<3; y1++){				// �z�u����
						for(x1=0; x1<3; x1++){
							if(rush_car_pat[car_numb][y1][x1]){
								rush_ban[y+y1][x+x1] = (car_numb<<5) + tooshi_bango;
							}
						}
					}
					rush_disp_ban();
					int tmp = (rush_ban[3][7] >> 5) & 0x7;
					if(tmp==5 || tmp==6){			// �p�Y������
//						lcd_clear(WHITE);
//						rush_disp_ban();
//						lcd_disp_str_x1(0,224,"                                ");
						rush_msg1 = rush_msg2 = rush_msg3 = rush_msg4 = 0;
						rush_display(color);
						lcd_disp_str_x2(0,176,"�ݾ��ϼ�");
						sw_updownup();		// sw wait
						return;
					}
					break;
				}
				else{							// �z�u�ł��Ȃ�
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
	{80, 1, "�ƭخ�"},
	{100,2, "ڲ�޲"},
	{0,0,0},
};
const struct menu rush_file_menu[]={
	{60, 1, "ڲ�޲ 1"},
	{80, 2, "ڲ�޲ 2"},
	{100,3, "ڲ�޲ 3"},
	{120,4, "ڲ�޲ 4"},
	{140,5, "ڲ�޲ 5"},
	{160,6, "ڲ�޲ 6"},
	{180,7, "ڲ�޲ 7"},
	{200,8, "ڲ�޲ 8"},
	{220,9, "ڲ�޲ 9"},
	{0,0,0},
};

void rush_main()
{
	int cmd;
	
	rush_ban_init();						// �Տ�����
	lcd_clear(WHITE);
//	lcd_disp_str_x1(0,0, "sw1: select");
//	lcd_disp_str_x1(0,20,"sw2: set");
//	lcd_disp_str_x1(0,40,"sw3: exit");
	cmd = menu_select(rush_main_menu);

	if(cmd==1){								// manual input
		rush_put_car();						// �Ԃ�z�u����
		rush_move_car();					// �Ԃ��ړ����ėV��
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
		rush_move_car();					// �Ԃ��ړ����ėV��
	}
}
