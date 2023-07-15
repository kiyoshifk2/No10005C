#include "apps.h"


const char *cc_d_c[4]={
	"A23456789TJQK",
	"2468TQA3579JK",
	"369Q258JA47TK",
	"48Q37J26TA59K"
};

int cc_d_num[4];
char cc_s[4][53+3];
char draw_c[53];						// 山札
int draw_n;								// 引いた枚数


int cc_rand(int k);

/********************************************************************************/
/*		cc_init																	*/
/********************************************************************************/
void cc_init()
{
	int i, j, r, tmp;
	
	srand(TickCount);
	
	draw_n = 0;
	strcpy(draw_c, cc_d_c[0]);
	strcat(draw_c, cc_d_c[0]);
	strcat(draw_c, cc_d_c[0]);
	strcat(draw_c, cc_d_c[0]);
	for(i=0; i<10; i++){
		for(j=0; j<52; j++){
			r = cc_rand(52);
			tmp = draw_c[j];
			draw_c[j] = draw_c[r];
			draw_c[r] = tmp;
		}
	}
	
    memset(cc_d_num, 0, sizeof(cc_d_num));
	
	memset(cc_s, 0, sizeof(cc_s));
}
/********************************************************************************/
/*		cc_disp																	*/
/********************************************************************************/
void cc_disp()
{
	int i, j, len;
	char buf[100];
	
	back_color = BRUE;
	text_color = WHITE;
	lcd_cdisp_str_x1(0, "Calculation");
	back_color = WHITE;
	text_color = BLACK;
	for(i=0; i<4; i++){						// i: deck number
		lcd_x = 0;
		lcd_y = i+2;
		lcd_printf("d%d: ", i+1);
		for(j=0; j<13; j++){
			if(cc_d_num[i]==j){
				text_color = RGB(255,128,128);
			}
			lcd_printf("%c", cc_d_c[i][j]);
		}
		text_color = BLACK;
	}
	for(i=0; i<4; i++){						// s: stack number
		sprintf(buf, "s%d: %s", i+1, cc_s[i]);
		len = strlen(buf);
		lcd_x = 0;
		lcd_y = i+7;
		if(len > 26){
			lcd_printf("%s", &buf[len-26]);
		}
		else{
			strcat(buf, "                       ");
			buf[26] = 0;
			lcd_printf("%s", buf);
		}
	}
}
/********************************************************************************/
/*		cc_rand																	*/
/********************************************************************************/
int cc_rand(int k)
{
	unsigned long long r1, r2, r3;
	unsigned int r4;
	
	if(k==0)
		return 0;
	r1 = rand();
	r2 = rand();
	r3 = r1 * r2;
	r4 = r3 >> 16;
	return r4 % (unsigned int)k;
}
/********************************************************************************/
/*		cc_debug																*/
/********************************************************************************/
int cc_debug_x;

void cc_debug(int c)
{
	lcd_x = cc_debug_x++;
	lcd_y = 0;
	lcd_printf("%c", c);
}
/********************************************************************************/
/*		cc_disp_err																*/
/********************************************************************************/
void cc_disp_err(const char *msg)
{
	text_color = RED;
	lcd_x = 0;
	lcd_y = 12;
	lcd_printf("%s", msg);
	text_color = BLACK;
    sw_updownup();
}
/********************************************************************************/
/*		cc_main																	*/
/********************************************************************************/
const char *cc_main_str1[]={
	"Draw card","Stk1","Stk2","Stk3","Stk4", 0
};
const char *cc_main_str2[]={
	"Stk1","Stk2","Stk3","Stk4","Deck1","Deck2","Deck3","Deck4", 0
};

void cc_main()
{
	int sw, num1, num2, len;
	char SourceCard;
	
	cc_init();
	for(;;){
		lcd_clear(WHITE);
		cc_disp();
		num1 = -1;
		len = 0;
		/*** source 入力	***/
		for(;;){
			if(cc_main_str1[++num1]==0)
				num1 = 0;
			if(num1==0 && draw_n>=52)		// 52 枚のカード全部引いた
				num1 = 1;
			lcd_x = 0;
			lcd_y = 12;
			lcd_printf("From %s              ", cc_main_str1[num1]);
			sw = sw_updownup();
			if(sw & 2)						// sw2
				break;
			if(sw & 4)						// sw3
				return;
		}
		if(num1==0){						// draw card
			SourceCard = draw_c[draw_n++];
		}
		else{								// stack から
			len = strlen(cc_s[num1-1]);
			if(len==0){
				cc_disp_err("no card on stack       ");
				continue;
			}
			SourceCard = cc_s[num1-1][len-1];
			cc_s[num1-1][len-1] = 0;
		}
		for(;;){
            /*** destination 入力	***/
			num2 = -1;
			for(;;){
				if(cc_main_str2[++num2]==0)
					num2 = 0;
				if(num1 != 0 && num2 < 4)  // stack から stack は禁止
					num2 = 4;
				lcd_x = 0;
				lcd_y = 12;
				lcd_printf("From %s(%c) To %s     ", cc_main_str1[num1], SourceCard, cc_main_str2[num2]);
				sw = sw_updownup();
				if(sw & 2)						// sw2
					break;
				if(sw & 4)						// sw3
					return;
			}
			/*** カード移動	***/
			if(num2 < 4){						// stack へ
				if(num1){						// stack から
					cc_disp_err("can not stack to stack      ");
					
					// stack からの取り消し
					cc_s[num1-1][len-1] = SourceCard;
					break;
				}
				
				//	draw card から stack へ
				len = strlen(cc_s[num2]);
				cc_s[num2][len] = SourceCard;
				break;
			}
			else{								// deck へ
				if(cc_d_c[num2-4][cc_d_num[num2-4]]==SourceCard){	// deck へ置ける
					++cc_d_num[num2-4];		// deck へ置く
					break;
				}
				else{							// deck へ置けない
					cc_disp_err("can not put on deck         ");
        			/***	error 処理	***/
        			if(num1==0){						// draw card
        				continue;						// 行先の再入力
        			}
        			else{								// stack から
        				len = strlen(cc_s[num1-1]);
        				cc_s[num1-1][len] = SourceCard;	// 取り消し
        				break;
        			}
				}
			}
		}
		/***	1 カード移動完了	***/
		if(cc_d_num[0]==13 && cc_d_num[1]==13 && cc_d_num[2]==13 && cc_d_num[3]==13){
			lcd_x = 0;
			lcd_y = 12;
			lcd_printf("%s", "Congratulations          ");
			sw_updownup();
			return;								// 終了
		}
	}
}
