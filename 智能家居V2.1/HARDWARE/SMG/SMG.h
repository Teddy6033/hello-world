#ifndef _SMG_H_
#define _SMG_H_

#include "stm32f4xx.h"



#define SER   GPIO_Pin_7
#define SRCK  GPIO_Pin_8
#define RCK   GPIO_Pin_9


#define SET_SER  GPIO_SetBits(GPIOC,SER)//÷√1
#define CLR_SER  GPIO_ResetBits(GPIOC,SER)//«Â0

#define SET_SRCK  GPIO_SetBits(GPIOC,SRCK)
#define CLR_SRCK  GPIO_ResetBits(GPIOC,SRCK)

#define SET_RCK  GPIO_SetBits(GPIOC,RCK)
#define CLR_RCK  GPIO_ResetBits(GPIOC,RCK)


#define switch_Wel(x)   (1<<(8-x))

void HC595_Init(void);
void HC595_WR(unsigned char dta);
void HC595_Over(void);
void SMG_Display(unsigned char data,unsigned char wel);
void key1(void);
void key2(void);
void Display_Num(u8 num);


#endif




