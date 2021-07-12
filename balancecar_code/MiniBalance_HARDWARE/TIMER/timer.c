#include "timer.h"
#include "mpu6050.h"
  /**************************************************************************
���ߣ�ƽ��С��֮��
�ҵ��Ա�С�꣺http://shop114407458.taobao.com/
**************************************************************************/
/**************************************************************************
�������ܣ���ʱ��3ͨ��3���벶���ʼ��
��ڲ�������ڲ�����arr���Զ���װֵ  psc��ʱ��Ԥ��Ƶ�� 
����  ֵ����
**************************************************************************/
TIM_ICInitTypeDef  TIM3_ICInitStructure;
void TIM3_Cap_Init(u16 arr,u16 psc)	
{	 
	
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
 	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);	//ʹ��TIM3ʱ��
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);  //ʹ��GPIOBʱ��
	
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_0; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; //PB0 ����  
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_1;     
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;     //PB1��� 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;     //2M
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	//��ʼ����ʱ��3 TIM3	 
	TIM_TimeBaseStructure.TIM_Period = arr; //�趨�������Զ���װֵ 
	TIM_TimeBaseStructure.TIM_Prescaler =psc; 	//Ԥ��Ƶ��   
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); //����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
  
	//��ʼ��TIM3���벶�����
	TIM3_ICInitStructure.TIM_Channel = TIM_Channel_3; //CC1S=03 	ѡ������� IC3ӳ�䵽TI1��
  TIM3_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;	//�����ز���
  TIM3_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
  TIM3_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;	 //���������Ƶ,����Ƶ 
  TIM3_ICInitStructure.TIM_ICFilter = 0x00;//���������˲��� ���˲�
  TIM_ICInit(TIM3, &TIM3_ICInitStructure);
	
	//�жϷ����ʼ��
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;  //TIM3�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;  //��ռ���ȼ�2��
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;  //�����ȼ�0��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQͨ����ʹ��
	NVIC_Init(&NVIC_InitStructure);  //����NVIC_InitStruct��ָ���Ĳ�����ʼ������NVIC�Ĵ��� 	
	TIM_ITConfig(TIM3,TIM_IT_Update|TIM_IT_CC3,ENABLE);//��������ж� ,����CC3IE�����ж�	
  TIM_Cmd(TIM3,ENABLE ); 	//ʹ�ܶ�ʱ��3
}
/**************************************************************************
�������ܣ����������ջز�����
��ڲ�������
����  ֵ����
**************************************************************************/
u16 TIM3CH3_CAPTURE_STA,TIM3CH3_CAPTURE_VAL;
void Read_Distane(void)
{   
	 PBout(1)=1;
	 delay_us(15);  
	 PBout(1)=0;	
			if(TIM3CH3_CAPTURE_STA&0X80)//�ɹ�������һ�θߵ�ƽ
		{
			Distance=TIM3CH3_CAPTURE_STA&0X3F;
			Distance*=65536;					        //���ʱ���ܺ�
			Distance+=TIM3CH3_CAPTURE_VAL;		//�õ��ܵĸߵ�ƽʱ��
			Distance=Distance*170/1000;
		//	printf("%d \r\n",Distance);
			TIM3CH3_CAPTURE_STA=0;			//������һ�β���
		}				
}
/**************************************************************************
�������ܣ��������ز������ȡ�ж�
��ڲ�������
����  ֵ����
��    �ߣ�ƽ��С��֮��
**************************************************************************/
//void TIM3_IRQHandler(void)
//{ 		    		  			    
//	u16 tsr;
//	tsr=TIM3->SR;
//	if((TIM3CH3_CAPTURE_STA&0X80)==0)//��δ�ɹ�����	
//				{
//								if(tsr&0X01)//���
//								{	    
//										if(TIM3CH3_CAPTURE_STA&0X40)//�Ѿ����񵽸ߵ�ƽ��
//										{
//											if((TIM3CH3_CAPTURE_STA&0X3F)==0X3F)//�ߵ�ƽ̫����
//											{
//												TIM3CH3_CAPTURE_STA|=0X80;//��ǳɹ�������һ��
//												TIM3CH3_CAPTURE_VAL=0XFFFF;
//											}else TIM3CH3_CAPTURE_STA++;
//										}	 
//								}
//						   	if(tsr&0x08)//����3���������¼�
//				    	{	
//											if(TIM3CH3_CAPTURE_STA&0X40)		//����һ���½��� 		
//											{	  			
//											TIM3CH3_CAPTURE_STA|=0X80;		//��ǳɹ�����һ�θߵ�ƽ����
//											TIM3CH3_CAPTURE_VAL=TIM3->CCR3;	//��ȡ��ǰ�Ĳ���ֵ.
//											TIM3->CCER&=~(1<<9);			//CC1P=0 ����Ϊ�����ز���
//									  	}else  								//��δ��ʼ,��һ�β���������
//				   	{
//											TIM3CH3_CAPTURE_STA=0;			//���
//											TIM3CH3_CAPTURE_VAL=0;
//											TIM3CH3_CAPTURE_STA|=0X40;		//��ǲ�����������
//											TIM3->CNT=0;					//���������
//											TIM3->CCER|=1<<9; 				//CC1P=1 ����Ϊ�½��ز���
//							}		    
//					    	}			     	    					   
//		   }
//			 TIM3->SR=0;//����жϱ�־λ 	     
//}


