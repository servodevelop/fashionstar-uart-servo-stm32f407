/*
 * 串口总线舵机示例代码
 * ---------------------------------------------------
 * 作者: 阿凯爱玩机器人
 * Email: kyle.xing@fashionstar.com.hk
 * 更新时间: 2021-03-16
 * ---------------------------------------------------
 */

#include "fashion_star_uart_servo_examples.h"

Usart_DataTypeDef *servo_usart = &usart2; // 串口总线舵机对应的USART

/* 根据例程编号执行测试例程 */
void FSUSExample_Run(uint8_t example_id)
{

	switch (example_id)
	{
	case FSUS_EXAMPLE_LED_BLINK:
		FSUSExample_LEDBlink();
		break;
	case FSUS_EXAMPLE_USART_ECHO:
		FSUSExample_UsartEcho();
		break;
	case FSUS_EXAMPLE_PING_SERVO:
		FSUSExample_PingServo();
		break;
	case FSUS_EXAMPLE_SET_SERVO_ANGLE:
		FSUSExample_SetServoAngle();
		break;
	case FSUS_EXAMPLE_SET_N_SERVO_ANGLE:
		FSUSExample_SetNServoAngle();
		break;
	case FSUS_EXAMPLE_SET_SERVO_ANGLE_MTURN:
		FSUSExample_SetServoAngleMTurn();
		break;
	case FSUS_EXAMPLE_SET_SERVO_DAMPING:
		FSUSExample_SetServoDamping();
		break;
	case FSUS_EXAMPLE_SET_WHEEL_KEEP_MOVE:
		FSUSExample_WheelKeepMove();
		break;
	case FSUS_EXAMPLE_SET_WHEEL_MOVE_NCIRCLE:
		FSUSExample_WheelMoveNCircle();
		break;
	case FSUS_EXAMPLE_SET_WHEEL_MOVE_TIME:
		FSUSExample_WheelMoveNTime();
		break;
	case FSUS_EXAMPLE_RESET_DATA:
		FSUSExample_ResetData();
		break;
	case FSUS_EXAMPLE_READ_DATA:
		FSUSExample_ReadData();
		break;
	case FSUS_EXAMPLE_WRITE_DATA:
		FSUSExample_WriteData();
		break;
	default:
		FSUSExample_LEDBlink();
		break;
	}
}

/* LED闪烁例程 */
void FSUSExample_LEDBlink(void)
{

	printf("===Test LED Blink===\r\n");
	// 重复执行 1s切换一下LED0的电平状态
	while (1)
	{
		SysTick_DelayMs(1000); // 延时1s
		LED0 = !LED0;		   // LED电平反转
	}
}

/* 
 * USART收发测试
 * 功能说明: 如果有数据接收到，则打印回终端
 */
void FSUSExample_UsartEcho(void)
{
	// 变量定义
	uint16_t nbyte;					// 记录字节个数
	uint8_t usart_echo_buffer[256]; // 接收缓冲区
	uint16_t count = 0;				// 计数

	printf("===Test Usart Echo===\r\n");
	while (1)
	{
		count += 1;
		if (count >= 100)
		{
			LED0 = 0; // LED开
			printf("Input Data\r\n");
			LED0 = 1; // LED关
			count = 0;
		}
		nbyte = RingBuffer_GetByteUsed(usart1.recvBuf); // 获取当前读入的字节个数
		if (nbyte != 0)
		{
			LED0 = 0; // LED开
			printf("Data Recv %d  Bytes\r\n", nbyte);
			printf("ECHO: ");
			RingBuffer_ReadByteArray(usart1.recvBuf, usart_echo_buffer, nbyte);
			Usart_SendByteArr(usart1.pUSARTx, usart_echo_buffer, nbyte);
			printf("\r\n");
			LED0 = 1; // LED关
		}
		SysTick_DelayMs(10); // 延时10ms
	}
}

