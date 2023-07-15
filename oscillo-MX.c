#include "apps.h"


struct osci_v osci_v={
	(short*)&osci_v.buf[sizeof(osci_v.buf)/4-0x4000*2/4],	// disp_buf
	40-1,								// osci_PR3
	5000,								// no_of_data
	1,									// trigger_mode  1:Auto+, 2:Auto-, 3:Single+, 4:Single-
	0,									// trigger_point
	"[ms]0   1   2   3   4   5",		// osci_footer
	999,								// spe_PR3
	1024,								// spe_no_of_data
	"KHz 0   4   8   12  16  20",		// spe_footer
};


extern const char *osci_footer[];


void graph_hokan1(unsigned char high[251], unsigned char low[251]);
void graph_draw(unsigned char high[251], unsigned char low[251]);
void graph_set1(short *ptr, float step, unsigned char high[251], unsigned char low[251]);
void graph_set2(short *ptr, float step, unsigned char high[251], unsigned char low[251]);
void  graph_set(short *ptr, float step, unsigned char high[251], unsigned char low[251]);
void top_mark(unsigned char high[251], float db_top);
void graph_draw(unsigned char high[251], unsigned char low[251]);
void graph_memori();
void graph_memori_s();
void graph_memori_spe();
float cabs1(struct C *c1);
char *exp_sweep_speed(int *no_of_data);

