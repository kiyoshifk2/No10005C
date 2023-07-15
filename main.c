
/*******************************************************************************
  Main Source File

  Company:
    Microchip Technology Inc.

  File Name:
    main.c

  Summary:
    This file contains the "main" function for a project.

  Description:
    This file contains the "main" function for a project.  The
    "main" function calls the "SYS_Initialize" function to initialize the state
    machines of all modules in the system
 *******************************************************************************/

// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************

#include <stddef.h>                     // Defines NULL
#include <stdbool.h>                    // Defines true
#include <stdlib.h>                     // Defines EXIT_FAILURE
#include "definitions.h"                // SYS function prototypes
#include "apps.h"


int buzzer_flag;						// 0:normal, 1:短音指定(50ms 1KHz), 2:長音指定(500ms 1KHz)
volatile int TickCount;
void lcd_printf(const char *fmt, ...);

extern int f7_init_flag;

// *****************************************************************************
// *****************************************************************************
// Section: Main Entry Point
// *****************************************************************************
// *****************************************************************************

/*
 *                       Main application
 *
 *    CPU clock 80MHz, I/O clock 40MHz
 *    LCD への書き込み速度 4MByte/sec
 *        1pixcel 2byte
 *
 *    wait 200ms at 40MHz clock
 *    wait 140ms at 80MHz clock (2wait)
 *    volatile int i;
 *    for(i=0; i<1000000; i++) ;
 * 
 *   i = j;  // 35ns at 80MHz clock
 *   
 */

int main ( void )
{
    /* Initialize all modules */
    SYS_Initialize ( NULL );

//	__builtin_enable_interrupts();

    while ( true )
    {
    	oscillo();
    }

    /* Execution should not come here during normal operation */

    return ( EXIT_FAILURE );
}


/*******************************************************************************
 End of File
*/


/********************************************************************************/
/*		timer_init																*/
/*      100μs                                                                  */
/********************************************************************************/
void timer_init()
{
	T2CON = 0;
	PR2 = 4000-1;							// 40MHz/4000 = 10KHz
	TMR2 = 0;
	IPC2bits.T2IP = 4;
	IFS0bits.T2IF = 0;
	IEC0bits.T2IE = 1;
	T2CONSET=0x8000;						// TIMER2 ON
}

void __ISR(_TIMER_2_VECTOR, IPL4AUTO) T2Interrupt()
{
	static int t2_500us, t2_cycle;
	
	++TickCount;
	IFS0bits.T2IF = 0;
	if(buzzer_flag==1){						// 1:短音指定(50ms 1KHz)
		buzzer_flag = 0;
		t2_500us = 5;
		t2_cycle= 100;
	}
	else if(buzzer_flag==2){				// 2:長音指定(500ms 1KHz)
		buzzer_flag = 0;
		t2_500us = 5;
		t2_cycle = 1000;
	}
	if(t2_500us){
		if(--t2_500us==0){
//			LATBINV = 0x4000;				// RB14 反転
			LATGINV = 0x0004;				// RG2 反転
			if(t2_cycle){
				if(--t2_cycle){
					t2_500us = 5;
				}
			}
		}
	}
}

/********************************************************************************/
/*		timer3																	*/
/*		ADC timer, impulseタイマー												*/
/********************************************************************************/
#if 0
int t3cnt;
int lfsr=1;
int timer3_M;

void __ISR(_TIMER_3_VECTOR, IPL5AUTO) T3Interrupt()
{
	IFS0bits.T3IF = 0;
    if(timer3_M){
    	lfsr = (lfsr>>1) ^ (-(lfsr & 1) & 0xe08);
        LATBbits.LATB12 = lfsr & 1;
        return;
    }
	if(++t3cnt==4096){
		t3cnt = 0;
		LATBSET = 0x1000;
		return;
	}
	LATBCLR = 0x1000;
}
/********************************************************************************/
/*		ADC																		*/
/********************************************************************************/

extern const char m_keiretu[4095];
int ADC_int_tmp;
    
void __ISR(_ADC_VECTOR, IPL5AUTO) ADCIntgerrupt()
{
	lfsr = (lfsr>>1) ^ (-(lfsr & 1) & 0xe08);
	LATBbits.LATB12 = lfsr & 1;

    ADC_int_tmp = ADC1BUF0;
	IFS0bits.AD1IF = 0;
}
#endif

/********************************************************************************/
/*		APP_Tasks																*/
/********************************************************************************/
void APP_Tasks()
{
}
/********************************************************************************/
/*		APP_Initialize															*/
/********************************************************************************/
void APP_Initialize()
{
	f7_init_flag = 0;
	timer_init();
	lcd_init();
	osci_init();
}