void TIM3_NVIC_Init (void){ //����TIM3�ж�����
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;	
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;	//������ռ�������ȼ�
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

void TIM3_Init(u16 arr,u16 psc){  //TIM3 ��ʼ�� arr��װ��ֵ pscԤ��Ƶϵ��
    TIM_TimeBaseInitTypeDef     TIM_TimeBaseInitStrue;
    
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);//ʹ��TIM3
    TIM3_NVIC_Init (); //����TIM3�ж�����
	      
    TIM_TimeBaseInitStrue.TIM_Period=arr; //�����Զ���װ��ֵ
    TIM_TimeBaseInitStrue.TIM_Prescaler=psc; //Ԥ��Ƶϵ��
    TIM_TimeBaseInitStrue.TIM_CounterMode=TIM_CounterMode_Up; //�������������
    TIM_TimeBaseInitStrue.TIM_ClockDivision=TIM_CKD_DIV1; //ʱ�ӵķ�Ƶ���ӣ�����һ������ʱ���ã�һ����ΪTIM_CKD_DIV1
    TIM_TimeBaseInit(TIM3,&TIM_TimeBaseInitStrue); //TIM3��ʼ������
    TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);//ʹ��TIM3�ж�    
    TIM_Cmd(TIM3,ENABLE); //ʹ��TIM3
}

void TIM3_IRQHandler(void){ //TIM3�жϴ�����
	
//		u8 i=0;
//		u16 t[14]={0};//{0x55,0x01,0x02,0x03,0x04,0x05,0x06,0xaa,0x22,0x22,0x22,0x22,0x22,0x22,0xaa,0x33,0x33,0x33,0x33,0x33,0x33};
//		static u8 ctl_period=0;
		
		
    if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)//�ж��Ƿ���TIM3�ж�
		{	
        TIM_ClearITPendingBit(TIM3, TIM_IT_Update);//���жϱ�־
//			oled_show();          //===��ʾ����

        //������̬����
				//MPU6050_READ(t);	//��ȡMPU6050����
			
//				//���ͱ����������������������������ٶ���423
//				Encoder_Left=(short)TIM2->CNT;
//				TIM2->CNT=0;
//				USART_SendData(USART2 ,Encoder_Left>>8); //���͵�����ֵ
//				while(USART_GetFlagStatus(USART2, USART_FLAG_TC)==RESET); //��鷢���жϱ�־λ
//				USART_SendData(USART2 ,Encoder_Left&0x00ff); //���͵�����ֵ
//				while(USART_GetFlagStatus(USART2, USART_FLAG_TC)==RESET); //��鷢���жϱ�־λ
//				Encoder_Right=(short)TIM4->CNT;
//				TIM4->CNT=0;
//				USART_SendData(USART2 ,Encoder_Right>>8); //���͵�����ֵ
//				while(USART_GetFlagStatus(USART2, USART_FLAG_TC)==RESET); //��鷢���жϱ�־λ
//				USART_SendData(USART2 ,Encoder_Right&0x00ff); //���͵�����ֵ
//				while(USART_GetFlagStatus(USART2, USART_FLAG_TC)==RESET); //��鷢���жϱ�־λ
//				if (ctl_period == 9)
//				{
//					ctl_period=0;
//						
//				}
//				ctl_period+=1;
//				TIM_SetCompare1(TIM1,0);
//				TIM_SetCompare2(TIM1,20);
//				TIM_SetCompare3(TIM1,20);
//				TIM_SetCompare4(TIM1,0);	
    }
}
