#ifndef _DISPLAYH_H
#define _DISPLAYH_H

#include"stm32f4xx.h" 

#define IIC_CLK_H(x) (x?GPIO_WriteBit(GPIOB,GPIO_Pin_5,Bit_SET):GPIO_WriteBit(GPIOB,GPIO_Pin_5,Bit_RESET))
#define IIC_DATA_OUT_H(x) (x?GPIO_WriteBit(GPIOB,GPIO_Pin_4,Bit_SET):GPIO_WriteBit(GPIOB,GPIO_Pin_4,Bit_RESET))
#define IIC_DATA_INT_H ((GPIOB->IDR & (1<<4)))

void Iic_Init_H(void);                       //初始化
void Iic_Data_Out_H(void);            //把DIO配置成输出模式
void Iic_Data_Input_H(void);          //把DIO配置成输入模式
void Iic_Start_H(void);                    //起始信号
void Iic_Stop_H(void);                    //结束信号
void Iic_Ack_H(void);                     //应答信号
void Iic_Write_Byte_H(u8 byte);    //写数据
void Display_Num_H(u16 num); //显示函数，调用这个函数就行了，前提是在main中调用了初始化函数

#endif
