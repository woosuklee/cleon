/**
@file       hal_gpio.h

@brief      GPIO configuration
*/

#ifndef __HAL_GPIO_H__
#define __HAL_GPIO_H__

#define LED1_ON()          LED1_PORT(OUT) &= ~BV(LED1_PIN)
#define LED1_OFF()         LED1_PORT(OUT) |=  BV(LED1_PIN)
#define LED1_TOGGLE()      LED1_PORT(OUT) ^=  BV(LED1_PIN)

#define LED2_ON()          LED2_PORT(OUT) &= ~BV(LED2_PIN)
#define LED2_OFF()         LED2_PORT(OUT) |=  BV(LED2_PIN)
#define LED2_TOGGLE()      LED2_PORT(OUT) ^=  BV(LED2_PIN)

#define LED3_ON()          LED3_PORT(OUT) &= ~BV(LED3_PIN)
#define LED3_OFF()         LED3_PORT(OUT) |=  BV(LED3_PIN)
#define LED3_TOGGLE()      LED3_PORT(OUT) ^=  BV(LED3_PIN)

#define LED4_ON()          LED4_PORT(OUT) &= ~BV(LED4_PIN)
#define LED4_OFF()         LED4_PORT(OUT) |=  BV(LED4_PIN)
#define LED4_TOGGLE()      LED4_PORT(OUT) ^=  BV(LED4_PIN)

#define LED5_ON()          LED5_PORT(OUT) &= ~BV(LED5_PIN)
#define LED5_OFF()         LED5_PORT(OUT) |=  BV(LED5_PIN)
#define LED5_TOGGLE()      LED5_PORT(OUT) ^=  BV(LED5_PIN)

#define LED6_ON()          LED6_PORT(OUT) &= ~BV(LED6_PIN)
#define LED6_OFF()         LED6_PORT(OUT) |=  BV(LED6_PIN)
#define LED6_TOGGLE()      LED6_PORT(OUT) ^=  BV(LED6_PIN)

void HAL_GPIO_Init(void);

#endif
