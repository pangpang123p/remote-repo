/*
 * Periph_init.c
 *
 *  Created on: Jun 11, 2025
 *      Author: 003
 */

#include "Periph_init.h"
#include "debug.h"
#include "uart_cmd.h"
extern volatile uint32_t SysTickCnt,SysTickCnt_tmp;
const uint8_t crc8_table[256] = {
    0x00, 0x07, 0x0E, 0x09, 0x1C, 0x1B, 0x12, 0x15, 0x38, 0x3F, 0x36, 0x31, 0x24, 0x23, 0x2A, 0x2D,
    0x70, 0x77, 0x7E, 0x79, 0x6C, 0x6B, 0x62, 0x65, 0x48, 0x4F, 0x46, 0x41, 0x54, 0x53, 0x5A, 0x5D,
    0xE0, 0xE7, 0xEE, 0xE9, 0xFC, 0xFB, 0xF2, 0xF5, 0xD8, 0xDF, 0xD6, 0xD1, 0xC4, 0xC3, 0xCA, 0xCD,
    0x90, 0x97, 0x9E, 0x99, 0x8C, 0x8B, 0x82, 0x85, 0xA8, 0xAF, 0xA6, 0xA1, 0xB4, 0xB3, 0xBA, 0xBD,
    0xC7, 0xC0, 0xC9, 0xCE, 0xDB, 0xDC, 0xD5, 0xD2, 0xFF, 0xF8, 0xF1, 0xF6, 0xE3, 0xE4, 0xED, 0xEA,
    0xB7, 0xB0, 0xB9, 0xBE, 0xAB, 0xAC, 0xA5, 0xA2, 0x8F, 0x88, 0x81, 0x86, 0x93, 0x94, 0x9D, 0x9A,
    0x27, 0x20, 0x29, 0x2E, 0x3B, 0x3C, 0x35, 0x32, 0x1F, 0x18, 0x11, 0x16, 0x03, 0x04, 0x0D, 0x0A,
    0x57, 0x50, 0x59, 0x5E, 0x4B, 0x4C, 0x45, 0x42, 0x6F, 0x68, 0x61, 0x66, 0x73, 0x74, 0x7D, 0x7A,
    0x89, 0x8E, 0x87, 0x80, 0x95, 0x92, 0x9B, 0x9C, 0xB1, 0xB6, 0xBF, 0xB8, 0xAD, 0xAA, 0xA3, 0xA4,
    0xF9, 0xFE, 0xF7, 0xF0, 0xE5, 0xE2, 0xEB, 0xEC, 0xC1, 0xC6, 0xCF, 0xC8, 0xDD, 0xDA, 0xD3, 0xD4,
    0x69, 0x6E, 0x67, 0x60, 0x75, 0x72, 0x7B, 0x7C, 0x51, 0x56, 0x5F, 0x58, 0x4D, 0x4A, 0x43, 0x44,
    0x19, 0x1E, 0x17, 0x10, 0x05, 0x02, 0x0B, 0x0C, 0x21, 0x26, 0x2F, 0x28, 0x3D, 0x3A, 0x33, 0x34,
    0x4E, 0x49, 0x40, 0x47, 0x52, 0x55, 0x5C, 0x5B, 0x76, 0x71, 0x78, 0x7F, 0x6A, 0x6D, 0x64, 0x63,
    0x3E, 0x39, 0x30, 0x37, 0x22, 0x25, 0x2C, 0x2B, 0x06, 0x01, 0x08, 0x0F, 0x1A, 0x1D, 0x14, 0x13,
    0xAE, 0xA9, 0xA0, 0xA7, 0xB2, 0xB5, 0xBC, 0xBB, 0x96, 0x91, 0x98, 0x9F, 0x8A, 0x8D, 0x84, 0x83,
    0xDE, 0xD9, 0xD0, 0xD7, 0xC2, 0xC5, 0xCC, 0xCB, 0xE6, 0xE1, 0xE8, 0xEF, 0xFA, 0xFD, 0xF4, 0xF3
};




