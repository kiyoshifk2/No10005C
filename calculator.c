#include "apps.h"

//
//  sprintf("%lg", a) がエラーになるときは -Wno-format を入れること)
//

#define abs1(a) ((a) >= 0 ? (a) : (-a))

struct cal_v cal_v;

struct keycode {
	char *key_str;
	char *code_str;
};
const struct keycode keycode[]={
	{" "," "},					// 0 未使用
	{"0","0"},					// 1 '0'
	{"1","1"},					// 2 '1'
	{"2","2"},					// 3 '2'
	{"3","3"},					// 4 '3'
	{"4","4"},					// 5 '4'
	{"5","5"},					// 6 '5'
	{"6","6"},					// 7 '6'
	{"7","7"},					// 8 '7'
	{"8","8"},					// 9 '8'
	{"9","9"},					// 10 '9'
    {"Entr","\n"},				// 11 CR
    {"\"","\""},				// 12 "
	{"/","/"},					// 13 '/'
	{"+","+"},					// 14 '+'
	{"-","-"},					// 15 '-'
	{"=","="},					// 16 '='
	{"Ans","Ans"},				// 17 Ans
	{"DEL","DEL"},				// 18 DEL
	{"AC","AC"},				// 19 AC
	{"end","end"},				// 20 end
	{"PAGE","PAGE"},			// 21 page
	{"rght","right"},			// 22 right
	{"left","left"},			// 23 left
	{"In","In("},				// 24 in
	{"Prt","Prt "},				// 25 prt
	{"Save","Save "},			// 26 save
	{"Load","Load "},			// 27 load
	{"Run","Run"},				// 28 run
	{"(","("},					// 29 (
	{")",")"},					// 30 )
	{"Sqr","Sqrt("},			// 31 sqrt
	{"Exp","Exp("},				// 32 exp
	{"x^y","^"},				// 33 pow
	{"Sin","Sin("},				// 34 sin
	{"Cos","Cos("},				// 35 cos
	{"Tan","Tan("},				// 36 tan
	{"Ln","Ln("},				// 37 ln
	{"Log","Log("},				// 38 log
	{"A","A"},					// 39 A
	{"B","B"},					// 40 B
	{"C","C"},					// 41 C
	{"D","D"},					// 42 D
	{"E","E"},					// 43 E
	{"F","F"},					// 44 F
	{"G","G"},					// 45 G
	{"H","H"},					// 46 H
	{"I","I"},					// 47 I
	{"J","J"},					// 48 J
	{"K","K"},					// 49 K
	{"L","L"},					// 50 L
	{"sto","sto "},				// 51 sto
    {"PRG","PRG"},              // 52 prg
    {"CAL","CAL"},				// 53 cal
    {"==","=="},				// 54 ==
    {"!=","!="},				// 55 !=
    {">",">"},					// 56 >
    {">=",">="},				// 57 >=
    {"<","<"},					// 58 <
    {"<=","<="},				// 59 <=
    {"List","List "},			// 60 LIST
	{".","."},					// 61 '.'
	{"*","*"},					// 62 '*'
	{",",","},					// 63 ','
	{"HELP","HELP"},			// 64 HELP
	{"Size","Size()"},			// 65 SIZE
	{"Goto","Goto "},			// 66 GOTO
	{"Stop","Stop"},			// 67 STOP
	{"Paus","Pause"},			// 68 PAUSE
	{"SW_B","SW_B()"},			// 69 SW_B
	{"Rand","Rand()"},			// 70 rand
	{"Cls","Cls"},				// 71 cls
	{"RGB","RGB("},				// 72 RGB
	{"TxtC","TxtC"},			// 73 txtc
	{"BakC","BakC"},			// 74 bakc
	{"SW","SW()"},				// 75 SW
	{"Time","Time()"},			// 76 time
	{"GphC","GphC"},			// 77 gphc
	{"Pset","Pset("},			// 78 pset
	{"Auto","Auto"},			// 79 auto
	{"IF","IF "},				// 80 if
	{"Edit","Edit"},			// 81 edit
	{"Gosb","Gosub"},			// 82 gosub
	{"Ret","Return"},			// 83 return
	{"A$","A$"},				// 84 AS
	{"B$","B$"},				// 85 BS
	{"C$","C$"},				// 86 CS
	{"D$","D$"},				// 87 DS
	{"E$","E$"},				// 88 ES
	{"F$","F$"},				// 89 FS
};

#define KEY_null 0
#define KEY_0 1
#define KEY_1 2
#define KEY_2 3
#define KEY_3 4
#define KEY_4 5
#define KEY_5 6
#define KEY_6 7
#define KEY_7 8
#define KEY_8 9
#define KEY_9 10
#define KEY_cr 11
#define KEY_dq 12
#define KEY_div 13
#define KEY_plus 14
#define KEY_minus 15
#define KEY_equal 16
#define KEY_ans 17
#define KEY_del 18
#define KEY_ac 19
#define KEY_end 20
#define KEY_page 21
#define KEY_right 22
#define KEY_left 23
#define KEY_in 24
#define KEY_prt 25
#define KEY_save 26
#define KEY_load 27
#define KEY_run 28
#define KEY_lkakko 29
#define KEY_rkakko 30
#define KEY_sqrt 31
#define KEY_exp 32
#define KEY_pow 33
#define KEY_sin 34
#define KEY_cos 35
#define KEY_tan 36
#define KEY_ln 37
#define KEY_log 38
#define KEY_A 39
#define KEY_B 40
#define KEY_C 41
#define KEY_D 42
#define KEY_E 43
#define KEY_F 44
#define KEY_G 45
#define KEY_H 46
#define KEY_I 47
#define KEY_J 48
#define KEY_K 49
#define KEY_L 50
#define KEY_sto 51
#define KEY_prg 52
#define KEY_cal 53
#define KEY_eqeq 54
#define KEY_neq 55
#define KEY_gt 56
#define KEY_ge 57
#define KEY_lt 58
#define KEY_le 59
#define KEY_list 60
#define KEY_period 61
#define KEY_mul 62
#define KEY_comma 63
#define KEY_help 64
#define KEY_size 65
#define KEY_goto 66
#define KEY_stop 67
#define KEY_pause 68
#define KEY_sw_b 69
#define KEY_rand 70
#define KEY_cls 71
#define KEY_rgb 72
#define KEY_txtc 73
#define KEY_bakc 74
#define KEY_sw 75
#define KEY_time 76
#define KEY_gphc 77
#define KEY_pset 78
#define KEY_auto 79
#define KEY_if 80
#define KEY_edit 81
#define KEY_gosub 82
#define KEY_return 83
#define KEY_AS 84
#define KEY_BS 85
#define KEY_CS 86
#define KEY_DS 87
#define KEY_ES 88
#define KEY_FS 89

int cal_get_key1(const int keyboard[8][6]);
long double cal_expr(unsigned char *kbuf, int *n);
char *str_expr(unsigned char *kbuf, int *n);
int str_expr_test(unsigned char *kbuf, int *n);
void clean_up(char *str);
void prg_disp_kbuf(unsigned char *kbuf, int n);			// 行末に CR は無し、0 が有る
void prg_disp_kbuf_p(unsigned char *kbuf);				// 行末に CR が有る、0 は無し
int prg_int_number(unsigned char *kbuf, int *n);
void prg_clear5line();									// 上の５行クリア
void prg_edit_delete(unsigned char *kbuf, int line_number);
void prg_edit_insert(unsigned char *kbuf, int n);
void error_msg(const char *msg);						// 赤字で msg を表示する
int prg_exec(unsigned char *kbuf, int *n);
int isdigit_k(unsigned char kcode);
int prg_str_keyin(unsigned char *kbuf, int *n);			// 文字列入力
void prg_disp_str(unsigned char **kbuf);
void prg_delete(unsigned char *kbuf, int *n);
void i2a_k(unsigned char *kbuf, int numb);
int prg_getline(int noclear);							// キー入力ルーチン

void text_save(int restore);
long double menu_input_numb(const struct menu2 *tbl, char *head);

/********************************************************************************/
/*		prg_help																*/
/********************************************************************************/
const char *help_msg[] = {
//   12345678901234567890123456
	"   === help message ===\n"
	"\n"
	"List: List\n"
	"      List 100\n"
	"Prt:  Prt <expr>,<expr>\n"
	"In:   In()\n"
	"      In(\"string\")\n"
	"Size: Program size\n" 
	"SW  : switch input\n"
	"SW_B: switch input\n"
	"Rand: Random 0.0-1.0\n"
	"Time: 100us time\n"
	,
//   12345678901234567890123456
	"\":    string\n"
	"Run:  Run\n"
	"      Run 100\n"
	"Goto: Goto 100\n"
	"Stop: program end\n"
	"Paus: Pause\n"
	"      Pause \"string\"\n"
	"IF:   IF<expr><statement>\n"
	,
//   12345678901234567890123456
	"Cls:  Clear screen\n"
	"RGB:  RGB(255,255,255)\n"
	"TxtC: text_color\n"
	"BakC: back_color\n"
	"GphC: graph_color\n"
	"Pset: Pset(x,y) x=0-319\n"
	"                y=0-239\n"
};