/********************************************************************************/
/*		osci_init																*/
/********************************************************************************/
void osci_init()
{
	/***	ADC		***/
	AD1CON1 = 0;
	AD1CON1bits.ON = 1;
	AD1CON1bits.FORM = 0;				// Integer 16bit
	AD1CON1bits.SSRC = 2;				// Trigger source: Timer3
	AD1CON1bits.ASAM = 1;				// sample auto start
//	AD1CON1bits.SAMP = 1;				// sampling
	
	AD1CON2 = 0;
	AD1CON2bits.VCFG = 0;				// VREFH/L = AVDD/AVSS
	AD1CON2bits.CSCNA = 0;				// Do not scan inputs
	
	AD1CON3 = 0;
	AD1CON3bits.ADRC = 0;				// PBclk �g�p
	AD1CON3bits.ADCS = 0;				// 2*TPB=TAD = 0.05us
	
	AD1CHS = 0;
	AD1CHSbits.CH0NA = 0;				// negative input is VREFL
	AD1CHSbits.CH0NB = 0;				// negative input is VREFL
	AD1CHSbits.CH0SA = 8;				// positive input is AN8;
	AD1CHSbits.CH0SB = 8;				// positive input is AN8;
    /***    timer3    ***/
	PR3 = 40-1;							// 40MHz/40=1MHz
    TMR3 = 0;
    T3CON = 0;
    IPC3bits.T3IP = 5;
    IFS0bits.T3IF = 0;
//    IEC0bits.T3IE = 1;
    T3CONbits.ON = 1;
	
	IPC5bits.AD1IP = 5;
	IFS0bits.AD1IF = 0;
//	IEC0bits.AD1IE = 1;

	//DMA
	DMACON = 0x8000;						// DMA Enable
	
	//DMA channel 1
	DCH1CON = 0;
	DCH1CONbits.CHAEN = 0;					// channel automatic off
	DCH1ECONbits.CHSIRQ = _ADC_IRQ;
	DCH1ECONbits.SIRQEN = 1;				// channel start IRQ enable bit
	DCH1SSA = ((uint32_t) (&ADC1BUF0))&0x1FFFFFFF;
//	DCH1DSA = ((uint32_t) wave)&0x1FFFFFFF;
	DCH1SSIZ = 2;
	DCH1DSIZ = 100*2;						// ���]���o�C�g��
	DCH1CSIZ = 2;							// 1��̓]���o�C�g��

	//DMA channel 2
	DCH2CON = 0;
	DCH2CONbits.CHAEN = 1;					// channel automatic on
	DCH2ECONbits.CHSIRQ = _TIMER_3_IRQ;
	DCH2ECONbits.SIRQEN = 1;				// channel start IRQ enable bit
	DCH2SSA = ((uint32_t) m_ushort)&0x1FFFFFFF;
	DCH2DSA = ((uint32_t) (&LATB))&0x1FFFFFFF;
	DCH2SSIZ = 4095*2;
	DCH2DSIZ = 2;							// ���]���o�C�g��
	DCH2CSIZ = 2;							// 1��̓]���o�C�g��
}
/********************************************************************************/
/*		osci_measure_spe														*/
/********************************************************************************/
void osci_measure_spe()
{
	int i, flag, tmp /*, top_x, top_y*/;
	struct C *x, *X;
	float step, db, db_top;
	unsigned char high[251+1], low[251+1];
	short *s_buf;
//	short *disp_buf;
	
	sw_up();
	flag = 0;
	graph_memori_spe();
	sw_up();
	for(;;){
		/*** get data	***/
		PR3 = osci_v.spe_PR3;
		TMR3 = 0;
		DCH1CONbits.CHAEN = 0;					// channel automatic off
		DCH1DSA = ((uint32_t)osci_v.buf) & 0x1FFFFFFF;
		DCH1DSIZ = (osci_v.spe_no_of_data+100)*2;	// ���]���o�C�g��
		DCH1INTbits.CHBCIF = 0;				// ���荞�݃t���O�N���A
		DCH1CONbits.CHEN = 1;				// DMA start
		while(DCH1INTbits.CHBCIF==0){
			i = sw_read();
			if(i & 4){						// sw3
				sw_downup();
				return;
			}
		}
		/*** fft	***/
		s_buf = (short*)osci_v.buf;
		x = (struct C*)&s_buf[osci_v.spe_no_of_data+100]; // ���������蓖��
		X = &x[osci_v.spe_no_of_data];                       // ���������蓖��
		for(i=0; i<osci_v.spe_no_of_data; i++){
			x[i].r = (float)(s_buf[i+100]-512);
			x[i].i = 0;
		}
		if(osci_v.spe_no_of_data != 1024)
			message("osci_mesure_spe()\nspe_no_of_data=%d\n", osci_v.spe_no_of_data);
		fft_mado1024(x);
		fft1024(x, X);
		/*** �O���t	***/
//AAAAA		ptr = osci_v.disp_buf;				//(short*)&X[1024];
		db_top = -100;
		for(i=0; i<osci_v.spe_no_of_data/2; i++){
			db = 2*4*(20*log10f(cabs1(&X[i]))-2);
			osci_v.disp_buf[i] = (short)floor(db);
			if(db_top < db)
				db_top = db;
			if(osci_v.spe_no_of_data != 1024)
				osci_v.disp_buf[i] -= 80+16;
			if(osci_v.disp_buf[i] < 0)
				osci_v.disp_buf[i] = 0;
		}
		db_top = db_top/8 - 104;
        if(osci_v.spe_no_of_data != 1024)
            db_top -= 12;
		step = (250.0+0.5) / (osci_v.spe_no_of_data/2-1);
		graph_set(osci_v.disp_buf, step, high, low);
        
		graph_draw(high, low);				// �O���t�`��
		top_mark(high, db_top);				// top ���}�[�N����
        
		do{
			tmp = sw_read();
            if(tmp){
                sw_downup();
            }
			if(tmp & 1)						// sw1: start/stop
				flag = !flag;
			if(tmp & 4)						// sw3
				return;
		}
        while(flag);
	}
}
/********************************************************************************/
/*		graph_memori_spe														*/
/********************************************************************************/
void graph_memori_spe()
{
	static const char *msg[]={"+10","0dB","-10","-20","-30","-40","-50","-60","-70","-80","-90"};
	int i;

	lcd_clear(WHITE);
	
	back_color = BRUE;
	text_color = WHITE;
	lcd_disp_str_x1(112, 0, "sw1:stop");
	
	back_color = WHITE;
	text_color = BLACK;
	for(i=0; i<=10; i++){
		lcd_disp_str_x1(6, i*20+20-7, msg[i]);
	}

	lcd_disp_str_x1(0, 224, osci_v.spe_footer);
}
/********************************************************************************/
/*		getbuf_auto																*/
/*		return 0:normall, 4:sw-exit												*/
/*		v->trigger_mode 1:auto+, 2:auto-, 3:single+, 4:single-					*/
/********************************************************************************/
int osci_getbuf_auto()
{
	int i, flag;
	short tmp;
	short *s_buf;
//
//	DMA �̌��݈ʒu��m��ɂ� DCH1DPTR ���Q�Ƃ���
//

	s_buf = (short*)osci_v.buf;
	PR3 = osci_v.osci_PR3;				// 40MHz/(PR3+1)
    TMR3 = 0;
	DCH1CONbits.CHAEN = 0;					// channel automatic off
	DCH1DSA = ((uint32_t)osci_v.buf) & 0x1FFFFFFF;
	DCH1DSIZ = 0x4000*2;				// ���]���o�C�g���� 0x4000x2�A�f�[�^���� 0x4000�f�[�^
	DCH1INTbits.CHBCIF = 0;				// ���荞�݃t���O�N���A
	DCH1CONbits.CHEN = 1;				// DMA start
	while(DCH1INTbits.CHBCIF==0){
		i = sw_read();
		if(i & 4){						// sw3
			return 4;
		}
	}
	flag = 0;
	for(i=100; i<0x4000 - (osci_v.no_of_data+100); i++){		// 0x4000�f�[�^�𒲂ׂ�
		tmp = s_buf[i];
		if(osci_v.trigger_mode==1 || osci_v.trigger_mode==3){	// + trigger
			if(flag){
				if(tmp > 512){			// triggered
					memmove(osci_v.disp_buf, &s_buf[i-10], (osci_v.no_of_data+100) * 2);
					return 0;			// normal end
				}
			}
			if(flag==0){
				if(tmp < 512-20)
					flag = 1;
			}
		}
		else{							// - trigger
			if(flag){
				if(tmp < 512){			// triggered
					memmove(osci_v.disp_buf, &s_buf[i-10], (osci_v.no_of_data+100) * 2);
					return 0;			// normal end
				}
			}
			if(flag==0){
				if(tmp > 512+20)
					flag = 1;
			}
		}
	}
	memmove(osci_v.disp_buf, &s_buf[i-10], (osci_v.no_of_data+100) * 2);
	return 0;						// normal end
}
/********************************************************************************/
/*		osci_move_data															*/
/*		s_buf: 0x4000�f�[�^�̃����O�o�b�t�@										*/
/*		disp_buf: 0x4000�f�[�^�̃X�g���[�g�o�b�t�@								*/
/********************************************************************************/
void osci_move_data(int src)
{
	int i;
	short *s_buf = (short*)osci_v.buf;
	
	for(i=0; i<0x4000; i++){
		osci_v.disp_buf[i] = s_buf[(src+i)&0x3fff];
	}
}
/********************************************************************************/
/*		getbuf_single															*/
/*		return 0:normall, 4:sw-exit												*/
/*		v->trigger_mode 1:auto+, 2:auto-, 3:single+, 4:single-					*/
/********************************************************************************/
int osci_getbuf_single()
{
//	int i, flag, tmp;
//
//	DMA �̌��݈ʒu��m��ɂ� DCH1DPTR ���Q�Ƃ���
//
	short *s_buf = (short*)osci_v.buf;

	PR3 = osci_v.osci_PR3;				// 40MHz/(PR3+1)
    TMR3 = 0;
	DCH1CONbits.CHAEN = 1;				// channel continuously enabled
	DCH1DSA = ((uint32_t)osci_v.buf) & 0x1FFFFFFF;
	DCH1DSIZ = 0x4000*2;				// ���]���o�C�g�� 0x4000x2�A�f�[�^�� 0x4000 �̃����O�o�b�t�@�ɂ���
	DCH1INTbits.CHBCIF = 0;				// ���荞�݃t���O�N���A
	DCH1CONbits.CHEN = 1;				// DMA start
	while(DCH1DPTR < 0x4000*2-0x3000)
		;
	if(osci_v.trigger_mode==3){			// +trigger
		for(;;){
			if(s_buf[(DCH1DPTR/2-1)&0x3fff] < 512-20)
				break;
			if(SW123_PORT_DATA & SW3_BIT)
				return 4;				// sw3 �ɂ�� exit
		}
		for(;;){
			if(s_buf[(DCH1DPTR/2-1)&0x3fff] > 512)
				break;
			if(SW123_PORT_DATA & SW3_BIT)
				return 4;				// sw3 �ɂ�� exit
		}
	}
	else{								// -trigger
		for(;;){
			if(s_buf[(DCH1DPTR/2-1)&0x3fff] > 512+20)
				break;
			if(SW123_PORT_DATA & SW3_BIT)
				return 4;				// sw3 �ɂ�� exit
		}
		for(;;){
			if(s_buf[(DCH1DPTR/2-1)&0x3fff] < 512)
				break;
			if(SW123_PORT_DATA & SW3_BIT)
				return 4;				// sw3 �ɂ�� exit
		}
	}
	osci_v.trigger_point = DCH1DPTR >> 1;
	while((((DCH1DPTR>>1)-osci_v.trigger_point)&0x3fff) < 0x3800)
		;
	DCH1CONbits.CHEN = 0;				// DMA stop
	osci_move_data((osci_v.trigger_point - osci_v.no_of_data/5) & 0x3fff);	// �g���K�|�C���g�̎�O����\������
	return 0;
}
/********************************************************************************/
/*		osci_measure															*/
/********************************************************************************/
void osci_measure()
{
    int i, tmp, pos, no_of_data;
	unsigned char high[251], low[251];
	float step;
	char *footer;
	
	graph_memori();
	sw_up();
	for(;;){
		if(osci_v.trigger_mode >= 3){		// single trigger
			memset(high, 0, sizeof(high));
			memset(low,  0, sizeof(low));
			graph_draw(high, low);			// �r���̂ݕ\��
		}
		if(osci_v.trigger_mode >= 3){		// single trigger
			if(osci_getbuf_single()==4){	// sw3 �ɂ�� exit
				sw_downup();
				return;
			}
		}
		else{								// auto trigger
			if(osci_getbuf_auto()){			// get data
				sw_downup();
				return;						// sw3: exit
			}
		}
		step = 250.0/(osci_v.no_of_data);
		graph_set(osci_v.disp_buf+10, step, high, low);
		graph_draw(high, low);

		tmp = sw_read();
		if(tmp & 1)						// sw1  scrnshot �̂�
			sw_downup();
		if(tmp & 4){                 	// sw3
			sw_downup();
			return;
		}

		if(osci_v.trigger_mode >= 3 || (tmp & 2)){	// single trigger || sw2
			if(osci_v.trigger_mode <= 2)					// Auto
				osci_v.trigger_point = 0x1000;
			for(;;){
				if(tmp){
					sw_downup();
				}
				if(tmp & 1){					// sw1: trigger ready, start
					break;
				}
				else if(tmp & 2){				// sw2: scroll
					footer = exp_sweep_speed(&no_of_data);
					lcd_clear(WHITE);
					if(footer==0)			// sw3
						return;
					graph_memori_s();
					lcd_disp_str_x1(0, 224, footer);
					pos = osci_v.trigger_point;
					for(;;){
						i = sw_read();
						if(i & 1){			// sw1: right
							pos += no_of_data/10;
						}
						else if(i & 2){		// sw2: left
							pos -= no_of_data/10;
						}
						else if(i & 4){		// sw3
							sw_downup();
							footer = exp_sweep_speed(&no_of_data);
							lcd_clear(WHITE);
							if(footer==0)			// sw3 �ɂ� menu �� exit ����
								return;
							graph_memori_s();
							lcd_disp_str_x1(0, 224, footer);
						}
						while(pos > 0x4000-no_of_data)
							pos -= no_of_data/10;
						while(pos < 0)
							pos += no_of_data/10;
						osci_move_data(pos);
						step = 250.0/no_of_data;
						graph_set(osci_v.disp_buf+10, step, high, low);
						graph_draw(high, low);
					}
				}
				else if(tmp & 4){				// sw3: exit
					return;
				}
				tmp = sw_read();
			}
		}
	}
}
/********************************************************************************/
/*		graph_memori															*/
/********************************************************************************/
void graph_memori_s()
{
	static const char *msg1[] = {"  5 ","  4 ","  3 ","  2 ","  1 ","  0 "," -1 "," -2 "," -3 "," -4 "," -5 "};
	int i;

	lcd_clearA(0,0, 47,239, WHITE);
	for(i=0; i<=10; i++){
		lcd_disp_str_x1(6, i*20+20-7, msg1[i]);
	}
}

