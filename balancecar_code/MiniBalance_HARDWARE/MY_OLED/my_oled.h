/*********************************************************************************************************************
 * COPYRIGHT NOTICE
 * Copyright (c) 2016,逐飞科技
 * All rights reserved.
 * 技术讨论QQ群：179029047
 *
 * 以下所有内容版权均属逐飞科技所有，未经允许不得用于商业用途，
 * 欢迎各位使用并传播本程序，修改内容时必须保留逐飞科技的版权声明。
 *
 * @file       		SEEKFREE_OLED.h
 * @brief      		0.96OLED函数库
 * @company	   		成都逐飞科技有限公司
 * @author     		Go For It(1325536866)
 * @version    		v1.0
 * @Software 		IAR 7.2 or MDK 5.17
 * @Target core		MK60DN512VLL10
 * @Taobao   		https://seekfree.taobao.com/
 * @date       		2016-02-25
 * @note	
					NRF24L01接线定义：
					------------------------------------ 
					    OLED液晶    单片机                        
    					D0          A15
    					D1          A16            
    					RES         A14    
    					DC          B16
					------------------------------------ 
 ********************************************************************************************************************/



#ifndef _MY_OELD_H
#define _MY_OELD_H

#include "stm32f10x.h"

//----宏定义OLED引脚----	 
#define  OLED_SCL_PIN	GP07
#define  OLED_SDA_PIN	GP05
#define  OLED_RST_PIN	GP04
#define  OLED_DC_PIN	GP03
#define  OLED_CS_PIN	GP02


void Dly_ms(u16 ms);
void OLED_Init(void);
void OLED_Fill(u8 dat);
void OLED_WrDat(u8 Data);
void OLED_Wr6Dat(u8 Data);
void OLED_PutPixel(u8 x,u8 y);
void OLED_ClrPixel(u8 x,u8 y);
void OLED_Set_Pos(u8 x, u8 y);
void OLED_P6x8Str(u8 x,u8 y,u8 ch[]);
void OLED_P8x16Str(u8 x,u8 y,u8 ch[]);
void OLED_HEXACSII(u16 hex,u8* Print);
void OLED_Print_Num(u8 x, u8 y, u16 num);
void OLED_Print_Num1(u8 x, u8 y, int16_t num);
void dis_bmp(u16 high, u16 width, u8 *p,u8 value);



/********************************************************************/

#endif
