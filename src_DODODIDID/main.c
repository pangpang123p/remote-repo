/********************************** (C) COPYRIGHT *******************************
 * File Name          : main.c
 * Author             : WCH
 * Version            : V1.0.0
 * Date               : 2024/01/01
 * Description        : Main program body.
 *********************************************************************************
 * Copyright (c) 2021 Nanjing Qinheng Microelectronics Co., Ltd.
 * Attention: This software (modified or not) and binary are used for 
 * microcontroller manufactured by Nanjing Qinheng Microelectronics.
 *******************************************************************************/
/*
 *@Note
 *GPIO routine:
 *PD0 push-pull output.
 *
*/

#include "debug.h"
#include "Periph_init.h"
#include "uart_cmd.h"
/* Global define */


/* Global Variable */
u8 B_ID=0x20;
volatile u8 KEY_status=0,power_status=0,power_key_cnt=0,x86_status=0;
volatile u16 module_status=0;
volatile u8 reply_cnt=0;//待回复状态查询消息个数
volatile uint32_t SysTickCnt,SysTickCnt_tmp;
void SysTick_Handler(void) __attribute__((interrupt("WCH-Interrupt-fast")));
void SysTick_Handler(void)
{
    SysTick->SR = 0;
    SysTickCnt++;
    if (SysTickCnt>0xfffffff0)
    {
        SysTickCnt=0;
    }
}


void USART1_IRQHandler(void) __attribute__((interrupt("WCH-Interrupt-fast")));
void USART1_IRQHandler(void)
{
    if (USART_GetITStatus(USART1, USART_IT_IDLE) != RESET)
    {
        DMA_ReceiveData();         // 接收数据、处理数据

        USART_ReceiveData(USART1); // 清除空闲中断标志位


    }
}


/*********************************************************************
 * @fn      main
 *
 * @brief   Main program.
 *
 * @return  none
 */
int main(void)
{
    u8 i = 0;
    u8 KEY_status_current=0;

    GPIO_InitTypeDef GPIO_InitStructure = {0};

    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
    SystemCoreClockUpdate();
    //初始化tx_ex
    RCC_PB2PeriphClockCmd(RCC_PB2Periph_GPIOD, ENABLE);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_30MHz;
    GPIO_Init(GPIOD, &GPIO_InitStructure);

#if (SDI_PRINT == SDI_PR_OPEN)
    SDI_Printf_Enable();
#else
    USART_Printf_Init(115200);
#endif
    SysTick_init();
    GPIO_INIT_1();
    Board_LED_Y(1);
    while(B_ID==0x20)
    {
        B_ID=0x20 | READ_ID;
        SysTick_delay_ms(1);
        if(B_ID !=0x20)
        {
            i=0;
            do{
                SysTick_delay_ms(100);
                if((0x20 | READ_ID)!=B_ID)
                {
                    B_ID=0x20;
                    break;
                }
                i++;
            }while(i<3);
            if(i==3)
            {
                printf( "%d:BoardID=0x%02x\r\n", SysTickCnt,B_ID);
                Board_LED_Y(0);
                break;
            }
        }
    }


    USARTx_CFG();

//    printf("SystemClk:%d\r\n", SystemCoreClock);
//    printf( "ChipID:%08x\r\n", DBGMCU_GetCHIPID() );
//    printf("GPIO Toggle TEST\r\n");
    
    if(B_ID==0x21)
    {
        GPIO_INIT_2();
        SysTick_delay_ms(1);
        KEY_status=READ_KEY;
    }
    else if(B_ID==0x22)//状态指示模块
    {

    }

    while(1)
    {
        DMA_TxData();

        if(SysTickCnt%501==0)//
        {
            Board_LED_G_Blink;
            power_key_cnt++;
            SysTick_delay_ms(1);

        }


        if(B_ID==0x21)//电源管理模块
        {

            KEY_status_current=READ_KEY;
            SysTick_delay_ms(1);
            if(KEY_status_current!=KEY_status)
            {
                i=0;
                do{
                    SysTick_delay_ms(1);
                    if(READ_KEY==KEY_status)
                    {
                        break;
                    }
                    i++;
                }while(i<3);
                if(i==3)
                {
                    KEY_status=KEY_status_current;
                }

            }

            switch (power_status)
                    {
                        case 0:
                            LED_Y(1);
                            if(KEY_status==1)
                            {
                                SET_PM_Key_Status(module_status,1);
                                power_status=4;
                            }

                            break;
                        case 1:
                            if(power_key_cnt>40 || x86_status==0)//等待上位机关机消息，或延时40*0.5s
                            {
                                AC_PWR2(0);
                                AC_PWR3(0);
                                AC_PWR4(0);
                                SysTick_delay_ms(500);
                                AC_PWR1(0);

                            }
                            else if(power_key_cnt>80)
                            {
                                power_key_cnt=0;
                                if(KEY_status==0)
                                {
                                    power_status=0;

                                }
                                else
                                {
                                    power_status=4;
                                }
                            }
                            break;
                        case 2:
                            //钥匙关机动作,上报关机消息
                            SET_PM_Key_Status(module_status,0);
                            reply_cnt++;
                            power_key_cnt=0;
                            power_status=1;
                            break;
                        case 4:
                            AC_PWR1(1);//处理机箱
                            SysTick_delay_ms(100);
//                            //处理机箱开关机信号输出
//                            GPIO_WriteBit(GPIOC, GPIO_Pin_1, Bit_RESET);
//                            SysTick_delay_ms(100);
//                            GPIO_WriteBit(GPIOC, GPIO_Pin_1, Bit_SET);
//                            SysTick_delay_ms(100);
                            AC_PWR2(1);
                            AC_PWR3(1);
                            AC_PWR4(1);
                            power_status=5;
                            break;
                        case 5:
                            if(power_key_cnt>20)//延时20*0.5s
                            {
                                if(KEY_status==1)
                                {
                                    power_status=6;
                                    LED_Y(0);
                                }
                                else
                                {
                                    power_status=2;
                                }
                            }
                            break;
                        case 6:
                            LED_Y(1);
                            if(KEY_status==0)
                            {
                                power_status=2;
                            }

                            break;
                    default:
                            break;
                    }




        }
        else if(B_ID==0x22)//状态指示模块
        {

        }

    }
}