void graph_memori()
{
	lcd_clear(WHITE);
	
	back_color = BRUE;
	text_color = WHITE;
	if(osci_v.trigger_mode <= 2)		// auto
		lcd_disp_str_x1(112, 0, "sw1:stop");
	else								// single trigger
		lcd_cdisp_str_x1(0, "sw1:clr,sw2:menu");
	
	back_color = WHITE;
	text_color = BLACK;
	
	graph_memori_s();
	lcd_disp_str_x1(0, 224, osci_v.osci_footer);
}
/********************************************************************************/
/*		graph_test																*/
/********************************************************************************/
void graph_test()
{
	unsigned char high[251], low[251];
	int i;
	
	for(i=0; i<251; i++){
		high[i] = low[i] = (int)(100*(sinf(5*3.14159f*i/250)) + 128);
	}
	graph_hokan1(high, low);
	graph_draw(high, low);
}
/********************************************************************************/
/*      top_mark                                                                */
/********************************************************************************/
void top_mark_s(int x, int y)
{
    if(x>=48 && x<=298 && y>=20 && y<=220)
        lcd_clearA(x,y, x,y, BRUE);
}

void top_mark(unsigned char high[251], float db_top)
{
    int i, top_x, top_y;
    char str[50];
    
    top_x = top_y = -1;
    for(i=0; i<251; i++){
        if(top_y < high[i]){
            top_y = high[i];
            top_x = i;
        }
    }
	top_x = 48  + top_x;
	top_y = 248 - top_y;
	for(i=-1; i<=1; i++){
		top_mark_s(top_x+i, top_y-1);
		top_mark_s(top_x+i, top_y+1);
	}
	top_mark_s(top_x+1, top_y);
	top_mark_s(top_x-1, top_y);
	
	sprintf(str, "%3.1fdB", (double)db_top);
	lcd_disp_str_x1(210, 22, str);
}
/********************************************************************************/
/*		graph_draw																*/
/*		�O���t�G���A�� 251x201 ���W�� (48,20)									*/
/*		high[251] �� low[251] ��`�悷��, data �� 28�`228 �܂ł�`��			*/
/*		back_color, keisen_color, graph_color ���g�p����						*/
/********************************************************************************/
void graph_draw(unsigned char high[251], unsigned char low[251])
{
	int x, y, off;
	
	off = 28;
	back_color = WHITE;
	text_color = BLACK;
	keisen_color = GRAY;
	graph_color  = RED;
	LCD_ALL_S;								// all set, idle
	LCD_CS_C;								// cs clear
	lcd_area_set(48, 20, 298, 220);
	lcd_write_cmd(MemoryWrite);
    for(y=200+off; y>=0+off; y--){
		for(x=0; x<251; x++){
			if(high[x] >= y && y >= low[x])
				lcd_write_color(graph_color);
			else if((y-off)%20==0 || x%25==0)
				lcd_write_color(keisen_color);
			else
				lcd_write_color(back_color);
		}
	}
	LCD_ALL_S;								// idle
}
/********************************************************************************/
/*		graph_dataset_test														*/
/********************************************************************************/
void graph_dataset_test()
{
	unsigned char high[251], low[251];
	short ptr[5000];
	float step = 5.3;
	int i;
	float p;
	
	p = 0;
	for(i=0; i<5000; i++){
		ptr[i] = (int)(300*(sinf(10*3.14159f*p/250)) + 512);
		p += step;
	}
	graph_set(ptr+100, step, high, low);
	graph_draw(high, low);
}
/********************************************************************************/
/*		graph_dataset															*/
/*		�O���t�G���A�� 251x201 ���W�� (48,20)									*/
/*		step �̒l�� graph_dataset1 ���� graph_dataset2 ���Ă�					*/
/*		*ptr �� 10bit data �� ��� 8bit ���̗p����								*/
/********************************************************************************/
void graph_set(short *ptr, float step, unsigned char high[251], unsigned char low[251])
{
	if(step > 1)
		graph_set2(ptr, step, high, low);	// �f�[�^���a
	else
		graph_set1(ptr, step, high, low);	// �f�[�^����
}
/********************************************************************************/
/*		graph_dataset2															*/
/*		�O���t�G���A�� 251x201 ���W�� (48,20)									*/
/*		�f�[�^���a�ȏꍇ�Astep >= 1												*/
/*		*ptr �� 10bit data �� ��� 8bit ���̗p����								*/
/********************************************************************************/
void graph_set2(short *ptr, float step, unsigned char high[251], unsigned char low[251])
{
	float p, x, y, y0, y1, y2, y3, a, b, c;
	int i, pp, tmp;
	
	if(step < 1){							// error
		memset(high, 128, 251);
		memset(low,  128, 251);
		return;
	}
	/*** ��Ԃ��� y = ax*x*x + bx*x + cx	***/
	p = 0;									// x ���W: 0, step, 2*step, ...
	pp = 0;									// x ���W: 0, 1, 2, ...
	i = 0;
	for(;;){
		y0 = (float)ptr[pp-1];
		y1 = (float)ptr[pp] - y0;
		y2 = (float)ptr[pp+1] - y0;
		y3 = (float)ptr[pp+2] - y0;
        a = (y1/2 - y2/2 + y3/6)/(step*step*step);
        b = (-y1*5/2 + y2*2 - y3/2)/(step*step);
        c = (y1*3 - y2*3/2 + y3/3)/step;
		
		for( ; i<251; i++){
			if(i > p+step)
				break;
			x = i-(p-step);
			y = a*x*x*x + b*x*x + c*x;
            tmp = (int)(y+y0+2)>>2;
            if(tmp<0) tmp = 0;
            if(tmp>255) tmp = 255;
			high[i] = low[i] = tmp;
		}
		if(i>=251)
			break;
		p += step;
		pp++;
	}
	graph_hokan1(high, low);
}

