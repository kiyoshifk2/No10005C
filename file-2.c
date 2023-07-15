#include "apps.h"


int f7_init_flag;

void f7_help();
const char *f7_error_msg();
int f7_init();
void f7_ls();
void f7_debug();
void f7_cd();
void f7_cat();
void f7_disp();
void f7_rm();
void f7_mv();
void f7_get_str(char *filename);
void f7_ClearReadOnly();
void f7_SetReadOnly();
void f7_disp_bmp(int x, int y, const char *filename);


const char *fs_error_msg[]={
	"OK",
	"DISK_ERR",
	"INT_ERR",
	"NOT_READY",
	"NO_FILE",
	"NO_PATH",
	"INVALID_NAME",
	"DENIED",
	"ERROR_EXIST",
	"INVALID_OBJECT",
	"WRITE_PROTECTED",
	"INVALID_DRIVE",
	"NOT_ENABLED",
	"NO_FILESYSTEM",
	"FORMAT_ABORTED",
	"TIMEOUT",
	"LOCKED",
	"NOT_ENOUGH_CORE",
	"TOO_MANY_OPEN_FILES",
	"INVALID_PARAMETER",
	"NOT_ENOUGH_FREE_VOLUME",
	"FS_NOT_SUPPORTED",
	"FS_NOT_MATCH_WITH_VOLUME",
	"NOT_SUPPORTED",
};