/* 舵机通讯检测 */
void FSUSExample_PingServo(void)
{

	FSUS_STATUS status_code; // 状态码
	uint8_t servo_id = 0;	 // 舵机ID = 0

	printf("===Test Uart Servo Ping===r\n");
	while (1)
	{
		// 舵机通信检测
		status_code = FSUS_Ping(servo_usart, servo_id);
		if (status_code == FSUS_STATUS_SUCCESS)
		{
			// 舵机在线， LED1闪烁(绿灯)
			printf("Servo Online \r\n");
			LED0 = LED_OFF;
			LED1 = !LED1;
		}
		else
		{
			// 舵机离线， LED0闪烁(红灯)
			printf("Servo Offline,Error Code=%d \r\n", status_code);
			LED0 = !LED0;
			LED1 = LED_OFF;
		}
		// 延时等待1s
		SysTick_DelayMs(1000);
	}
}

/* 控制单个舵机的角度 */
void FSUSExample_SetServoAngle(void)
{
	// 舵机控制相关的参数
	// 舵机的ID号
	uint8_t servo_id = 0;
	// 舵机的目标角度
	// 舵机角度在-135度到135度之间, 精确到小数点后一位
	float angle = 0;
	// 时间间隔ms
	// 可以尝试修改设置更小的时间间隔，例如500ms
	uint16_t interval;
	// 目标转速
	float velocity;
	// 加速时间
	uint16_t t_acc;
	// 减速时间
	uint16_t t_dec;
	// 舵机执行功率 mV 默认为0
	uint16_t power = 0;
	// 设置舵机角度的时候, 是否为阻塞式
	// 0:不等待 1:等待舵机旋转到特定的位置;
	uint8_t wait = 1;
	// 读取的角度
	float angle_read;

	while (1)
	{
		printf("GOTO: 90.0f\r\n");
		// 控制舵机角度
		angle = 90.0;
		interval = 2000;
		FSUS_SetServoAngle(servo_usart, servo_id, angle, interval, power, wait);
		FSUS_QueryServoAngle(servo_usart, servo_id, &angle_read);
		printf("Cur Angle: %.1f\r\n", angle_read);

		// 等待2s
		SysTick_DelayMs(2000);

		// 控制舵机角度 + 指定时间
		printf("GOTO+Interval: 0.0f\r\n");
		angle = 0.0f;
		interval = 1000;
		t_acc = 100;
		t_dec = 150;
		FSUS_SetServoAngleByInterval(servo_usart, servo_id, angle, interval, t_acc, t_dec, power, wait);
		FSUS_QueryServoAngle(servo_usart, servo_id, &angle_read);
		printf("Cur Angle: %.1f\r\n", angle_read);

		// 等待2s
		SysTick_DelayMs(2000);

		// 控制舵机角度 + 指定转速
		printf("GOTO+Velocity: -9.0f\r\n");
		angle = -90.0f;
		velocity = 200.0f;
		t_acc = 100;
		t_dec = 150;
		FSUS_SetServoAngleByVelocity(servo_usart, servo_id, angle, velocity, t_acc, t_dec, power, wait);
		FSUS_QueryServoAngle(servo_usart, servo_id, &angle_read);
		printf("Cur Angle: %.1f\r\n", angle_read);
	}
}

/* 控制多个舵机的角度 */
void FSUSExample_SetNServoAngle(void)
{
	//// 舵机控制相关的参数
	// 时间间隔ms
	// 可以尝试修改设置更小的时间间隔，例如500ms
	uint16_t interval = 2000;
	// 舵机执行功率 mV 默认为0
	uint16_t power = 0;
	// 设置舵机角度的时候, 是否为阻塞式
	// 0:不等待 1:等待舵机旋转到特定的位置;
	uint8_t wait = 0;
	// 是否为多圈模式
	// 0: 单圈模式; 1: 多圈模式;
	uint8_t is_mturn = 0;
	while (1)
	{
		// 控制舵机云台角度
		FSUS_SetServoAngle(servo_usart, 0, 90.0, interval, power, wait);
		FSUS_SetServoAngle(servo_usart, 1, 45.0, interval, power, wait);
		// 阻塞式等待, 等待旋转到目标角度
		// 注意要跟设定值相同
		FSUS_Wait(servo_usart, 0, 90.0, is_mturn);
		FSUS_Wait(servo_usart, 1, 45.0, is_mturn);

		// 等待2s
		SysTick_DelayMs(2000);

		// 控制舵机旋转到另外一个角度
		FSUS_SetServoAngle(servo_usart, 0, -90.0, interval, power, wait);
		FSUS_SetServoAngle(servo_usart, 1, -45.0, interval, power, wait);
		// 阻塞式等待, 等待旋转到目标角度
		// 注意要跟设定值相同
		FSUS_Wait(servo_usart, 0, -90.0, is_mturn);
		FSUS_Wait(servo_usart, 1, -45.0, is_mturn);

		// 等待2s
		SysTick_DelayMs(2000);
	}
}