/********************************************************************************/
/*		graph_dataset1															*/
/*		�O���t�G���A�� 251x201 ���W�� (48,20)                                    */
/*		�f�[�^�����ȏꍇ�Astep <= 1												*/
/*		*ptr �� 10bit data �� ��� 8bit ���̗p����								*/
/********************************************************************************/
void graph_set1(short *ptr, float step, unsigned char high[251], unsigned char low[251])
{
	float p;
	int i, d, pp;
	char flag[251];
	
	if(step > 1){							// error
		memset(high, 128, 251);
		memset(low,  128, 251);
		return;
	}
	memset(flag, -1, sizeof(flag));
	p = 0;
	pp = 0;
	for(;;){
		i = (int)(p+0.5);
		if(i >= 251)
			break;
		d = ptr[pp++]>>2;					// ��� 8bit �ɂ���
		if(flag[i]<0){
			flag[i] = 0;
			high[i] = low[i] = d;
		}
		else{
			if(d > high[i])
				high[i] = d;
			if(d < low[i])
				low[i] = d;
		}
		p += step;
	}
	graph_hokan1(high, low);
}
/********************************************************************************/
/*		graph_hokan1															*/
/*		�O���t�G���A�� 251x201 ���W�� (48,20)									*/
/*		high[251] �� low[251] �͔�΂��ɑS���L���f�[�^�������Ă���				*/
/*		�O���t���㉺�����ڑ�����												*/
/********************************************************************************/
void graph_hokan1(unsigned char high[251], unsigned char low[251])
{
	int i, tmp;
	
	for(i=0; i<251; i++){
		if(i != 0){
			if(low[i-1] > high[i]){
				tmp = (low[i-1]+high[i])/2;
				high[i] = tmp;
				low[i-1] = tmp+1;
			}
			if(low[i] > high[i-1]){
				tmp = (low[i]+high[i-1])/2;
				high[i-1] = tmp;
				low[i] = tmp+1;
			}
		}
		if(i != 250){
			if(low[i+1] > high[i]){
				tmp = (low[i+1]+high[i])/2;
				high[i] = tmp;
				low[i+1] = tmp+1;
			}
			if(low[i] > high[i+1]){
				tmp = (low[i]+high[i+1])/2;
				high[i+1] = tmp;
				low[i] = tmp+1;
			}
		}
	}
}
/********************************************************************************/
/*		osci_spectrum_setting													*/
/********************************************************************************/
const struct menu2 spectrum_menu[]={
	{0,   50,   1, "500KHz"},
	{0,   66,   2, "200KHz"},
	{0,   82,   3, "100KHz"},
	{0,   98,   4, " 50KHz"},
	{0,  114,   5, " 20KHz"},
	{0,  130,   6, " 10KHz"},
	{0,  146,   7, "  5KHz"},
	{0,  162,   8, "  2KHz"},
	{0,  178,   9, "  1KHz"},
	{0,  194,  10, " 500Hz"},
//	{156, 50,  31, "500KHz(4096)"},
//	{156, 66,  32, "200KHz(4096)"},
//	{156, 82,  33, "100KHz(4096)"},
//	{156, 98,  34, " 50KHz(4096)"},
//	{156, 114, 35, " 20KHz(4096)"},
//	{156, 130, 36, " 10KHz(4096)"},
//	{156, 146, 37, "  5KHz(4096)"},
//	{156, 162, 38, "  2KHz(4096)"},
//	{156, 178, 39, "  1KHz(4096)"},
//	{156, 194, 40, " 500Hz(4096)"},
	{0,0,0}
};
const int spe_PR3[]			={	39,		// 40MHz/(   39+1)=1MHz   �� 500KHz
								99,		// 40MHz/(   99+1)=400KHz �� 200KHz
								199,	// 40MHz/(  199+1)=200KHz �� 100KHz
								399,	// 40MHz/(  399+1)=100KHz ��  50KHz
								999,	// 40MHz/(  999+1)= 40KHz ��  20KHz
								1999,	// 40MHz/( 1999+1)= 20KHz ��  10KHz
								3999,	// 40MHz/( 3999+1)= 10KHz ��   5KHz
								9999,	// 40MHz/( 9999+1)=  4KHz ��   2KHz
								19999,	// 40MHz/(19999+1)=  2KHz ��   1KHz
								39999,	// 40MHz/(39999+1)=  1KHz ��  500Hz
};