void prg_help()
{
	int i, sw;
	
	lcd_noofline = 15;
	i = 0;
	for(;;){
		lcd_clear(WHITE);
		lcd_puts(help_msg[i]);
		sw = sw_updownup();
		if(sw & 3){							// sw2
			if(++i >= 3)
				i = 0;
		}
		if(sw & 4)							// sw3
			return;
	}
}
/********************************************************************************/
/*		cmd_SAVE																*/
/********************************************************************************/
int cmd_SAVE(/*unsigned char *kbuf, int *n*/)
{
	lcd_clear(WHITE);
	lcd_noofline = 15;

	calc_save();
//	lcd_printf("push any sw ");
//	sw_updown();
	
	return 0;
}
/********************************************************************************/
/*		cmd_LOAD																*/
/********************************************************************************/
int cmd_LOAD(/*unsigned char *kbuf, int *n*/)
{
	lcd_clear(WHITE);
	lcd_noofline = 15;

	calc_load();
//	lcd_printf("push any sw ");
//	sw_updown();
	
	return 0;
}
/********************************************************************************/
/*		cmd_LIST																*/
/********************************************************************************/
void put_to_buf(char text[30][26], int *x, int *y, char c)
{
	text[*y][*x] = c;
	if(++(*x) >= 26){
		*x = 0;
		++(*y);
	}
}

