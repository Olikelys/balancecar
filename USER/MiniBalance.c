#include "stm32f10x.h"
#include "sys.h"
  /**************************************************************************
���ߣ�ƽ��С��֮��
�ҵ��Ա�С�꣺http://shop114407458.taobao.com/
**************************************************************************/
u8 Way_Angle=1;                             //��ȡ�Ƕȵ��㷨��1����Ԫ��  2��������  3�������˲� 
u8 Flag_Qian,Flag_Hou,Flag_Left,Flag_Right,Flag_sudu=2; //����ң����صı���
u8 Flag_Stop=1,Flag_Show=0;                 //ֹͣ��־λ�� ��ʾ��־λ Ĭ��ֹͣ ��ʾ��
int Encoder_Left,Encoder_Right;             //���ұ��������������
int Moto1,Moto2;                            //���PWM���� Ӧ��Motor�� ��Moto�¾�	
int Temperature=0;                            //��ʾ�¶�
int Voltage;                                //��ص�ѹ������صı���
float Angle_Balance,Gyro_Balance,Gyro_Turn; //ƽ����� ƽ�������� ת��������
float Show_Data_Mb;                         //ȫ����ʾ������������ʾ��Ҫ�鿴������
u32 Distance;                               //���������
u8 delay_50,delay_flag,Bi_zhang=0,PID_Send,Flash_Send; //��ʱ�͵��εȱ���
float Acceleration_Z;                       //Z����ٶȼ�  
float Balance_Kp=400,Balance_Kd=2.5,Velocity_Kp=160/*10*/,Velocity_Ki=0.4;//PID����
u16 PID_Parameter[10],Flash_Parameter[10];  //Flash�������

//��ʱ��ʱ����㹫ʽTout = ((��װ��ֵ+1)*(Ԥ��Ƶϵ��+1))/ʱ��Ƶ��;
//���磺1�붨ʱ����װ��ֵ=9999��Ԥ��Ƶϵ��=7199


int main(void)
  {
		  delay_init();	    	            //=====��ʱ������ʼ��	
//		uart_init(115200);	            //=====���ڳ�ʼ��Ϊ
//		JTAG_Set(JTAG_SWD_DISABLE);     //=====�ر�JTAG�ӿ�
//		JTAG_Set(SWD_ENABLE);           //=====��SWD�ӿ� �������������SWD�ӿڵ���
		//LED_Init();                     //=====��ʼ���� LED ���ӵ�Ӳ���ӿ�
	  //KEY_Init();                     //=====������ʼ��
//		MY_NVIC_PriorityGroupConfig(2);	//=====�����жϷ���
    MiniBalance_PWM_Init(7199,0);   //=====��ʼ��PWM 10KHZ������������� �����ʼ������ӿ� 
		//uart3_init(9600);               //=====����3��ʼ��
		USART2_Init(115200);
		//TIM3_Init(499,7199);
    Encoder_Init_TIM2();            //=====�������ӿ�
    Encoder_Init_TIM4();            //=====��ʼ��������2
		Adc_Init();                     //=====adc��ʼ��
    IIC_Init();                     //=====IIC��ʼ��
    MPU6050_initialize();           //=====MPU6050��ʼ��	
    DMP_Init();                     //=====��ʼ��DMP 
    OLED_Init();                    //=====OLED��ʼ��	    
		
		//TIM3_Cap_Init(0XFFFF,72-1);	    //=====��������ʼ��
	  MiniBalance_EXTI_Init();        //=====MPU6050 5ms��ʱ�жϳ�ʼ��
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
					USART_SendData(USART2 ,t[i]); //���͵�����ֵ
					while(USART_GetFlagStatus(USART2, USART_FLAG_TC)==RESET); //��鷢���жϱ�־λ
				}
				oled_show();          //===��ʾ����
				delay_flag=1;	
				delay_50=0;
				while(delay_flag);	     //ͨ��MPU6050��INT�ж�ʵ�ֵ�50ms��׼��ʱ	
				
				
//			 USART_SendData(USART2 ,0X11); //���͵�����ֵ
//			while(USART_GetFlagStatus(USART2, USART_FLAG_TC)==RESET); //��鷢���жϱ�־λ
//			 
//		    if(Flag_Show==0)          //ʹ��MiniBalance APP��OLED��ʾ��
//					{
//							APP_Show();	
//							oled_show();          //===��ʾ����
//					}
//					else                      //ʹ��MiniBalance��λ�� ��λ��ʹ�õ�ʱ����Ҫ�ϸ��ʱ�򣬹ʴ�ʱ�ر�app��ز��ֺ�OLED��ʾ��
//					{
//				      DataScope();          //����MiniBalance��λ��
//					}	
//				  delay_flag=1;	
//					delay_50=0;
//					while(delay_flag);	     //ͨ��MPU6050��INT�ж�ʵ�ֵ�50ms��׼��ʱ	
	   } 
}