const char *spe_footer[]   ={"KHz 0  100 200 300 400 500",
                             "KHz 0  40  80  120 160 200",
                             "KHz 0  20  40   60  80 100",
                             "KHz 0  10  20   30  40  50",
                             "KHz 0   4   8   12  16  20",
                             "KHz 0   2   4   6   8   10",
                             "KHz 0   1   2   3   4   5",
                             "KHz 0  0.4 0.8 1.2 1.6  2",
                             "KHz 0  0.2 0.4 0.6 0.8  1",
                             " Hz 0  100 200 300 400 500",
};

void osci_spectrum_setting()
{
	int cmd;
	
	lcd_clear(WHITE);
	back_color = BRUE;
	text_color = WHITE;
	lcd_cdisp_str_x2(4, "Max Frequency");
	back_color = WHITE;
	text_color = BLACK;
	cmd = menu_select2(spectrum_menu);
	if(cmd==-1)									// sw3: exit
		return;
	osci_v.spe_PR3 = spe_PR3[cmd-1];
	osci_v.spe_footer = spe_footer[cmd-1];
	osci_v.spe_no_of_data = 1024;
}
/********************************************************************************/
/*		osci_trigger_mode														*/
/********************************************************************************/
const struct menu trigger_mode_menu[]={
	{50,  1, "Auto+"},
	{70,  2, "Auto-"},
	{90,  3, "Single+"},
	{110, 4, "Single-"},
	{0,0,0}
};

