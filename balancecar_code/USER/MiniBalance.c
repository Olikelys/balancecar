#include "stm32f10x.h"
#include "sys.h"
  /**************************************************************************
作者：平衡小车之家
我的淘宝小店：http://shop114407458.taobao.com/
**************************************************************************/
u8 Way_Angle=1;                             //获取角度的算法，1：四元数  2：卡尔曼  3：互补滤波 
u8 Flag_Qian,Flag_Hou,Flag_Left,Flag_Right,Flag_sudu=2; //蓝牙遥控相关的变量
u8 Flag_Stop=1,Flag_Show=0;                 //停止标志位和 显示标志位 默认停止 显示打开
int Encoder_Left,Encoder_Right;             //左右编码器的脉冲计数
int Moto1,Moto2;                            //电机PWM变量 应是Motor的 向Moto致敬	
int Temperature=0;                            //显示温度
int Voltage;                                //电池电压采样相关的变量
float Angle_Balance,Gyro_Balance,Gyro_Turn; //平衡倾角 平衡陀螺仪 转向陀螺仪
float Show_Data_Mb;                         //全局显示变量，用于显示需要查看的数据
u32 Distance;                               //超声波测距
u8 delay_50,delay_flag,Bi_zhang=0,PID_Send,Flash_Send; //延时和调参等变量
float Acceleration_Z;                       //Z轴加速度计  
float Balance_Kp=400,Balance_Kd=2.5,Velocity_Kp=160/*10*/,Velocity_Ki=0.4;//PID参数
u16 PID_Parameter[10],Flash_Parameter[10];  //Flash相关数组

//定时器时间计算公式Tout = ((重装载值+1)*(预分频系数+1))/时钟频率;
//例如：1秒定时，重装载值=9999，预分频系数=7199


int main(void)
  {
		  delay_init();	    	            //=====延时函数初始化	
//		uart_init(115200);	            //=====串口初始化为
//		JTAG_Set(JTAG_SWD_DISABLE);     //=====关闭JTAG接口
//		JTAG_Set(SWD_ENABLE);           //=====打开SWD接口 可以利用主板的SWD接口调试
		//LED_Init();                     //=====初始化与 LED 连接的硬件接口
	  //KEY_Init();                     //=====按键初始化
//		MY_NVIC_PriorityGroupConfig(2);	//=====设置中断分组
    MiniBalance_PWM_Init(7199,0);   //=====初始化PWM 10KHZ，用于驱动电机 如需初始化电调接口 
		//uart3_init(9600);               //=====串口3初始化
		USART2_Init(115200);
		//TIM3_Init(499,7199);
    Encoder_Init_TIM2();            //=====编码器接口
    Encoder_Init_TIM4();            //=====初始化编码器2
		Adc_Init();                     //=====adc初始化
    IIC_Init();                     //=====IIC初始化
    MPU6050_initialize();           //=====MPU6050初始化	
    DMP_Init();                     //=====初始化DMP 
    OLED_Init();                    //=====OLED初始化	    
		
		//TIM3_Cap_Init(0XFFFF,72-1);	    //=====超声波初始化
	  MiniBalance_EXTI_Init();        //=====MPU6050 5ms定时中断初始化
    while(1)
	   {
				u8 i=0;
				u16 t[20]={0};
				
			 	t[0] = accel[0]>>8;
				t[1] = accel[0];
				t[2] = accel[1]>>8;
				t[3] = accel[1];
				t[4] = accel[2]>>8;
				t[5] = accel[2];
			  t[6] = gyro[0]>>8;
				t[7] = gyro[0];
				t[8] = gyro[1]>>8;
				t[9] = gyro[1];
				t[10] = gyro[2]>>8;
				t[11] = gyro[2];
				t[12] = Temperature>>8;
				t[13] = Temperature;
				t[14] = Encoder_Left>>8;
				t[15] = Encoder_Left;
				t[16] = Encoder_Right>>8;
				t[17] = Encoder_Right;
				t[18] = Voltage>>8;
				t[19] = Voltage;
				
				for (i=0; i<20; i++)
				{
					USART_SendData(USART2 ,t[i]); //发送单个数值
					while(USART_GetFlagStatus(USART2, USART_FLAG_TC)==RESET); //检查发送中断标志位
				}
				oled_show();          //===显示屏打开
				delay_flag=1;	
				delay_50=0;
				while(delay_flag);	     //通过MPU6050的INT中断实现的50ms精准延时	
				
				
//			 USART_SendData(USART2 ,0X11); //发送单个数值
//			while(USART_GetFlagStatus(USART2, USART_FLAG_TC)==RESET); //检查发送中断标志位
//			 
//		    if(Flag_Show==0)          //使用MiniBalance APP和OLED显示屏
//					{
//							APP_Show();	
//							oled_show();          //===显示屏打开
//					}
//					else                      //使用MiniBalance上位机 上位机使用的时候需要严格的时序，故此时关闭app监控部分和OLED显示屏
//					{
//				      DataScope();          //开启MiniBalance上位机
//					}	
//				  delay_flag=1;	
//					delay_50=0;
//					while(delay_flag);	     //通过MPU6050的INT中断实现的50ms精准延时	
	   } 
}