int cmd_LIST(unsigned char *kbuf, int *n)
{
	int i, start, numb, numb_mae, flag, nn, n_top, n_mae, tmp, x, y, kcode, c;
	unsigned char *kkb;
	char text[30][26];						// １画面１５行の２６文字
	
	kkb = cal_v.prg_buf;
	nn = n_mae = start = 0;
	if(kkb[0]==0){
		lcd_puts("=== no program ===\n");
		return 0;
	}
	if(isdigit_k(kbuf[*n])){
		start = prg_int_number(kbuf, n);
		for(;;){
			n_top = nn;
			numb = prg_int_number(kkb, &nn);
			if(numb >= start){
				nn = n_top;
				start = numb;
				break;
			}
			while(kkb[nn++] != KEY_cr)
				;
		}
	}
	/**********	nn から表示する、行番号 start	********/
	flag = 0;
	for(;;){
		memset(text, ' ', sizeof(text));
		x = y = 0;
		while((kcode = kkb[nn++])){
			if(kcode==KEY_cr){				// CR
				x = 0;
				++y;
			}
			else if(kcode==KEY_dq){			// "string"
				put_to_buf(text, &x, &y, '\"');
				while((c=kkb[nn++]) != KEY_dq){
					put_to_buf(text, &x, &y, c);
				}
                put_to_buf(text, &x, &y, '\"');
//				++nn;
			}
			else{							// その他のキー
				for(i=0; (c=keycode[kcode].code_str[i]); ++i){
					put_to_buf(text, &x, &y, c);
				}
			}
			if(y >= 15){
				break;
			}
		}
		text[14][25] = 0;
//		lcd_clear(WHITE);
        lcd_x = lcd_y = 0;
		lcd_puts((char*)text);						// 表示
		i = sw_updownup();
		if(i & 1){
			flag = 1;
		}
		else if(i & 2){
			flag = -1;
		}
		else if(i & 4){
			lcd_clear(WHITE);
			return 0;
		}
		
		nn = n_mae = numb_mae = 0;
		for(;;){
			n_top = nn;
			numb = prg_int_number(kkb, &nn);
			if(numb < 0)
				fatal("cmd_LIST", "line number miss", 0);
			if(numb >= start){
				if(flag < 0){				// 手前から
					nn = n_mae;
					start = numb_mae;
				}
				else if(flag==0){			// 一致が行から
					nn = n_top;
					start = numb;
				}
				else if(flag > 0){			// 次の行から
					while(kkb[nn++] != KEY_cr)
						;
					tmp = nn;
					numb = prg_int_number(kkb, &tmp);
                    if(numb>=0)
                        start = numb;
				}
				break;
			}
			while(kkb[nn++] != KEY_cr)
				;
			n_mae = n_top;
			numb_mae = numb;
		}
	}
}
/********************************************************************************/
/*		cmd_PRT																	*/
/********************************************************************************/
int cmd_PRT(unsigned char *kbuf, int *n)
{
	long double a;
	char str[50], /*c,*/ *ptr;
	int nn;
	
	for(;;){
		nn = *n;
		if(str_expr_test(kbuf, &nn) >= 0){
			ptr = str_expr(kbuf, n);
			if(ptr==0)
				return -1;					// error
			lcd_puts(ptr);
			clean_up(ptr);
		}
		else if(kbuf[*n]==KEY_cr){			// Enter
			lcd_putc('\n');
			return 0;
		}
		else{								// expr
			a = cal_expr(kbuf, n);
			if(cal_v.error){
//				cal_v.error_msg = "*** expr error";
				return -1;
			}
			sprintf(str, "%1.10lg", a);
			lcd_puts(str);
		}
		
		if(kbuf[*n]==KEY_comma){
			if(kbuf[++(*n)]==KEY_cr){
				return 0;
			}
			lcd_putc(' ');
		}
		else if(kbuf[*n]==KEY_cr){
			lcd_putc('\n');
			return 0;
		}
		else{
			cal_v.error_msg = "*** syntax error";
			return -1;
		}
	}
}
/********************************************************************************/
/*		cmd_SIZE																*/
/********************************************************************************/
int cmd_SIZE(unsigned char *kbuf, int *n)
{
	char buf[100];
	
	sprintf(buf, "program size=%d/4095\n", (int)strlen((char*)cal_v.prg_buf));
    lcd_puts(buf);
	return 0;
}
/********************************************************************************/
/*		cmd_GOTO																*/
/********************************************************************************/
int cmd_GOTO(unsigned char *kbuf, int *n)
{
	int numb, number, n_top;
	
	if(isdigit_k(kbuf[*n])){
		numb = prg_int_number(kbuf, n);
	}
	else if(kbuf[*n]>=KEY_A && kbuf[*n]>=KEY_L){
		numb = (int)(cal_v.A[kbuf[*n]-KEY_A] + 0.5);
		++(*n);
	}
	else{
		cal_v.error_msg = "*** syntax error";
		return -1;
	}
	if(kbuf[*n] != KEY_cr){
		cal_v.error_msg = "*** line end error";
		return -1;
	}
	*n = 0;
	for(;;){
		n_top = *n;
		if(kbuf[*n]==0){
			cal_v.error_msg = "*** line numb not found";
			return -1;
		}
		number = prg_int_number(kbuf, n);
		if(number==numb){
			break;
		}
		while(kbuf[(*n)++] != KEY_cr)
			;
	}
	*n = n_top;								// 行の先頭にセットする
	return 1;								// 1: GOTO 文実行
}
/********************************************************************************/
/*		cmd_RUN																	*/
/********************************************************************************/
int cmd_RUN(unsigned char *kbuf, int *n)
{
	int numb, number, nn, nn_top, ret;
	unsigned char *kk/*, c*/;
	
	sw_up();
	memset(cal_v.str_buf, 0, sizeof(cal_v.str_buf));
	memset(cal_v.AS, 0, sizeof(cal_v.AS));
	memset(cal_v.A, 0, sizeof(cal_v.A));	// 変数クリア
	cal_v.start_time = TickCount;
	srand(TickCount);
	numb = prg_int_number(kbuf, n);
	if(kbuf[*n] != KEY_cr){
		cal_v.error_msg = "*** line end error";
		return -1;
	}
	
	kk = cal_v.prg_buf;					// プログラム先頭から
	nn = nn_top = 0;						// 先頭から
	if(numb >= 0){							// Run 100
		for(;;){
			nn_top = nn;
			if(kk[nn]==0){
				cal_v.error_msg = "*** line numb not found";
				return -1;
			}
			number = prg_int_number(kk, &nn);
			if(number==numb){
				break;
			}
			while(kk[nn++] != KEY_cr)
				;
		}
	}
	else{
		while(isdigit_k(kk[nn])){			// skip line number
			++nn;
		}
	}
	/*** execute	***/
	for(;;){
		if(kk[nn]==0){
			lcd_puts("Program stop\n");
			return 0;
		}
		if(kk[nn]==KEY_return){				// return
			prg_disp_kbuf_p(&kk[nn_top]);
			error_msg("*** return without gosub\n");
			return 0;
		}

		ret = prg_exec(kk, &nn);			// 実行

		if(ret < 0){						// error
			prg_disp_kbuf_p(&kk[nn_top]);
			error_msg(cal_v.error_msg);
			return 0;
		}
		else if(ret==0 || ret==10){			// cr チェック & スキップあり
			if(kk[nn++] != KEY_cr){
				prg_disp_kbuf_p(&kk[nn_top]);
				error_msg("*** line end error\n");	// error
				return 0;
			}
		}
//		if(ret==12){						// stop
//			return 0;
//		}
		/*** line number skip	***/
        nn_top = nn;
		while(isdigit_k(kk[nn])){
			++nn;
		}
	}
}
/********************************************************************************/
/*		cmd_STOP																*/
/********************************************************************************/
int cmd_STOP(unsigned char *kbuf, int *n)
{
	lcd_puts("progrum stop\n");
	while(kbuf[++(*n)])
		;
	longjmp(cal_v.env1, 1);
//	return 12;
}
/********************************************************************************/
/*		cmd_PAUSE																*/
/********************************************************************************/
int cmd_PAUSE(unsigned char *kbuf, int *n)
{
	int c;
	
	lcd_puts("Pause ");
	if(kbuf[*n]==KEY_dq){					// "string"
		while((c=kbuf[++(*n)]) != KEY_dq)
			lcd_putc(c);
		++(*n);
	}
	lcd_putc('\n');
	sw_updownup();
    return 0;
}
/********************************************************************************/
/*		cmd_MEM																	*/
/********************************************************************************/
int cmd_MEM(unsigned char *kbuf, int *n, int kcode)
{
	if(kbuf[++(*n)]==KEY_equal){			// =
		++(*n);
		cal_v.A[kcode-KEY_A] = cal_expr(kbuf, n);
		if(cal_v.error){
			return -1;
		}
		return 0;
	}
	else{
		cal_v.error_msg = "*** syntax error";
		return -1;
	}
}
/********************************************************************************/
/*		cmd_TXTC																*/
/********************************************************************************/
int cmd_TXTC(unsigned char *kbuf, int *n)
{
	if(kbuf[*n]==KEY_equal){			// =
		++(*n);
		text_color = (int)(cal_expr(kbuf, n) + 0.5);
		if(cal_v.error){
			return -1;
		}
		return 0;
	}
	else{
		cal_v.error_msg = "*** syntax error";
		return -1;
	}
}
/********************************************************************************/
/*		cmd_BAKC																*/
/********************************************************************************/
int cmd_BAKC(unsigned char *kbuf, int *n)
{
	if(kbuf[*n]==KEY_equal){			// =
		++(*n);
		back_color = (int)(cal_expr(kbuf, n) + 0.5);
		if(cal_v.error){
			return -1;
		}
		return 0;
	}
	else{
		cal_v.error_msg = "*** syntax error";
		return -1;
	}
}
/********************************************************************************/
/*		cmd_GPHC																*/
/********************************************************************************/
int cmd_GPHC(unsigned char *kbuf, int *n)
{
	if(kbuf[*n]==KEY_equal){			// =
		++(*n);
		graph_color = (int)(cal_expr(kbuf, n) + 0.5);
		if(cal_v.error){
			return -1;
		}
		return 0;
	}
	else{
		cal_v.error_msg = "*** syntax error";
		return -1;
	}
}
/********************************************************************************/
/*		cmd_PSET																*/
/********************************************************************************/
int cmd_PSET(unsigned char *kbuf, int *n)
{
	int x, y;
	
	x = (int)(cal_expr(kbuf, n) + 0.5);
	if(kbuf[(*n)++]==KEY_comma){
		y = (int)(cal_expr(kbuf, n) + 0.5);
		if(kbuf[(*n)++]==KEY_rkakko){
			if(cal_v.error)
				return -1;
			lcd_clearA(x,y, x,y, graph_color);
			return 0;
		}
	}
	cal_v.error_msg = "*** syntax error";
	return -1;
}
/********************************************************************************/
/*		cmd_IF																	*/
/********************************************************************************/
int cmd_IF(unsigned char *kbuf, int *n)
{
	long double a;
	
	a = cal_expr(kbuf, n);
	if(cal_v.error){
		return -1;
	}
	if(a==0.0){
		while(kbuf[*n] != KEY_cr)
			++(*n);
	}
	else{
		return prg_exec(kbuf, n);
	}
	return 0;
}
/********************************************************************************/
/*		cmd_GOSUB																*/
/********************************************************************************/
int cmd_GOSUB(unsigned char *kbuf, int *n)
{
	int numb, number, nn, nn_top, ret;
	unsigned char *kkb;
	
	if(isdigit_k(kbuf[*n])){
		numb = prg_int_number(kbuf, n);
	}
	else if(kbuf[*n]>=KEY_A && kbuf[*n]>=KEY_L){
		numb = (int)(cal_v.A[kbuf[*n]-KEY_A] + 0.5);
		++(*n);
	}
	else{
		cal_v.error_msg = "*** syntax error";
		return -1;
	}
	kkb = cal_v.prg_buf;					// プログラム先頭から
	nn = nn_top = 0;						// 先頭から
	for(;;){
		nn_top = nn;
		if(kkb[nn]==0){
			cal_v.error_msg = "*** line numb not found";
			return -1;
		}
		number = prg_int_number(kkb, &nn);
		if(number==numb){
			break;
		}
		while(kkb[nn++] != KEY_cr)
			;
	}
	/*** execute	***/
	for(;;){
		if(kkb[nn]==KEY_return){			// return
			if(kkb[++nn]==KEY_cr){
				return 0;
			}
			prg_disp_kbuf_p(&kkb[nn_top]);
			error_msg("*** line end error");	// error
			longjmp(cal_v.env1, 2);
//			return 12;						// stop
		}

		ret = prg_exec(kkb, &nn);			// 実行

		if(ret < 0){						// error
			prg_disp_kbuf_p(&kkb[nn_top]);
			error_msg(cal_v.error_msg);
			longjmp(cal_v.env1, 3);
//			return 12;						// stop
		}
		else if(ret==0 || ret==10){			// cr チェック & スキップあり
			if(kkb[nn++] != KEY_cr){
				prg_disp_kbuf_p(&kkb[nn_top]);
				error_msg("*** line end error");	// error
				longjmp(cal_v.env1, 4);
//				return 12;					// stop
			}
		}
//		else if(ret==12){					// stop
//			return 12;
//		}

		if(kkb[nn]==0){
			lcd_puts("progrum stop\n");
			longjmp(cal_v.env1, 5);
//			return 12;						// stop
		}
		/*** line number skip	***/
        nn_top = nn;
		while(isdigit_k(kkb[nn])){
			++nn;
		}
	}
}
/********************************************************************************/
/*		clean_up																*/
/********************************************************************************/
void clean_up(char *str)
{
	if(str==0)
		return;
	while(*str)
		*str++ = 0;
}
/********************************************************************************/
/*		cmd_str_MEM																*/
/********************************************************************************/
int cmd_str_MEM(unsigned char *kbuf, int *n)
{
	int kcode;
	char *str;
	
	kcode = kbuf[(*n)++];
	if(kbuf[(*n)++]==KEY_equal){
		str = str_expr(kbuf, n);
		if(str==0)
			return -1;						// error
		clean_up(cal_v.AS[kcode-KEY_AS]);
		cal_v.AS[kcode-KEY_AS] = str;
		return 0;
	}
	cal_v.error_msg = "*** syntax error";
	return -1;
}
/********************************************************************************/
/*		prg_exec																*/
/*		コマンドを１個実行する													*/
/*		return 0:正常、-1:error、1:cr チェック & スキップ無し					*/
/*			  10:linebuf no clear、12:stop(廃)									*/
/********************************************************************************/
int prg_exec(unsigned char *kbuf, int *n)
{
	int ret;
	unsigned char kcode = kbuf[*n];
//	char *str;
//	long double a;
	
	if(kcode>=KEY_A && kcode<=KEY_L){
		return cmd_MEM(kbuf, n, kcode);
	}
	if(kcode>=KEY_AS && kcode<=KEY_FS){		// A$～F$
		return cmd_str_MEM(kbuf, n);
	}
	++(*n);
	switch(kcode){
		case KEY_txtc:						// TxtC
			return cmd_TXTC(kbuf, n);
		case KEY_bakc:						// BakC
			return cmd_BAKC(kbuf, n);
		case KEY_gphc:						// GphC
			return cmd_GPHC(kbuf, n);
		case KEY_dq:						// "string"
			while(kbuf[(*n)++] != KEY_dq)
				;
			return 0;
		case KEY_list:						// LIST
			return cmd_LIST(kbuf, n);
		case KEY_save:						// SAVE
			ret = cmd_SAVE(kbuf, n);
//			lcd_clear(WHITE);
			return ret;
		case KEY_load:						// LOAD
			ret = cmd_LOAD(kbuf, n);
//			lcd_clear(WHITE);
			return ret;
		case KEY_prt:						// PRT
			return cmd_PRT(kbuf, n);
		case KEY_size:						// SIZE()
			return cmd_SIZE(kbuf, n);
		case KEY_run:						// RUN
			return cmd_RUN(kbuf, n);
		case KEY_goto:						// GOTO
			return cmd_GOTO(kbuf, n);
		case KEY_stop:						// STOP
			return cmd_STOP(kbuf, n);
		case KEY_pause:						// PAUSE
			return cmd_PAUSE(kbuf, n);
		case KEY_cls:						// CLS
			lcd_clear(WHITE);
			return 0;
		case KEY_pset:						// PSET
			return cmd_PSET(kbuf, n);
		case KEY_if:						// IF
			return cmd_IF(kbuf, n);
//		case KEY_auto:						// AUTO
//			return cmd_AUTO(kbuf, n);
//		case KEY_edit:						// EDIT
//			return cmd_EDIT(kbuf, n);
		case KEY_gosub:						// GOSUB
			return cmd_GOSUB(kbuf, n);
		case KEY_help:						// HELP
			prg_help();
			return 0;
		default:
			cal_v.error_msg = "*** non exec key";
			return -1;						// error
	}
}
/********************************************************************************/
/*		prg_edit_delete															*/
/********************************************************************************/
void prg_edit_delete(unsigned char *kbuf, int line_number)
{
	int numb, n, n_kouho;
	char *ptr, *dst;
	
	n = n_kouho = 0;
	for(;;){
		n_kouho = n;
		if(kbuf[n]==0)						// buf 終端
			return;							// なかった
		numb = prg_int_number(kbuf, &n);	// buf の次の行番号
		if(numb < 0)
			fatal("prg_edit_delete error 1",0,0);
		ptr = strchr((char*)&kbuf[n], KEY_cr);
		n += (ptr - (char*)&kbuf[n]) + 1;
		if(numb==line_number){				// 一致した、削除する
			++ptr;
			dst = (char*)&kbuf[n_kouho];
			while((*dst++ = *ptr++))
				;
			return;
		}
	}
}
/********************************************************************************/
/*		prg_edit_insert															*/
/********************************************************************************/
void prg_edit_insert(unsigned char *kbuf, int n)
{
	int len, byte;
	
	len = strlen((char*)cal_v.linebuf);
	byte = strlen((char*)&kbuf[n]);
	memmove(&kbuf[n+len], &kbuf[n], byte);
	kbuf[n+len+byte] = 0;
	memmove(&kbuf[n], cal_v.linebuf, len);
	if(n+len+byte >= 4095){					// buf overflow
		prg_clear5line();
		error_msg("*** buf overflow");
		sw_updownup();
	}
}
/********************************************************************************/
/*		prg_editor																*/
/********************************************************************************/
void prg_editor()
{
	int numb_new, numb, n, n_kouho;
	char *ptr;
	
	n = 0;
	numb_new = prg_int_number(cal_v.linebuf, &n);
	if(numb_new < 0)
		fatal("prg_editor error 1",0,0);
	if(cal_v.linebuf[n]==KEY_cr){			// 行番号のみの行 → 削除
		prg_edit_delete(cal_v.prg_buf, numb_new);
		return;
	}
	n_kouho = 0;							// prg_buf[] の先頭
	n = 0;									// prg_buf[] の現在ポインタ
	for(;;){
		n_kouho = n;
		if(cal_v.prg_buf[n]==0){			// buf 終端
			prg_edit_insert(cal_v.prg_buf, n);
			return;
		}
		numb = prg_int_number(cal_v.prg_buf, &n);	// buf[] の次の行の行番号
		if(numb < 0)
			fatal("prg_editor error 2",0,0);
		if(numb_new < numb){
			prg_edit_insert(cal_v.prg_buf, n_kouho);
			return;
		}
		else if(numb_new==numb){
			prg_edit_delete(cal_v.prg_buf, numb_new);
			prg_edit_insert(cal_v.prg_buf, n_kouho);
			return;
		}
		ptr = strchr((char*)&cal_v.prg_buf[n], KEY_cr);
		n += (ptr - (char*)&cal_v.prg_buf[n]) + 1;
	}
}
/********************************************************************************/
/*		prg_buf_process															*/
/*		１行入力された行を処理する												*/
/*		return 0:normal, 10:linebuf no clear									*/
/********************************************************************************/
int prg_buf_process()
{
	int n, kcode, ret;
	
	cal_v.error = 0;
	cal_v.error_msg = "";
	cal_v.linetop = cal_v.linebuf;			// 不要のようだ
	n = 0;
	kcode = cal_v.linebuf[0];
	if(isdigit_k(kcode)){
		prg_editor();						// エディタ
		return 0;
	}
	else if(kcode==KEY_cr)
		return 0;
	else{
        lcd_clear(WHITE);
        lcd_noofline = 15;
		prg_disp_kbuf_p(cal_v.linebuf);
		ret = prg_exec(cal_v.linebuf, &n);	// 実行
		if(ret < 0){
			error_msg(cal_v.error_msg);     // error
		}
		else if(ret==0 || ret==10){			// 行末テスト有
			if(cal_v.linebuf[n] != KEY_cr){    // no error, 行末テスト
				error_msg("*** line end error");    // error
			}
		}
		back_color = WHITE;
		text_color = BLACK;
		lcd_printf("push any sw\n");
		sw_updownup();
        return ret==10 ? 10 : 0;
	}
}
/********************************************************************************/
/*		str_DQ_test																*/
/*		作成される string の長さを返す（本体のみ）								*/
/*		return -1:error															*/
/********************************************************************************/
int str_DQ_test(unsigned char *kbuf, int *n)
{
	int i;
	
	i = 0;
	while(kbuf[(*n)++] != KEY_dq)
		++i;
	return i;
}
/********************************************************************************/
/*		str_factor_test															*/
/*		作成される string の長さを返す（本体のみ）								*/
/*		return -1:error															*/
/********************************************************************************/
int str_factor_test(unsigned char *kbuf, int *n)
{
	int kcode;
	
	kcode = kbuf[(*n)++];
	if(kcode>=KEY_AS && kcode<=KEY_FS){
		if(cal_v.AS[kcode-KEY_AS]==0)
			return 0;
		return strlen(cal_v.AS[kcode-KEY_AS]);
	}
	switch(kcode){
		case KEY_dq:						// "
			return str_DQ_test(kbuf, n);
		default:
			cal_v.error_msg = "*** not string";
			return -1;
	}
}
/********************************************************************************/
/*		str_expr_test															*/
/*		作成される string の長さを返す（本体のみ）								*/
/*		return -1:error															*/
/********************************************************************************/
int str_expr_test(unsigned char *kbuf, int *n)
{
	int len1, len2;
	
	len1 = str_factor_test(kbuf, n);
	if(len1 < 0)
		return -1;							// error
	for(;;){
		if(kbuf[*n]==KEY_plus){				// +
			++(*n);
			len2 = str_factor_test(kbuf, n);
			if(len2 < 0)
				return -1;					// error
			len1 += len2;
			continue;
		}
		else{
			return len1;					// success
		}
	}
}
/********************************************************************************/
/*		str_DQ																	*/
/*		*str へ文字列作成														*/
/*		return -1 error, 0 success												*/
/********************************************************************************/
int str_DQ(unsigned char *kbuf, int *n, char *str)
{
	while(kbuf[*n] != KEY_dq)
		*str++ = kbuf[(*n)++];
	++(*n);
    return 0;
}
/********************************************************************************/
/*		str_factor																*/
/*		*str へ文字列作成														*/
/*		return -1 error, 0 success												*/
/********************************************************************************/
int str_factor(unsigned char *kbuf, int *n, char *str)
{
	int kcode;
	
	kcode = kbuf[(*n)++];
	if(kcode>=KEY_AS && kcode<=KEY_FS){
		if(cal_v.AS[kcode-KEY_AS]==0)
			return 0;
		strcpy(str, cal_v.AS[kcode-KEY_AS]);
		return 0;
	}
	switch(kcode){
		case KEY_dq:						// "
			return str_DQ(kbuf, n, str);
		default:
			cal_v.error_msg = "*** not string";
			return -1;
	}
}
/********************************************************************************/
/*		str_expr																*/
/*		cal_v.str_buf[] へ文字列作成 先頭アドレスを返す 0:error					*/
/********************************************************************************/
char *str_expr(unsigned char *kbuf, int *n)
{
	int i, len, nn, s, tmp;
	char *sbuf, *str;
	
	nn = *n;
	len = str_expr_test(kbuf, &nn);
	if(len < 0)
		return 0;							// error
	sbuf = cal_v.str_buf;
	s = 0;
	for(;;){
		if(s+len+1 >= sizeof(cal_v.str_buf)){
			cal_v.error_msg = "*** buf overflow";
			return 0;						// error
		}
		tmp = 0;
		for(i=s; i<s+len+1; i++)
			tmp |= sbuf[i];
		if(tmp==0)
			break;							// 空きエリア有
		for(i=s+len; sbuf[i]==0; --i)
			;
		while(sbuf[i++])
			;
		s = i;
	}
	sbuf += s;								// エリア先頭アドレス
	str = sbuf;
	
	if(str_factor(kbuf, n, str) < 0)
		return 0;							// error
	for(;;){
		if(kbuf[*n]==KEY_plus){				// +
			++(*n);
			str += strlen(str);
			if(str_factor(kbuf, n, str) < 0)
				return 0;					// error
			continue;
		}
		else{
			return sbuf;					// success
		}
	}
}
/********************************************************************************/
/*		i2a_k																	*/
/********************************************************************************/
void i2a_k(unsigned char *kbuf, int numb)
{
	int kcode;
	
	sprintf((char*)kbuf, "%d", numb);
	while((kcode=*kbuf)){
		*kbuf = KEY_0 + *kbuf - '0';
		++kbuf;
	}
}
/********************************************************************************/
/*		prg_delete																*/
/********************************************************************************/
void prg_delete(unsigned char *kbuf, int *n)
{
	int len = strlen((char*)kbuf);
	int /*i,*/ kcode;
	
	if(len==0 || *n==0)
		return;
	kcode = kbuf[*n-1];
	memmove(&kbuf[*n-1], &kbuf[*n], len - *n + 1);	// " 消去
	--len;
	--(*n);
	if(kcode==KEY_dq){						// "string"
		while(*n > 0){
			kcode = kbuf[*n-1];
			memmove(&kbuf[*n-1], &kbuf[*n], len - *n + 1);	// " 消去
			--len;
			--(*n);
			if(kcode==KEY_dq){
				return;
			}
		}
		fatal("prg_delete","string not found",0);
	}
}
/********************************************************************************/
/*		prg_str_keyin															*/
/*		文字列入力																*/
/*		<文字列>(KEY_dq)														*/
/********************************************************************************/
int prg_str_keyin(unsigned char *kbuf, int *n)
{
	char str[200], c;
	int i, x, y, sw, cnt, kcode;
	
    memset(str, 0, sizeof(str));
	lcd_clear(WHITE);
	lcd_noofline = 4;
	for(y=0; y<10; y++){					// keyboard display
		for(x=0; x<16; x++){
			c = y<6 ? 0x20+y*0x10+x : 0x40+y*0x10+x;
			lcd_disp_char_x1(64+x*12, 64+16*y, c);
		}
	}
//	lcd_cdisp_str_x1(224, "end");
	lcd_disp_str_x1(100, 224, "end");
	lcd_disp_str_x1(184, 224, "DEL");
	
	x = y = cnt = 0;
	for(;;){
		lcd_x = lcd_y = 0;
		for(i=0; i< *n; ){
			kcode = kbuf[i++];
			if(kcode==KEY_dq){
				lcd_putc('\"');
				while((kcode=kbuf[i++]) != KEY_dq){
					lcd_putc(kcode);
				}
				lcd_putc('\"');
			}
			else{
				lcd_puts(keycode[kcode].code_str);
			}
		}
		lcd_putc('\"');
        lcd_puts(str);						// 入力 string 表示
		for(;;){
			if((kcode=kbuf[i++])==0)
				break;
			if(kcode==KEY_dq){
				lcd_putc('\"');
				while((kcode=kbuf[i++]) != KEY_dq){
					lcd_putc(kcode);
				}
				lcd_putc('\"');
			}
			else{
				lcd_puts(keycode[kcode].code_str);
			}
		}
        lcd_puts(" ");
		back_color = BRUE;					// カーソルキー反転表示
		text_color = WHITE;
		if(y < 10){
			c = y<6 ? 0x20+y*0x10+x : 0x40+y*0x10+x;
			lcd_disp_char_x1(64+x*12, 64+16*y, c);
		}
		else{
			if(x < 8)
				lcd_disp_str_x1(100, 224, "end");
			else
				lcd_disp_str_x1(184, 224, "DEL");
		}
		back_color = WHITE;
		text_color = BLACK;
		
		sw = sw_updownup();					// sw 入力
		if(y < 10){							// カーソルキーノーマル表示
			c = y<6 ? 0x20+y*0x10+x : 0x40+y*0x10+x;
			lcd_disp_char_x1(64+x*12, 64+16*y, c);
		}
		else{
			if(x < 8)
				lcd_disp_str_x1(100, 224, "end");
			else
				lcd_disp_str_x1(184, 224, "DEL");
		}
		
		if(sw & 1){							// sw1: right
			if(y < 10){
				if(++x >= 16)
					x = 0;
			}
			else{
				if((x += 8) >= 16)
					x -= 16;
			}
		}
		else if(sw & 2){					// sw2: down
			if(++y > 10)
				y = 0;
		}
		else if(sw & 4){					// sw3: set
			if(y==10){
				if(x < 8)					// end
					break;
				else{						// DEL
					if(cnt)
						str[--cnt] = 0;
				}
			}
			else{
				str[cnt++] = c;
				if(cnt > 50)				// buffer full
					break;
			}
		}
	}
	memmove(&kbuf[*n + cnt + 2], &kbuf[*n], 200);	// 入れる場所を作る
	kbuf[*n] = KEY_dq;
	memmove(&kbuf[*n+1], str, cnt);
	kbuf[*n + cnt + 1] = KEY_dq;
	*n += cnt+2;
	lcd_clear(WHITE);
	return cnt;
}
/********************************************************************************/
/*		prg_disp_str															*/
/********************************************************************************/
void prg_disp_str(unsigned char **kbuf)
{
	int c;

	for(;;){
		c = *(*kbuf)++;
		if(c==KEY_dq){						// 終端
			lcd_putc('\"');
			return;
		}
		lcd_putc(c);
	}
}
/********************************************************************************/
/*		prg_disp_kbuf_p															*/
/*		行末に CR が有る、0 は無し												*/
/********************************************************************************/
void prg_disp_kbuf_p(unsigned char *kbuf)
{
	while(*kbuf != KEY_cr){
		lcd_puts(keycode[*kbuf].code_str);
		if(*kbuf++ == KEY_dq){					// "string"
			prg_disp_str(&kbuf);
		}
	}
	lcd_putc('\n');
}
/********************************************************************************/
/*		isdigit_k																*/
/********************************************************************************/
int isdigit_k(unsigned char kcode)
{
	return kcode>=KEY_0 && kcode<=KEY_9;
}
/********************************************************************************/
/*		error_msg																*/
/********************************************************************************/
void error_msg(const char *msg)
{
	text_color = RED;
	lcd_puts(msg);
	back_color = WHITE;
	text_color = BLACK;
}
/********************************************************************************/
/*		prg_clear5line															*/
/********************************************************************************/
void prg_clear5line()
{
	lcd_clearA(0, 0, 319, 79, back_color);	// テキストエリアクリア
	lcd_x = lcd_y = 0;						// 左上隅
//	lcd_noofline = 5;						// 5行でスクロール
}
/********************************************************************************/
/*		prg_int_number															*/
/*		return  -1:number でない, 0～:number									*/
/********************************************************************************/
int prg_int_number(unsigned char *kbuf, int *n)
{
	int numb;
	unsigned tmp;
	
	numb = 0;
	tmp = kbuf[*n] - KEY_0;
	if(tmp > 9)
		return -1;							// not number
	numb = tmp;
	for(;;){
		++(*n);
		tmp = kbuf[*n] - KEY_0;
		if(tmp > 9)
			return numb;					// success
		numb = numb*10 + tmp;
	}
}
/********************************************************************************/
/*		prg_disp_kbuf															*/
/*		行末に CR は無し、0 が有る												*/
/*		n: 反転表示位置															*/
/********************************************************************************/
void prg_disp_kbuf(unsigned char *kbuf, int n)
{
	int i, c, kcode;
	
	prg_clear5line();
	lcd_noofline = 5;						// 5行でスクロール
	i = 0;
	kcode = 1;
	for(;;){
		if(kcode==0)
			return;
		kcode = kbuf[i++];
		if(i==n+1){							// 反転表示
			back_color = BRUE;
			text_color = WHITE;
			lcd_puts(keycode[kcode].code_str);
			if(kcode==KEY_dq){
				while((c=kbuf[i++]) != KEY_dq){
					lcd_putc(c);
				}
				lcd_putc('\"');
			}
			back_color = WHITE;
			text_color = BLACK;
		}
		else{
			lcd_puts(keycode[kcode].code_str);
			if(kcode==KEY_dq){
				while((c=kbuf[i++]) != KEY_dq){
					lcd_putc(c);
				}
				lcd_putc('\"');
			}
		}
	}
}
/********************************************************************************/
/*		cal_disp_ans															*/
/********************************************************************************/
void cal_disp_ans()
{
	char str[100];
	
	sprintf(str, "%1.10lg                          ", cal_v.ans);
	lcd_disp_str_x1(0, 20*3+2, str);
}
/********************************************************************************/
/*		cal_number																*/
/*		res: success なら無変更, error 時は -1 をセットする」					*/
/********************************************************************************/
long double cal_number(unsigned char *kbuf, int *n)
{
	char buf[100];
	int cnt_t, cnt_n, p;
	
	p = cnt_t = cnt_n = 0;
	memset(buf, 0, sizeof(buf));
	for(;;){
		if(isdigit_k(kbuf[*n])){
			buf[p++] = kbuf[*n] -1 + '0';
            ++cnt_n;
        }
		else if(kbuf[*n]== KEY_period){
			buf[p++] = '.';
			++cnt_t;
		}
		else{
			if(cnt_t < 2 && cnt_n)
				return atof(buf);			// success
            else{
				cal_v.error = -1;
				if(cnt_n==0)
					cal_v.error_msg = "*** not expr";
				else
					cal_v.error_msg = "*** syntax error";
				return 1.0;					// error
			}
		}
		++(*n);
	}
}
/********************************************************************************/
/*		in																		*/
/********************************************************************************/
const struct menu2 input_numb_menu[]={
	{12,  80, 1000, "DEL"},
	{12,  96, 1001, "Enter"},
	{12, 112, '.', "."},
	{12, 128, '-', "-"},
	{12, 144, '0',  "0"},
	{12, 160, '1',  "1"},
	{12, 176, '2',  "2"},
	{12, 192, '3',  "3"},
	{12, 208, '4',  "4"},
	{12, 224, '5',  "5"},
	{84, 128, '6',  "6"},
	{84, 144, '7',  "7"},
	{84, 160, '8',  "8"},
	{84, 176, '9',  "9"},
	{0,0,0,0}
};

