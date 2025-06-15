/*
 * uart_cmd.c
 *
 *  Created on: Feb 6, 2025
 *      Author: 002
 */


#include "Periph_init.h"
#include "uart_cmd.h"
#include <string.h>


volatile u8 Tx_flag=0;
extern volatile u8 reply_cnt;
bus_status tx_buf1_status=IDLE;
bus_status tx_buf2_status=IDLE;

u8 RxBuffer[RxBuffer_len]={0};
u8 TxBuffer1[msg_len]={0};
u8 TxBuffer2[msg_len]={0};

extern u8 B_ID;
extern volatile u16 module_status;
extern volatile uint32_t SysTickCnt;
extern volatile u8 x86_status;
u8 reply_cmd(u8* cmd_buffer)
{

    if(crc8(cmd_buffer,11)==*(cmd_buffer+11) )
    {


        if(*(cmd_buffer+2)==0xFA && *(cmd_buffer+3)==0xff  && *(cmd_buffer+8)==msg_Execute && U8_TO_U16(cmd_buffer+9)==0)//授时
        {
            if(U8_TO_U32(cmd_buffer+4)!=0)
            {
                SysTickCnt=U8_TO_U32(cmd_buffer+4);
                x86_status=1;//上位机开机状态
            }
        }
        else if(*(cmd_buffer+2)==0xFA && *(cmd_buffer+3)==B_ID && *(cmd_buffer+8)==msg_Execute && U8_TO_U16(cmd_buffer+9)!=0)//电源控制
        {
            switch(GET_PM_DO_CH(U8_TO_U16(cmd_buffer+9)))
            {
                case 5://仅在上位机执行完关机程序后发出
                    x86_status=0;
                    break;
                case 6:
                    AC_PWR2(GET_PM_DO_VAL(U8_TO_U16(cmd_buffer+9)));
                    SET_PM_DO6_Status(module_status,(GET_PM_DO_VAL(U8_TO_U16(cmd_buffer+9))));
                    break;
                case 7:
                    AC_PWR3(GET_PM_DO_VAL(U8_TO_U16(cmd_buffer+9)));
                    SET_PM_DO7_Status(module_status,(GET_PM_DO_VAL(U8_TO_U16(cmd_buffer+9))));
                    break;
                case 8:
                    AC_PWR4(GET_PM_DO_VAL(U8_TO_U16(cmd_buffer+9)));
                    SET_PM_DO8_Status(module_status,(GET_PM_DO_VAL(U8_TO_U16(cmd_buffer+9))));
                    break;
                default:

                    break;

            }
        }
        else if(*(cmd_buffer+2)==0xFA && *(cmd_buffer+3)==B_ID && *(cmd_buffer+8)==msg_Query && U8_TO_U16(cmd_buffer+9)==1)//状态查询
        {
            reply_cnt++;
        }
        else
        {
            SET_PM_Undefined(module_status,1);
        }


    }
    else//消息内容校验错误
    {
        SET_PM_CRC_ERROR(module_status,1);
    }

    return 0;
}

void DMA_TxData(void)
{
//    u8* tx_buf_sel=0;
    u32 tick_tmp=0;
    TxBuffer1[2]=B_ID;
    TxBuffer1[3]=0xFA;
    TxBuffer1[8]=msg_Report;

    TxBuffer2[2]=B_ID;
    TxBuffer2[3]=0xFA;
    TxBuffer2[8]=msg_Report;

    if(reply_cnt>0 )
    {
        if(tx_buf1_status==IDLE)
        {

            TxBuffer1[9]=(u8)(module_status>>8);
            TxBuffer1[10]=(u8)module_status;
            tick_tmp=SysTickCnt;
            TxBuffer1[4]=(u8)(tick_tmp>>24);
            TxBuffer1[5]=(u8)(tick_tmp>>16);
            TxBuffer1[6]=(u8)(tick_tmp>>8);
            TxBuffer1[7]=(u8)(tick_tmp);
            TxBuffer1[11]=crc8(TxBuffer1,11);
            tx_buf1_status=PENDING;
            reply_cnt--;

        }
        else if(tx_buf2_status==IDLE)
        {

            TxBuffer2[9]=(u8)(module_status>>8);
            TxBuffer2[10]=(u8)module_status;
            tick_tmp=SysTickCnt;
            TxBuffer2[4]=(u8)(tick_tmp>>24);
            TxBuffer2[5]=(u8)(tick_tmp>>16);
            TxBuffer2[6]=(u8)(tick_tmp>>8);
            TxBuffer2[7]=(u8)(tick_tmp);
            TxBuffer2[11]=crc8(TxBuffer1,11);

            tx_buf2_status=PENDING;
            reply_cnt--;
        }
    }


     switch(Tx_flag)
     {
         case 0:
             if(tx_buf1_status==PENDING)
             {
                 Tx_flag=1;
                 DMA_SetCurrDataCounter(DMA1_Channel4, 12);
             }
             else if(tx_buf1_status==IDLE && tx_buf2_status==PENDING)
             {
                 Tx_flag=3;
                 DMA_SetCurrDataCounter(DMA1_Channel4, 12);
             }
             break;

         case 1:
             DMA1_Channel4->MADDR=(u32)TxBuffer1;
             tx_en(1);
             DMA_Cmd(DMA1_Channel4, ENABLE);
             tx_buf1_status=EXECUTING;
             Tx_flag=2;
             break;
         case 2:
             if(DMA_GetFlagStatus( DMA1_FLAG_TC4))
             {
                 SysTick_delay_ms(1);
                 tx_buf1_status=IDLE;
                 tx_en(0);
                 DMA_ClearFlag( DMA1_FLAG_TC4);
                 DMA_Cmd(DMA1_Channel4, DISABLE);

                 SET_PM_Undefined(module_status,0);
                 SET_PM_CRC_ERROR(module_status,0);

                 if(tx_buf2_status==IDLE)
                 {
                     Tx_flag=0;
                 }
                 else if(tx_buf2_status==PENDING)
                 {
                     Tx_flag=3;
                     DMA_SetCurrDataCounter(DMA1_Channel4, 12);
                 }
             }
             break;
         case 3:
             DMA1_Channel4->MADDR=(u32)TxBuffer2;
             tx_en(1);
             DMA_Cmd(DMA1_Channel4, ENABLE);
             tx_buf2_status=EXECUTING;
             Tx_flag=4;
             break;
         case 4:
             if(DMA_GetFlagStatus( DMA1_FLAG_TC4))
             {
                 SysTick_delay_ms(1);
                 tx_buf2_status=IDLE;
                 tx_en(0);
                 DMA_ClearFlag( DMA1_FLAG_TC4);
                 DMA_Cmd(DMA1_Channel4, DISABLE);

                 SET_PM_Undefined(module_status,0);
                 SET_PM_CRC_ERROR(module_status,0);

                 if(tx_buf1_status==IDLE)
                 {
                     Tx_flag=0;
                 }
                 else if(tx_buf1_status==PENDING)
                 {
                     Tx_flag=1;
                     DMA_SetCurrDataCounter(DMA1_Channel4, 12);
                 }
             }
             break;
         default:
             break;

     }

}

