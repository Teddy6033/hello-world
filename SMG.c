#include "SMG.h"
#include "stm32f10x.h"                  // Device header

void HC595_Init()
{
		
	GPIO_InitTypeDef IO;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);
	IO.GPIO_Mode = GPIO_Mode_Out_PP;
	IO.GPIO_Speed = GPIO_Speed_50MHz;
	IO.GPIO_Pin = SER|SRCK|RCK;
	GPIO_Init(GPIOC,&IO);

}

void HC595_WR(unsigned char dta)
{
		u8 i;
	  for(i = 0;i<8;i++)
		{
		     CLR_SRCK;
				 if(dta&0x80)//取最高位
								SET_SER;//写1
				 else
								CLR_SER;//写0
				 dta<<=1;//将次高位置为最高位
				 SET_SRCK;//时钟上升沿
		
		}
}

void HC595_Over()//控制RCLK时钟上升沿
{

		CLR_RCK;
		SET_RCK;
}


void SMG_Display(unsigned char data,unsigned char wel)
{

	HC595_WR(data);//数字
	HC595_WR(wel);//位选
	HC595_Over();//输出数据
}

