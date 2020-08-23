#include"Display.h"
#include"delay.h"

u8 Num_Dis[16]={0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,
					0x7f,0x6f,0x77,0x7c,0x39,0x5e,0x79,0x71};//��ʾ0~F��ֵ

void Iic_Data_Out(void)          //��DIO���ó����ģʽ
{
    GPIOB->MODER&=~(3<<(6*2));
	GPIOB->MODER|=1<<6*2;
}					
void Iic_Data_Input(void)        //��DIO���ó�����ģʽ
{
	GPIOB->MODER&=~(3<<(6*2));
	GPIOB->MODER|=0<<6*2;
}

void Iic_Init(void)
{	
GPIO_InitTypeDef GPIO_InitStatus;
RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE);
GPIO_InitStatus.GPIO_Pin=GPIO_Pin_6|GPIO_Pin_7;//��������ΪPB6,PB7.
GPIO_InitStatus.GPIO_Mode=GPIO_Mode_OUT;    //��������ģʽΪ���ģʽ
GPIO_InitStatus.GPIO_Speed=GPIO_Speed_50MHz;   //�������ŵ��ٶ�Ϊ���٣�50M
GPIO_Init(GPIOB,&GPIO_InitStatus);             //��ʼ��GPIO
IIC_CLK(1);                                    //һ��ʼ����ʱ���ߣ������Ǹ��궨�塣
IIC_DATA_OUT(1);          //����������ߵ�ƽ
}

void Iic_Start(void)
{
 Iic_Data_Out(); //��DIO���������ó����ģʽ��
 IIC_CLK(1);     //CLKʱ��������ߵ�ƽ
 IIC_DATA_OUT(1);//DIO����������ߵ�ƽ
 delay_us(2);    //��ʱ2us
    IIC_DATA_OUT(0);//DIO����������͵�ƽ
}

void Iic_Stop(void)
{
 Iic_Data_Out(); //��DIO���������ó����ģʽ��
 IIC_CLK(0);     //CLKʱ��������͵�ƽ
 delay_us(2);    //��ʱ2us
 IIC_DATA_OUT(0);//DIO����������͵�ƽ
 delay_us(2);    //��ʱ2us
 IIC_CLK(1);     //CLKʱ��������ߵ�ƽ
 delay_us(2);    //��ʱ2us
 IIC_DATA_OUT(1);//DIO����������ߵ�ƽ
}

void Iic_Ack(void)
{
 Iic_Data_Input();   //��DIO���������ó�����ģʽ��������ȡ����
 IIC_CLK(0);         //CLKʱ��������͵�ƽ
 delay_us(5);        //��ʱ5us
 while(IIC_DATA_INT);//�ȴ�DIO�����߱����ͣ����û�����ͻῨ������ȴ�Ӧ��
 IIC_CLK(1);         //CLKʱ��������ߵ�ƽ
 delay_us(2);        //��ʱ2us
 IIC_CLK(0);         //CLKʱ��������͵�ƽ
}

void Iic_Write_Byte(u8 byte)  //��������Ҫд������ܵ�����
{
 u8 Num;                   //����һ������ѭ��8�ν������ݵı�����������ѭ��        
 Iic_Data_Out();           //��DIO������       
 for(Num=0;Num<8;Num++)    //ѭ��8��
 { 
  IIC_CLK(0);           //ÿ��д��ʱ��CLK����͵�ƽ
  if(byte&0x01)         //��λ��ǰ   �ж����ݵĵ�ǰλΪ1����0
  {
       IIC_DATA_OUT(1); //DIO����������ߵ�ƽ   ��ǰ����λΪ1
  }
  else
  {
       IIC_DATA_OUT(0); //DIO����������͵�ƽ   ��ǰ����λΪ0
  }
  delay_us(3);          //��ʱ3us
  byte=byte>>1;         //ÿ�η�����һ��λ�������ݶ�����һλ
  IIC_CLK(1);           //ÿ��д��ʱ��CLK����ߵ�ƽ
  delay_us(3);          //��ʱ3us
 }
}

void Display_Num(u16 num)
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
 number[3] =12;
 Iic_Start();
 Iic_Write_Byte(0x40);                //������д����ģʽ�� 0x40��
 Iic_Ack();
 Iic_Stop();
 Iic_Start();
 Iic_Write_Byte(0xc0);                //�ӵ�һλ�������ʾ������0xc0��
 Iic_Ack();
 Iic_Write_Byte(Num_Dis[number[3]]);  //����λ�������ʾ������
 Iic_Ack();
 Iic_Write_Byte(Num_Dis[number[2]]);  //����λ�������ʾ������
 Iic_Ack();
 Iic_Write_Byte(Num_Dis[number[1]]);  //�ڶ�λ�������ʾ������
 Iic_Ack();
 Iic_Write_Byte(Num_Dis[number[0]]);  //��һλ�������ʾ������
 Iic_Ack();
 Iic_Stop();
 Iic_Start();
 Iic_Write_Byte(0x8f);                //��ʼ��ʾ
 Iic_Ack();
 Iic_Stop();
 //memset(number,0,sizeof(number));     //��ʾ��֮����������е�����
}
