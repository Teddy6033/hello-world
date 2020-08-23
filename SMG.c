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
				 if(dta&0x80)//ȡ���λ
								SET_SER;//д1
				 else
								CLR_SER;//д0
				 dta<<=1;//���θ�λ��Ϊ���λ
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

