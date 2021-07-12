#include "timer.h"
#include "mpu6050.h"
  /**************************************************************************
作者：平衡小车之家
我的淘宝小店：http://shop114407458.taobao.com/
**************************************************************************/
/**************************************************************************
函数功能：定时器3通道3输入捕获初始化
入口参数：入口参数：arr：自动重装值  psc：时钟预分频数 
返回  值：无
**************************************************************************/
TIM_ICInitTypeDef  TIM3_ICInitStructure;
void TIM3_Cap_Init(u16 arr,u16 psc)	
{	 
	
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
 	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);	//使能TIM3时钟
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);  //使能GPIOB时钟
	
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_0; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; //PB0 输入  
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_1;     
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;     //PB1输出 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;     //2M
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	//初始化定时器3 TIM3	 
	TIM_TimeBaseStructure.TIM_Period = arr; //设定计数器自动重装值 
	TIM_TimeBaseStructure.TIM_Prescaler =psc; 	//预分频器   
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); //根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位
  
	//初始化TIM3输入捕获参数
	TIM3_ICInitStructure.TIM_Channel = TIM_Channel_3; //CC1S=03 	选择输入端 IC3映射到TI1上
  TIM3_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;	//上升沿捕获
  TIM3_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
  TIM3_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;	 //配置输入分频,不分频 
  TIM3_ICInitStructure.TIM_ICFilter = 0x00;//配置输入滤波器 不滤波
  TIM_ICInit(TIM3, &TIM3_ICInitStructure);
	
	//中断分组初始化
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;  //TIM3中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;  //先占优先级2级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;  //从优先级0级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道被使能
	NVIC_Init(&NVIC_InitStructure);  //根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器 	
	TIM_ITConfig(TIM3,TIM_IT_Update|TIM_IT_CC3,ENABLE);//允许更新中断 ,允许CC3IE捕获中断	
  TIM_Cmd(TIM3,ENABLE ); 	//使能定时器3
}
/**************************************************************************
函数功能：超声波接收回波函数
入口参数：无
返回  值：无
**************************************************************************/
u16 TIM3CH3_CAPTURE_STA,TIM3CH3_CAPTURE_VAL;
void Read_Distane(void)
{   
	 PBout(1)=1;
	 delay_us(15);  
	 PBout(1)=0;	
			if(TIM3CH3_CAPTURE_STA&0X80)//成功捕获到了一次高电平
		{
			Distance=TIM3CH3_CAPTURE_STA&0X3F;
			Distance*=65536;					        //溢出时间总和
			Distance+=TIM3CH3_CAPTURE_VAL;		//得到总的高电平时间
			Distance=Distance*170/1000;
		//	printf("%d \r\n",Distance);
			TIM3CH3_CAPTURE_STA=0;			//开启下一次捕获
		}				
}
/**************************************************************************
函数功能：超声波回波脉宽读取中断
入口参数：无
返回  值：无
作    者：平衡小车之家
**************************************************************************/
//void TIM3_IRQHandler(void)
//{ 		    		  			    
//	u16 tsr;
//	tsr=TIM3->SR;
//	if((TIM3CH3_CAPTURE_STA&0X80)==0)//还未成功捕获	
//				{
//								if(tsr&0X01)//溢出
//								{	    
//										if(TIM3CH3_CAPTURE_STA&0X40)//已经捕获到高电平了
//										{
//											if((TIM3CH3_CAPTURE_STA&0X3F)==0X3F)//高电平太长了
//											{
//												TIM3CH3_CAPTURE_STA|=0X80;//标记成功捕获了一次
//												TIM3CH3_CAPTURE_VAL=0XFFFF;
//											}else TIM3CH3_CAPTURE_STA++;
//										}	 
//								}
//						   	if(tsr&0x08)//捕获3发生捕获事件
//				    	{	
//											if(TIM3CH3_CAPTURE_STA&0X40)		//捕获到一个下降沿 		
//											{	  			
//											TIM3CH3_CAPTURE_STA|=0X80;		//标记成功捕获到一次高电平脉宽
//											TIM3CH3_CAPTURE_VAL=TIM3->CCR3;	//获取当前的捕获值.
//											TIM3->CCER&=~(1<<9);			//CC1P=0 设置为上升沿捕获
//									  	}else  								//还未开始,第一次捕获上升沿
//				   	{
//											TIM3CH3_CAPTURE_STA=0;			//清空
//											TIM3CH3_CAPTURE_VAL=0;
//											TIM3CH3_CAPTURE_STA|=0X40;		//标记捕获到了上升沿
//											TIM3->CNT=0;					//计数器清空
//											TIM3->CCER|=1<<9; 				//CC1P=1 设置为下降沿捕获
//							}		    
//					    	}			     	    					   
//		   }
//			 TIM3->SR=0;//清除中断标志位 	     
//}


