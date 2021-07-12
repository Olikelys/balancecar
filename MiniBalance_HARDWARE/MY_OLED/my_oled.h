/*********************************************************************************************************************
 * COPYRIGHT NOTICE
 * Copyright (c) 2016,��ɿƼ�
 * All rights reserved.
 * ��������QQȺ��179029047
 *
 * �����������ݰ�Ȩ������ɿƼ����У�δ��������������ҵ��;��
 * ��ӭ��λʹ�ò������������޸�����ʱ���뱣����ɿƼ��İ�Ȩ������
 *
 * @file       		SEEKFREE_OLED.h
 * @brief      		0.96OLED������
 * @company	   		�ɶ���ɿƼ����޹�˾
 * @author     		Go For It(1325536866)
 * @version    		v1.0
 * @Software 		IAR 7.2 or MDK 5.17
 * @Target core		MK60DN512VLL10
 * @Taobao   		https://seekfree.taobao.com/
 * @date       		2016-02-25
 * @note	
					NRF24L01���߶��壺
					------------------------------------ 
					    OLEDҺ��    ��Ƭ��                        
    					D0          A15
    					D1          A16            
    					RES         A14    
    					DC          B16
					------------------------------------ 
 ********************************************************************************************************************/



#ifndef _MY_OELD_H
#define _MY_OELD_H

#include "stm32f10x.h"

//----�궨��OLED����----	 
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
