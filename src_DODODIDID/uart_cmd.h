/*
 * uart_cmd.h
 *
 *  Created on: Feb 6, 2025
 *      Author: 002
 */

#ifndef USER_UART_CMD_H_
#define USER_UART_CMD_H_


extern void core_init(void);

extern void update_sys_status(void);

extern u8 reply_cmd(u8* cmd_buffer);

extern void DMA_TxData(void);

typedef enum
{
    IDLE = 0,
    PENDING=1,
    EXECUTING = 2
} bus_status;//0=空闲 1=接收到写入指令但未执行 2=操作中

#define RxBuffer_len    30


//消息类别
#define msg_Report          0x00
#define msg_Query           0x01
#define msg_Execute         0x02

//消息内容----------------------------------------------------------------------------------------------------------------


//电源管理-动作-外设控制
#define GET_PM_DO_CH(msg)                    ((msg & 0xff00)>>8)
#define GET_PM_DO_VAL(msg)                   (msg & 0x00ff)

//模块状态-状态
#define SET_PM_Key_Status(msg,a)                    { msg = (msg & 0xfffe) | a; }
#define SET_PM_DO8_Status(msg,a)                     { msg = (msg & 0xffd) | (a<<1); }
#define SET_PM_DO7_Status(msg,a)                     { msg = (msg & 0xffb) | (a<<2); }
#define SET_PM_DO6_Status(msg,a)                     { msg = (msg & 0xff7) | (a<<3); }
#define SET_PM_DO5_Status(msg,a)                     { msg = (msg & 0xffef) | (a<<4); }
#define SET_PM_Undefined(msg,a)                     { msg = (msg & 0xffdf) | (a<<7); }
#define SET_PM_CRC_ERROR(msg,a)                     { msg = (msg & 0xffbf) | (a<<8); }


#define msg_len                     12

#define msg_head1 0xAA
#define msg_head2 0x55




//  b=u8首个数指针
#define U8_TO_U16(b) ((((uint16_t)(*(b)))<<8) | ((uint16_t)(*(b+1))))
#define U8_TO_U32(b) ((((uint32_t)(*(b)))<<24) | (((uint32_t)(*(b+1)))<<16) | (((uint32_t)(*(b+2)))<<8) | ((uint32_t)(*(b+3))))



#endif /* USER_UART_CMD_H_ */