long double in(unsigned char *kbuf, int *n)
{
	int c;
	char head[100];
//	char str[50];
//	long double a;
	
	lcd_clear(WHITE);
	memset(head, 0, sizeof(head));
	if(kbuf[*n]==KEY_dq){			// "
		++(*n);
		for(;;){
			c = kbuf[(*n)++];
			if(c==KEY_dq){				// "
				if(kbuf[(*n)++]==KEY_rkakko){		// )
					break;
				}
				cal_v.error = -1;
				cal_v.error_msg = "*** syntax error";
				return 1.0;
				break;					// error
			}
			head[strlen(head)] = c;
		}
	}
	else{								// error
		if(kbuf[(*n)++]==KEY_rkakko){
			strcpy(head, "Input=");
		}
		else{
			cal_v.error = -1;
			cal_v.error_msg = "*** syntax error";
			return 1.0;
		}
	}
	/*** 入力	***/
	return menu_input_numb(input_numb_menu, head);
}
/********************************************************************************/
/*		cal_rgb																	*/
/*		cal_v.error: success なら無変更, error 時は -1 をセットする」			*/
/********************************************************************************/
long double cal_rgb(unsigned char *kbuf, int *n)
{
	int R, G, B;
	
	R = (int)(cal_expr(kbuf, n) + 0.5);
	if(R > 255) R = 255;
	if(R < 0)   R = 0;
	if(kbuf[(*n)++]==KEY_comma){
		G = (int)(cal_expr(kbuf, n) + 0.5);
		if(G > 255) G = 255;
		if(G < 0)   G = 0;
		if(kbuf[(*n)++]==KEY_comma){
			B = (int)(cal_expr(kbuf, n) + 0.5);
			if(B > 255) B = 255;
			if(B < 0)   B = 0;
			if(kbuf[(*n)++]==KEY_rkakko){
				return (long double)RGB(R, G, G);
			}
		}
	}
	cal_v.error_msg = "*** syntax error";
	return -1;
}
/********************************************************************************/
/*		cal_factor																*/
/*		factor: 数値、(expr)、Ans、sin()...										*/
/*		cal_v.error: success なら無変更, error 時は -1 をセットする」			*/
/********************************************************************************/
long double cal_factor(unsigned char *kbuf, int *n)
{
	long double a;
	int kcode /*, ret*/;
	
	kcode = kbuf[*n];
	if(kcode>=KEY_A && kcode<=KEY_L){		// A～L
		++(*n);
		return cal_v.A[kcode-KEY_A];
	}
	if(kcode==KEY_txtc){					// TxtC
		++(*n);
		return (long double)text_color;
	}
	switch(kcode){
		case KEY_lkakko:					// (
			++(*n);
			a = cal_expr(kbuf, n);
			break;
		case KEY_ans:						// Ans
            ++(*n);
			return cal_v.ans;
		case KEY_sqrt:						// sqrt(
			++(*n);
			a = sqrt(cal_expr(kbuf, n));
			break;
		case KEY_exp:						// exp(
			++(*n);
			a = exp(cal_expr(kbuf, n));
			break;
		case KEY_sin:						// sin(
			++(*n);
			a = sin(cal_expr(kbuf, n));
			break;
		case KEY_cos:						// cos(
			++(*n);
			a = cos(cal_expr(kbuf, n));
			break;
		case KEY_tan:						// tan(
			++(*n);
			a = tan(cal_expr(kbuf, n));
			break;
		case KEY_ln:						// ln(
			++(*n);
			a = log(cal_expr(kbuf, n));
			break;
		case KEY_log:						// log(
			++(*n);
			a = log10f(cal_expr(kbuf, n));
			break;
		case KEY_in:						// in
			++(*n);
			return in(kbuf, n);
		case KEY_size:						// size()
			++(*n);
			return (long double)strlen((char*)cal_v.prg_buf);
		case KEY_sw_b:						// sw_b()
			++(*n);
			return (long double)sw_updownup();
		case KEY_sw:						// sw()
			++(*n);
			return (long double)sw_read();
		case KEY_rand:						// rand()
			++(*n);
			return (long double)rand()/(long double)RAND_MAX;
		case KEY_rgb:						// RGB()
			++(*n);
			return cal_rgb(kbuf, n);
		case KEY_txtc:						// TxtC
			++(*n);
			return (long double)text_color;
		case KEY_bakc:						// BakC
			++(*n);
			return (long double)back_color;
		case KEY_gphc:						// GphC
			++(*n);
			return (long double)graph_color;
		case KEY_time:						// time()
			++(*n);
			return (long double)(TickCount - cal_v.start_time);
		default:							// 数値
			return cal_number(kbuf, n);
	}

	if(kbuf[*n]==KEY_rkakko){	// )
		++(*n);
		return a;							// success
	}
	cal_v.error = -1;
	cal_v.error_msg = "*** expr end error";
	return 1.0;								// error
}
/********************************************************************************/
/*		cal_term2																*/
/*		term2: factor ^ factor													*/
/*		cal_v.error: success なら無変更, error 時は -1 をセットする」			*/
/********************************************************************************/
long double cal_term2(unsigned char *kbuf, int *n)
{
	long double a, b;
	
	a = cal_factor(kbuf, n);
	if(kbuf[*n]==KEY_pow){					// ^
		++(*n);
		b = cal_factor(kbuf, n);
		a = pow(a, b);
	}
	return a;
}
/********************************************************************************/
/*		cal_term																*/
/*		term: term2 * or / term2												*/
/*		cal_v.error: success なら無変更, error 時は -1 をセットする」			*/
/********************************************************************************/
long double cal_term(unsigned char *kbuf, int *n)
{
	long double a, b;
	
	a = cal_term2(kbuf, n);
	for(;;){
		switch(kbuf[*n]){
			case KEY_mul:					// *
				++(*n);
				b = cal_term2(kbuf, n);
				a = a*b;
				break;
			case KEY_div:					// '/'
				++(*n);
				b = cal_term2(kbuf, n);
				a = a/b;
				break;
            default:
                return a;
		}
	}
}
/********************************************************************************/
/*		cal_term3																*/
/*		term3: term + or - term													*/
/*		cal_v.error: success なら無変更, error 時は -1 をセットする」			*/
/********************************************************************************/
long double cal_term3(unsigned char *kbuf, int *n)
{
	long double a, b;
	
	switch(kbuf[*n]){
		case KEY_plus:
			++(*n);
			a = cal_term(kbuf, n);
			break;
		case KEY_minus:
			++(*n);
			a = -cal_term(kbuf, n);
			break;
		default:
			a = cal_term(kbuf, n);
			break;
	}
	for(;;){
		switch(kbuf[*n]){
			case KEY_plus:					// +
				++(*n);
				b = cal_term(kbuf, n);
				a = a+b;
				break;
			case KEY_minus:					// -
				++(*n);
				b = cal_term(kbuf, n);
				a = a-b;
				break;
            default:
                return a;
		}
	}
}
/********************************************************************************/
/*		cal_expr																*/
/*		expr: term3 >= term3													*/
/*		cal_v.error: success なら無変更, error 時は -1 をセットする」			*/
/********************************************************************************/
long double cal_expr(unsigned char *kbuf, int *n)
{
	long double a, b;
	
	a = cal_term3(kbuf, n);
	switch(kbuf[*n]){
		case KEY_eqeq:						// ==
			++(*n);
			b = cal_term3(kbuf, n);
			return a==b ? 1 : 0;
		case KEY_neq:						// !=
			++(*n);
			b = cal_term3(kbuf, n);
			return a!=b ? 1 : 0;
		case KEY_gt:						// >
			++(*n);
			b = cal_term3(kbuf, n);
			return a>b ? 1 : 0;
		case KEY_ge:						// >=
			++(*n);
			b = cal_term3(kbuf, n);
			return a>=b ? 1 : 0;
		case KEY_lt:						// <
			++(*n);
			b = cal_term3(kbuf, n);
			return a<b ? 1 : 0;
		case KEY_le:						// <=
			++(*n);
			b = cal_term3(kbuf, n);
			return a<=b ? 1 : 0;
		default:
			return a;
	}
}

