#include"DisplayH.h"
#include"delay.h"

u8 num_Dis[16]={0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,
					0x7f,0x6f,0x77,0x7c,0x39,0x5e,0x79,0x71};//��ʾ0~F��ֵ

void Iic_Data_Out_H(void)          //��DIO���ó����ģʽ
{
    GPIOB->MODER&=~(3<<(4*2));
	GPIOB->MODER|=1<<4*2;
}					
void Iic_Data_Input_H(void)        //��DIO���ó�����ģʽ
{
	GPIOB->MODER&=~(3<<(4*2));
	GPIOB->MODER|=0<<4*2;
}

void Iic_Init_H(void)
{	
GPIO_InitTypeDef GPIO_InitStatus;
RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE);
GPIO_InitStatus.GPIO_Pin=GPIO_Pin_4|GPIO_Pin_5;//��������ΪPB6,PB7.
GPIO_InitStatus.GPIO_Mode=GPIO_Mode_OUT;    //��������ģʽΪ���ģʽ
GPIO_InitStatus.GPIO_Speed=GPIO_Speed_50MHz;   //�������ŵ��ٶ�Ϊ���٣�50M
GPIO_Init(GPIOB,&GPIO_InitStatus);             //��ʼ��GPIO
IIC_CLK_H(1);                                    //һ��ʼ����ʱ���ߣ������Ǹ��궨�塣
IIC_DATA_OUT_H(1);          //����������ߵ�ƽ
}

void Iic_Start_H(void)
{
 Iic_Data_Out_H(); //��DIO���������ó����ģʽ��
 IIC_CLK_H(1);     //CLKʱ��������ߵ�ƽ
 IIC_DATA_OUT_H(1);//DIO����������ߵ�ƽ
 delay_us(2);    //��ʱ2us
    IIC_DATA_OUT_H(0);//DIO����������͵�ƽ
}

void Iic_Stop_H(void)
{
 Iic_Data_Out_H(); //��DIO���������ó����ģʽ��
 IIC_CLK_H(0);     //CLKʱ��������͵�ƽ
 delay_us(2);    //��ʱ2us
 IIC_DATA_OUT_H(0);//DIO����������͵�ƽ
 delay_us(2);    //��ʱ2us
 IIC_CLK_H(1);     //CLKʱ��������ߵ�ƽ
 delay_us(2);    //��ʱ2us
 IIC_DATA_OUT_H(1);//DIO����������ߵ�ƽ
}

void Iic_Ack_H(void)
{
 Iic_Data_Input_H();   //��DIO���������ó�����ģʽ��������ȡ����
 IIC_CLK_H(0);         //CLKʱ��������͵�ƽ
 delay_us(5);        //��ʱ5us
 while(IIC_DATA_INT_H);//�ȴ�DIO�����߱����ͣ����û�����ͻῨ������ȴ�Ӧ��
 IIC_CLK_H(1);         //CLKʱ��������ߵ�ƽ
 delay_us(2);        //��ʱ2us
 IIC_CLK_H(0);         //CLKʱ��������͵�ƽ
}

void Iic_Write_Byte_H(u8 byte)  //��������Ҫд������ܵ�����
{
 u8 Num;                   //����һ������ѭ��8�ν������ݵı�����������ѭ��        
 Iic_Data_Out_H();           //��DIO������       
 for(Num=0;Num<8;Num++)    //ѭ��8��
 { 
  IIC_CLK_H(0);           //ÿ��д��ʱ��CLK����͵�ƽ
  if(byte&0x01)         //��λ��ǰ   �ж����ݵĵ�ǰλΪ1����0
  {
       IIC_DATA_OUT_H(1); //DIO����������ߵ�ƽ   ��ǰ����λΪ1
  }
  else
  {
       IIC_DATA_OUT_H(0); //DIO����������͵�ƽ   ��ǰ����λΪ0
  }
  delay_us(3);          //��ʱ3us
  byte=byte>>1;         //ÿ�η�����һ��λ�������ݶ�����һλ
  IIC_CLK_H(1);           //ÿ��д��ʱ��CLK����ߵ�ƽ
  delay_us(3);          //��ʱ3us
 }
}

void Display_Num_H(u16 num)
{
 u16 number[4] = {0};
 if(num < 10)                 //�жϲ����Ǽ�λ���ݣ�Ȼ�����λ���ֽ�����
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
 Iic_Write_Byte_H(0x40);                //������д����ģʽ�� 0x40��
 Iic_Ack_H();
 Iic_Stop_H();
 Iic_Start_H();
 Iic_Write_Byte_H(0xc0);                //�ӵ�һλ�������ʾ������0xc0��
 Iic_Ack_H();
 Iic_Write_Byte_H(0x25);  //����λ�������ʾ������
 Iic_Ack_H();
 Iic_Write_Byte_H(num_Dis[number[2]]);  //����λ�������ʾ������
 Iic_Ack_H();
 Iic_Write_Byte_H(num_Dis[number[1]]);  //�ڶ�λ�������ʾ������
 Iic_Ack_H();
 Iic_Write_Byte_H(num_Dis[number[0]]);  //��һλ�������ʾ������
 Iic_Ack_H();
 Iic_Stop_H();
 Iic_Start_H();
 Iic_Write_Byte_H(0x8f);                //��ʼ��ʾ
 Iic_Ack_H();
 Iic_Stop_H();
 //memset(number,0,sizeof(number));     //��ʾ��֮����������е�����
}
