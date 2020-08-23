#ifndef _DISPLAY_H
#define _DISPLAY_H

#include"stm32f4xx.h" 

#define IIC_CLK(x) (x?GPIO_WriteBit(GPIOB,GPIO_Pin_7,Bit_SET):GPIO_WriteBit(GPIOB,GPIO_Pin_7,Bit_RESET))
#define IIC_DATA_OUT(x) (x?GPIO_WriteBit(GPIOB,GPIO_Pin_6,Bit_SET):GPIO_WriteBit(GPIOB,GPIO_Pin_6,Bit_RESET))
#define IIC_DATA_INT ((GPIOB->IDR & (1<<6)))

void Iic_Init(void);                       //初始化
void Iic_Data_Out(void);            //把DIO配置成输出模式
void Iic_Data_Input(void);          //把DIO配置成输入模式
void Iic_Start(void);                    //起始信号
void Iic_Stop(void);                    //结束信号
void Iic_Ack(void);                     //应答信号
void Iic_Write_Byte(u8 byte);    //写数据
void Display_Num(u16 num); //显示函数，调用这个函数就行了，前提是在main中调用了初始化函数

#endif
