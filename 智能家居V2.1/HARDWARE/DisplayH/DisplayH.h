#ifndef _DISPLAYH_H
#define _DISPLAYH_H

#include"stm32f4xx.h" 

#define IIC_CLK_H(x) (x?GPIO_WriteBit(GPIOB,GPIO_Pin_5,Bit_SET):GPIO_WriteBit(GPIOB,GPIO_Pin_5,Bit_RESET))
#define IIC_DATA_OUT_H(x) (x?GPIO_WriteBit(GPIOB,GPIO_Pin_4,Bit_SET):GPIO_WriteBit(GPIOB,GPIO_Pin_4,Bit_RESET))
#define IIC_DATA_INT_H ((GPIOB->IDR & (1<<4)))

void Iic_Init_H(void);                       //��ʼ��
void Iic_Data_Out_H(void);            //��DIO���ó����ģʽ
void Iic_Data_Input_H(void);          //��DIO���ó�����ģʽ
void Iic_Start_H(void);                    //��ʼ�ź�
void Iic_Stop_H(void);                    //�����ź�
void Iic_Ack_H(void);                     //Ӧ���ź�
void Iic_Write_Byte_H(u8 byte);    //д����
void Display_Num_H(u16 num); //��ʾ����������������������ˣ�ǰ������main�е����˳�ʼ������

#endif
