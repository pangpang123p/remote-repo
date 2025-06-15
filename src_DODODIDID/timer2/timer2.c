#include "timer2.h"

extern u8 flag;


void TIM2_Int_Init(u16 period, u16 prescaler)
{
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    NVIC_InitTypeDef        NVIC_InitStructure;


    // 打开TIM2的时钟源
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);


    // 配置TIM2的工作参数
    TIM_TimeBaseStructure.TIM_Period = period;                  // 指定自动重装值
    TIM_TimeBaseStructure.TIM_Prescaler = prescaler;            // 指定预分频器的值
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_1;        // 
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; // 指定计数模式：向上计数
    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);

    
    // 打开TIM2的更新中断
    TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);                  // 使能TIM2的更新中断

    

    // 配置TIM优先级
    NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;             // 指定中断通道为TIM2中断
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;   // 指定抢占优先级为0
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;          // 指定响应优先级为1
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;             // 指定打开TIM2中断
    NVIC_Init(&NVIC_InitStructure);


    // 使能TIM2计数器
    TIM_Cmd(TIM2, ENABLE);
}

void TIM2_IRQHandler(void) __attribute__((interrupt("WCH-Interrupt-fast")));
void TIM2_IRQHandler(void)
{
    if(TIM_GetITStatus(TIM2, TIM_IT_Update) == SET) {
        TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
        flag = (flag == SET) ? RESET : SET;
        GPIO_WriteBit(GPIOC, GPIO_Pin_3, flag);
    }
}