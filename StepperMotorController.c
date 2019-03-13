// StepperMotorController.c starter file EE319K Lab 5
// Runs on TM4C123
// Finite state machine to operate a stepper motor.  
// Dustin Boettcher, Chris Clark
// March 13, 2019

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
	#define home 1
	#define wipe 2
	#define wash 19
	
STyp FSM[40] = {
	{1, 0, 1, {home, home, home, home}},    //Initialize
	{1, 0, 50, {home, wash, wipe, wash}}, //Home
	
//Wipe
	{2, 0, 50, {3, 3, 3, 3}}, //wipe
	{4, 0, 50, {4, 4, 4, 4}}, //3
	{8, 0, 50, {5, 5, 5, 5}}, //4
	{16, 0, 50, {6, 6, 6, 6}}, //5
	{1, 0, 50, {7, 7, 7, 7}}, //6
	{2, 0, 50, {8, 8, 8, 8}}, //7
	{4, 0, 50, {9, 9, 9, 9}}, //8
	{8, 0, 50, {10, 10, 10, 10}}, //9
	{16, 0, 50, {36, 36, 36, 36}}, //10
	{8, 0, 50, {12, 12, 12, 12}}, //11
	{4, 0, 50, {13, 13, 13, 13}}, //12
	{2, 0, 50, {14, 14, 14, 14}}, //13
	{1, 0, 50, {15, 15, 15, 15}}, //14
	{16, 0, 50, {16, 16, 16, 16}}, //15
	{8, 0, 50, {17, 17, 17, 17}}, //16
	{4, 0, 50, {18, 18, 18, 18}}, //17
	{2, 0, 50, {home, home, home, home}}, //18
	
//Wash
	{2, 1, 50, {20, 20, 20, 20}}, //Wash
	{4, 0, 50, {21, 21, 21, 21}}, //20
	{8, 1, 50, {22, 22, 22, 22}}, //21
	{16, 0, 50, {23, 23, 23, 23}}, //22
	{1, 1, 50, {24, 24, 24, 24}}, //23
	{2, 0, 50, {25, 25, 25, 25}}, //24
	{4, 1, 50, {26, 26, 26, 26}}, //25
	{8, 0, 50, {27, 27, 27, 27}}, //26
	{16, 1, 50, {38, 38, 38, 38}}, //27
	{8, 0, 50, {29, 29, 29, 29}}, //28
	{4, 1, 50, {30, 30, 30, 30}}, //29
	{2, 0, 50, {31, 31, 31, 31}}, //30
	{1, 1, 50, {32, 32, 32, 32}}, //31
	{16, 0, 50, {33, 33, 33, 33}}, //32
	{8, 1, 50, {34, 34, 34, 34}}, //33
	{4, 0, 50, {35, 35, 35, 35}}, //34
	{2, 1, 50, {home, home, home, home}}, //35
	
	{1, 0, 50, {37, 37, 37, 37}}, //36
	{16, 0, 50, {11, 11, 11, 11}}, //37
	
	{1, 0, 50, {39, 39, 39, 39}}, //38
	{16, 1, 50, {28, 28, 28, 28}} //39
	
};
void EnableInterrupts(void);
// edit the following only if you need to move pins from PA4, PE3-0      
// logic analyzer on the real board
#define PA4       (*((volatile unsigned long *)0x40004040))
#define PE50      (*((volatile unsigned long *)0x400240FC))
void SendDataToLogicAnalyzer(void){
  UART0_DR_R = 0x80|(PA4<<2)|PE50;
}

int main(void){ 
	
	uint16_t State=0;
	uint16_t input;
  TExaS_Init(&SendDataToLogicAnalyzer);    // activate logic analyzer and set system clock to 80 MHz
  SysTick_Init();   
  
		
	SYSCTL_RCGCGPIO_R|=0x33;
	while((SYSCTL_RCGCGPIO_R&0x33)!=0x33){}
	
	GPIO_PORTA_DEN_R|=0x30;
	GPIO_PORTA_DIR_R&=0xCF;
	
	GPIO_PORTB_DEN_R|=0x40;
	GPIO_PORTB_DIR_R|=0x40;
	
	GPIO_PORTE_DEN_R|=0x3F;
	GPIO_PORTE_DIR_R|=0x3F;
	
  EnableInterrupts();
	

  while(1){
		GPIO_PORTE_DATA_R = (GPIO_PORTE_DATA_R & 0xC0) + FSM[State].Output + (FSM[State].flash<<5); //adds output
		SysTick_Wait1ms(FSM[State].Time); //wait
		input = ((GPIO_PORTA_DATA_R & 0x30)>>4); // input is the two buttons PA4-5
		State = FSM[State].next[input];// next	
  }
}
