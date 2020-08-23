#include"DisplayH.h"
#include"delay.h"

u8 num_Dis[16]={0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,
					0x7f,0x6f,0x77,0x7c,0x39,0x5e,0x79,0x71};//显示0~F的值

void Iic_Data_Out_H(void)          //把DIO配置成输出模式
{
    GPIOB->MODER&=~(3<<(4*2));
	GPIOB->MODER|=1<<4*2;
}					
void Iic_Data_Input_H(void)        //把DIO配置成输入模式
{
	GPIOB->MODER&=~(3<<(4*2));
	GPIOB->MODER|=0<<4*2;
}

void Iic_Init_H(void)
{	
GPIO_InitTypeDef GPIO_InitStatus;
RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE);
GPIO_InitStatus.GPIO_Pin=GPIO_Pin_4|GPIO_Pin_5;//定义引脚为PB6,PB7.
GPIO_InitStatus.GPIO_Mode=GPIO_Mode_OUT;    //配置引脚模式为输出模式
GPIO_InitStatus.GPIO_Speed=GPIO_Speed_50MHz;   //配置引脚的速度为高速：50M
GPIO_Init(GPIOB,&GPIO_InitStatus);             //初始化GPIO
IIC_CLK_H(1);                                    //一开始拉高时钟线，这算是个宏定义。
IIC_DATA_OUT_H(1);          //数据线输出高电平
}

void Iic_Start_H(void)
{
 Iic_Data_Out_H(); //把DIO数据线配置成输出模式，
 IIC_CLK_H(1);     //CLK时钟线输出高电平
 IIC_DATA_OUT_H(1);//DIO数据线输出高电平
 delay_us(2);    //延时2us
    IIC_DATA_OUT_H(0);//DIO数据线输出低电平
}

void Iic_Stop_H(void)
{
 Iic_Data_Out_H(); //把DIO数据线配置成输出模式，
 IIC_CLK_H(0);     //CLK时钟线输出低电平
 delay_us(2);    //延时2us
 IIC_DATA_OUT_H(0);//DIO数据线输出低电平
 delay_us(2);    //延时2us
 IIC_CLK_H(1);     //CLK时钟线输出高电平
 delay_us(2);    //延时2us
 IIC_DATA_OUT_H(1);//DIO数据线输出高电平
}

void Iic_Ack_H(void)
{
 Iic_Data_Input_H();   //把DIO数据线配置成输入模式，用来读取数据
 IIC_CLK_H(0);         //CLK时钟线输出低电平
 delay_us(5);        //延时5us
 while(IIC_DATA_INT_H);//等待DIO数据线被拉低，如果没有拉低会卡在这里等待应答
 IIC_CLK_H(1);         //CLK时钟线输出高电平
 delay_us(2);        //延时2us
 IIC_CLK_H(0);         //CLK时钟线输出低电平
}

void Iic_Write_Byte_H(u8 byte)  //参数：需要写进数码管的数据
{
 u8 Num;                   //定义一个用来循环8次接受数据的变量用来计数循环        
 Iic_Data_Out_H();           //把DIO数据线       
 for(Num=0;Num<8;Num++)    //循环8次
 { 
  IIC_CLK_H(0);           //每次写的时候CLK输出低电平
  if(byte&0x01)         //低位在前   判断数据的当前位为1还是0
  {
       IIC_DATA_OUT_H(1); //DIO数据线输出高电平   当前数据位为1
  }
  else
  {
       IIC_DATA_OUT_H(0); //DIO数据线输出低电平   当前数据位为0
  }
  delay_us(3);          //延时3us
  byte=byte>>1;         //每次发送完一次位数后，数据都左移一位
  IIC_CLK_H(1);           //每次写完时候CLK输出高电平
  delay_us(3);          //延时3us
 }
}

void Display_Num_H(u16 num)
{
 u16 number[4] = {0};
 if(num < 10)                 //判断参数是几位数据，然后根据位数分解数字
 {
  number[0] = num;
 }
 else if(num >=10 && num <100)
 {
  number[0] = num%10;
  number[1] = num/10;
 }
 else if(num >=100 && num < 1000)
 {
  number[0] = num%10;
  number[1] = (num/10)%10;
  number[2] = num/100;
 }
    else if(num >= 1000)          //1234
 {
  number[0] = num%10;       //1234 % 10 = 4 
  number[1] = (num/10)%10;  //(1234 / 10) = 123 ; 123 % 10 = 3
  number[2] = (num/100)%10; //(1234 / 100) = 12 ; 12 % 10 = 2
        number[3] = num/1000;     //(1234 / 1000) = 1
 }
 Iic_Start_H();
 Iic_Write_Byte_H(0x40);                //启动该写数据模式。 0x40；
 Iic_Ack_H();
 Iic_Stop_H();
 Iic_Start_H();
 Iic_Write_Byte_H(0xc0);                //从第一位数码管显示，发送0xc0；
 Iic_Ack_H();
 Iic_Write_Byte_H(0x25);  //第四位数码管显示的数据
 Iic_Ack_H();
 Iic_Write_Byte_H(num_Dis[number[2]]);  //第三位数码管显示的数据
 Iic_Ack_H();
 Iic_Write_Byte_H(num_Dis[number[1]]);  //第二位数码管显示的数据
 Iic_Ack_H();
 Iic_Write_Byte_H(num_Dis[number[0]]);  //第一位数码管显示的数据
 Iic_Ack_H();
 Iic_Stop_H();
 Iic_Start_H();
 Iic_Write_Byte_H(0x8f);                //开始显示
 Iic_Ack_H();
 Iic_Stop_H();
 //memset(number,0,sizeof(number));     //显示完之后清除数组中的内容
}
