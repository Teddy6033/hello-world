#include "SMG.h"
#include "stm32f4xx.h"                  // Device header
#include "delay.h"

const unsigned char smg[24]={
//01234567
0xC0,0xF9,0xA4,0xB0,0x99,0x92,0x82,0xF8,
//89ABCDEF
0x80,0x90,0x88,0x83,0xC6,0xA1,0x86,0x8E,
//时钟效果
0xF9,0xBF,0x99,0xBF,0x82,0xF9,0xC0,0xA4
};

//共阳数码管段码“0123456789AbCdEF熄灭-”
const unsigned char wei[8]={0x01,0x02,0x04,0x08,0x10,0x20,0x40,0x80};
//第一位至第八位位码

void HC595_Init()
{
		
	GPIO_InitTypeDef IO;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC,ENABLE);
	IO.GPIO_Mode = GPIO_Mode_OUT;
	IO.GPIO_Speed = GPIO_Speed_50MHz;
	IO.GPIO_Pin = SER|SRCK|RCK;
	IO.GPIO_OType=GPIO_OType_PP;
	GPIO_Init(GPIOC,&IO);

}

void HC595_WR(unsigned char dta)
{
		u8 i;
	    for(i = 0;i<8;i++)
		{
				 if(dta&0x80)//取最高位
								SET_SER;//写1
				 else
								CLR_SER;//写0
				 dta<<=1;//将次高位置为最高位
				 CLR_SRCK;
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

void Display_Num(u8 num)
{
  u8 number[2] = {0};
  number[0] = num%10;
  number[1] = num/10;
 
	SMG_Display(smg[number[0]],0x04);
    SMG_Display(smg[number[1]],0x08); 
	
 //memset(number,0,sizeof(number));     //显示完之后清除数组中的内容
}