/********************************************************************************/
/*		cal_strlen																*/
/*		n: カーソル位置、n の手前までの文字数									*/
/********************************************************************************/
int cal_strlen(unsigned char *kstr, int n)
{
	int sum;
	
	sum = 0;
	while(--n >= 0){
		sum += strlen(keycode[*kstr++].code_str);
	}
	return sum;
}
/********************************************************************************/
/*		cal_makestr																*/
/********************************************************************************/
void cal_makestr(char *dst, unsigned char *src)
{
	dst[0] = '\0';
	while(*src){
		strcat(dst, keycode[*src++].code_str);
		if(strlen(dst)>26)
			return;
	}
}
/********************************************************************************/
/*		cal_disp_kbuf															*/
/*		n はカーソル位置, y: 0～3												*/
/********************************************************************************/
void cal_disp_kbuf(int y, unsigned char *kbuf, int n)
{
	int p, len;
	char buf[100];
	
	for(p=0; ; p++){
		len = cal_strlen(&kbuf[p], n-p);
		if(len <= 26-strlen(keycode[kbuf[n]].code_str)){
			cal_makestr(buf, &kbuf[p]);
			lcd_clearA(0, y*20, 319, y*20+19, WHITE);	// １行クリア
			lcd_disp_str_x1(0, y*20+2, buf);			// 表示
			back_color = BRUE;
			text_color = WHITE;
			lcd_disp_str_x1(len*12, y*20+2, keycode[kbuf[n]].code_str);
			back_color = WHITE;
			text_color = BLACK;
			return;
		}
	}
}
/********************************************************************************/
/*		cal_cur_disp															*/
/*		カーソル表示															*/
/*		x:0～25, y:0～3															*/
/********************************************************************************/
void cal_cur_disp(int x, int y)
{
	lcd_clearA(x*12, y*20+18, x*12+11, y*20+20, BLACK);
}