void TIM3_NVIC_Init (void){ //开启TIM3中断向量
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;	
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;	//设置抢占和子优先级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

void TIM3_Init(u16 arr,u16 psc){  //TIM3 初始化 arr重装载值 psc预分频系数
    TIM_TimeBaseInitTypeDef     TIM_TimeBaseInitStrue;
    
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);//使能TIM3
    TIM3_NVIC_Init (); //开启TIM3中断向量
	      
    TIM_TimeBaseInitStrue.TIM_Period=arr; //设置自动重装载值
    TIM_TimeBaseInitStrue.TIM_Prescaler=psc; //预分频系数
    TIM_TimeBaseInitStrue.TIM_CounterMode=TIM_CounterMode_Up; //计数器向上溢出
    TIM_TimeBaseInitStrue.TIM_ClockDivision=TIM_CKD_DIV1; //时钟的分频因子，起到了一点点的延时作用，一般设为TIM_CKD_DIV1
    TIM_TimeBaseInit(TIM3,&TIM_TimeBaseInitStrue); //TIM3初始化设置
    TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);//使能TIM3中断    
    TIM_Cmd(TIM3,ENABLE); //使能TIM3
}

void TIM3_IRQHandler(void){ //TIM3中断处理函数
	
//		u8 i=0;
//		u16 t[14]={0};//{0x55,0x01,0x02,0x03,0x04,0x05,0x06,0xaa,0x22,0x22,0x22,0x22,0x22,0x22,0xaa,0x33,0x33,0x33,0x33,0x33,0x33};
//		static u8 ctl_period=0;
		
		
    if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)//判断是否是TIM3中断
		{	
        TIM_ClearITPendingBit(TIM3, TIM_IT_Update);//清中断标志
//			oled_show();          //===显示屏打开

        //发送姿态参数
				//MPU6050_READ(t);	//读取MPU6050数据
			
//				//发送编码器参数，编码器测出来的最快速度是423
//				Encoder_Left=(short)TIM2->CNT;
//				TIM2->CNT=0;
//				USART_SendData(USART2 ,Encoder_Left>>8); //发送单个数值
//				while(USART_GetFlagStatus(USART2, USART_FLAG_TC)==RESET); //检查发送中断标志位
//				USART_SendData(USART2 ,Encoder_Left&0x00ff); //发送单个数值
//				while(USART_GetFlagStatus(USART2, USART_FLAG_TC)==RESET); //检查发送中断标志位
//				Encoder_Right=(short)TIM4->CNT;
//				TIM4->CNT=0;
//				USART_SendData(USART2 ,Encoder_Right>>8); //发送单个数值
//				while(USART_GetFlagStatus(USART2, USART_FLAG_TC)==RESET); //检查发送中断标志位
//				USART_SendData(USART2 ,Encoder_Right&0x00ff); //发送单个数值
//				while(USART_GetFlagStatus(USART2, USART_FLAG_TC)==RESET); //检查发送中断标志位
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