void osci_trigger_mode()
{
	int cmd;
	
	lcd_clear(WHITE);
	back_color = BRUE;
	text_color = WHITE;
	lcd_disp_str_x2(52, 5, "�ضްӰ��");
	back_color = WHITE;
	text_color = BLACK;
	cmd = menu_select(trigger_mode_menu);
	if(cmd== -1)
		return;
	osci_v.trigger_mode = cmd;
}
/********************************************************************************/
/*		exp_sweep_speed															*/
/*		return 0:sw3 exit														*/
/********************************************************************************/
const struct menu exp_1Msps_menu[]={
	{40,  0, "20us  (20 sample)"},
	{60,  1, "50us  (50 sample)"},
	{80,  2, "100us (100 sample)"},
	{100, 3, "200us (200 sample)"},
	{120, 4, "500us (500 sample)"},
	{140, 5, "1ms   (1000 sample)"},
	{160, 6, "2ms   (2000 sample)"},
	{180, 7, "5ms   (5000 sample)"},
	{200, 8, "10ms  (10000 sample)"},
	{0,0,0}
};
const int no_of_data_1Msps[]={20,50,100,200,500,1000,2000,5000,10000};

const struct menu exp_500Ksps_menu[]={
	{40,  0, "50us  (25 sample)"},
	{60,  1, "100us (50 sample)"},
	{80,  2, "200us (100 sample)"},
	{100, 3, "500us (250 sample)"},
	{120, 4, "1ms   (500 sample)"},
	{140, 5, "2ms   (1000 sample)"},
	{160, 6, "5ms   (2500 sample)"},
	{180, 7, "10ms  (5000 sample)"},
	{200, 8, "20ms  (10000 sample)"},
	{0,0,0}
};
const int no_of_data_500Ksps[]={25,50,100,250,500,1000,2500,5000,10000};

const struct menu exp_200Ksps_menu[]={
	{40,  0, "100us (20 sample)"},
	{60,  1, "200us (40 sample)"},
	{80,  2, "500us (100 sample)"},
	{100, 3, "1ms   (200 sample)"},
	{120, 4, "2ms   (400 sample)"},
	{140, 5, "5ms   (1000 sample)"},
	{160, 6, "10ms  (2000 sample)"},
	{180, 7, "20ms  (4000 sample)"},
	{200, 8, "50ms  (10000 sample)"},
	{0,0,0}
};
const int no_of_data_200Ksps[]={20,40,100,200,400,1000,2000,4000,10000};

const struct menu exp_100Ksps_menu[]={
	{40,  0, "200us (20 sample)"},
	{60,  1, "500us (50 sample)"},
	{80,  2, "1ms   (100 sample)"},
	{100, 3, "2ms   (200 sample)"},
	{120, 4, "5ms   (500 sample)"},
	{140, 5, "10ms  (1000 sample)"},
	{160, 6, "20ms  (2000 sample)"},
	{180, 7, "50ms  (5000 sample)"},
	{200, 8, "100ms (10000 sample)"},
	{0,0,0}
};
const int no_of_data_100Ksps[]={20,50,100,200,500,1000,2000,5000,10000};

const struct menu exp_50Ksps_menu[]={
	{40,  0, "500us (25 sample)"},
	{60,  1, "1ms   (50 sample)"},
	{80,  2, "2ms   (100 sample)"},
	{100, 3, "5ms   (250 sample)"},
	{120, 4, "10ms  (500 sample)"},
	{140, 5, "20ms  (1000 sample)"},
	{160, 6, "50ms  (2500 sample)"},
	{180, 7, "100ms (5000 sample)"},
	{200, 8, "200ms (10000 sample)"},
	{0,0,0}
};
const int no_of_data_50Ksps[]={25,50,100,250,500,1000,2500,5000,10000};

const struct menu exp_20Ksps_menu[]={
	{40,  0, "1ms   (20 sample)"},
	{60,  1, "2ms   (40 sample)"},
	{80,  2, "5ms   (100 sample)"},
	{100, 3, "10ms  (200 sample)"},
	{120, 4, "20ms  (400 sample)"},
	{140, 5, "50ms  (1000 sample)"},
	{160, 6, "100ms (2000 sample)"},
	{180, 7, "200ms (4000 sample)"},
	{200, 8, "500ms (10000 sample)"},
	{0,0,0}
};
const int no_of_data_20Ksps[]={20,40,100,200,400,1000,2000,4000,10000};

const struct menu exp_10Ksps_menu[]={
	{40,  0, "2ms   (20 sample)"},
	{60,  1, "5ms   (50 sample)"},
	{80,  2, "10ms  (100 sample)"},
	{100, 3, "20ms  (200 sample)"},
	{120, 4, "50ms  (500 sample)"},
	{140, 5, "100ms (1000 sample)"},
	{160, 6, "200ms (2000 sample)"},
	{180, 7, "500ms (5000 sample)"},
	{200, 8, "1s    (10000 sample)"},
	{0,0,0}
};
const int no_of_data_10Ksps[]={20,50,100,200,500,1000,2000,5000,10000};