void cal_cur_clear(int x, int y)
{
	lcd_clearA(x*12, y*20+18, x*12+11, y*20+20, WHITE);
}
/********************************************************************************/
/*		cal_disp_key1_1															*/
/*		キー文字１個表示、x:0～5. y:0～7										*/
/********************************************************************************/
void cal_disp_key1_1(int x, int y, int kcode)
{
	lcd_disp_str_x1(15+7+(3-strlen(keycode[kcode].key_str))*6+x*49, 81+2+y*20, keycode[kcode].key_str);
}
/********************************************************************************/
/*		cal_disp_keyboard														*/
/*		キー１の面 を表示する 横 6キー、縦 8キー、座標(15,79)					*/
/********************************************************************************/
void cal_disp_keyboard(const int keyboard[8][6])
{
	int x, x48, y, y20;
	
	LCD_ALL_S;								// idle
	LCD_CS_C;								// CS clear
	lcd_area_set(15, 80, 15+6*49, 80+159);
	lcd_write_cmd(MemoryWrite);
	for(y=0; y<160; y++){
		for(x=0; x<=6*49; x++){
			y20 = y%20;
			x48 = x%49;
			if(y20==0)
				lcd_write_color(BLACK);
			else if(x48==0)
				lcd_write_color(BLACK);
			else
				lcd_write_color(WHITE);
		}
	}
	LCD_ALL_S;								// idle
    back_color = WHITE;
    text_color = BLACK;
	for(y=0; y<8; y++){
		for(x=0; x<6; x++){
			cal_disp_key1_1(x, y, keyboard[y][x]);
		}
	}
}
/********************************************************************************/
/*		cal_get_key1															*/
/*		sw1: 右移動、sw2: 左移動、sw3:キーイン									*/
/********************************************************************************/
int cal_get_key1(const int keyboard[8][6])
{
	int tmp, kcode;
	
	cal_disp_keyboard(keyboard);			// キーを全面表示
	for(;;){
		back_color = BRUE;
		text_color = WHITE;
        kcode = keyboard[cal_v.key1_y][cal_v.key1_x];
		cal_disp_key1_1(cal_v.key1_x, cal_v.key1_y, kcode);	// 現在キーを青表示
		back_color = WHITE;
		text_color = BLACK;
		tmp = sw_updownup();					// sw 入力
		cal_disp_key1_1(cal_v.key1_x, cal_v.key1_y, kcode);	// 現在キーをノーマル表示
		if(tmp & 1){						// sw1 sw
			if(++cal_v.key1_x >= 6)
				cal_v.key1_x = 0;
		}
		if(tmp & 2){						// sw2 sw
			if(++cal_v.key1_y >= 8)
				cal_v.key1_y = 0;
		}
		if(tmp & 4){						// sw3 sw
            if(kcode)
    			return kcode;
		}
	}
}
/********************************************************************************/
/*		text_save																*/
/********************************************************************************/
void text_save(int restore)
{
	int i;
	static int x, y;
	static char text_buf[15][26];

	if(restore){
		lcd_x = lcd_y = 0;
		for(i=0; i<sizeof(text_buf); i++){
			lcd_putc(((char*)text_buf)[i]);
		}
		lcd_x = x;
		lcd_y = y;
	}
	else{
		memcpy(text_buf, lcd_text_buf, sizeof(text_buf));
		x = lcd_x;
		y = lcd_y;
	}
}
/********************************************************************************/
/*		menu_input_numb															*/
/*		return: number															*/
/********************************************************************************/
long double menu_input_numb(const struct menu2 *tbl, char *head)
{
	int i, tmp, sw, len, cmd;
	char buf[100];
	long double a;
	
	text_save(0);
	lcd_clear(WHITE);
	memset(buf, 0, sizeof(buf));
	for(i=0; tbl[i].msg ; ++i){
		lcd_disp_str_x1(tbl[i].x, tbl[i].y, tbl[i].msg);
	}
	i = 0;
	for(;;){
		lcd_x = lcd_y = 0;
		lcd_printf(head);
		lcd_printf(buf);
		lcd_printf("   ");

		tmp = back_color;
		back_color = text_color;
		text_color = tmp;
		lcd_disp_str_x1(tbl[i].x, tbl[i].y, tbl[i].msg);	// 反転表示
		tmp = back_color;
		back_color = text_color;
		text_color = tmp;
		
		sw = sw_updownup();
		if(sw & 1){						// sw1
			lcd_disp_str_x1(tbl[i].x, tbl[i].y, tbl[i].msg);	// 非反転表示
			if(tbl[++i].msg==0)
				i = 0;
		}
		else if(sw & 2){					// sw2
			cmd = tbl[i].cmd;
			if(cmd==1000){					// DEL
				len = strlen(buf);
				if(len)
					buf[len-1] = 0;
			}
			else if(cmd==1001){				// Enter
				a = atof(buf);
				break;
			}
			else{							// kcode 入力
				buf[strlen(buf)] = cmd;
			}
		}
		else if(sw & 4){					// sw3
			memset(buf, 0, sizeof(buf));
		}
	}
	text_save(1);
	return a;
}
/********************************************************************************/
/*		menu_input																*/
/*		return 0:exit, 1:Enter													*/
/*		画面上５行に linebuf を表示する											*/
/********************************************************************************/
int menu_input(const struct menu2 *tbl, unsigned char *linebuf)
{
	int i, tmp, sw, ret, kcode, len;
	
	text_save(0);
	lcd_clear(WHITE);
	for(i=0; tbl[i].msg ; ++i){
		lcd_disp_str_x1(tbl[i].x, tbl[i].y, tbl[i].msg);
	}
	i = 0;
	for(;;){
		prg_disp_kbuf(linebuf, -1);
		tmp = back_color;
		back_color = text_color;
		text_color = tmp;
		lcd_disp_str_x1(tbl[i].x, tbl[i].y, tbl[i].msg);	// 反転表示
		tmp = back_color;
		back_color = text_color;
		text_color = tmp;
		
		sw = sw_updownup();
		if(sw & 1){						// sw1
			lcd_disp_str_x1(tbl[i].x, tbl[i].y, tbl[i].msg);	// 非反転表示
			if(tbl[++i].msg==0)
				i = 0;
		}
		else if(sw & 2){					// sw2
			kcode = tbl[i].cmd;
			if(kcode==1000){				// DEL
				len = strlen((char*)cal_v.linebuf);
				if(len)
					cal_v.linebuf[len-1] = 0;
			}
			else{							// kcode 入力
				cal_v.linebuf[strlen((char*)cal_v.linebuf)] = kcode;

				if(kcode==KEY_cr){			// Enter
					ret = 1;
					break;
				}
			}
		}
		else if(sw & 4){					// sw3
			ret = 0;
			break;
		}
	}
	text_save(1);
	return ret;
}
/********************************************************************************/
/*		in_linenumb_select														*/
/********************************************************************************/
const struct menu linenumb_select_menu[]={
	{80,   1, " Linenumb manual input"},
	{96,   2, " Linenumb auto"},
	{112,  3, " Linenumb nothing"},
	{0,0,0}
};

