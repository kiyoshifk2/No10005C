#include "apps.h"


#define macrox(pos) ((pos)%6)
#define macroy(pos) ((pos)/6)
#define macropos(x,y) ((x)+(y)*6)
#define U (-6)
#define D 6
#define L (-1)
#define R 1


extern const char hako_kif[][7*6];

const int hako_color[12]={
	0xc618,0x53d7,0xcd7f,0x0e5f,0xb73d,0x1f36,0xeed3,0xbc8e,0xa254,0xca47,0x4d84,0xed00,
};
const char hako_ban_dat[7][6]={
	{-100, -100, -100, -100, -100,-100},
	{-100,   11,   22,   22,   31,-100},
	{-100,   11,   22,   22,   31,-100},
	{-100,   41,   53,   53,   61,-100},
	{-100,   41,   74,   84,   61,-100},
	{-100,   94,    0,    0,  104,-100},
	{-100, -100, -100, -100, -100,-100}
};

char hako_ban[7][6];
char *hako_banp = (char*)hako_ban;

/********************************************************************************/
/*		hako_disp_ban															*/
/********************************************************************************/
void hako_disp_ban()
{
	int i, x, y, k, p[5], pcnt, pos, u, d, r, l, color;
	
	for(k=0; k<=10; k++){					// 駒ループ
		memset(p, 0, sizeof(p));
		pcnt = 0;
		for(pos=0; pos<7*6; pos++){
			if(k==hako_banp[pos]/10){
				p[pcnt++] = pos;
				if(pcnt>=5)
					message("hako_disp_ban err k=%d\n", k);
			}
		}
		color = k==0 ? WHITE : hako_color[k];
		for(i=0; i<pcnt; i++){
			x = macrox(p[i]);
			y = macroy(p[i]);
			lcd_clearA(80+(x-1)*40, 20+(y-1)*40, 80+x*40-1, 20+y*40-1, color);
			if(k){
				u = d = r = l = 1;
				if(hako_banp[p[i]+U]/10==k)		// 同じ駒が上に続く
					u = 0;
				if(hako_banp[p[i]+D]/10==k)
					d = 0;
				if(hako_banp[p[i]+R]/10==k)
					r = 0;
				if(hako_banp[p[i]+L]/10==k)
					l = 0;
				if(u)
					lcd_clearA(80+(x-1)*40, 20+(y-1)*40, 80+x*40-1, 20+(y-1)*40, BLACK);
				if(d)
					lcd_clearA(80+(x-1)*40, 20+y*40-1, 80+x*40-1, 20+y*40-1, BLACK);
				if(l)
					lcd_clearA(80+(x-1)*40, 20+(y-1)*40, 80+(x-1)*40, 20+y*40-1, BLACK);
				if(r)
					lcd_clearA(80+x*40-1, 20+(y-1)*40, 80+x*40-1, 20+y*40-1, BLACK);
			}
		}
	}
	lcd_clearA(80-1, 20-1, 80+160, 20-1, BLACK);
	lcd_clearA(80-1, 20+200, 80+160, 20+200, BLACK);
	lcd_clearA(80-1, 20-1, 80-1, 20+200, BLACK);
	lcd_clearA(80+160, 20-1, 80+160, 20+200, BLACK);
}
/********************************************************************************/
/*		hako_main																*/
/********************************************************************************/
void hako_main()
{
	int i, j, sw;
	
	lcd_clear(WHITE);
	srand(TickCount);
	memcpy(hako_ban, hako_ban_dat, sizeof(hako_ban));
	hako_disp_ban();								// 表示
	lcd_disp_str_x1(250,20,"Push");
	lcd_disp_str_x1(250,40,"any");
	lcd_disp_str_x1(250,60,"sw");
	sw_updownup();
	lcd_clear(WHITE);
	hako_disp_ban();
	for(i=0; ; i++){
		if(hako_kif[i][0]==0)
			break;
		memcpy(hako_ban, hako_kif[i], 7*6);
		hako_disp_ban();
		for(j=0; j<5; j++){
			sw = sw_read();
			if(sw)
				sw_downup();
            if(sw & 4)              // sw3: return
                return;
			wait_ms(100);
		}
	}
	lcd_disp_str_x1(250,20,"End");
	sw_updownup();
}