void SysTick_init(void)
{

    /*配置定时器*/
    SysTick->CTLR= 0;
    SysTick->SR  = 0;
    SysTick->CNT = 0;
    SysTick->CMP = SystemCoreClock/1000;//后面的1000代表1000HZ(那就是1ms进一次中断)
    SysTick->CTLR= 0xf;
    SysTickCnt=0;
    SysTickCnt_tmp=0;

//    NVIC_SetPriority(SysTicK_IRQn, 0);     //设置SysTick中断优先级
    NVIC_EnableIRQ(SysTicK_IRQn);           //使能开启Systick中断


}

void SysTick_delay_ms(u16 ms)
{
    SysTickCnt_tmp=SysTickCnt;
    while(1)
    {
        if((SysTickCnt-SysTickCnt_tmp)>ms)
        {
            break;
        }
    }

}


void GPIO_INIT_1(void)
{
    GPIO_InitTypeDef GPIO_InitStructure = {0};
    RCC_PB2PeriphClockCmd(RCC_PB2Periph_GPIOD | RCC_PB2Periph_GPIOC | RCC_PB2Periph_GPIOB | RCC_PB2Periph_GPIOA, ENABLE);

    // * b5-id4
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_30MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    // * b4-id3
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_30MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    // * b3-id2
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_30MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    // * b2-id1
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_30MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);


    //a5--do1
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_30MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    GPIO_WriteBit(GPIOA, GPIO_Pin_5, Bit_SET);

    //a6--do2
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_30MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    GPIO_WriteBit(GPIOA, GPIO_Pin_5, Bit_SET);

    //a7--do3
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_30MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    GPIO_WriteBit(GPIOA, GPIO_Pin_5, Bit_SET);

    //a4--do4
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_30MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    GPIO_WriteBit(GPIOA, GPIO_Pin_5, Bit_SET);

    //a3--do5
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_30MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    GPIO_WriteBit(GPIOA, GPIO_Pin_5, Bit_RESET);

    //B0--do6
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_30MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    GPIO_WriteBit(GPIOB, GPIO_Pin_0, Bit_RESET);

    //B0--do7
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_30MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    GPIO_WriteBit(GPIOB, GPIO_Pin_1, Bit_RESET);

    //C0--do8
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_30MHz;
    GPIO_Init(GPIOC, &GPIO_InitStructure);
    GPIO_WriteBit(GPIOC, GPIO_Pin_0, Bit_RESET);


    //c3-EN_IO_P
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_30MHz;
    GPIO_Init(GPIOC, &GPIO_InitStructure);
    GPIO_WriteBit(GPIOC, GPIO_Pin_3, Bit_RESET);

    //a0--OB_LED_G
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_30MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    GPIO_WriteBit(GPIOA, GPIO_Pin_0, Bit_RESET);

    //a1--OB_LED_B
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_30MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    GPIO_WriteBit(GPIOA, GPIO_Pin_1, Bit_RESET);

    //D0--OB_LED_Y
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_30MHz;
    GPIO_Init(GPIOD, &GPIO_InitStructure);
    GPIO_WriteBit(GPIOD, GPIO_Pin_0, Bit_RESET);





}


extern u8 B_ID;
void GPIO_INIT_2(void)
{
    GPIO_InitTypeDef GPIO_InitStructure = {0};
//    EXTI_InitTypeDef EXTI_InitStructure = {0};
//    NVIC_InitTypeDef NVIC_InitStructure = {0};


    //A2--GPIO3
    if(B_ID==0x21)//电源管理模块
    {
        //C2--GPIO1 钥匙开关
        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_30MHz;
        GPIO_Init(GPIOC, &GPIO_InitStructure);

        //C1--GPIO0 处理机箱开关机信号
        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_30MHz;
        GPIO_Init(GPIOC, &GPIO_InitStructure);
        GPIO_WriteBit(GPIOC, GPIO_Pin_1, Bit_SET);

        //B6--GPIO2
        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_30MHz;
        GPIO_Init(GPIOB, &GPIO_InitStructure);
        GPIO_WriteBit(GPIOB, GPIO_Pin_6, Bit_SET);



    }



}


