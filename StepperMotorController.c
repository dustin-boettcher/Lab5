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
STyp FSM[40] = {
	{0, 0, 50, {0, 0, 1, 0}}, //initial
	{1, 0, 50, {2, 0, 2, 0}}, //steps through all 20 states when windshield wiper pressed
	{2, 0, 50, {3, 0, 3, 0}},
	{4, 0, 50, {4, 0, 4, 0}},
	{8, 0, 50, {5, 0, 5, 0}},
	{16, 0, 50, {6, 0, 6, 0}},
	{1, 0, 50, {7, 0, 7, 0}},
	{2, 0, 50, {8, 0, 8, 0}},
	{4, 0, 50, {9, 0, 9, 0}},
	{8, 0, 50, {10, 0, 10, 0}},
	{16, 0, 50, {11, 0, 11, 0}},
	{8, 0, 50, {12, 0, 12, 0}},
	{4, 0, 50, {13, 0, 13, 0}},
	{2, 0, 50, {14, 0, 14, 0}},
	{1, 0, 50, {15, 0, 15, 0}},
	{16, 0, 50, {16, 0, 16, 0}},
	{8, 0, 50, {17, 0, 17, 0}},
	{4, 0, 50, {18, 0, 18, 0}},
	{2, 0, 50, {19, 0, 19, 0}},
	{1, 0, 50, {0, 0, 0, 0}}, //19
	{0, 0, 0, {0, 0, 0, 0}},
	{0, 0, 0, {0, 0, 0, 0}},
	{0, 0, 0, {0, 0, 0, 0}},
	{0, 0, 0, {0, 0, 0, 0}},
	{0, 0, 0, {0, 0, 0, 0}},
	{0, 0, 0, {0, 0, 0, 0}},
	{0, 0, 0, {0, 0, 0, 0}},
	{0, 0, 0, {0, 0, 0, 0}},
	{0, 0, 0, {0, 0, 0, 0}},
	{0, 0, 0, {0, 0, 0, 0}},
	{0, 0, 0, {0, 0, 0, 0}},
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
// you initialize your system here
	
	
	
  EnableInterrupts();
  uint16_t S=0;	
  while(1){
     PE50 = FSM[S].Output; //Output
     PA4 = FSM[S].flash; //Output
     SysTick_Wait1ms(FSM[S].Time); //wait
     input = (GPIO_PORTA_DATA_R&(0x20)>>5)+(GPIO_PORTA_DATA_R&(0x10)>>3); // input should be the two buttons 
     S=FSM[S].next[input];// next		
  }
}


