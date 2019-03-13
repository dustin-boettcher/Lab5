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
	{1, 0, 1, {1, 1, 1, 1}},    //Initialize
	{1, 0, 50, {1, 19, 2, 1}}, //Home
	
//Wipe Only
	{2, 0, 50, {3, 3, 3, 3}},
	{4, 0, 50, {4, 4, 4, 4}},
	{8, 0, 50, {5, 5, 5, 5}},
	{16, 0, 50, {6, 6, 6, 6}},
	{1, 0, 50, {7, 7, 7, 7}},
	{2, 0, 50, {8, 8, 8, 8}},
	{4, 0, 50, {9, 9, 9, 9}},
	{8, 0, 50, {10, 10, 10, 10}},
	{16, 0, 50, {11, 11, 11, 11}},
	{8, 0, 50, {12, 12, 12, 12}},
	{4, 0, 50, {13, 13, 13, 13}},
	{2, 0, 50, {14, 14, 14, 14}},
	{1, 0, 50, {15, 15, 15, 15}},
	{16, 0, 50, {16, 16, 16, 16}},
	{8, 0, 50, {17, 17, 17, 17}},
	{4, 0, 50, {18, 18, 18, 18}},
	{2, 0, 50, {1, 1, 1, 1}},
	
//Wash and Wipe
	{2, 1, 50, {20, 20, 20, 20}},
	{4, 0, 50, {21, 21, 21, 21}},
	{8, 1, 50, {22, 22, 22, 22}},
	{16, 0, 50, {23, 23, 23, 23}},
	{1, 1, 50, {24, 24, 24, 24}},
	{2, 0, 50, {25, 25, 25, 25}},
	{4, 1, 50, {26, 26, 26, 26}},
	{8, 0, 50, {27, 27, 27, 27}},
	{16, 1, 50, {28, 28, 28, 28}},
	{8, 0, 50, {29, 29, 29, 29}},
	{4, 1, 50, {30, 30, 30, 30}},
	{2, 0, 50, {31, 31, 31, 31}},
	{1, 1, 50, {32, 32, 32, 32}},
	{16, 0, 50, {33, 33, 33, 33}},
	{8, 1, 50, {34, 34, 34, 34}},
	{4, 0, 50, {35, 35, 35, 35}},
	{2, 1, 50, {1, 1, 1, 1}},
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
	
	uint16_t S=0;
  TExaS_Init(&SendDataToLogicAnalyzer);    // activate logic analyzer and set system clock to 80 MHz
  SysTick_Init();   
  
		
	SYSCTL_RCGCGPIO_R|=0x33;
	while((SYSCTL_RCGCGPIO_R&0x33)!=0x33){
	}
	
	GPIO_PORTA_DEN_R|=0x30;
	GPIO_PORTA_DIR_R&=0xCF;
	
	GPIO_PORTB_DEN_R|=0x40;
	GPIO_PORTB_DIR_R|=0x40;
	
	GPIO_PORTE_DEN_R|=0x3F;
	GPIO_PORTE_DIR_R|=0x3F;
	
	//GPIO_PORTF_DEN_R|=0x1;
	//GPIO_PORTF_DIR_R|=0x1;
	

	
  EnableInterrupts();
	

  while(1){
		GPIO_PORTE_DATA_R = FSM[S].Output; //Output
		
		GPIO_PORTE_DATA_R &= 0xEF;
		GPIO_PORTE_DATA_R += (FSM[S].flash<<5);
		
		SysTick_Wait1ms(FSM[S].Time); //wait
		
		input = (GPIO_PORTA_DATA_R & 0x30); // input should be the two buttons
		input = input>>4;
		
    S=FSM[S].next[input];// next	
  }
}

