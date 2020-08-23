#include "SMG.h"
#include "stm32f4xx.h"                  // Device header
#include "delay.h"

const unsigned char smg[24]={
//01234567
0xC0,0xF9,0xA4,0xB0,0x99,0x92,0x82,0xF8,
//89ABCDEF
0x80,0x90,0x88,0x83,0xC6,0xA1,0x86,0x8E,
//ʱ��Ч��
0xF9,0xBF,0x99,0xBF,0x82,0xF9,0xC0,0xA4
};

//��������ܶ��롰0123456789AbCdEFϨ��-��
const unsigned char wei[8]={0x01,0x02,0x04,0x08,0x10,0x20,0x40,0x80};
//��һλ���ڰ�λλ��

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
				 if(dta&0x80)//ȡ���λ
								SET_SER;//д1
				 else
								CLR_SER;//д0
				 dta<<=1;//���θ�λ��Ϊ���λ
				 CLR_SRCK;
				 SET_SRCK;//ʱ��������
		
		}
}

void HC595_Over()//����RCLKʱ��������
{

		CLR_RCK;
		SET_RCK;
}


void SMG_Display(unsigned char data,unsigned char wel)
{

	HC595_WR(data);//����
	HC595_WR(wel);//λѡ
	HC595_Over();//�������
}

void Display_Num(u8 num)
{
  u8 number[2] = {0};
  number[0] = num%10;
  number[1] = num/10;
 
	SMG_Display(smg[number[0]],0x04);
    SMG_Display(smg[number[1]],0x08); 
	
 //memset(number,0,sizeof(number));     //��ʾ��֮����������е�����
}
