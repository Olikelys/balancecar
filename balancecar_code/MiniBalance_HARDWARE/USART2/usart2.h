#ifndef __USRAT2_H
#define __USRAT2_H 
#include "sys.h"	  	
  /**************************************************************************
作者：平衡小车之家
我的淘宝小店：http://shop114407458.taobao.com/
**************************************************************************/
extern u8 Usart2_Receive;
void USART2_IRQHandler(void);
void USART2_Init(u32 bound);
#endif

