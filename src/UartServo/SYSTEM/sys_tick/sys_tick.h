/*
 * 系统时间管理
 * ---------------------------------------------------
 * 作者: 阿凯爱玩机器人
 * Email: kyle.xing@fashionstar.com.hk
 * 更新时间: 2021-03-16
 * ---------------------------------------------------
 */
#ifndef __SYSTICK_H
#define __SYSTICK_H

#include "stm32f4xx.h" 

// 系统定时器初始化
void SysTick_Init(void);

// 延时
// 延时 us
void SysTick_DelayUs(__IO u32 nTime);
// 延时 ms
void SysTick_DelayMs(__IO u32 nTime);
// 延时 s
void SysTick_DelayS(__IO u32 nTime);

// 倒计时
// 设置倒计时(非阻塞式)
void SysTick_CountdownBegin(__IO u32 nTime);
// 撤销倒计时
void SysTick_CountdownCancel(void);
// 判断倒计时是否超时
uint8_t SysTick_CountdownIsTimeout(void);

#endif