const char hako_kif[][7*6]={
{0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,0x0b,0x16,0x16,0x1f,0x9c,0x9c,0x0b,0x16,0x16,0x1f,0x9c,0x9c,0x29,0x35,0x35,0x3d,0x9c,0x9c,0x29,0x4a,0x54,0x3d,0x9c,0x9c,0x5e,0x00,0x00,0x68,0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,},
{0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,0x0b,0x16,0x16,0x1f,0x9c,0x9c,0x0b,0x16,0x16,0x1f,0x9c,0x9c,0x29,0x35,0x35,0x3d,0x9c,0x9c,0x29,0x00,0x54,0x3d,0x9c,0x9c,0x5e,0x4a,0x00,0x68,0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,},
{0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,0x0b,0x16,0x16,0x1f,0x9c,0x9c,0x0b,0x16,0x16,0x1f,0x9c,0x9c,0x29,0x35,0x35,0x3d,0x9c,0x9c,0x29,0x00,0x54,0x3d,0x9c,0x9c,0x5e,0x4a,0x68,0x00,0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,},
{0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,0x0b,0x16,0x16,0x1f,0x9c,0x9c,0x0b,0x16,0x16,0x1f,0x9c,0x9c,0x29,0x35,0x35,0x00,0x9c,0x9c,0x29,0x00,0x54,0x3d,0x9c,0x9c,0x5e,0x4a,0x68,0x3d,0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,},
{0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,0x0b,0x16,0x16,0x1f,0x9c,0x9c,0x0b,0x16,0x16,0x1f,0x9c,0x9c,0x29,0x00,0x35,0x35,0x9c,0x9c,0x29,0x00,0x54,0x3d,0x9c,0x9c,0x5e,0x4a,0x68,0x3d,0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,},
{0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,0x0b,0x16,0x16,0x1f,0x9c,0x9c,0x0b,0x16,0x16,0x1f,0x9c,0x9c,0x00,0x29,0x35,0x35,0x9c,0x9c,0x00,0x29,0x54,0x3d,0x9c,0x9c,0x5e,0x4a,0x68,0x3d,0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,},
{0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,0x0b,0x16,0x16,0x1f,0x9c,0x9c,0x0b,0x16,0x16,0x1f,0x9c,0x9c,0x00,0x29,0x35,0x35,0x9c,0x9c,0x5e,0x29,0x54,0x3d,0x9c,0x9c,0x00,0x4a,0x68,0x3d,0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,},
{0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,0x0b,0x16,0x16,0x1f,0x9c,0x9c,0x0b,0x16,0x16,0x1f,0x9c,0x9c,0x00,0x29,0x35,0x35,0x9c,0x9c,0x5e,0x29,0x54,0x3d,0x9c,0x9c,0x4a,0x00,0x68,0x3d,0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,},
{0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,0x0b,0x16,0x16,0x1f,0x9c,0x9c,0x0b,0x16,0x16,0x1f,0x9c,0x9c,0x00,0x00,0x35,0x35,0x9c,0x9c,0x5e,0x29,0x54,0x3d,0x9c,0x9c,0x4a,0x29,0x68,0x3d,0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,},
{0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,0x0b,0x16,0x16,0x1f,0x9c,0x9c,0x0b,0x16,0x16,0x1f,0x9c,0x9c,0x00,0x35,0x35,0x00,0x9c,0x9c,0x5e,0x29,0x54,0x3d,0x9c,0x9c,0x4a,0x29,0x68,0x3d,0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,},
{0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,0x0b,0x16,0x16,0x1f,0x9c,0x9c,0x0b,0x16,0x16,0x1f,0x9c,0x9c,0x35,0x35,0x00,0x00,0x9c,0x9c,0x5e,0x29,0x54,0x3d,0x9c,0x9c,0x4a,0x29,0x68,0x3d,0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,},
{0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,0x0b,0x16,0x16,0x1f,0x9c,0x9c,0x0b,0x16,0x16,0x1f,0x9c,0x9c,0x35,0x35,0x54,0x00,0x9c,0x9c,0x5e,0x29,0x00,0x3d,0x9c,0x9c,0x4a,0x29,0x68,0x3d,0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,},
{0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,0x0b,0x16,0x16,0x1f,0x9c,0x9c,0x0b,0x16,0x16,0x1f,0x9c,0x9c,0x35,0x35,0x00,0x54,0x9c,0x9c,0x5e,0x29,0x00,0x3d,0x9c,0x9c,0x4a,0x29,0x68,0x3d,0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,},
{0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,0x0b,0x16,0x16,0x1f,0x9c,0x9c,0x0b,0x16,0x16,0x1f,0x9c,0x9c,0x35,0x35,0x00,0x54,0x9c,0x9c,0x5e,0x29,0x68,0x3d,0x9c,0x9c,0x4a,0x29,0x00,0x3d,0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,},
{0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,0x0b,0x16,0x16,0x1f,0x9c,0x9c,0x0b,0x16,0x16,0x1f,0x9c,0x9c,0x35,0x35,0x68,0x54,0x9c,0x9c,0x5e,0x29,0x00,0x3d,0x9c,0x9c,0x4a,0x29,0x00,0x3d,0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,},
{0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,0x0b,0x16,0x16,0x1f,0x9c,0x9c,0x0b,0x16,0x16,0x1f,0x9c,0x9c,0x35,0x35,0x68,0x54,0x9c,0x9c,0x5e,0x00,0x29,0x3d,0x9c,0x9c,0x4a,0x00,0x29,0x3d,0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,},
{0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,0x0b,0x16,0x16,0x1f,0x9c,0x9c,0x0b,0x16,0x16,0x1f,0x9c,0x9c,0x35,0x35,0x68,0x54,0x9c,0x9c,0x00,0x5e,0x29,0x3d,0x9c,0x9c,0x4a,0x00,0x29,0x3d,0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,},
{0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,0x0b,0x16,0x16,0x1f,0x9c,0x9c,0x0b,0x16,0x16,0x1f,0x9c,0x9c,0x35,0x35,0x68,0x54,0x9c,0x9c,0x00,0x00,0x29,0x3d,0x9c,0x9c,0x4a,0x5e,0x29,0x3d,0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,},
{0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,0x0b,0x16,0x16,0x1f,0x9c,0x9c,0x0b,0x16,0x16,0x1f,0x9c,0x9c,0x00,0x00,0x68,0x54,0x9c,0x9c,0x35,0x35,0x29,0x3d,0x9c,0x9c,0x4a,0x5e,0x29,0x3d,0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,},
{0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,0x0b,0x16,0x16,0x1f,0x9c,0x9c,0x0b,0x16,0x16,0x1f,0x9c,0x9c,0x00,0x68,0x00,0x54,0x9c,0x9c,0x35,0x35,0x29,0x3d,0x9c,0x9c,0x4a,0x5e,0x29,0x3d,0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,},
{0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,0x0b,0x16,0x16,0x1f,0x9c,0x9c,0x0b,0x16,0x16,0x1f,0x9c,0x9c,0x68,0x00,0x00,0x54,0x9c,0x9c,0x35,0x35,0x29,0x3d,0x9c,0x9c,0x4a,0x5e,0x29,0x3d,0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,},
{0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,0x0b,0x16,0x16,0x1f,0x9c,0x9c,0x0b,0x16,0x16,0x1f,0x9c,0x9c,0x68,0x00,0x54,0x00,0x9c,0x9c,0x35,0x35,0x29,0x3d,0x9c,0x9c,0x4a,0x5e,0x29,0x3d,0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,},
{0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,0x0b,0x16,0x16,0x1f,0x9c,0x9c,0x0b,0x16,0x16,0x1f,0x9c,0x9c,0x68,0x54,0x00,0x00,0x9c,0x9c,0x35,0x35,0x29,0x3d,0x9c,0x9c,0x4a,0x5e,0x29,0x3d,0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,},
{0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,0x0b,0x16,0x16,0x1f,0x9c,0x9c,0x0b,0x16,0x16,0x1f,0x9c,0x9c,0x68,0x54,0x29,0x00,0x9c,0x9c,0x35,0x35,0x29,0x3d,0x9c,0x9c,0x4a,0x5e,0x00,0x3d,0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,},
{0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,0x0b,0x16,0x16,0x1f,0x9c,0x9c,0x0b,0x16,0x16,0x1f,0x9c,0x9c,0x68,0x54,0x29,0x3d,0x9c,0x9c,0x35,0x35,0x29,0x3d,0x9c,0x9c,0x4a,0x5e,0x00,0x00,0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,},
{0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,0x0b,0x16,0x16,0x1f,0x9c,0x9c,0x0b,0x16,0x16,0x1f,0x9c,0x9c,0x68,0x54,0x29,0x3d,0x9c,0x9c,0x35,0x35,0x29,0x3d,0x9c,0x9c,0x4a,0x00,0x5e,0x00,0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,},
{0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,0x0b,0x16,0x16,0x1f,0x9c,0x9c,0x0b,0x16,0x16,0x1f,0x9c,0x9c,0x68,0x54,0x29,0x3d,0x9c,0x9c,0x35,0x35,0x29,0x3d,0x9c,0x9c,0x4a,0x00,0x00,0x5e,0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,},
{0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,0x0b,0x16,0x16,0x1f,0x9c,0x9c,0x0b,0x16,0x16,0x1f,0x9c,0x9c,0x68,0x54,0x29,0x3d,0x9c,0x9c,0x35,0x35,0x29,0x3d,0x9c,0x9c,0x00,0x4a,0x00,0x5e,0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,},
{0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,0x0b,0x16,0x16,0x1f,0x9c,0x9c,0x0b,0x16,0x16,0x1f,0x9c,0x9c,0x68,0x54,0x29,0x3d,0x9c,0x9c,0x35,0x35,0x29,0x3d,0x9c,0x9c,0x00,0x00,0x4a,0x5e,0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,},
{0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,0x0b,0x16,0x16,0x1f,0x9c,0x9c,0x0b,0x16,0x16,0x1f,0x9c,0x9c,0x68,0x54,0x29,0x3d,0x9c,0x9c,0x00,0x00,0x29,0x3d,0x9c,0x9c,0x35,0x35,0x4a,0x5e,0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,},
{0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,0x0b,0x16,0x16,0x1f,0x9c,0x9c,0x0b,0x16,0x16,0x1f,0x9c,0x9c,0x68,0x00,0x29,0x3d,0x9c,0x9c,0x00,0x54,0x29,0x3d,0x9c,0x9c,0x35,0x35,0x4a,0x5e,0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,},
{0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,0x0b,0x16,0x16,0x1f,0x9c,0x9c,0x0b,0x16,0x16,0x1f,0x9c,0x9c,0x68,0x00,0x29,0x3d,0x9c,0x9c,0x54,0x00,0x29,0x3d,0x9c,0x9c,0x35,0x35,0x4a,0x5e,0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,},
{0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,0x0b,0x16,0x16,0x1f,0x9c,0x9c,0x0b,0x16,0x16,0x1f,0x9c,0x9c,0x68,0x29,0x00,0x3d,0x9c,0x9c,0x54,0x29,0x00,0x3d,0x9c,0x9c,0x35,0x35,0x4a,0x5e,0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,},
{0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,0x0b,0x16,0x16,0x1f,0x9c,0x9c,0x0b,0x16,0x16,0x1f,0x9c,0x9c,0x68,0x29,0x3d,0x00,0x9c,0x9c,0x54,0x29,0x3d,0x00,0x9c,0x9c,0x35,0x35,0x4a,0x5e,0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,},
{0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,0x0b,0x16,0x16,0x00,0x9c,0x9c,0x0b,0x16,0x16,0x1f,0x9c,0x9c,0x68,0x29,0x3d,0x1f,0x9c,0x9c,0x54,0x29,0x3d,0x00,0x9c,0x9c,0x35,0x35,0x4a,0x5e,0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,},
{0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,0x0b,0x16,0x16,0x00,0x9c,0x9c,0x0b,0x16,0x16,0x00,0x9c,0x9c,0x68,0x29,0x3d,0x1f,0x9c,0x9c,0x54,0x29,0x3d,0x1f,0x9c,0x9c,0x35,0x35,0x4a,0x5e,0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,},
{0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,0x0b,0x00,0x16,0x16,0x9c,0x9c,0x0b,0x00,0x16,0x16,0x9c,0x9c,0x68,0x29,0x3d,0x1f,0x9c,0x9c,0x54,0x29,0x3d,0x1f,0x9c,0x9c,0x35,0x35,0x4a,0x5e,0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,},
{0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,0x00,0x0b,0x16,0x16,0x9c,0x9c,0x00,0x0b,0x16,0x16,0x9c,0x9c,0x68,0x29,0x3d,0x1f,0x9c,0x9c,0x54,0x29,0x3d,0x1f,0x9c,0x9c,0x35,0x35,0x4a,0x5e,0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,},
{0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,0x00,0x0b,0x16,0x16,0x9c,0x9c,0x68,0x0b,0x16,0x16,0x9c,0x9c,0x00,0x29,0x3d,0x1f,0x9c,0x9c,0x54,0x29,0x3d,0x1f,0x9c,0x9c,0x35,0x35,0x4a,0x5e,0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,},
{0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,0x68,0x0b,0x16,0x16,0x9c,0x9c,0x00,0x0b,0x16,0x16,0x9c,0x9c,0x00,0x29,0x3d,0x1f,0x9c,0x9c,0x54,0x29,0x3d,0x1f,0x9c,0x9c,0x35,0x35,0x4a,0x5e,0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,},
{0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,0x68,0x0b,0x16,0x16,0x9c,0x9c,0x00,0x0b,0x16,0x16,0x9c,0x9c,0x54,0x29,0x3d,0x1f,0x9c,0x9c,0x00,0x29,0x3d,0x1f,0x9c,0x9c,0x35,0x35,0x4a,0x5e,0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,},
{0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,0x68,0x0b,0x16,0x16,0x9c,0x9c,0x54,0x0b,0x16,0x16,0x9c,0x9c,0x00,0x29,0x3d,0x1f,0x9c,0x9c,0x00,0x29,0x3d,0x1f,0x9c,0x9c,0x35,0x35,0x4a,0x5e,0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,},
{0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,0x68,0x0b,0x16,0x16,0x9c,0x9c,0x54,0x0b,0x16,0x16,0x9c,0x9c,0x29,0x00,0x3d,0x1f,0x9c,0x9c,0x29,0x00,0x3d,0x1f,0x9c,0x9c,0x35,0x35,0x4a,0x5e,0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,},
{0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,0x68,0x00,0x16,0x16,0x9c,0x9c,0x54,0x0b,0x16,0x16,0x9c,0x9c,0x29,0x0b,0x3d,0x1f,0x9c,0x9c,0x29,0x00,0x3d,0x1f,0x9c,0x9c,0x35,0x35,0x4a,0x5e,0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,},
{0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,0x68,0x00,0x16,0x16,0x9c,0x9c,0x54,0x00,0x16,0x16,0x9c,0x9c,0x29,0x0b,0x3d,0x1f,0x9c,0x9c,0x29,0x0b,0x3d,0x1f,0x9c,0x9c,0x35,0x35,0x4a,0x5e,0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,},
{0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,0x68,0x16,0x16,0x00,0x9c,0x9c,0x54,0x16,0x16,0x00,0x9c,0x9c,0x29,0x0b,0x3d,0x1f,0x9c,0x9c,0x29,0x0b,0x3d,0x1f,0x9c,0x9c,0x35,0x35,0x4a,0x5e,0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,},
{0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,0x68,0x16,0x16,0x00,0x9c,0x9c,0x54,0x16,0x16,0x1f,0x9c,0x9c,0x29,0x0b,0x3d,0x1f,0x9c,0x9c,0x29,0x0b,0x3d,0x00,0x9c,0x9c,0x35,0x35,0x4a,0x5e,0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,},
{0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,0x68,0x16,0x16,0x1f,0x9c,0x9c,0x54,0x16,0x16,0x1f,0x9c,0x9c,0x29,0x0b,0x3d,0x00,0x9c,0x9c,0x29,0x0b,0x3d,0x00,0x9c,0x9c,0x35,0x35,0x4a,0x5e,0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,},
{0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,0x68,0x16,0x16,0x1f,0x9c,0x9c,0x54,0x16,0x16,0x1f,0x9c,0x9c,0x29,0x0b,0x00,0x3d,0x9c,0x9c,0x29,0x0b,0x00,0x3d,0x9c,0x9c,0x35,0x35,0x4a,0x5e,0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,},
{0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,0x68,0x16,0x16,0x1f,0x9c,0x9c,0x54,0x16,0x16,0x1f,0x9c,0x9c,0x29,0x0b,0x00,0x3d,0x9c,0x9c,0x29,0x0b,0x4a,0x3d,0x9c,0x9c,0x35,0x35,0x00,0x5e,0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,},
{0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,0x68,0x16,0x16,0x1f,0x9c,0x9c,0x54,0x16,0x16,0x1f,0x9c,0x9c,0x29,0x0b,0x4a,0x3d,0x9c,0x9c,0x29,0x0b,0x00,0x3d,0x9c,0x9c,0x35,0x35,0x00,0x5e,0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,},
{0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,0x68,0x16,0x16,0x1f,0x9c,0x9c,0x54,0x16,0x16,0x1f,0x9c,0x9c,0x29,0x0b,0x4a,0x3d,0x9c,0x9c,0x29,0x0b,0x00,0x3d,0x9c,0x9c,0x35,0x35,0x5e,0x00,0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,},
{0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,0x68,0x16,0x16,0x1f,0x9c,0x9c,0x54,0x16,0x16,0x1f,0x9c,0x9c,0x29,0x0b,0x4a,0x3d,0x9c,0x9c,0x29,0x0b,0x5e,0x3d,0x9c,0x9c,0x35,0x35,0x00,0x00,0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,},
{0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,0x68,0x16,0x16,0x1f,0x9c,0x9c,0x54,0x16,0x16,0x1f,0x9c,0x9c,0x29,0x0b,0x4a,0x3d,0x9c,0x9c,0x29,0x0b,0x5e,0x3d,0x9c,0x9c,0x00,0x35,0x35,0x00,0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,},
{0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,0x68,0x16,0x16,0x1f,0x9c,0x9c,0x54,0x16,0x16,0x1f,0x9c,0x9c,0x29,0x0b,0x4a,0x3d,0x9c,0x9c,0x29,0x0b,0x5e,0x3d,0x9c,0x9c,0x00,0x00,0x35,0x35,0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,},
{0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,0x68,0x16,0x16,0x1f,0x9c,0x9c,0x54,0x16,0x16,0x1f,0x9c,0x9c,0x29,0x00,0x4a,0x3d,0x9c,0x9c,0x29,0x0b,0x5e,0x3d,0x9c,0x9c,0x00,0x0b,0x35,0x35,0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,},
{0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,0x68,0x16,0x16,0x1f,0x9c,0x9c,0x54,0x16,0x16,0x1f,0x9c,0x9c,0x00,0x00,0x4a,0x3d,0x9c,0x9c,0x29,0x0b,0x5e,0x3d,0x9c,0x9c,0x29,0x0b,0x35,0x35,0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,},
{0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,0x68,0x16,0x16,0x1f,0x9c,0x9c,0x54,0x16,0x16,0x1f,0x9c,0x9c,0x00,0x4a,0x00,0x3d,0x9c,0x9c,0x29,0x0b,0x5e,0x3d,0x9c,0x9c,0x29,0x0b,0x35,0x35,0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,},
{0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,0x68,0x16,0x16,0x1f,0x9c,0x9c,0x54,0x16,0x16,0x1f,0x9c,0x9c,0x4a,0x00,0x00,0x3d,0x9c,0x9c,0x29,0x0b,0x5e,0x3d,0x9c,0x9c,0x29,0x0b,0x35,0x35,0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,},
{0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,0x68,0x00,0x00,0x1f,0x9c,0x9c,0x54,0x16,0x16,0x1f,0x9c,0x9c,0x4a,0x16,0x16,0x3d,0x9c,0x9c,0x29,0x0b,0x5e,0x3d,0x9c,0x9c,0x29,0x0b,0x35,0x35,0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,},
{0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,0x00,0x68,0x00,0x1f,0x9c,0x9c,0x54,0x16,0x16,0x1f,0x9c,0x9c,0x4a,0x16,0x16,0x3d,0x9c,0x9c,0x29,0x0b,0x5e,0x3d,0x9c,0x9c,0x29,0x0b,0x35,0x35,0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,},
{0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,0x00,0x00,0x68,0x1f,0x9c,0x9c,0x54,0x16,0x16,0x1f,0x9c,0x9c,0x4a,0x16,0x16,0x3d,0x9c,0x9c,0x29,0x0b,0x5e,0x3d,0x9c,0x9c,0x29,0x0b,0x35,0x35,0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,},
{0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,0x54,0x00,0x68,0x1f,0x9c,0x9c,0x00,0x16,0x16,0x1f,0x9c,0x9c,0x4a,0x16,0x16,0x3d,0x9c,0x9c,0x29,0x0b,0x5e,0x3d,0x9c,0x9c,0x29,0x0b,0x35,0x35,0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,},
{0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,0x00,0x54,0x68,0x1f,0x9c,0x9c,0x00,0x16,0x16,0x1f,0x9c,0x9c,0x4a,0x16,0x16,0x3d,0x9c,0x9c,0x29,0x0b,0x5e,0x3d,0x9c,0x9c,0x29,0x0b,0x35,0x35,0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,},
{0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,0x00,0x54,0x68,0x1f,0x9c,0x9c,0x4a,0x16,0x16,0x1f,0x9c,0x9c,0x00,0x16,0x16,0x3d,0x9c,0x9c,0x29,0x0b,0x5e,0x3d,0x9c,0x9c,0x29,0x0b,0x35,0x35,0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,},
{0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,0x4a,0x54,0x68,0x1f,0x9c,0x9c,0x00,0x16,0x16,0x1f,0x9c,0x9c,0x00,0x16,0x16,0x3d,0x9c,0x9c,0x29,0x0b,0x5e,0x3d,0x9c,0x9c,0x29,0x0b,0x35,0x35,0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,},
{0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,0x4a,0x54,0x68,0x1f,0x9c,0x9c,0x00,0x16,0x16,0x1f,0x9c,0x9c,0x29,0x16,0x16,0x3d,0x9c,0x9c,0x29,0x0b,0x5e,0x3d,0x9c,0x9c,0x00,0x0b,0x35,0x35,0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,},
{0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,0x4a,0x54,0x68,0x1f,0x9c,0x9c,0x29,0x16,0x16,0x1f,0x9c,0x9c,0x29,0x16,0x16,0x3d,0x9c,0x9c,0x00,0x0b,0x5e,0x3d,0x9c,0x9c,0x00,0x0b,0x35,0x35,0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,},
{0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,0x4a,0x54,0x68,0x1f,0x9c,0x9c,0x29,0x16,0x16,0x1f,0x9c,0x9c,0x29,0x16,0x16,0x3d,0x9c,0x9c,0x0b,0x00,0x5e,0x3d,0x9c,0x9c,0x0b,0x00,0x35,0x35,0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,},
{0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,0x4a,0x54,0x68,0x1f,0x9c,0x9c,0x29,0x16,0x16,0x1f,0x9c,0x9c,0x29,0x16,0x16,0x3d,0x9c,0x9c,0x0b,0x5e,0x00,0x3d,0x9c,0x9c,0x0b,0x00,0x35,0x35,0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,},
{0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,0x4a,0x54,0x68,0x1f,0x9c,0x9c,0x29,0x16,0x16,0x1f,0x9c,0x9c,0x29,0x16,0x16,0x3d,0x9c,0x9c,0x0b,0x00,0x00,0x3d,0x9c,0x9c,0x0b,0x5e,0x35,0x35,0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,},
{0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,0x4a,0x54,0x68,0x1f,0x9c,0x9c,0x29,0x00,0x00,0x1f,0x9c,0x9c,0x29,0x16,0x16,0x3d,0x9c,0x9c,0x0b,0x16,0x16,0x3d,0x9c,0x9c,0x0b,0x5e,0x35,0x35,0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,},
{0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,0x4a,0x54,0x00,0x1f,0x9c,0x9c,0x29,0x00,0x68,0x1f,0x9c,0x9c,0x29,0x16,0x16,0x3d,0x9c,0x9c,0x0b,0x16,0x16,0x3d,0x9c,0x9c,0x0b,0x5e,0x35,0x35,0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,},
{0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,0x4a,0x54,0x00,0x1f,0x9c,0x9c,0x29,0x68,0x00,0x1f,0x9c,0x9c,0x29,0x16,0x16,0x3d,0x9c,0x9c,0x0b,0x16,0x16,0x3d,0x9c,0x9c,0x0b,0x5e,0x35,0x35,0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,},
{0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,0x4a,0x54,0x1f,0x00,0x9c,0x9c,0x29,0x68,0x1f,0x00,0x9c,0x9c,0x29,0x16,0x16,0x3d,0x9c,0x9c,0x0b,0x16,0x16,0x3d,0x9c,0x9c,0x0b,0x5e,0x35,0x35,0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,},
{0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,0x4a,0x54,0x1f,0x00,0x9c,0x9c,0x29,0x68,0x1f,0x3d,0x9c,0x9c,0x29,0x16,0x16,0x3d,0x9c,0x9c,0x0b,0x16,0x16,0x00,0x9c,0x9c,0x0b,0x5e,0x35,0x35,0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,},
{0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,0x4a,0x54,0x1f,0x3d,0x9c,0x9c,0x29,0x68,0x1f,0x3d,0x9c,0x9c,0x29,0x16,0x16,0x00,0x9c,0x9c,0x0b,0x16,0x16,0x00,0x9c,0x9c,0x0b,0x5e,0x35,0x35,0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,},
{0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,0x4a,0x54,0x1f,0x3d,0x9c,0x9c,0x29,0x68,0x1f,0x3d,0x9c,0x9c,0x29,0x00,0x16,0x16,0x9c,0x9c,0x0b,0x00,0x16,0x16,0x9c,0x9c,0x0b,0x5e,0x35,0x35,0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,},
{0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,0x4a,0x54,0x1f,0x3d,0x9c,0x9c,0x29,0x00,0x1f,0x3d,0x9c,0x9c,0x29,0x68,0x16,0x16,0x9c,0x9c,0x0b,0x00,0x16,0x16,0x9c,0x9c,0x0b,0x5e,0x35,0x35,0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,},
{0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,0x4a,0x54,0x1f,0x3d,0x9c,0x9c,0x29,0x00,0x1f,0x3d,0x9c,0x9c,0x29,0x00,0x16,0x16,0x9c,0x9c,0x0b,0x68,0x16,0x16,0x9c,0x9c,0x0b,0x5e,0x35,0x35,0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,},
{0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,0x4a,0x00,0x1f,0x3d,0x9c,0x9c,0x29,0x54,0x1f,0x3d,0x9c,0x9c,0x29,0x00,0x16,0x16,0x9c,0x9c,0x0b,0x68,0x16,0x16,0x9c,0x9c,0x0b,0x5e,0x35,0x35,0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,},
{0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,0x00,0x4a,0x1f,0x3d,0x9c,0x9c,0x29,0x54,0x1f,0x3d,0x9c,0x9c,0x29,0x00,0x16,0x16,0x9c,0x9c,0x0b,0x68,0x16,0x16,0x9c,0x9c,0x0b,0x5e,0x35,0x35,0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,},
{0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,0x29,0x4a,0x1f,0x3d,0x9c,0x9c,0x29,0x54,0x1f,0x3d,0x9c,0x9c,0x00,0x00,0x16,0x16,0x9c,0x9c,0x0b,0x68,0x16,0x16,0x9c,0x9c,0x0b,0x5e,0x35,0x35,0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,},
{0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,0x29,0x4a,0x1f,0x3d,0x9c,0x9c,0x29,0x54,0x1f,0x3d,0x9c,0x9c,0x0b,0x00,0x16,0x16,0x9c,0x9c,0x0b,0x68,0x16,0x16,0x9c,0x9c,0x00,0x5e,0x35,0x35,0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,},
{0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,0x29,0x4a,0x1f,0x3d,0x9c,0x9c,0x29,0x54,0x1f,0x3d,0x9c,0x9c,0x0b,0x00,0x16,0x16,0x9c,0x9c,0x0b,0x68,0x16,0x16,0x9c,0x9c,0x5e,0x00,0x35,0x35,0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,},
{0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,0x29,0x4a,0x1f,0x3d,0x9c,0x9c,0x29,0x54,0x1f,0x3d,0x9c,0x9c,0x0b,0x00,0x16,0x16,0x9c,0x9c,0x0b,0x00,0x16,0x16,0x9c,0x9c,0x5e,0x68,0x35,0x35,0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,},
{0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,0x29,0x4a,0x1f,0x3d,0x9c,0x9c,0x29,0x54,0x1f,0x3d,0x9c,0x9c,0x0b,0x16,0x16,0x00,0x9c,0x9c,0x0b,0x16,0x16,0x00,0x9c,0x9c,0x5e,0x68,0x35,0x35,0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,},
{0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,0x29,0x4a,0x1f,0x00,0x9c,0x9c,0x29,0x54,0x1f,0x3d,0x9c,0x9c,0x0b,0x16,0x16,0x3d,0x9c,0x9c,0x0b,0x16,0x16,0x00,0x9c,0x9c,0x5e,0x68,0x35,0x35,0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,},
{0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,0x29,0x4a,0x1f,0x00,0x9c,0x9c,0x29,0x54,0x1f,0x00,0x9c,0x9c,0x0b,0x16,0x16,0x3d,0x9c,0x9c,0x0b,0x16,0x16,0x3d,0x9c,0x9c,0x5e,0x68,0x35,0x35,0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,},
{0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,0x29,0x4a,0x00,0x1f,0x9c,0x9c,0x29,0x54,0x00,0x1f,0x9c,0x9c,0x0b,0x16,0x16,0x3d,0x9c,0x9c,0x0b,0x16,0x16,0x3d,0x9c,0x9c,0x5e,0x68,0x35,0x35,0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,},
{0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,0x29,0x00,0x4a,0x1f,0x9c,0x9c,0x29,0x54,0x00,0x1f,0x9c,0x9c,0x0b,0x16,0x16,0x3d,0x9c,0x9c,0x0b,0x16,0x16,0x3d,0x9c,0x9c,0x5e,0x68,0x35,0x35,0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,},
{0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,0x29,0x00,0x4a,0x1f,0x9c,0x9c,0x29,0x00,0x54,0x1f,0x9c,0x9c,0x0b,0x16,0x16,0x3d,0x9c,0x9c,0x0b,0x16,0x16,0x3d,0x9c,0x9c,0x5e,0x68,0x35,0x35,0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,},
{0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,0x00,0x29,0x4a,0x1f,0x9c,0x9c,0x00,0x29,0x54,0x1f,0x9c,0x9c,0x0b,0x16,0x16,0x3d,0x9c,0x9c,0x0b,0x16,0x16,0x3d,0x9c,0x9c,0x5e,0x68,0x35,0x35,0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,},
{0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,0x00,0x29,0x4a,0x1f,0x9c,0x9c,0x0b,0x29,0x54,0x1f,0x9c,0x9c,0x0b,0x16,0x16,0x3d,0x9c,0x9c,0x00,0x16,0x16,0x3d,0x9c,0x9c,0x5e,0x68,0x35,0x35,0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,},
{0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,0x0b,0x29,0x4a,0x1f,0x9c,0x9c,0x0b,0x29,0x54,0x1f,0x9c,0x9c,0x00,0x16,0x16,0x3d,0x9c,0x9c,0x00,0x16,0x16,0x3d,0x9c,0x9c,0x5e,0x68,0x35,0x35,0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,},
{0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,0x0b,0x29,0x4a,0x1f,0x9c,0x9c,0x0b,0x29,0x54,0x1f,0x9c,0x9c,0x16,0x16,0x00,0x3d,0x9c,0x9c,0x16,0x16,0x00,0x3d,0x9c,0x9c,0x5e,0x68,0x35,0x35,0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,},
{0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,0x0b,0x29,0x4a,0x1f,0x9c,0x9c,0x0b,0x29,0x00,0x1f,0x9c,0x9c,0x16,0x16,0x54,0x3d,0x9c,0x9c,0x16,0x16,0x00,0x3d,0x9c,0x9c,0x5e,0x68,0x35,0x35,0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,},
{0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,0x0b,0x29,0x4a,0x1f,0x9c,0x9c,0x0b,0x29,0x00,0x1f,0x9c,0x9c,0x16,0x16,0x00,0x3d,0x9c,0x9c,0x16,0x16,0x54,0x3d,0x9c,0x9c,0x5e,0x68,0x35,0x35,0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,},
{0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,0x0b,0x29,0x00,0x1f,0x9c,0x9c,0x0b,0x29,0x4a,0x1f,0x9c,0x9c,0x16,0x16,0x00,0x3d,0x9c,0x9c,0x16,0x16,0x54,0x3d,0x9c,0x9c,0x5e,0x68,0x35,0x35,0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,},
{0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,0x0b,0x29,0x00,0x1f,0x9c,0x9c,0x0b,0x29,0x00,0x1f,0x9c,0x9c,0x16,0x16,0x4a,0x3d,0x9c,0x9c,0x16,0x16,0x54,0x3d,0x9c,0x9c,0x5e,0x68,0x35,0x35,0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,},
{0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,0x0b,0x29,0x1f,0x00,0x9c,0x9c,0x0b,0x29,0x1f,0x00,0x9c,0x9c,0x16,0x16,0x4a,0x3d,0x9c,0x9c,0x16,0x16,0x54,0x3d,0x9c,0x9c,0x5e,0x68,0x35,0x35,0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,},
{0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,0x0b,0x29,0x1f,0x00,0x9c,0x9c,0x0b,0x29,0x1f,0x3d,0x9c,0x9c,0x16,0x16,0x4a,0x3d,0x9c,0x9c,0x16,0x16,0x54,0x00,0x9c,0x9c,0x5e,0x68,0x35,0x35,0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,},
{0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,0x0b,0x29,0x1f,0x3d,0x9c,0x9c,0x0b,0x29,0x1f,0x3d,0x9c,0x9c,0x16,0x16,0x4a,0x00,0x9c,0x9c,0x16,0x16,0x54,0x00,0x9c,0x9c,0x5e,0x68,0x35,0x35,0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,},
{0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,0x0b,0x29,0x1f,0x3d,0x9c,0x9c,0x0b,0x29,0x1f,0x3d,0x9c,0x9c,0x16,0x16,0x4a,0x00,0x9c,0x9c,0x16,0x16,0x00,0x54,0x9c,0x9c,0x5e,0x68,0x35,0x35,0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,},
{0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,0x0b,0x29,0x1f,0x3d,0x9c,0x9c,0x0b,0x29,0x1f,0x3d,0x9c,0x9c,0x16,0x16,0x4a,0x54,0x9c,0x9c,0x16,0x16,0x00,0x00,0x9c,0x9c,0x5e,0x68,0x35,0x35,0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,},
{0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,0x0b,0x29,0x1f,0x3d,0x9c,0x9c,0x0b,0x29,0x1f,0x3d,0x9c,0x9c,0x16,0x16,0x4a,0x54,0x9c,0x9c,0x16,0x16,0x35,0x35,0x9c,0x9c,0x5e,0x68,0x00,0x00,0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,},
{0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,0x0b,0x29,0x1f,0x3d,0x9c,0x9c,0x0b,0x29,0x1f,0x3d,0x9c,0x9c,0x16,0x16,0x4a,0x54,0x9c,0x9c,0x16,0x16,0x35,0x35,0x9c,0x9c,0x5e,0x00,0x68,0x00,0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,},
{0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,0x0b,0x29,0x1f,0x3d,0x9c,0x9c,0x0b,0x29,0x1f,0x3d,0x9c,0x9c,0x16,0x16,0x4a,0x54,0x9c,0x9c,0x16,0x16,0x35,0x35,0x9c,0x9c,0x5e,0x00,0x00,0x68,0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,},
{0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,0x0b,0x29,0x1f,0x3d,0x9c,0x9c,0x0b,0x29,0x1f,0x3d,0x9c,0x9c,0x16,0x16,0x4a,0x54,0x9c,0x9c,0x16,0x16,0x35,0x35,0x9c,0x9c,0x00,0x5e,0x00,0x68,0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,},
{0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,0x0b,0x29,0x1f,0x3d,0x9c,0x9c,0x0b,0x29,0x1f,0x3d,0x9c,0x9c,0x16,0x16,0x4a,0x54,0x9c,0x9c,0x16,0x16,0x35,0x35,0x9c,0x9c,0x00,0x00,0x5e,0x68,0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,},
{0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,0x0b,0x29,0x1f,0x3d,0x9c,0x9c,0x0b,0x29,0x1f,0x3d,0x9c,0x9c,0x00,0x00,0x4a,0x54,0x9c,0x9c,0x16,0x16,0x35,0x35,0x9c,0x9c,0x16,0x16,0x5e,0x68,0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,},
{0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,0x0b,0x29,0x1f,0x3d,0x9c,0x9c,0x0b,0x29,0x1f,0x3d,0x9c,0x9c,0x00,0x4a,0x00,0x54,0x9c,0x9c,0x16,0x16,0x35,0x35,0x9c,0x9c,0x16,0x16,0x5e,0x68,0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,},
{0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,0x0b,0x29,0x1f,0x3d,0x9c,0x9c,0x0b,0x29,0x1f,0x3d,0x9c,0x9c,0x4a,0x00,0x00,0x54,0x9c,0x9c,0x16,0x16,0x35,0x35,0x9c,0x9c,0x16,0x16,0x5e,0x68,0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,},
{0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,0x0b,0x29,0x1f,0x3d,0x9c,0x9c,0x0b,0x29,0x1f,0x3d,0x9c,0x9c,0x4a,0x00,0x54,0x00,0x9c,0x9c,0x16,0x16,0x35,0x35,0x9c,0x9c,0x16,0x16,0x5e,0x68,0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,},
{0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,0x0b,0x29,0x1f,0x3d,0x9c,0x9c,0x0b,0x29,0x1f,0x3d,0x9c,0x9c,0x4a,0x54,0x00,0x00,0x9c,0x9c,0x16,0x16,0x35,0x35,0x9c,0x9c,0x16,0x16,0x5e,0x68,0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,},
{0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,0x0b,0x29,0x1f,0x3d,0x9c,0x9c,0x0b,0x29,0x1f,0x3d,0x9c,0x9c,0x4a,0x54,0x35,0x35,0x9c,0x9c,0x16,0x16,0x00,0x00,0x9c,0x9c,0x16,0x16,0x5e,0x68,0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,},
{0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,0x0b,0x29,0x1f,0x3d,0x9c,0x9c,0x0b,0x29,0x1f,0x3d,0x9c,0x9c,0x4a,0x54,0x35,0x35,0x9c,0x9c,0x16,0x16,0x5e,0x00,0x9c,0x9c,0x16,0x16,0x00,0x68,0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,},
{0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,0x0b,0x29,0x1f,0x3d,0x9c,0x9c,0x0b,0x29,0x1f,0x3d,0x9c,0x9c,0x4a,0x54,0x35,0x35,0x9c,0x9c,0x16,0x16,0x00,0x5e,0x9c,0x9c,0x16,0x16,0x00,0x68,0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,},
{0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,0x0b,0x29,0x1f,0x3d,0x9c,0x9c,0x0b,0x29,0x1f,0x3d,0x9c,0x9c,0x4a,0x54,0x35,0x35,0x9c,0x9c,0x00,0x16,0x16,0x5e,0x9c,0x9c,0x00,0x16,0x16,0x68,0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,0x9c,},
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,},
};