/********************************************************************************/
/*		f7_mount_x																	*/
/********************************************************************************/
void f7_mount_x()
{
	SYS_FS_RESULT ret;

	if(f7_init_flag==0){
		f7_init_flag = 1;

		SYS_FS_Unmount("/mnt/myDrive1");
		ret = SYS_FS_Mount
		(
			"/dev/mmcblka1",				// const char *devName, 
			"/mnt/myDrive1",				// const char *mountName,
			FAT,							// SYS_FS_FILE_SYSTEM_TYPE filesystemtype, 
			0,								// reserved, unsigned long mountflags, 
			NULL							// reserved, const void *data
		);
		if(ret==SYS_FS_RES_FAILURE){
			SYS_FS_Unmount("/mnt/myDrive1");
			ret = SYS_FS_Mount
			(
				"/dev/sda1",					// const char *devName, 
				"/mnt/myDrive1",				// const char *mountName,
				FAT,							// SYS_FS_FILE_SYSTEM_TYPE filesystemtype, 
				0,								// reserved, unsigned long mountflags, 
				NULL							// reserved, const void *data
			);
		}
	}
}
/********************************************************************************/
/*		f7_text_save																*/
/********************************************************************************/
void f7_text_save(int restore)
{
	int i;
	static int x, y;
	static char text_buf[15][26];

	f7_mount_x();
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
/*		f7_main																	*/
/********************************************************************************/
const struct menu f7_main_menu[]={
	//        12345678901234567890123456
	{20,  2, "ls   ... directory list"},
	{40,  3, "cd   ... change directory"},
	{60,  4, "cat  ... disp text file"},
	{80,  5, "disp ... disp BMP file"},
	{100, 6, "rm   ... delete file"},
	{120, 7, "mv   ... rename file"},
	{140, 8, "debug... for test"},
	{160, 9, "ClearReadOnly"},
	{180,10, "SetReadOnly"},
	{0,0,0}
};

void f7_main()
{
	int cmd;
	
	lcd_clear(WHITE);
	for(;;){
		sw_up();
		
		f7_text_save(0);
		lcd_clear(WHITE);
		cmd = menu_select(f7_main_menu);
		f7_text_save(1);
		
		if(cmd < 0)						// exit
			return;
		switch(cmd){
//			case 1:						// help
//				f7_help();
//				break;
			case 2:						// ls
				f7_ls();
				break;
			case 3:						// cd
				f7_cd();
				break;
			case 4:						// cat
				f7_cat();
				break;
			case 5:						// disp
				f7_disp();
				break;
			case 6:						// rm
				f7_rm();
				break;
			case 7:						// mv
				f7_mv();
				break;
			case 8:						// debug
				f7_debug();
				break;
			case 9:						// ClearReadOnly
				f7_ClearReadOnly();
				break;
			case 10:						// SetReadOnly
				f7_SetReadOnly();
				break;
		}
        lcd_printf("push any sw\n");
        sw_updownup();
	}
}
/********************************************************************************/
/*		f7_help																	*/
/********************************************************************************/
//void f7_help()
//{
//	lcd_printf("\n");
//	lcd_puts(	"help  ... this message\n"
//				"ls    ... directory list\n"
//				"cd    ... change directory\n"
//				"cat   ... disp text file\n"
//				"disp  ... disp scrn shot\n"
//			);
//}
/********************************************************************************/
/*		f7_ls																	*/
/********************************************************************************/
void f7_ls_sub(SYS_FS_FSTAT stat)
{
	uint8_t fattrib = stat.fattrib;
	char *name;
	
	if(*stat.lfname==0)
		name = stat.fname;
	else
		name = stat.lfname;
	
	if(fattrib & SYS_FS_ATTR_DIR){
		text_color = BRUE;
		lcd_printf(name);
		text_color = BLACK;
	}
	else if(fattrib & SYS_FS_ATTR_RDO){
		text_color = RED;
		lcd_printf(name);
		text_color = BLACK;
	}
	else{
		lcd_printf(name);
	}
	lcd_printf("\n");
	
//	lcd_x = 13;
//	if(fattrib & SYS_FS_ATTR_DIR){
//		lcd_printf("<DIR>\n");
//	}
//	else{
//		lcd_printf("%d\n", stat.fsize);
//	}
}

void f7_ls()
{
	int i, sw;
	char buf[256];
	SYS_FS_HANDLE fd_dir;
	SYS_FS_FSTAT stat;
	char lfname[300];

	f7_mount_x();
	lcd_printf("\n");
	stat.lfname = lfname;				// long file name
	stat.lfsize = 300;
	if((fd_dir = SYS_FS_DirOpen("."))==SYS_FS_HANDLE_INVALID){
		lcd_printf("*** DirOpen error\n");
		lcd_printf("%s\n", f7_error_msg());
		return;							// error
	}
	buf[256-1] = 0;
	if(SYS_FS_CurrentWorkingDirectoryGet(buf, 256-1)==SYS_FS_RES_SUCCESS){
		lcd_puts(buf);
		lcd_printf("\n");
	}
	else{
		lcd_printf("*** current dir err\n");
		lcd_printf("%s\n", f7_error_msg());
	}
	for(;;){
		for(i=0; i<11; i++){
			if(SYS_FS_DirSearch(fd_dir, "*", SYS_FS_ATTR_MASK, &stat)== SYS_FS_RES_FAILURE){
				SYS_FS_DirClose(fd_dir);
				return;					// �I��
			}
			f7_ls_sub(stat);				// 1 ���C���\��
		}
		sw = sw_updownup();
		if(sw & 4){						// exit
			SYS_FS_DirClose(fd_dir);
            lcd_printf("=== aborted\n");
			return;
		}
	}
}

void f7_debug()
{
	int i, sw;
	char buf[256];
	SYS_FS_HANDLE fd_dir;
	SYS_FS_FSTAT stat;
	SYS_FS_RESULT ret;
	char lfname[300];

LCD_DIR_IN;
for(;;);



	lcd_printf("\n");
	SYS_FS_Unmount("/mnt/myDrive1");
	lcd_printf("mount test mmcblka1 ");
	ret = SYS_FS_Mount
	(
		"/dev/mmcblka1",				// const char *devName, 
		"/mnt/myDrive1",				// const char *mountName,
		FAT,							// SYS_FS_FILE_SYSTEM_TYPE filesystemtype, 
		0,								// reserved, unsigned long mountflags, 
		NULL							// reserved, const void *data
	);
	if(ret==SYS_FS_RES_FAILURE){
		lcd_printf("\n*** mount error, mmcblka1, %s\n", f7_error_msg);
		SYS_FS_Unmount("/mnt/myDrive1");
		lcd_printf("mount test sda1 ");
		ret = SYS_FS_Mount
		(
			"/dev/sda1",					// const char *devName, 
			"/mnt/myDrive1",				// const char *mountName,
			FAT,							// SYS_FS_FILE_SYSTEM_TYPE filesystemtype, 
			0,								// reserved, unsigned long mountflags, 
			NULL							// reserved, const void *data
		);
		if(ret==SYS_FS_RES_FAILURE){
			lcd_printf("\n*** mount error, sda1, %s\n", f7_error_msg);
			return;
		}
		else{
			lcd_printf("success\n\n");
		}
	}
	else{
		lcd_printf("success\n\n");
	}

	
	stat.lfname = lfname;				// long file name
	stat.lfsize = 300;
	if((fd_dir = SYS_FS_DirOpen("."))==SYS_FS_HANDLE_INVALID){
		lcd_printf("*** DirOpen error\n");
		lcd_printf("%s\n", f7_error_msg());
		return;							// error
	}
	buf[256-1] = 0;
	if(SYS_FS_CurrentWorkingDirectoryGet(buf, 256-1)==SYS_FS_RES_SUCCESS){
		lcd_puts(buf);
		lcd_printf("\n");
	}
	else{
		lcd_printf("*** current dir err\n");
		lcd_printf("%s\n", f7_error_msg());
	}
	for(;;){
		for(i=0; i<11; i++){
			if(SYS_FS_DirSearch(fd_dir, "*", SYS_FS_ATTR_MASK, &stat)== SYS_FS_RES_FAILURE){
				SYS_FS_DirClose(fd_dir);
				return;					// �I��
			}
			f7_ls_sub(stat);				// 1 ���C���\��
		}
		sw = sw_updownup();
		if(sw & 4){						// exit
			SYS_FS_DirClose(fd_dir);
            lcd_printf("=== aborted\n");
			return;
		}
	}
}
/********************************************************************************/
/*		f7_init																	*/
/********************************************************************************/
#if 0

#define SYS_FS_MAX_FILE_SYSTEM_TYPE            1

// Function pointer table for FAT FS
const SYS_FS_FUNCTIONS FatFsFunctions =
{
    .mount   = f7_mount_x,
    .unmount = f7_unmount,
    .open    = f7_open,
    .read    = f7_read,
    .write   = f7_write,
    .close   = f7_close,
    .seek    = f7_lseek,
    .tell    = f7_tell,
    .eof     = f7_eof,
    .size    = f7_size,
    .fstat   = f7_stat,
};

const SYS_FS_REGISTRATION_TABLE sysFSInit [ SYS_FS_MAX_FILE_SYSTEM_TYPE ] =
{
    {
    .nativeFileSystemType = FAT,
    .nativeFileSystemFunctions = &FatFsFunctions
    }
};


int f7_init()
{
	SYS_FS_RESULT ret;
	
	ret = SYS_FS_Initialize((const void *)sysFSInit);
	if(ret==SYS_FS_RES_FAILURE){			// error
		lcd_printf("*** file system error\n");
		lcd_printf("%s\n", f7_error_msg());
		return -1;							// error
	}
	else{
		lcd_printf("=== file system OK\n");
	}
	
	if (SYS_FS_RES_SUCCESS == SYS_FS_Mount("/dev/mmcblka1", "/mnt/myDrive1", FAT, 0, NULL)){
		lcd_printf("=== Mount success\n");
	}
	else{
		lcd_printf("*** Mount error\n");
		lcd_printf("%s\n", f7_error_msg());
		return -1;							// error
	}
	return 0;								// success
}
#endif
/********************************************************************************/
/*		ut_fs_error_msg															*/
/********************************************************************************/
const char *f7_error_msg()
{
	return fs_error_msg[SYS_FS_Error()];
}
/********************************************************************************/
/*		f7_cd																	*/
/********************************************************************************/
void f7_cd()
{
//	char *ptr;
//	int xsave = lcd_x+1;
	int first = 1;
	int sw;
	SYS_FS_HANDLE fd_dir;
	SYS_FS_FSTAT stat;
	char lfname[300], *name;
	
	f7_mount_x();
	stat.lfname = lfname;					// long file name
	stat.lfsize = 300;
	if((fd_dir = SYS_FS_DirOpen("."))==SYS_FS_HANDLE_INVALID){
		lcd_printf("\n*** DirOpen error\n");
		lcd_printf("%s\n", f7_error_msg());
		return;							// error
	}
	for(;;){
		if(SYS_FS_DirSearch(fd_dir, "*", SYS_FS_ATTR_DIR, &stat)== SYS_FS_RES_FAILURE){
			if(first){
				lcd_printf("\n=== no directory\n");
				SYS_FS_DirClose(fd_dir);
				return;
			}
			if(SYS_FS_DirRewind(fd_dir)==SYS_FS_RES_FAILURE){
				first = 1;
				SYS_FS_DirClose(fd_dir);
				lcd_printf("\n*** Rewind error\n");
				lcd_printf("%s\n", f7_error_msg());
				return;
			}
			continue;
		}
		if(*stat.lfname==0)
			name = stat.fname;
		else
			name = stat.lfname;
		first = 0;
		lcd_x = 0;
		lcd_printf("                         ");
        lcd_x = 0;
		lcd_printf("cd %s", name);
		sw = sw_updownup();
		if(sw & 1){							// sw1
		}
		else if(sw & 2){					// sw2
			SYS_FS_DirClose(fd_dir);
			
			if(SYS_FS_DirectoryChange(name)==SYS_FS_RES_SUCCESS){
				lcd_printf("\n=== success\n");
				return;
			}
			lcd_printf("\n*** chdir error\n");
			lcd_printf("%s\n", f7_error_msg());
			return;
		}
		else if(sw & 4){					// sw3
			SYS_FS_DirClose(fd_dir);
            lcd_printf("=== aborted\n");
			return;
		}
	}
}
/********************************************************************************/
/*		f7_scrn_shot																*/
/********************************************************************************/
//struct BITMAPFILEHEADER { 
//  WORD    bfType; 				// "BM" �� 66,77
//  DWORD   bfSize; 				// file size, 0 �ł��ǂ�
//  WORD    bfReserved1; 			// 0
//  WORD    bfReserved2; 			// 0
//  DWORD   bfOffBits; 				// �C���[�W�擪�܂ł̈ʒu, 0 �ł��ǂ�
//} BITMAPFILEHEADER;

char BITMAPFILEHEADER[14]={'B','M',0,0,0,0,0,0,0,0,0,0,0,0};

struct BITMAPINFOHEADER{
  int  biSize; 				// 40 �Œ�
  int  biWidth; 			// ����
  int  biHeight; 			// ����
  short   biPlanes; 		// 1 �Œ�
  short   biBitCount; 		// �s�N�Z���̃r�b�g�� 0, 1, 4, 8, 16, 24, 32 �̂����ꂩ
  int  biCompression; 		// 0 �Ȃ�񈳏k
  int  biSizeImage; 		// �C���[�W�f�[�^�T�C�Y�A�񈳏k�Ȃ� 0 �ŗǂ�
  int  biXPelsPerMeter; 	// �����𑜓x�A0 �ŗǂ�
  int  biYPelsPerMeter; 	// �����𑜓x�A0 �ŗǂ�
  int  biClrUsed; 			// �J���[�p���b�g�Ɋi�[�����F�����������������l�œ���B
							// ���̒l�� 0 �Ńs�N�Z�����̃r�b�g���� 8 �ȉ��̏ꍇ�A�F����2���s�N�Z�����̃r�b�g���̒l�ŗݏ悵�����ɂȂ�B
							// ���Ȃ킿1�r�b�g�Ȃ��2�F�A4�r�b�g�Ȃ��16�F�A8�r�b�g�Ȃ��256�F�ƂȂ�B
  int  biClrImportant; 		// �d�v�F���A0 �Ȃ�S�ďd�v�F�ɂȂ�
};

const struct BITMAPINFOHEADER BITMAPINFOHEADER={
	40,
	320,
	240,
	1,
	24,
	0,
	0,
	0,
	0,
	0,
	0,
};

int f7_scrn_shot()
{
	SYS_FS_HANDLE fd_ss;
	struct eeprom_data data;
	char *dir = "/mnt/myDrive1/scrnshot";
	char filename[60];
	int i, line;
	char c1, c2, c3;
	
	f7_mount_x();
	SYS_FS_DirectoryMake(dir);				// make directory
	ee_read(&data, sizeof(data), 0);
	++data.ss_numb;
	ee_write(&data, sizeof(data), 0);
	sprintf(filename, "%s/ss_%d.bmp", dir, data.ss_numb);	// �t�@�C�����쐬
	
	if((fd_ss=SYS_FS_FileOpen(filename, SYS_FS_FILE_OPEN_WRITE))==SYS_FS_HANDLE_INVALID){	// open
		/*** error ***/
		if(lcd_x != 0)
			lcd_printf("\n");
		lcd_printf("*** Open error\n");
		lcd_printf("%s\n", f7_error_msg());
		return -1;							// error
	}
	if(SYS_FS_FileWrite(fd_ss, BITMAPFILEHEADER, sizeof(BITMAPFILEHEADER)) != sizeof(BITMAPFILEHEADER))
		goto err_end;
	if(SYS_FS_FileWrite(fd_ss, &BITMAPINFOHEADER, sizeof(BITMAPINFOHEADER)) != sizeof(BITMAPINFOHEADER))
		goto err_end;
	for(line=239; line>=0; --line){
		LCD_ALL_S;								// idle
		LCD_CS_C;								// CS clear
		lcd_area_set(0, line, 319, line);
		lcd_write_cmd(MemoryRead);

		LCD_DIR_IN;								// ���̓|�[�g
	    lcd_read_data();
		lcd_read_data();
		for(i=0; i<320; i++){
			c1 = lcd_read_data();
			c2 = lcd_read_data();
			c3 = lcd_read_data();
			if(SYS_FS_FileCharacterPut(fd_ss, c1)==SYS_FS_RES_FAILURE)
				goto err_end;
			if(SYS_FS_FileCharacterPut(fd_ss, c2)==SYS_FS_RES_FAILURE)
				goto err_end;
			if(SYS_FS_FileCharacterPut(fd_ss, c3)==SYS_FS_RES_FAILURE)
				goto err_end;
		}
		LCD_DIR_OUT;							// �o�̓|�[�g
		LCD_ALL_S;								// idle
	}
	SYS_FS_FileClose(fd_ss);
    f7_text_save(0);
//	message("screen shot\n%s", filename);;
    f7_text_save(1);
	return 0;								// success
	
err_end:;
	SYS_FS_FileClose(fd_ss);
	if(lcd_x)
		lcd_printf("\n");
	lcd_printf("*** file write error\n");
	lcd_printf("%s\n", f7_error_msg());
	return -1;								// error
}
/********************************************************************************/
/*		f7_cat																	*/
/********************************************************************************/
void f7_cat()
{
	int i, len, sw;
	char buf[50];
//	int xsave = lcd_x+1;
	int first = 1;
	SYS_FS_HANDLE fd_txt, fd_dir;
	SYS_FS_FSTAT stat;
	char lfname[300], *name;
	
	f7_mount_x();
	stat.lfname = lfname;					// long file name
	stat.lfsize = 300;
	if((fd_dir = SYS_FS_DirOpen("."))==SYS_FS_HANDLE_INVALID){
		lcd_printf("\n*** DirOpen error\n");
		lcd_printf("%s\n", f7_error_msg());
		return;							// error
	}
	for(;;){
		if(SYS_FS_DirSearch(fd_dir, "*", SYS_FS_ATTR_ARC, &stat)== SYS_FS_RES_FAILURE){
			if(first){
				lcd_printf("\n=== no file\n");
				SYS_FS_DirClose(fd_dir);
				return;
			}
			if(SYS_FS_DirRewind(fd_dir)==SYS_FS_RES_FAILURE){
				first = 1;
				SYS_FS_DirClose(fd_dir);
				lcd_printf("\n*** Rewind error\n");
				lcd_printf("%s\n", f7_error_msg());
				return;
			}
			continue;
		}
		if(*stat.lfname==0)
			name = stat.fname;
		else
			name = stat.lfname;
		first = 0;
		lcd_x = 0;
		lcd_printf("                         ");
        lcd_x = 0;
		lcd_printf("cat %s", name);
		sw = sw_updownup();
		if(sw & 1){							// sw1
		}
		else if(sw & 2){					// sw2
            lcd_putc('\n');
			SYS_FS_DirClose(fd_dir);
			if((fd_txt=SYS_FS_FileOpen(name, SYS_FS_FILE_OPEN_READ))==SYS_FS_HANDLE_INVALID){
				lcd_printf("*** Open error\n");
				lcd_printf("%s\n", f7_error_msg());
				return;
			}
			for(;;){
				len = SYS_FS_FileRead(fd_txt, buf, 50);
				if(len<=0){
					SYS_FS_FileClose(fd_txt);
					return;
				}
				for(i=0; i<len; i++){
					lcd_putc(buf[i]);
				}
				while((sw=sw_read())){
					if(sw & 4){
						SYS_FS_FileClose(fd_txt);
						return;
					}
				}
			}
		}
		else if(sw & 4){					// sw3
			SYS_FS_DirClose(fd_dir);
			return;
		}
	}
}
/********************************************************************************/
/*		f7_disp																	*/
/********************************************************************************/
void f7_disp()
{
	int sw;
//	char c[3];
//	char buf[54];
//	int xsave = lcd_x+1;
	int first = 1;
	SYS_FS_HANDLE  fd_dir;
	SYS_FS_FSTAT stat;
	char lfname[300], *name;
	
	f7_mount_x();
	stat.lfname = lfname;					// long file name
	stat.lfsize = 300;
	if((fd_dir = SYS_FS_DirOpen("."))==SYS_FS_HANDLE_INVALID){
		lcd_printf("\n*** DirOpen error\n");
		lcd_printf("%s\n", f7_error_msg());
		return;							// error
	}
	for(;;){
		if(SYS_FS_DirSearch(fd_dir, "*.bmp", SYS_FS_ATTR_ARC, &stat)== SYS_FS_RES_FAILURE){
			if(first){
				lcd_printf("\n=== no file\n");
				SYS_FS_DirClose(fd_dir);
				return;
			}
			if(SYS_FS_DirRewind(fd_dir)==SYS_FS_RES_FAILURE){
				first = 1;
				SYS_FS_DirClose(fd_dir);
				lcd_printf("\n*** Rewind error\n");
				lcd_printf("%s\n", f7_error_msg());
				return;
			}
			continue;
		}
		if(*stat.lfname==0)
			name = stat.fname;
		else
			name = stat.lfname;
		first = 0;
		lcd_x = 0;
		lcd_printf("                         ");
        lcd_x = 0;
		lcd_printf("disp %s", name);
		sw = sw_updownup();
		if(sw & 1){							// sw1
		}
		else if(sw & 2){					// sw2
            lcd_putc('\n');
			SYS_FS_DirClose(fd_dir);
#if 0
			if((fd_txt=SYS_FS_FileOpen(name, SYS_FS_FILE_OPEN_READ))==SYS_FS_HANDLE_INVALID){
				lcd_printf("*** Open error\n");
				lcd_printf("%s\n", f7_error_msg());
				return;
			}
            f7_text_save(0);
			SYS_FS_FileRead(fd_txt, buf, 54);	// header skip
			for(line=239; line>=0; --line){
				LCD_ALL_S;						// idle
				LCD_CS_C;						// CS clear
				lcd_area_set(0, line, 319, line);
				lcd_write_cmd(MemoryWrite);
				for(i=0; i<320; i++){
					SYS_FS_FileRead(fd_txt, c, 3);
					lcd_write_color(RGB(c[2],c[1],c[0]));
				}
				LCD_ALL_S;						// idle
			}
			SYS_FS_FileClose(fd_txt);
#endif
			f7_disp_bmp(0, 0, name);
			sw_updownup();
            f7_text_save(1);
			return;
		}
		else if(sw & 4){					// sw3: exit
			SYS_FS_DirClose(fd_dir);
			return;
		}
	}
}
/********************************************************************************/
/*		calc_save																*/
/*		�d��v���O�����̒��̊֐��ł�											*/
/********************************************************************************/
void calc_save()
{
	SYS_FS_HANDLE fd_dir, fd_prg;
	SYS_FS_FSTAT stat;
	struct eeprom_data data;
	char *dir = "/mnt/myDrive1/prg-calc";
//	char *srch= "/mnt/myDrive1/prg-calc/*.prg";
	char filename[300];
	char lfname[300], *name;
	int numb, sw;
	
	f7_mount_x();
	SYS_FS_DirectoryMake(dir);				// make directory

	if(SYS_FS_DirectoryChange(dir)==SYS_FS_RES_FAILURE){
		lcd_printf("*** chdir error\n");
		lcd_printf("%s\n", f7_error_msg());
		return;
	}
	stat.lfname = lfname;					// long file name
	stat.lfsize = 300;
	if((fd_dir = SYS_FS_DirOpen("."))==SYS_FS_HANDLE_INVALID){
		lcd_printf("*** DirOpen error\n");
		lcd_printf("%s\n", f7_error_msg());
		return;							// error
	}
	numb = 0;
    name = "";
	for(;;){
		lcd_x = 0;
		lcd_printf("                          ");
		lcd_x = 0;
		if(numb==0){
			lcd_printf("SAVE <<New file>>");
		}
		else{
			if(SYS_FS_DirSearch(fd_dir, "*.prg", SYS_FS_ATTR_ARC, &stat)== SYS_FS_RES_FAILURE){
				if(SYS_FS_DirRewind(fd_dir)==SYS_FS_RES_FAILURE){
					SYS_FS_DirClose(fd_dir);
					lcd_printf("\n*** Rewind error\n");
					lcd_printf("%s\n", f7_error_msg());
					return;
				}
				numb = 0;
				continue;
			}
			if(*stat.lfname==0)
				name = stat.fname;
			else
				name = stat.lfname;
			lcd_printf("SAVE %s", name);
		}
		sw = sw_updownup();
		if(sw & 1){						// sw1: select
			numb = 1;
		}
		else if(sw & 2){				// sw2: set
			SYS_FS_DirClose(fd_dir);
			if(numb==0){				// new file
				ee_read(&data, sizeof(data), 0);
				++data.prg_numb;
				ee_write(&data, sizeof(data), 0);
				sprintf(filename, "%s/prg%d.prg", dir, data.prg_numb);
next1:;
				if((fd_prg=SYS_FS_FileOpen(filename, SYS_FS_FILE_OPEN_WRITE))==SYS_FS_HANDLE_INVALID){	// open
					lcd_printf("\n*** Open error\n");
					lcd_printf("%s\n", f7_error_msg());
					return;
				}
				if(SYS_FS_FileWrite(fd_prg, cal_v.prg_buf, sizeof(cal_v.prg_buf)) != sizeof(cal_v.prg_buf)){
					SYS_FS_FileClose(fd_prg);
					lcd_printf("\n*** write error\n");
					lcd_printf("%s\n", f7_error_msg());
					return;
				}
				SYS_FS_FileClose(fd_prg);
				lcd_printf("\n=== success\n");
				return;
			}
			else{
				sprintf(filename, "%s/%s", dir, name);
				goto next1;
			}
		}
		else if(sw & 4){				// sw3: exit
			SYS_FS_DirClose(fd_dir);
			lcd_printf("\n=== aborted\n");
			return;
		}
	}
}
/********************************************************************************/
/*		calc_load																*/
/*		�d��v���O�����̒��̊֐��ł�											*/
/********************************************************************************/
void calc_load()
{
	SYS_FS_HANDLE fd_dir, fd_prg;
	SYS_FS_FSTAT stat;
//	struct eeprom_data data;
	char *dir = "/mnt/myDrive1/prg-calc";
//	char *srch= "/mnt/myDrive1/prg-calc/*.prg";
	char filename[300];
	char lfname[300], *name;
	int first, sw;

	f7_mount_x();
	if(SYS_FS_DirectoryChange(dir)==SYS_FS_RES_FAILURE){
		lcd_printf("*** chdir error\n");
		lcd_printf("%s\n", f7_error_msg());
		return;
	}
	stat.lfname = lfname;					// long file name
	stat.lfsize = 300;
	if((fd_dir = SYS_FS_DirOpen("."))==SYS_FS_HANDLE_INVALID){
		lcd_printf("*** DirOpen error\n");
		lcd_printf("%s\n", f7_error_msg());
		return;							// error
	}
	first = 1;
	for(;;){
		lcd_x = 0;
		lcd_printf("                          ");
		lcd_x = 0;
		if(SYS_FS_DirSearch(fd_dir, "*.prg", SYS_FS_ATTR_ARC, &stat)== SYS_FS_RES_FAILURE){
			if(first){
				lcd_printf("\n*** no file\n");
				lcd_printf("%s\n", f7_error_msg());
				return;
			}
			if(SYS_FS_DirRewind(fd_dir)==SYS_FS_RES_FAILURE){
				first = 1;
				SYS_FS_DirClose(fd_dir);
				lcd_printf("\n*** Rewind error\n");
				lcd_printf("%s\n", f7_error_msg());
				return;
			}
			continue;
		}
		if(*stat.lfname==0)
			name = stat.fname;
		else
			name = stat.lfname;
		first = 0;
		lcd_printf("LOAD %s", name);
		sw = sw_updownup();
		if(sw & 1){						// sw1: select
		}
		else if(sw & 2){				// sw2: set
			SYS_FS_DirClose(fd_dir);
			sprintf(filename, "%s/%s", dir, name);
			if((fd_prg=SYS_FS_FileOpen(filename, SYS_FS_FILE_OPEN_READ))==SYS_FS_HANDLE_INVALID){	// open
				lcd_printf("\n*** Open error\n");
				lcd_printf("%s\n", f7_error_msg());
				return;
			}
			if(SYS_FS_FileRead(fd_prg, cal_v.prg_buf, sizeof(cal_v.prg_buf)) < 0){
				SYS_FS_FileClose(fd_prg);
				lcd_printf("\n*** read error\n");
				lcd_printf("%s\n", f7_error_msg());
				return;
			}
			SYS_FS_FileClose(fd_prg);
			lcd_printf("\n=== success\n");
			return;
		}
		else if(sw & 4){				// sw3: exit
			SYS_FS_DirClose(fd_dir);
			lcd_printf("\n=== aborted\n");
			return;
		}
	}
}
/********************************************************************************/
/*		f7_rm																	*/
/********************************************************************************/
void f7_rm()
{
	SYS_FS_HANDLE fd_dir;
	SYS_FS_FSTAT stat;
//	char filename[100];
	char lfname[300], *name;
	int first, sw;

	f7_mount_x();
	stat.lfname = lfname;					// long file name
	stat.lfsize = 300;
	if((fd_dir = SYS_FS_DirOpen("."))==SYS_FS_HANDLE_INVALID){
		lcd_printf("*** DirOpen error\n");
		lcd_printf("%s\n", f7_error_msg());
		return;							// error
	}
	first = 1;
	for(;;){
		lcd_x = 0;
		lcd_printf("                          ");
		lcd_x = 0;
		if(SYS_FS_DirSearch(fd_dir, "*", SYS_FS_ATTR_ARC, &stat)== SYS_FS_RES_FAILURE){
			if(first){
				lcd_printf("\n*** no file\n");
				lcd_printf("%s\n", f7_error_msg());
				return;
			}
			if(SYS_FS_DirRewind(fd_dir)==SYS_FS_RES_FAILURE){
				first = 1;
				SYS_FS_DirClose(fd_dir);
				lcd_printf("\n*** Rewind error\n");
				lcd_printf("%s\n", f7_error_msg());
				return;
			}
			continue;
		}
		if(*stat.lfname==0)
			name = stat.fname;
		else
			name = stat.lfname;
		first = 0;
		lcd_printf("rm %s", name);
		sw = sw_updownup();
		if(sw & 1){						// sw1: select
		}
		else if(sw & 2){				// set
			lcd_printf("\nDelete OK ?\n");
			sw = sw_updownup();
			if(sw & 1)					// sw2: sel
				continue;
			else if(sw & 4){			// sw3: exit
				SYS_FS_DirClose(fd_dir);
				lcd_printf("\n=== aborted\n");
				return;
			}
			SYS_FS_DirClose(fd_dir);
			if(SYS_FS_FileDirectoryRemove(name)==SYS_FS_RES_FAILURE){
				lcd_printf("\n*** remove error\n");
				lcd_printf("%s\n", f7_error_msg());
				return;
			}
			lcd_printf("\n=== deleted\n");
			return;
		}
		else if(sw & 4){				// exit
			SYS_FS_DirClose(fd_dir);
			lcd_printf("\n=== aborted\n");
			return;
		}
	}
}
/********************************************************************************/
/*		f7_mv																	*/
/********************************************************************************/
void f7_mv()
{
	SYS_FS_HANDLE fd_dir;
	SYS_FS_FSTAT stat;
	char filename[300];
	char lfname[300], *name;
	int first, sw;

	f7_mount_x();
	stat.lfname = lfname;					// long file name
	stat.lfsize = 300;
	if((fd_dir = SYS_FS_DirOpen("."))==SYS_FS_HANDLE_INVALID){
		lcd_printf("*** DirOpen error\n");
		lcd_printf("%s\n", f7_error_msg());
		return;							// error
	}
	first = 1;
	for(;;){
		lcd_x = 0;
		lcd_printf("                          ");
		lcd_x = 0;
		if(SYS_FS_DirSearch(fd_dir, "*", SYS_FS_ATTR_ARC, &stat)== SYS_FS_RES_FAILURE){
			if(first){
				lcd_printf("\n*** no file\n");
				lcd_printf("%s\n", f7_error_msg());
				return;
			}
			if(SYS_FS_DirRewind(fd_dir)==SYS_FS_RES_FAILURE){
				first = 1;
				SYS_FS_DirClose(fd_dir);
				lcd_printf("\n*** Rewind error\n");
				lcd_printf("%s\n", f7_error_msg());
				return;
			}
			continue;
		}
		if(*stat.lfname==0)
			name = stat.fname;
		else
			name = stat.lfname;
		first = 0;
		lcd_printf("mv %s", name);
		sw = sw_updownup();
		if(sw & 1){						// sw1: select
		}
		else if(sw & 2){				// sw2: set
			f7_text_save(0);
			f7_get_str(filename);
			f7_text_save(1);
			
			lcd_x = 0;
			lcd_printf("mv %s %s\n", name, filename);
			lcd_printf("Rename OK ?\n");
			sw = sw_updownup();
			if(sw & 1){					// sw3: sel
				continue;
			}
			else if(sw & 4){
				SYS_FS_DirClose(fd_dir);
				lcd_printf("\n=== aborted\n");
				return;
			}
			SYS_FS_DirClose(fd_dir);
			if(SYS_FS_FileDirectoryRenameMove(name, filename)==SYS_FS_RES_FAILURE){
				lcd_printf("\n*** rename error\n");
				lcd_printf("%s\n", f7_error_msg());
				return;
			}
			lcd_printf("\n=== success\n");
			return;
		}
		else if(sw & 4){				// exit
			SYS_FS_DirClose(fd_dir);
			lcd_printf("\n=== aborted\n");
			return;
		}
	}
}
/********************************************************************************/
/*		f7_get_str																*/
/********************************************************************************/
void f7_get_str(char *str)
{
	int x, y, cnt, sw, c;
	
	lcd_clear(WHITE);
	for(y=0; y<10; y++){					// keyboard display
		for(x=0; x<16; x++){
			c = y<6 ? 0x20+y*0x10+x : 0x40+y*0x10+x;
			lcd_disp_char_x1(64+x*12, 64+16*y, c);
		}
	}
	lcd_disp_str_x1(100, 224, "end");
	lcd_disp_str_x1(184, 224, "DEL");
	
	x = y = cnt = 0;
	str[cnt] = 0;
	for(;;){								// input
		lcd_x = lcd_y = 0;
		lcd_printf("                          ");
		lcd_x = lcd_y = 0;
		lcd_printf(str);
		
		back_color = BRUE;					// �J�[�\���L�[���]�\��
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
		
		sw = sw_updownup();					// sw ����
		if(y < 10){							// �J�[�\���L�[�m�[�}���\��
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
					return;
				else{						// DEL
					if(cnt)
						str[--cnt] = 0;
				}
			}
			else{
				str[cnt++] = c;
				str[cnt] = 0;
				if(cnt > 50)				// buffer full
					return;
			}
		}
	}
}
/********************************************************************************/
/*		f7_ClearReadOnly															*/
/********************************************************************************/
void f7_ClearReadOnly()
{
	SYS_FS_HANDLE fd_dir;
	SYS_FS_FSTAT stat;
	int first, sw;
	char lfname[300], *name;

	f7_mount_x();
	stat.lfname = lfname;					// long file name
	stat.lfsize = 300;
	if((fd_dir = SYS_FS_DirOpen("."))==SYS_FS_HANDLE_INVALID){
		lcd_printf("*** DirOpen error\n");
		lcd_printf("%s\n", f7_error_msg());
		return;							// error
	}
	first = 1;
	for(;;){
		lcd_x = 0;
		lcd_printf("                          ");
		lcd_x = 0;
		if(SYS_FS_DirSearch(fd_dir, "*", SYS_FS_ATTR_ARC, &stat)== SYS_FS_RES_FAILURE){
			if(first){
				lcd_printf("\n*** no file\n");
				lcd_printf("%s\n", f7_error_msg());
				return;
			}
			if(SYS_FS_DirRewind(fd_dir)==SYS_FS_RES_FAILURE){
				first = 1;
				SYS_FS_DirClose(fd_dir);
				lcd_printf("\n*** Rewind error\n");
				lcd_printf("%s\n", f7_error_msg());
				return;
			}
			continue;
		}
		if(*stat.lfname==0)
			name = stat.fname;
		else
			name = stat.lfname;
		first = 0;
		lcd_printf("Clear %s", name);
		sw = sw_updownup();
		if(sw & 1){						// sw1: select
		}
		else if(sw & 2){				// sw2: set
			SYS_FS_DirClose(fd_dir);
			if(SYS_FS_FileDirectoryModeSet(name, 0, SYS_FS_ATTR_RDO)==SYS_FS_RES_FAILURE){
				lcd_printf("\n*** Clear Rdonly error\n");
				lcd_printf("%s\n", f7_error_msg());
				return;
			}
			lcd_printf("\n=== success\n");
			return;
		}
		else if(sw & 4){				// sw3: exit
			SYS_FS_DirClose(fd_dir);
			lcd_printf("\n=== aborted\n");
			return;
		}
	}
}
/********************************************************************************/
/*		f7_SetReadOnly															*/
/********************************************************************************/
void f7_SetReadOnly()
{
	SYS_FS_HANDLE fd_dir;
	SYS_FS_FSTAT stat;
	int first, sw;
	char lfname[300], *name;

	f7_mount_x();
	stat.lfname = lfname;					// long file name
	stat.lfsize = 300;
	if((fd_dir = SYS_FS_DirOpen("."))==SYS_FS_HANDLE_INVALID){
		lcd_printf("*** DirOpen error\n");
		lcd_printf("%s\n", f7_error_msg());
		return;							// error
	}
	first = 1;
	for(;;){
		lcd_x = 0;
		lcd_printf("                          ");
		lcd_x = 0;
		if(SYS_FS_DirSearch(fd_dir, "*", SYS_FS_ATTR_ARC, &stat)== SYS_FS_RES_FAILURE){
			if(first){
				lcd_printf("\n*** no file\n");
				lcd_printf("%s\n", f7_error_msg());
				return;
			}
			if(SYS_FS_DirRewind(fd_dir)==SYS_FS_RES_FAILURE){
				first = 1;
				SYS_FS_DirClose(fd_dir);
				lcd_printf("\n*** Rewind error\n");
				lcd_printf("%s\n", f7_error_msg());
				return;
			}
			continue;
		}
		if(*stat.lfname==0)
			name = stat.fname;
		else
			name = stat.lfname;
		first = 0;
		lcd_printf("Set %s", name);
		sw = sw_updownup();
		if(sw & 1){						// sw1: select
		}
		else if(sw & 2){				// sw2: set
			SYS_FS_DirClose(fd_dir);
			if(SYS_FS_FileDirectoryModeSet(name, SYS_FS_ATTR_RDO, SYS_FS_ATTR_RDO)==SYS_FS_RES_FAILURE){
				lcd_printf("\n*** Clear Rdonly error\n");
				lcd_printf("%s\n", f7_error_msg());
				return;
			}
			lcd_printf("\n=== success\n");
			return;
		}
		else if(sw & 4){				// sw3: exit
			SYS_FS_DirClose(fd_dir);
			lcd_printf("\n=== aborted\n");
			return;
		}
	}
}
/********************************************************************************/
/*		f7_disp_bmp																*/
/********************************************************************************/
void f7_disp_bmp(int x, int y, const char *filename)
{
	SYS_FS_HANDLE fd_bmp;
	char buf[14];
	struct BITMAPINFOHEADER header;
	int i, k, line, cnt, x_stop;
	char c[3];
	
	f7_mount_x();
	if(x<0 || x>=320 || y<0 || y>=240){
		lcd_printf("*** position error");
		return;
	}
	if((fd_bmp=SYS_FS_FileOpen(filename, SYS_FS_FILE_OPEN_READ))==SYS_FS_HANDLE_INVALID){
		lcd_printf("*** Open error\n");
		lcd_printf("%s\n", f7_error_msg());
		return;
	}
//	SYS_FS_FileRead(fd_txt, buf, 54);	// header skip
	SYS_FS_FileRead(fd_bmp, buf, 14);	// header read
	if(buf[0] != 'B' || buf[1] != 'M'){
		lcd_printf("*** file header error");
		return;
	}
	SYS_FS_FileRead(fd_bmp, &header, sizeof(header));	// header read
    f7_text_save(0);
	
	line = y+header.biHeight-1;
	x_stop = x+header.biWidth-1;
	if(x_stop >= 320)
		x_stop = 320-1;
	for(i=0; i<header.biHeight; ++i){
		if(line>=0 && line<240){
			LCD_ALL_S;						// idle
			LCD_CS_C;						// CS clear
			lcd_area_set(x, line, x_stop, line);
			lcd_write_cmd(MemoryWrite);
			cnt = 0;
			for(k=0; k<header.biWidth; k++){
				cnt += 3;
				SYS_FS_FileRead(fd_bmp, c, 3);
				if(x+k < 320)
    				lcd_write_color(RGB(c[2],c[1],c[0]));
			}
			if(cnt%4){
				SYS_FS_FileRead(fd_bmp, c, 4-cnt%4);
			}
			LCD_ALL_S;						// idle
		}
		else{								// �\���͈͊O
			cnt = 0;
			for(k=0; k<header.biWidth; k++){
				cnt += 3;
				SYS_FS_FileRead(fd_bmp, c, 3);
			}
			if(cnt%4){
				SYS_FS_FileRead(fd_bmp, c, 4-cnt%4);
			}
		}
		--line;
	}
	SYS_FS_FileClose(fd_bmp);
}