/* 设置舵机角度(多圈模式) */
void FSUSExample_SetServoAngleMTurn(void)
{
	//// 舵机控制相关的参数
	// 舵机的ID号
	uint8_t servo_id = 0;
	// 舵机的目标角度
	// 舵机角度在-135度到135度之间, 精确到小数点后一位
	float angle;
	uint32_t interval; // 时间间隔ms
	float velocity;	   // 电机转速, 单位dps,°/s
	// 舵机执行功率 mV 默认为0
	uint16_t power = 0;
	// 设置舵机角度的时候, 是否为阻塞式
	// 0:不等待 1:等待舵机旋转到特定的位置;
	uint8_t wait = 1;
	// 加速时间(单位ms)
	uint16_t t_acc;
	// 减速时间
	uint16_t t_dec;
	// 读取的角度
	float angle_read;

	while (1)
	{
		printf("MTurn GOTO: 720.0f\r\n");
		// 控制舵机角度(多圈)
		angle = 720.0f;
		interval = 2000;
		FSUS_SetServoAngleMTurn(servo_usart, servo_id, angle, interval, power, wait);
		FSUS_QueryServoAngleMTurn(servo_usart, servo_id, &angle_read);
		printf("Cur Angle: %.1f\r\n", angle_read);

		// 等待2s
		SysTick_DelayMs(2000);

		// 控制舵机旋转到另外一个角度(多圈)
		printf("MTurn GOTO: 0.0f\r\n");
		angle = 0.0;
		FSUS_SetServoAngleMTurn(servo_usart, servo_id, angle, interval, power, wait);
		FSUS_QueryServoAngleMTurn(servo_usart, servo_id, &angle_read);
		printf("Cur Angle: %.1f\r\n", angle_read);

		// 等待2s
		SysTick_DelayMs(2000);

		// 控制舵机角度(多圈+指定周期)
		printf("MTurn+Interval GOTO: -180.0f\r\n");
		angle = 180.0f;
		interval = 1000;
		t_acc = 100;
		t_dec = 200;
		FSUS_SetServoAngleMTurnByInterval(servo_usart, servo_id, angle, interval, t_acc, t_dec, power, wait);
		FSUS_QueryServoAngleMTurn(servo_usart, servo_id, &angle_read);
		printf("Cur Angle: %.1f\r\n", angle_read);

		// 等待2s
		SysTick_DelayMs(2000);

		// 控制舵机角度(多圈+指定转速)
		printf("MTurn+Velocity GOTO: -180.0f\r\n");
		angle = -180.0f;
		velocity = 100.0f;
		t_acc = 100;
		t_dec = 200;
		FSUS_SetServoAngleMTurnByVelocity(servo_usart, servo_id, angle, velocity, t_acc, t_dec, power, wait);
		FSUS_QueryServoAngleMTurn(servo_usart, servo_id, &angle_read);
		printf("Cur Angle: %.1f\r\n", angle_read);

		// 等待2s
		SysTick_DelayMs(2000);
	}
}

/* 舵机阻尼模式与角度回传 */
void FSUSExample_SetServoDamping(void)
{
	FSUS_STATUS status_code; // 请求包的状态码
	uint8_t servo_id = 0;	 // 连接在转接板上的串口舵机ID号
	uint16_t power = 500;	 // 阻尼模式下的功率，功率越大阻力越大
	float angle = 0;		 // 舵机的角度

	// 设置舵机为阻尼模式
	FSUS_DampingMode(servo_usart, servo_id, power);
	while (1)
	{
		// 读取一下舵机的角度
		status_code = FSUS_QueryServoAngle(servo_usart, servo_id, &angle);

		if (status_code == FSUS_STATUS_SUCCESS)
		{
			// 成功的读取到了舵机的角度
			printf("[INFO] servo id= %d ; angle = %f\r\n", servo_id, angle);
		}
		else
		{
			// 没有正确的读取到舵机的角度
			printf("\r\n[INFO] read servo %d angle, status code: %d \r\n", servo_id, status_code);
			printf("[ERROR]failed to read servo angle\r\n");
		}
		// 等待1000ms
		SysTick_DelayMs(500);
	}
}

