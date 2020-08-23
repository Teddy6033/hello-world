#include"stm32f4xx.h"
#include"led.h"
#include"delay.h"
#include"dht11.h"
#include"smg.h"


int main()
{
	int t=0;
	u8 temp=0;
	u8 humi=0;
	delay_init(168);
	LED_Init();
	HC595_Init();
	
	while(1)
	{   t++;
		if(t%150==0)
		{
		  DHT11_Read_Data(&temp,&humi);
		}
		Display_Num(temp);	
	}
}
