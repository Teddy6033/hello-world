#ifndef _LED_H
#define _LED_H

void LED_Init(void );

#define LED_ON  GPIO_ResetBits(GPIOF,GPIO_Pin_10);
#define LED_OFF GPIO_SetBits(GPIOF,GPIO_Pin_10);
#define LED PFout(9)

#endif