int in_linenumb_select()
{
	int cmd;
	
	text_save(0);
    lcd_clear(WHITE);
	cmd = menu_select(linenumb_select_menu);
	text_save(1);
	return cmd;							// 1:input, 2:auto, 3:nothing, -1:exit
}
/********************************************************************************/
/*		in_expr_statement														*/
/********************************************************************************/
const struct menu expr_statement[]={
	{80,    1, " ﾍﾝｽｳ"},
	{96,    2, " ｽｳﾁ"},
	{112,   3, " ｴﾝｻﾞﾝｼ"},
	{128,   4, " ｽｳﾁｶﾝｽｳ"},
	{144,   6, " ｶﾝｽｳ"},
	{160,   5, " ｺﾏﾝﾄﾞ"},
	{176,   7, " ﾓｼﾞﾚﾂ"},
//	{192,   8, "ｸﾞﾗﾌｨｯｸ"},
	{0,0,0}
};

int in_expr_statement()
{
	int cmd;
	
	text_save(0);
	lcd_clear(WHITE);
	prg_disp_kbuf(cal_v.linebuf, -1);
	for(;;){
		cmd = menu_select(expr_statement);
		if(cmd >= 0)
			break;
	}
	text_save(1);
	return cmd;
}
/********************************************************************************/
/*		in_linenumb_manu														*/
/********************************************************************************/
const struct menu2 linenumb_manu[]={
	{12,  80, 1000, "DEL"},
	{12,  96, KEY_cr, "Enter"},
	{12, 112, KEY_period, "."},
	{12, 128, KEY_minus, "-"},
	{12, 144, KEY_0,  "0"},
	{12, 160, KEY_1,  "1"},
	{12, 176, KEY_2,  "2"},
	{12, 192, KEY_3,  "3"},
	{12, 208, KEY_4,  "4"},
	{12, 224, KEY_5,  "5"},
	{84, 128, KEY_6,  "6"},
	{84, 144, KEY_7,  "7"},
	{84, 160, KEY_8,  "8"},
	{84, 176, KEY_9,  "9"},
	{0,0,0,0}
};