char *exp_sweep_speed(int *no_of_data)
{
	char str[50];
	int cmd;
	
	lcd_clear(WHITE);
	back_color = BRUE;
	text_color = WHITE;
	lcd_cdisp_str_x2(0, "�ճ ��۰�");
	back_color = WHITE;
	text_color = BLACK;
	switch(osci_v.osci_PR3){
		case 39:							// 1Msps
			cmd = menu_select(exp_1Msps_menu);
			if(cmd < 0)
				break;
			*no_of_data = no_of_data_1Msps[cmd];
			return (char*)osci_footer[cmd];
		case 79:							// 500Ksps
			cmd = menu_select(exp_500Ksps_menu);
			if(cmd < 0)
				break;
			*no_of_data = no_of_data_500Ksps[cmd];
			return (char*)osci_footer[cmd+1];
		case 199:							// 200Ksps
			cmd = menu_select(exp_200Ksps_menu);
			if(cmd < 0)
				break;
			*no_of_data = no_of_data_200Ksps[cmd];
			return (char*)osci_footer[cmd+2];
		case 399:							// 100Ksps
			cmd = menu_select(exp_100Ksps_menu);
			if(cmd < 0)
				break;
			*no_of_data = no_of_data_100Ksps[cmd];
			return (char*)osci_footer[cmd+3];
		case 799:							// 50Ksps
			cmd = menu_select(exp_50Ksps_menu);
			if(cmd < 0)
				break;
			*no_of_data = no_of_data_50Ksps[cmd];
			return (char*)osci_footer[cmd+4];
		case 1999:							// 20Ksps
			cmd = menu_select(exp_20Ksps_menu);
			if(cmd < 0)
				break;
			*no_of_data = no_of_data_20Ksps[cmd];
			return (char*)osci_footer[cmd+5];
		case 3999:							// 10Ksps
			cmd = menu_select(exp_10Ksps_menu);
			if(cmd < 0)
				break;
			*no_of_data = no_of_data_10Ksps[cmd];
			return (char*)osci_footer[cmd+6];
		default:
			sprintf(str, "osci_PR3=%d", osci_v.osci_PR3);
			fatal("exp_sweep_speed", str, 0);
			return 0;
	}
	return 0;
}
/********************************************************************************/
/*		osci_sweep_speed														*/
/********************************************************************************/
const struct menu sweep_speed_menu[]={
	{0,    1, "20us  (1Msps)"},
	{16,   2, "50us  (1Msps)"},
	{32,   3, "100us (1Msps)"},
	{48,   4, "200us (1Msps)"},
	{64,   5, "500us (1Msps)"},
	{80,   6, "1ms   (1Msps)"},
	{96,   7, "2ms   (1Msps)"},
	{112,  8, "5ms   (1Msps)"},
	{128,  9, "10ms  (1Msps)"},
	{144, 10, "20ms  (500Ksps)"},
	{160, 11, "50ms  (200Ksps)"},
	{176, 12, "100ms (100Ksps)"},
	{192, 13, "200ms (50Ksps)"},
	{208, 14, "500ms (20Ksps)"},
	{224, 15, "1s    (10Ksps)"},
	{0,0,0}
};
//                          01234567890123456789012345
const char *osci_footer[]={
                           "[us]0   4   8   12  16  20",
                           "[us]0  10  20   30  40  50",
                           "[us]0  20  40   60  80 100",
                           "[us]0  40  80  120 160 200",
                           "[us]0  100 200 300 400 500",
                           "[ms]0  0.2 0.4 0.6 0.8  1",
                           "[ms]0  0.4 0.8 1.2 1.6  2",
                           "[ms]0   1   2   3   4   5",
                           "[ms]0   2   4   6   8   10",
                           "[ms]0   4   8   12  16  20",
                           "[ms]0  10  20   30  40  50",
                           "[ms]0  20  40   60  80 100",
                           "[ms]0  40  80  120 160 200",
                           "[ms]0  100 200 300 400 500",
                           "[s] 0  0.2 0.4 0.6 0.8  1",
};

const int osci_PR3[]={39,39,39,39,39,39,39,39,39,79,199,399,799,1999,3999};

const int no_of_data[]={20,50,100,200,500,1000,2000,5000,10000,10000,10000,10000,10000,10000,10000};

void osci_sweep_speed()
{
	int cmd;
	
	lcd_clear(WHITE);
	back_color = WHITE;
	text_color = BLACK;
	cmd = menu_select(sweep_speed_menu);
	if(cmd== -1)							// sw3: exit
		return;
	osci_v.osci_PR3    = osci_PR3[cmd-1];
	osci_v.no_of_data  = no_of_data[cmd-1];
	osci_v.osci_footer = osci_footer[cmd-1];
}
/********************************************************************************/
/*		game																	*/
/********************************************************************************/
const struct menu game_menu[]={
	{0,    0, "ׯ���ܰ"},
	{20,   1, "���"},
	{40,   2, "������"},
	{60,   3, "������(fast)"},
    {80,   5, "ʺ��ѽ�"},
    {100,  6, "���޸"},
	{0,0,0}
};

void game_main()
{
	int cmd;

	lcd_clear(WHITE);
	cmd = menu_select(game_menu);
	switch(cmd){
		case 0:
			rush_main();
			break;
		case 1:								// Othello
			othe_main();
			break;
		case 2:								// pentomino
			pentomino(0);
			break;
		case 3:								// pentomino(fast)
			pentomino(1);
			break;
        case 5:
			hako_main();
            break;
        case 6:
			su_main();
            break;
	}
}
/********************************************************************************/
/*		oscillo																	*/
/********************************************************************************/
const struct menu main_menu[]={
	{50,   8, "Game"},
	{70,   1, "Oscillo measure"},
	{90,   2, "Oscillo setup"},
	{110,  3, "Spectrumanalizer measure"},
	{130,  4, "Spectrumanalizer setup"},
	{150,  5, "Frequency response(M����)"},
	{170,  6, "File manager"},
	{190,  7, "Program Calculator"},
	{0,0,0}
};