/* 轮式模式控制(一直旋转) */
void FSUSExample_WheelKeepMove(void)
{
	uint8_t servo_id = 0; // 连接在转接板上的串口舵机ID号
	uint16_t speed = 20;  // 舵机的旋转方向 20°/s
	uint8_t is_cw = 0;	  // 舵机的旋转方向

	while (1)
	{
		// 舵机轮式模式定速控制 顺时针旋转3s
		is_cw = 1;
		FSUS_WheelKeepMove(servo_usart, servo_id, is_cw, speed);
		SysTick_DelayMs(3000);

		// 舵机刹车 停顿2s
		FSUS_WheelStop(servo_usart, servo_id);
		SysTick_DelayMs(1000);

		// 舵机轮式模式定速控制 逆时针旋转3s
		is_cw = 0;
		FSUS_WheelKeepMove(servo_usart, servo_id, is_cw, speed);
		SysTick_DelayMs(3000);

		// 舵机刹车 停顿2s
		FSUS_WheelStop(servo_usart, servo_id);
		SysTick_DelayMs(1000);
	}
}

// 估计旋转圈数所需要花费的时间
uint16_t FSUSExample_EstimateTimeMs(uint16_t n_circle, uint16_t speed)
{
	return (uint16_t)((float)n_circle * 360.0f / (float)speed * 1000.0f);
}

/* 轮式模式控制(定圈) */
void FSUSExample_WheelMoveNCircle(void)
{
	uint8_t servo_id = 0;  // 连接在转接板上的串口舵机ID号
	uint16_t speed = 200;  // 舵机的旋转方向 单位°/s
	uint8_t is_cw = 0;	   // 舵机的旋转方向
	uint16_t n_circle = 1; // 舵机旋转的圈数

	while (1)
	{
		// 舵机轮式模式定速控制 顺时针旋转1圈
		is_cw = 1;
		FSUS_WheelMoveNCircle(servo_usart, servo_id, is_cw, speed, n_circle);
		// FSUS_WheelMoveNCircle是非阻塞的,因为有时候需要控制多个舵机同时旋转
		// 延时估算所需时间
		SysTick_DelayMs(FSUSExample_EstimateTimeMs(n_circle, speed));

		// 停顿1s
		SysTick_DelayMs(1000);

		// 舵机轮式模式定速控制 逆时针旋转1圈
		is_cw = 0;
		FSUS_WheelMoveNCircle(servo_usart, servo_id, is_cw, speed, n_circle);
		// 注意: FSUS_WheelMoveNCircle是非阻塞的,因为有时候需要控制多个舵机同时旋转
		// 延时估算所需时间
		SysTick_DelayMs(FSUSExample_EstimateTimeMs(n_circle, speed));

		// 停顿1s
		SysTick_DelayMs(1000);
	}
}

/* 轮式控制模式(定时) */
void FSUSExample_WheelMoveNTime(void)
{
	uint8_t servo_id = 0;	// 连接在转接板上的串口舵机ID号
	uint16_t speed = 20;	// 舵机的旋转方向 20°/s
	uint8_t is_cw = 0;		// 舵机的旋转方向
	uint16_t n_time = 3000; // 延时时间

	while (1)
	{
		// 舵机轮式模式定速控制 顺时针旋转3s
		is_cw = 1;
		FSUS_WheelMoveTime(servo_usart, servo_id, is_cw, speed, n_time);
		// FSUS_WheelMoveTime是非阻塞的,因为有时候需要控制多个舵机同时旋转
		// 所以在后面要手动加延迟
		SysTick_DelayMs(n_time);

		// 停顿1s
		SysTick_DelayMs(1000);

		// 舵机轮式模式定速控制 逆时针旋转3s
		is_cw = 0;
		FSUS_WheelMoveTime(servo_usart, servo_id, is_cw, speed, n_time);
		SysTick_DelayMs(n_time);

		// 停顿1s
		SysTick_DelayMs(1000);
	}
}

