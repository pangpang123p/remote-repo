/*
 * Periph_init.h
 *
 *  Created on: Jun 11, 2025
 *      Author: 003
 */

#ifndef USER_PERIPH_INIT_H_
#define USER_PERIPH_INIT_H_
#include "debug.h"

extern void SysTick_init(void);
extern void SysTick_delay_ms(u16 ms);




#define tx_en(on_off)                   on_off ? GPIO_WriteBit(GPIOD, GPIO_Pin_7, Bit_SET):GPIO_WriteBit(GPIOD, GPIO_Pin_7, Bit_RESET)
#define READ_ID                         ((u8)(((GPIO_ReadInputData(GPIOB))>>2) & 0x000f))
#define READ_KEY                        GPIO_ReadInputDataBit(GPIOC,2)

//DO5678
#define AC_PWR1(on_off)                     on_off ? GPIO_WriteBit(GPIOA, GPIO_Pin_3, Bit_SET):GPIO_WriteBit(GPIOA, GPIO_Pin_3, Bit_RESET)
#define AC_PWR2(on_off)                     on_off ? GPIO_WriteBit(GPIOB, GPIO_Pin_0, Bit_SET):GPIO_WriteBit(GPIOB, GPIO_Pin_0, Bit_RESET)
#define AC_PWR3(on_off)                     on_off ? GPIO_WriteBit(GPIOB, GPIO_Pin_1, Bit_SET):GPIO_WriteBit(GPIOB, GPIO_Pin_1, Bit_RESET)
#define AC_PWR4(on_off)                     on_off ? GPIO_WriteBit(GPIOC, GPIO_Pin_0, Bit_SET):GPIO_WriteBit(GPIOC, GPIO_Pin_0, Bit_RESET)

//板载指示灯
#define Board_LED_G(on_off)              on_off ? GPIO_WriteBit(GPIOA, GPIO_Pin_0, Bit_SET):GPIO_WriteBit(GPIOA, GPIO_Pin_0, Bit_RESET)
#define Board_LED_G_Blink                GPIO_WriteBit(GPIOA, GPIO_Pin_0,~GPIO_ReadOutputDataBit(GPIOA, GPIO_Pin_0) & 0x1)

#define Board_LED_B(on_off)              on_off ? GPIO_WriteBit(GPIOA, GPIO_Pin_1, Bit_SET):GPIO_WriteBit(GPIOA, GPIO_Pin_1, Bit_RESET)
#define Board_LED_B_Blink                GPIO_WriteBit(GPIOA, GPIO_Pin_1,~GPIO_ReadOutputDataBit(GPIOA, GPIO_Pin_1) & 0x1)

#define Board_LED_Y(on_off)              on_off ? GPIO_WriteBit(GPIOD, GPIO_Pin_0, Bit_SET):GPIO_WriteBit(GPIOD, GPIO_Pin_0, Bit_RESET)
#define Board_LED_Y_Blink                GPIO_WriteBit(GPIOD, GPIO_Pin_0,~GPIO_ReadOutputDataBit(GPIOD, GPIO_Pin_0) & 0x1)

//外部指示灯
#define LED_Y(on_off)              on_off ? GPIO_WriteBit(GPIOB, GPIO_Pin_6, Bit_SET):GPIO_WriteBit(GPIOB, GPIO_Pin_6, Bit_RESET)
#define LED_Y_Blink                GPIO_WriteBit(GPIOB, GPIO_Pin_6,~GPIO_ReadOutputDataBit(GPIOB, GPIO_Pin_6) & 0x1)

void GPIO_INIT_1(void);
void GPIO_INIT_2(void);
void DMA_ReceiveData(void);
u8 crc8(const u8 *data, u32 length);
void USARTx_CFG(void);

#endif /* USER_PERIPH_INIT_H_ */
