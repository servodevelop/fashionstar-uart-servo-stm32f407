#include "fashion_star_uart_servo_examples.h"


int main(void)
{ 
	// ϵͳ��Դ��ʼ�� 
	SysTick_Init(); 					// ϵͳʱ�ӳ�ʼ��
	LED_Init(); 							// LED��ʼ��
	Usart_Init();							// ���ڳ�ʼ��
	
	// ��������ID���в�������
	// �����fashion_star_uart_servo_examples.h����Ķ���
	FSUSExample_Run(FSUS_EXAMPLE_SET_SERVO_ANGLE); // ��������
}