const struct menu2 variable_menu[]={
	{12,  80, 1000, "DEL"},
	{12,  96, KEY_cr, "Enter"},
	{12, 112, KEY_equal, "="},
	{12, 128, KEY_A,  "A"},
	{12, 144, KEY_B,  "B"},
	{12, 160, KEY_C,  "C"},
	{12, 176, KEY_D,  "D"},
	{12, 192, KEY_E,  "E"},
	{12, 208, KEY_F,  "F"},
	{84, 128, KEY_G,  "G"},
	{84, 144, KEY_H,  "H"},
	{84, 160, KEY_I,  "I"},
	{84, 176, KEY_J,  "J"},
	{84, 192, KEY_K,  "K"},
	{84, 208, KEY_L,  "L"},
	{0,0,0,0}
};

const struct menu2 operation_menu[]={
	{12,  80, 1000, "DEL"},
	{12,  96, KEY_cr, "Enter"},
	{12, 112, KEY_plus, "+"},
	{12, 128, KEY_minus,  "-"},
	{12, 144, KEY_div,  "/"},
	{12, 160, KEY_mul,  "*"},
	{12, 176, KEY_lkakko, "("},
	{12, 192, KEY_rkakko, ")"},
	{84,  80, KEY_eqeq,  "=="},
	{84,  96, KEY_neq,  "!="},
	{84, 112, KEY_gt,  ">"},
	{84, 128, KEY_ge,  ">="},
	{84, 144, KEY_lt,  "<"},
	{84, 160, KEY_le,  "<="},
	{84, 176, KEY_comma, ","},
	{0,0,0,0}
};

const struct menu2 command_menu[]={
	{12,  80, 1000, "DEL"},
	{12,  96, KEY_cr, "Enter"},
	{12, 112, KEY_prt, "Print"},
	{12, 128, KEY_save,  "Save"},
	{12, 144, KEY_load,  "Load"},
	{12, 160, KEY_run,  "Run"},
	{12, 176, KEY_list, "List"},
	{12, 192, KEY_help, "Help"},
	{84,  80, KEY_goto,  "Goto"},
	{84,  96, KEY_stop,  "Stop"},
	{84, 112, KEY_pause,  "Pause"},
	{84, 128, KEY_if,  "IF"},
	{84, 144, KEY_gosub,  "Gosub"},
	{84, 160, KEY_return,  "Return"},
	{84, 176, KEY_edit, "Edit"},
	{0,0,0,0}
};

const struct menu2 numb_func_menu[]={
	{12,  80, 1000, "DEL"},
	{12,  96, KEY_cr, "Enter"},
	{12, 112, KEY_sqrt, "Sqrt"},
	{12, 128, KEY_exp,  "Exp"},
	{12, 144, KEY_sin,  "Sin"},
	{12, 160, KEY_cos,  "Cos"},
	{12, 176, KEY_tan, "Tan"},
	{12, 192, KEY_ln, "ln"},
	{84,  80, KEY_log, "Log"},
	{84,  96, KEY_pow, "x^y"},
	{0,0,0,0}
};

const struct menu2 func_menu[]={
	{12,  80, 1000, "DEL"},
	{12,  96, KEY_cr, "Enter"},
	{12, 112, KEY_in, "Input"},
	{12, 128, KEY_size,  "Size"},
	{12, 144, KEY_sw_b,  "SW_B"},
	{12, 160, KEY_sw,  "SW"},
	{12, 176, KEY_rand,  "Rand"},
	{84,  80, KEY_rgb, "RGB"},
	{84,  96, KEY_pset, "Pset"},
	{84, 112, KEY_bakc, "BackC"},
	{84, 128, KEY_txtc, "TxtC"},
	{84, 144, KEY_gphc, "GraphC"},
	{84, 160, KEY_cls, "Cls"},
	{0,0,0,0}
};

/********************************************************************************/
/*		in_getline																*/
/*		cal_v.linebuf へ１行入力												*/
/********************************************************************************/
void in_getline()
{
	int ret, n, tmp;
	
	memset(cal_v.linebuf, 0, sizeof(cal_v.linebuf));
    
    /***    行番号入力  ***/
	ret = in_linenumb_select();				// 行番号の有り無しを入力
	if(ret==1){								// 行番号手入力
		ret = menu_input(linenumb_manu, cal_v.linebuf);
		n = 0;
		tmp = prg_int_number(cal_v.linebuf, &n);
		if(tmp > 0)
			cal_v.alinenumb = tmp;
		if(ret==1)							// Enter で終端した
			return;
	}
	else if(ret==2){						// 自動行番号
		if(cal_v.alinenumb==0)
			cal_v.alinenumb = 100;
		else
			cal_v.alinenumb += 10;
		i2a_k(cal_v.linebuf, cal_v.alinenumb);	// 行番号セット
	}
	
	/*** 選択 main	***/
	for(;;){
		ret = in_expr_statement();
		if(ret==1){							// 変数
			if(menu_input(variable_menu, cal_v.linebuf)==1)	// Enter で終端した
				return;
		}
		else if(ret==2){					// 数値
			if(menu_input(linenumb_manu, cal_v.linebuf)==1)	// Enter で終端した
				return;
		}
		else if(ret==3){					// 演算子
			if(menu_input(operation_menu, cal_v.linebuf)==1)	// Enter で終端した
				return;
		}
		else if(ret==4){					// 数値関数
			if(menu_input(numb_func_menu, cal_v.linebuf)==1)	// Enter で終端した
				return;
		}
		else if(ret==6){					// 関数
			if(menu_input(func_menu, cal_v.linebuf)==1)	// Enter で終端した
				return;
		}
		else if(ret==5){					// コマンド
			if(menu_input(command_menu, cal_v.linebuf)==1)	// Enter で終端した
				return;
		}
		else if(ret==7){					// 文字列
			n = strlen((char*)cal_v.linebuf);
			prg_str_keyin(cal_v.linebuf, &n);
		}
//		else if(ret==8){					// グラフィック
//			if(menu_input(graph_menu, cal_v.linebuf)==1)	// Enter で終端した
//				return;
//		}
	}
}
/********************************************************************************/
/*		cal_program																*/
/*		プログラム電卓モード													*/
/********************************************************************************/
int cal_program()
{
	if(setjmp(cal_v.env1)){
        sw_updownup();
    }
	lcd_clear(WHITE);
	for(;;){
		in_getline();			// 1行入力, prg_getline() 相当
		prg_buf_process();
	}
}