extern u8 TxBuffer1[msg_len],TxBuffer2[msg_len],RxBuffer[RxBuffer_len];

void USARTx_CFG(void)
{
    GPIO_InitTypeDef  GPIO_InitStructure = {0};
    USART_InitTypeDef USART_InitStructure = {0};
    NVIC_InitTypeDef  NVIC_InitStructure = {0};
    DMA_InitTypeDef DMA_InitStructure = {0};

    RCC_PB2PeriphClockCmd(RCC_PB2Periph_GPIOD | RCC_PB2Periph_USART1, ENABLE);
    /* USART1 TX-->D.5   RX-->D.6 */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_30MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOD, &GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOD, &GPIO_InitStructure);

    USART_InitStructure.USART_BaudRate = 115200;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;

    USART_Init(USART1, &USART_InitStructure);


    USART_ITConfig(USART1, USART_IT_IDLE, ENABLE);
    NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    RCC_HBPeriphClockCmd(RCC_HBPeriph_DMA1, ENABLE);

    TxBuffer1[0]=0xaa;
    TxBuffer1[1]=0x55;
    TxBuffer2[0]=0xaa;
    TxBuffer2[1]=0x55;
    DMA_DeInit(DMA1_Channel4);
    DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)(&USART1->DATAR);
    DMA_InitStructure.DMA_MemoryBaseAddr = (u32)TxBuffer1;
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;
    DMA_InitStructure.DMA_BufferSize = msg_len;
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
    DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
    DMA_InitStructure.DMA_Priority = DMA_Priority_High;
    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
    DMA_Init(DMA1_Channel4, &DMA_InitStructure);

    DMA_DeInit(DMA1_Channel5);
    DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;
    DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)(&USART1->DATAR);
    DMA_InitStructure.DMA_MemoryBaseAddr = (u32)RxBuffer;
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
    DMA_InitStructure.DMA_BufferSize = RxBuffer_len;
    DMA_Init(DMA1_Channel5, &DMA_InitStructure);


    USART_Cmd(USART1, ENABLE);
    USART_DMACmd(USART1, USART_DMAReq_Rx, ENABLE);
    USART_DMACmd(USART1, USART_DMAReq_Tx, ENABLE);


    DMA_Cmd(DMA1_Channel4, DISABLE);/* USART1 Tx */
    DMA_Cmd(DMA1_Channel5, ENABLE); /* USART1 Rx */
}


void DMA_ReceiveData(void)
{

    u8 i=0;
    u8 RxCnt = 0;
    DMA_Cmd(DMA1_Channel5, DISABLE); // 关闭DMA，防止干扰
    RxCnt = RxBuffer_len - DMA_GetCurrDataCounter(DMA1_Channel5);
//    printf("RxCnt1:%d,RxBuffer1:%s\r\n",RxCnt1 ,RxBuffer1);
    if(RxCnt>(msg_len-1))
    {
        do
        {
            if(RxBuffer[i]==msg_head1 && RxBuffer[i+1]==msg_head2)
            {
                reply_cmd(&RxBuffer[i]);
                break;
            }
            i++;
            if(RxCnt-i < msg_len) break;
        } while (i<RxCnt);

    }

    DMA_SetCurrDataCounter(DMA1_Channel5, RxBuffer_len); // 重新赋值计数值，必须大于等于最大可能接收到的数据帧数目
    DMA_Cmd(DMA1_Channel5, ENABLE);

}

// 计算 CRC8 校验值
u8 crc8(const u8 *data, u32 length)
{
    u8 crc = 0,tmp=0;
    for (u32 i = 0; i < length; i++)
    {
        tmp=crc ^ data[i];
        crc = *(crc8_table+(tmp));
    }
    return crc;
}
