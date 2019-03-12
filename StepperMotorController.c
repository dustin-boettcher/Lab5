// StepperMotorController.c starter file EE319K Lab 5
// Runs on TM4C123
// Finite state machine to operate a stepper motor.  
// Jonathan Valvano
// January 18, 2019

// Hardware connections (External: two input buttons and four outputs to stepper motor)
//  PA5 is Wash input  (1 means pressed, 0 means not pressed)
//  PA4 is Wiper input  (1 means pressed, 0 means not pressed)
//  PE5 is Water pump output (toggle means washing)
//  PE4-0 are stepper motor outputs 
//  PF1 PF2 or PF3 control the LED on Launchpad used as a heartbeat
//  PB6 is LED output (1 activates external LED on protoboard)

#include "SysTick.h"
#include "TExaS.h"
#include <stdint.h>
#include "../inc/tm4c123gh6pm.h"

struct State {
	uint16_t Output;
	uint16_t flash;
	uint16_t Time;
	uint16_t next[4];
};
typedef const struct State STyp;

#define initial 0
	#define p1off 1
	#define p2off 2
	#define p3off 3
	#define p4off 4
	#define p5off 5
	#define p6off 6
	#define p7off 7
	#define p8off 8
	#define p9off 9
	#define p10off 10
	#define p11off 11
	#define p12off 12
	#define p13off 13
	#define p14off 14
	#define p15off 15
	#define p16off 16
	#define p17off 17
	#define p18off 18
	#define p19off 19
	#define initial_on 20
	
STyp FSM[40] = {
	{1, 1, 50, {initial, initial_on, p1off, 21}}, //initial
	{1, 0, 50, {2, 2, 2, 2}}, //steps through all 20 states when windshield wiper pressed
	{2, 0, 50, {3, 22, 3, 22}},
	{4, 0, 50, {4, 4, 4, 4}},
	{8, 0, 50, {5, 0, 5, 0}},
	{16, 0, 50, {6, 6, 6, 6}},
	{1, 0, 50, {7, 0, 7, 0}},
	{2, 0, 50, {8, 8, 8, 8}},
	{4, 0, 50, {9, 0, 9, 0}},
	{8, 0, 50, {10, 10, 10, 10}},
	{16, 0, 50, {11, 0, 11, 0}},
	{8, 0, 50, {12, 12, 12, 12}},
	{4, 0, 50, {13, 0, 13, 0}},
	{2, 0, 50, {14, 14, 14, 14}},
	{1, 0, 50, {15, 0, 15, 0}},
	{16, 0, 50, {16, 16, 16, 16}},
	{8, 0, 50, {17, 0, 17, 0}},
	{4, 0, 50, {18, 18, 18, 18}},
	{2, 0, 50, {19, 0, 19, 0}},
	{1, 0, 50, {0, 0, 0, 0}}, //19
	{0, 1, 50, {0, 0, 0, 0}}, //on state for initial
	{1, 1, 50, {2, 22, 2, 22}}, //21 - 1
	{0, 0, 0, {3, 23, 3, 23}}, //22 - 2
	{0, 0, 0, {4, 24, 4, 24}}, //23 - 3
	{0, 0, 0, {5, 25, 5, 25}}, //24 -4
	{0, 0, 0, {6, 6, 6, 6}}, //25 - 5
	{0, 0, 0, {12, 27, 12, 27}}, //26 - 11
	{0, 0, 0, {0, 0, 0, 0}}, //27 - 12
	{0, 0, 0, {0, 0, 0, 0}}, //28- 13
	{0, 0, 0, {0, 0, 0, 0}}, //29 - 14
	{0, 0, 0, {0, 0, 0, 0}}, //30 - 15
	{0, 0, 0, {0, 0, 0, 0}},
	{0, 0, 0, {0, 0, 0, 0}},
	{0, 0, 0, {0, 0, 0, 0}},
	{0, 0, 0, {0, 0, 0, 0}},
	{0, 0, 0, {0, 0, 0, 0}},
	{0, 0, 0, {0, 0, 0, 0}},
	{0, 0, 0, {0, 0, 0, 0}},
	{0, 0, 0, {0, 0, 0, 0}},
	{0, 0, 0, {0, 0, 0, 0}}
	
	
	
	
	
	
};
void EnableInterrupts(void);
// edit the following only if you need to move pins from PA4, PE3-0      
// logic analyzer on the real board
#define PA4       (*((volatile unsigned long *)0x40004040))
#define PE50      (*((volatile unsigned long *)0x400240FC))
uint16_t input;
void SendDataToLogicAnalyzer(void){
  UART0_DR_R = 0x80|(PA4<<2)|PE50;
}

int main(void){ 
  TExaS_Init(&SendDataToLogicAnalyzer);    // activate logic analyzer and set system clock to 80 MHz
  SysTick_Init();   
  uint16_t S=0;	
// you initialize your system here
	
	SYSCTL_RCGCGPIO_R|=0x33;
	while(SYSCTL_RCGCGPIO_R&=0x33==0){}
	
	GPIO_PORTA_DEN_R|=0x30;
	GPIO_PORTA_DIR_R&=0xCF;
	
	GPIO_PORTB_DEN_R|=0x40;
	GPIO_PORTB_DIR_R|=0x40;
	
	GPIO_PORTE_DEN_R|=0x3F;
	GPIO_PORTE_DIR_R|=0x3F;
	
	GPIO_PORTF_DEN_R|=0x1;
	GPIO_PORTF_DIR_R|=0x1;

	
  EnableInterrupts();
  while(1){
		GPIO_PORTE_DATA_R = FSM[S].Output; //Output
		GPIO_PORTA_DATA_R = ((FSM[S].flash)<<4); //Output
		SysTick_Wait1ms(FSM[S].Time); //wait
		input = ((GPIO_PORTA_DATA_R&(0x20)>>5)+(GPIO_PORTA_DATA_R&(0x10)>>3))&(3); // input should be the two buttons
    S=FSM[S].next[input];// next	
  }
}