void oscillo()
{
    int cmd;
    
//    osci_title();
//    wait_ms(2000);
//	lcd_test();
//	sw_updownup();
	pentomino(0);
	
	for(;;){
		lcd_clear(WHITE);
		back_color = BRUE;
		text_color = WHITE;
		lcd_disp_str_x2(52, 5, "Main Menu");
		back_color = WHITE;
		text_color = BLACK;
		cmd = menu_select(main_menu);
		switch(cmd){
			case 1:							// Oscillo measure
				osci_measure();
				break;
			case 2:							// Oscillo setup
				osci_sweep_speed();
				osci_trigger_mode();
				osci_measure();
				break;
			case 3:							// Spectrumanalizer measure
				osci_measure_spe();
				break;
			case 4:							// Spectrumanalizer setup
				osci_spectrum_setting();
				osci_measure_spe();
				break;
			case 5:							// Frequency response
				m_measure();
				break;
			case 6:							// File manager
				f7_main();
				break;
			case 7:							// program calculator
				cal_program();
				break;
			case 8:							// Game
				game_main();
				break;
			default:						// sw3: exit
                return;
				break;
		}
	}
}
/********************************************************************************/
/*		menu_select																*/
/*		return: cmd ��Ԃ��Aexit ���� -1										*/
/********************************************************************************/
int menu_select(const struct menu *tbl)
{
	int i, key, tmp;
	
	for(i=0; tbl[i].msg ; ++i){
		lcd_disp_str_x1(0, tbl[i].line_numb, tbl[i].msg);
	}
	i = 0;
	for(;;){
		tmp = back_color;
		back_color = text_color;
		text_color = tmp;
		lcd_disp_str_x1(0, tbl[i].line_numb, tbl[i].msg);	// ���]�\��
		tmp = back_color;
		back_color = text_color;
		text_color = tmp;
		
		key = sw_updownup();
//		sw_up();
		if(key & 1){						// sw1
			lcd_disp_str_x1(0, tbl[i].line_numb, tbl[i].msg);	// �񔽓]�\��
			if(tbl[++i].msg==0)
				i = 0;
		}
		else if(key & 2){					// sw2
			return tbl[i].cmd;
		}
		else if(key & 4){					// sw3: exit
			return -1;
		}
	}
}

int menu_select2(const struct menu2 *tbl)
{
	int i, key, tmp;
	
	for(i=0; tbl[i].msg ; ++i){
		lcd_disp_str_x1(tbl[i].x, tbl[i].y, tbl[i].msg);
	}
	i = 0;
	for(;;){
		tmp = back_color;
		back_color = text_color;
		text_color = tmp;
		lcd_disp_str_x1(tbl[i].x, tbl[i].y, tbl[i].msg);	// ���]�\��
		tmp = back_color;
		back_color = text_color;
		text_color = tmp;
		
		key = sw_updownup();
//		sw_up();
		if(key & 1){						// sw1
			lcd_disp_str_x1(tbl[i].x, tbl[i].y, tbl[i].msg);	// �񔽓]�\��
			if(tbl[++i].msg==0)
				i = 0;
		}
		else if(key & 2){					// sw2
			return tbl[i].cmd;
		}
		else if(key & 4){					// sw3
			return -1;
		}
	}
}
/********************************************************************************/
/*		sw																		*/
/*		bit0 : sw1																*/
/*		bit1 : sw2																*/
/*		bit2 : sw3																*/
/********************************************************************************/
int sw_readA()
{
	return (SW123_PORT_DATA & (SW1_BIT | SW2_BIT | SW3_BIT)) >> 9;
}

int sw_read()
{
	SYS_Tasks();
//    buzzer_flag = 1;                    // �Z��(50ms 1KHz)
	return sw_readA();
}

void sw_up()
{
	int i;
	
	/*** 50ms sw ���͖������m�F����	***/
	for(;;){
		for(i=0; i<50; i++){
			wait_1ms();
            if(sw_read())
				break;
		}
		if(i==50)
			break;
	}
//	sw_add = 0;
}

int sw_down()
{
	int i, tmp1, tmp2;
	
	/*** �������͂� 10ms �������� ret	***/
	tmp1 = 0;
	for(;;){
		for(i=0; i<10; i++){
			wait_1ms();
			tmp2 = sw_read();
			if(tmp2==0 || tmp1 != tmp2){
				tmp1 = tmp2;
				break;
			}
		}
		if(i==10)
			return tmp1;
	}
}
int sw_downup()
{
	int sw, time1;

	sw = sw_down();						// �������͂� 10ms ������
	time1 = TickCount;
	sw_up();
    buzzer_flag = 1;                    // �Z��(50ms 1KHz)
	if(TickCount-time1 > 20000){		// 2�b�ȏ㉟���Ă�����X�N���[���V���b�g
        buzzer_flag = 2;                // ����(500ms 1KHz)
		f7_scrn_shot();
        buzzer_flag = 2;                // ����(500ms 1KHz)
	}
	return sw;
}
int sw_updownup()
{
	sw_up();
	return sw_downup();
}
/********************************************************************************/
/*		osci_title																*/
/********************************************************************************/
void osci_title()
{
	lcd_clear(GRAY);					// �D�F��ʂɂ���
	lcd_clearA(60, 100, 260, 143, RED);
	back_color = WHITE;
	text_color = BRUE;
	lcd_disp_str_x2(64, 106, "��۽����");
	back_color = WHITE;
	text_color = BLACK;
	lcd_disp_str_x1(18, 190, "Copyright(c) K.Fukumoto");
}