/* 用户自定义数据-重置 */
void FSUSExample_ResetData(void)
{

	// 连接在转接板上的串口舵机ID号
	uint8_t servo_id = 0;
	// 状态码
	FSUS_STATUS status_code;

	// 发送重置用户数据指令
	status_code = FSUS_ResetUserData(servo_usart, servo_id);
	printf("=====reset user data======\r\n status code: %d\r\n", status_code);
	if (status_code == FSUS_STATUS_SUCCESS)
	{
		printf("sucess\r\n");
	}
	else
	{
		printf("fail\r\n");
	}

	// 死循环
	while (1)
	{
	}
}

/* 用户自定义数据-读取 */
void FSUSExample_ReadData(void)
{
	uint8_t servo_id = 0;	  // 连接在转接板上的串口舵机ID号
	FSUS_STATUS statusCode;	  // 状态码
	float angleLimitHigh = 0; // 舵机角度上限

	// 读取用户自定义数据
	// 数据表里面的数据字节长度一般为1个字节/2个字节/4个字节
	// 查阅通信协议可知,舵机角度上限的数据类型是有符号短整型(UShort, 对应STM32里面的int16_t),长度为2个字节
	// 所以这里设置value的数据类型为int16_t
	int16_t value;
	uint8_t dataSize;
	// 传参数的时候, 要将value的指针强行转换为uint8_t
	statusCode = FSUS_ReadData(servo_usart, servo_id, FSUS_PARAM_ANGLE_LIMIT_HIGH, (uint8_t *)&value, &dataSize);
	printf("=====read user data======\r\n status code: %d\r\n", statusCode);
	if (statusCode == FSUS_STATUS_SUCCESS)
	{
		// 用户自定义表里面，角度的单位是0.1度. 需要改写为度
		angleLimitHigh = value / 10.0;
		printf("read sucess, angle limit high: %f\r\n", angleLimitHigh);
	}
	else
	{
		printf("fail\r\n");
	}

	// 死循环
	while (1)
	{
	}
}

/* 用户自定义数据-写入 */
void FSUSExample_WriteData(void)
{
	uint8_t servo_id = 0;	 // 连接在转接板上的串口舵机ID号
	FSUS_STATUS status_code; // 状态码
	// 限位角度
	float angle_limit_high = 90.0;	   // 舵机角度上限 (默认值 135)
	float angle_limit_low = -90.0;	   // 舵机角度下限 (默认值-135)
	uint8_t angle_limit_switch = 0x01; // 0x01: 开启限位; 0x00: 关闭限位

	uint16_t value;

	// 写入舵机角度上限
	value = (int16_t)(angle_limit_high * 10); // 舵机角度上限 转换单位为0.1度
	status_code = FSUS_WriteData(servo_usart, servo_id, FSUS_PARAM_ANGLE_LIMIT_HIGH, (uint8_t *)&value, 2);
	printf("write angle limit high = %f, status code: %d\r\n", angle_limit_high, status_code);

	// 写入舵机角度下限制
	value = (int16_t)(angle_limit_low * 10); // 舵机角度下限 转换单位为0.1度
	status_code = FSUS_WriteData(servo_usart, servo_id, FSUS_PARAM_ANGLE_LIMIT_LOW, (uint8_t *)&value, 2);
	printf("write angle limit low = %f, status code: %d\r\n", angle_limit_low, status_code);

	// 打开舵机角度限位开关, 配置生效
	status_code = FSUS_WriteData(servo_usart, servo_id, FSUS_PARAM_ANGLE_LIMIT_LOW, &angle_limit_switch, 1);
	printf("enable angle limit mode, status code: %d\r\n", status_code);

	while (1)
	{
		// 控制舵机角度
		FSUS_SetServoAngle(servo_usart, servo_id, 90.0, 2000, 0, 1);
		FSUS_SetServoAngle(servo_usart, servo_id, -90.0, 2000, 0, 1);
	}
}
