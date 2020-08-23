#ifndef _DISPLAY_H
#define _DISPLAY_H

#include"stm32f4xx.h" 

#define IIC_CLK(x) (x?GPIO_WriteBit(GPIOB,GPIO_Pin_7,Bit_SET):GPIO_WriteBit(GPIOB,GPIO_Pin_7,Bit_RESET))
#define IIC_DATA_OUT(x) (x?GPIO_WriteBit(GPIOB,GPIO_Pin_6,Bit_SET):GPIO_WriteBit(GPIOB,GPIO_Pin_6,Bit_RESET))
#define IIC_DATA_INT ((GPIOB->IDR & (1<<6)))

void Iic_Init(void);                       //��ʼ��
void Iic_Data_Out(void);            //��DIO���ó����ģʽ
void Iic_Data_Input(void);          //��DIO���ó�����ģʽ
void Iic_Start(void);                    //��ʼ�ź�
void Iic_Stop(void);                    //�����ź�
void Iic_Ack(void);                     //Ӧ���ź�
void Iic_Write_Byte(u8 byte);    //д����
void Display_Num(u16 num); //��ʾ����������������������ˣ�ǰ������main�е����˳�ʼ������

#endif
