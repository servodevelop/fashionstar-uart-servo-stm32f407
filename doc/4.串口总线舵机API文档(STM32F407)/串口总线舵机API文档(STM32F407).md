# 串口总线舵机API文档(STM32F407)



[toc]



* 作者: 阿凯爱玩机器人
* Email: kyle.xing@fashionstar.com.hk



## `FSUS_Ping`舵机通讯检测

接下来就是舵机通讯检测函数`FSUS_Ping`，依次传入串口数据结构体指针`servoUsart`，还有舵机的ID号`servoId`。

```c
statusCode = FSUS_Ping(servoUsart, servoId);
```

`statusCode`是返回的状态码`FSUS_STATUS`，如果是请求成功则返回`0`，如果是其他的数值则意味着舵机通讯检测失败。可以在`fashion_star_uart_servo.h` 文件里面查阅不同的statusCode对应的错误。

```c
// FSUS状态码
#define FSUS_STATUS uint8_t
#define FSUS_STATUS_SUCCESS 0 // 设置/读取成功
#define FSUS_STATUS_FAIL 1 // 设置/读取失败
#define FSUS_STATUS_TIMEOUT 2 // 等待超时 
#define FSUS_STATUS_WRONG_RESPONSE_HEADER 3 // 响应头不对
#define FSUS_STATUS_UNKOWN_CMD_ID 4 // 未知的控制指令
#define FSUS_STATUS_SIZE_TOO_BIG 5 // 参数的size大于FSUS_PACK_RESPONSE_MAX_SIZE里面的限制
#define FSUS_STATUS_CHECKSUM_ERROR 6 // 校验和错误
#define FSUS_STATUS_ID_NOT_MATCH 7 // 请求的舵机ID跟反馈回来的舵机ID不匹配
```



## `FSUS_SetServoAngle`舵机角度控制

**函数原型**

```c
FSUS_STATUS FSUS_SetServoAngle(Usart_DataTypeDef *usart, uint8_t servo_id, float angle, uint16_t interval, uint16_t power, uint8_t wait);
```

* `usart` 舵机控制对应的串口数据对象`Usart_DataTypeDef`
* `servo_id` 舵机的ID
* `angle`  舵机的目标角度，取值范围$[-135.0, 135.0]$，精度精确到$0.1°$
* `interval` 舵机角度的旋转周期，单位ms
* `power`  舵机执行功率 mV 默认为0
* `wait`  设置舵机角度的时候，是否为阻塞式 
  * `0`：不阻塞
  * `1`：等待舵机旋转到特定的位置

**使用示例**

```c
//// 舵机控制相关的参数
// 舵机的ID号
uint8_t servoId = 0;  
// 舵机的目标角度
// 舵机角度在-135度到135度之间, 精确到小数点后一位
float angle = 0; 
// 时间间隔ms  
// 可以尝试修改设置更小的时间间隔，例如500ms
uint16_t interval = 2000; 
// 舵机执行功率 mV 默认为0	
uint16_t power = 0;
// 设置舵机角度的时候, 是否为阻塞式 
// 0:不等待 1:等待舵机旋转到特定的位置; 
uint8_t wait = 1; 

FSUS_SetServoAngle(servoUsart, servoId, angle, interval, power, wait);
```



## `FSUS_SetServoAngleByInterval`舵机角度控制(指定周期)

**函数原型**

```c
/* 设置舵机的角度(指定周期) */
FSUS_STATUS FSUS_SetServoAngleByInterval(Usart_DataTypeDef *usart, uint8_t servo_id, \
				float angle, uint16_t interval, uint16_t t_acc, \
				uint16_t t_dec, uint16_t  power, uint8_t wait);
```



* `usart` 舵机控制对应的串口数据对象`Usart_DataTypeDef`
* `servo_id` 舵机的ID
* `angle`  舵机的目标角度，取值范围$[-135.0, 135.0]$，精度精确到$0.1°$
* `interval` 舵机角度的旋转周期，单位ms
* `t_acc` 加速时间, 单位ms
* `t_dec`减速时间,单位ms
* `power`  舵机执行功率 mV 默认为0
* `wait`  设置舵机角度的时候，是否为阻塞式 
  * `0`：不阻塞
  * `1`：等待舵机旋转到特定的位置



## `FSUS_SetServoAngleByVelocity` 舵机角度控制(指定转速)

**函数原型**

```c
/* 设置舵机的角度(指定转速) */
FSUS_STATUS FSUS_SetServoAngleByVelocity(Usart_DataTypeDef *usart, uint8_t servo_id, \
				float angle, float velocity, uint16_t t_acc, \
				uint16_t t_dec, uint16_t  power, uint8_t wait);
```

* `usart` 舵机控制对应的串口数据对象`Usart_DataTypeDef`
* `servo_id` 舵机的ID
* `angle`  舵机的目标角度，取值范围$[-135.0, 135.0]$，精度精确到$0.1°$
* `velocity` 舵机目标转速，单位°/s
* `t_acc` 加速时间, 单位ms
* `t_dec`减速时间,单位ms
* `power`  舵机执行功率 mV 默认为0
* `wait`  设置舵机角度的时候，是否为阻塞式 
  * `0`：不阻塞
  * `1`：等待舵机旋转到特定的位置



## `FSUS_QueryServoAngle`舵机角度查询

**函数原型**

```c
// 查询单个舵机的角度信息 angle 单位度
FSUS_STATUS FSUS_QueryServoAngle(Usart_DataTypeDef *usart, uint8_t servo_id, float *angle);
```

* `usart` 舵机控制对应的串口数据对象`Usart_DataTypeDef`
* `servo_id` 舵机的ID
* `angle`舵机当前的角度，取值范围$[-135.0, 135.0]$，精度精确到$0.1°$



**使用示例**

```c
uint8_t servoId = 0;   	// 舵机的ID号
float curAngle = 0;		// 舵机当前所在的角度
FSUS_QueryServoAngle(servoUsart, servoId, &curAngle); // 读取一下舵机的角度
```



## `FSUS_SetServoAngleMTurn`舵机角度控制(多圈模式)

**函数原型**

```c
/* 设置舵机的角度(多圈模式) */
FSUS_STATUS FSUS_SetServoAngleMTurn(Usart_DataTypeDef *usart, uint8_t servo_id, float angle, \
			uint32_t interval, uint16_t power, uint8_t wait);
```

* `usart` 舵机控制对应的串口数据对象`Usart_DataTypeDef`
* `servo_id` 舵机的ID
* `angle`  舵机的目标角度，取值范围$[-135.0, 135.0]$，精度精确到$0.1°$
* `interval` 舵机角度的旋转周期，单位ms
* `power`  舵机执行功率 mV 默认为0
* `wait`  设置舵机角度的时候，是否为阻塞式 
  * `0`：不阻塞
  * `1`：等待舵机旋转到特定的位置



## `FSUS_SetServoAngleMTurnByInterval`舵机角度控制(多圈模式+指定周期)

**函数原型**

```c
/* 设置舵机的角度(多圈模式, 指定周期) */
FSUS_STATUS FSUS_SetServoAngleMTurnByInterval(Usart_DataTypeDef *usart, uint8_t servo_id, float angle, \
			uint32_t interval,  uint16_t t_acc,  uint16_t t_dec, uint16_t power, uint8_t wait);
```

* `usart` 舵机控制对应的串口数据对象`Usart_DataTypeDef`
* `servo_id` 舵机的ID
* `angle`  舵机的目标角度，取值范围$[-135.0, 135.0]$，精度精确到$0.1°$
* `interval` 舵机角度的旋转周期，单位ms
* `t_acc` 加速时间, 单位ms
* `t_dec`减速时间,单位ms
* `power`  舵机执行功率 mV 默认为0
* `wait`  设置舵机角度的时候，是否为阻塞式 
  * `0`：不阻塞
  * `1`：等待舵机旋转到特定的位置



## `FSUS_SetServoAngleMTurnByVelocity`舵机角度控制(多圈模式+指定转速)

**函数原型**

```c
/* 设置舵机的角度(多圈模式, 指定转速) */
FSUS_STATUS FSUS_SetServoAngleMTurnByVelocity(Usart_DataTypeDef *usart, uint8_t servo_id, float angle, \
			float velocity, uint16_t t_acc,  uint16_t t_dec, uint16_t power, uint8_t wait);
```

* `usart` 舵机控制对应的串口数据对象`Usart_DataTypeDef`
* `servo_id` 舵机的ID
* `angle`  舵机的目标角度，取值范围$[-135.0, 135.0]$，精度精确到$0.1°$
* `velocity` 舵机目标转速，单位°/s
* `t_acc` 加速时间, 单位ms
* `t_dec`减速时间,单位ms
* `power`  舵机执行功率 mV 默认为0
* `wait`  设置舵机角度的时候，是否为阻塞式 
  * `0`：不阻塞
  * `1`：等待舵机旋转到特定的位置

## `FSUS_QueryServoAngleMTurn`舵机角度查询(多圈模式)

**函数原型**

```c
/* 查询舵机的角度(多圈模式) */
FSUS_STATUS FSUS_QueryServoAngleMTurn(Usart_DataTypeDef *usart, uint8_t servo_id, float *angle);
```



* `usart` 舵机控制对应的串口数据对象`Usart_DataTypeDef`
* `servo_id` 舵机的ID
* `angle`舵机当前的角度，精度精确到$0.1°$



## `FSUS_DampingMode`设置阻尼模式并设置功率

```c
// 舵机阻尼模式
FSUS_STATUS FSUS_DampingMode(Usart_DataTypeDef *usart, uint8_t servoId, uint16_t power);
```

* `usart` 舵机控制对应的串口数据对象`Usart_DataTypeDef`
* `servoId` 舵机的ID
* `power` 舵机的功率，单位mW

**使用示例**

```c
// 连接在转接板上的串口舵机ID号
uint8_t servoId = 0; 
// 阻尼模式下的功率，功率越大阻力越大
uint16_t power = 500;
// 设置舵机为阻尼模式
FSUS_DampingMode(servoUsart, servoId, power);
```



## `FSUS_WheelKeepMove`轮子定速持续旋转

```c
// 轮式模式 不停的旋转
FSUS_STATUS FSUS_WheelKeepMove(Usart_DataTypeDef *usart, uint8_t servoId, uint8_t is_cw, uint16_t speed);
```

* `usart` 舵机控制对应的串口数据对象`Usart_DataTypeDef`
* `servoId` 舵机的ID
* `is_cw` 轮子是否是顺时针旋转
* `0`：否，轮子逆时针旋转

* `1`：是，轮子顺时针旋转
* `speed` 舵机旋转的速度  单位 $°/s$

**使用示例**

```c
uint8_t servoId = 0; 	// 连接在转接板上的串口舵机ID号
uint16_t speed = 20; 	// 舵机的旋转方向 20°/s
uint8_t is_cw = 0; 		// 舵机的旋转方向
FSUS_WheelKeepMove(servoUsart, servoId, is_cw, speed);
```



## `FSUS_WheelStop`轮子停止旋转

```c
// 轮式模式, 舵机停止转动
FSUS_STATUS FSUS_WheelStop(Usart_DataTypeDef *usart, uint8_t servoId);
```

* `usart` 舵机控制对应的串口数据对象`Usart_DataTypeDef`
* `servoId` 舵机的ID

**使用示例**

```c
uint8_t servoId = 0; 	// 连接在转接板上的串口舵机ID号
FSUS_WheelStop(servoUsart, servoId); // 舵机刹车
```



## `FSUS_WheelMoveTime`轮子旋转特定的时间

```c
// 轮式模式 按照特定的速度旋转特定的时间
FSUS_STATUS FSUS_WheelMoveTime(Usart_DataTypeDef *usart, uint8_t servoId, uint8_t is_cw, uint16_t speed, uint16_t nTime);
```

* `usart` 舵机控制对应的串口数据对象`Usart_DataTypeDef`
* `servoId` 舵机的ID
* `is_cw` 轮子是否是顺时针旋转
  * `0`：否，轮子逆时针旋转
  * `1`：是，轮子顺时针旋转
* `speed` 舵机旋转的速度  单位 $°/s$
* `nTime` 舵机持续旋转的时间，单位ms。

**使用示例**

```c
uint8_t servoId = 0; 	// 连接在转接板上的串口舵机ID号
uint16_t speed = 20; 	// 舵机的旋转方向 20°/s
uint8_t is_cw = 0; 		// 舵机的旋转方向
uint16_t nTime = 3000; 	// 延时时间
FSUS_WheelMoveTime(servoUsart, servoId, is_cw, speed, nTime);
```



## `FSUS_WheelMoveNCircle`轮子旋转特定的圈数

```c
// 轮式模式 旋转特定的圈数
FSUS_STATUS FSUS_WheelMoveNCircle(Usart_DataTypeDef *usart, uint8_t servoId, uint8_t is_cw, uint16_t speed, uint16_t nCircle);
```

* `usart` 舵机控制对应的串口数据对象`Usart_DataTypeDef`
* `servoId` 舵机的ID
* `is_cw` 轮子是否是顺时针旋转
  * `0`：否，轮子逆时针旋转
  * `1`：是，轮子顺时针旋转
* `speed` 舵机旋转的速度  单位 $°/s$
* `nCircle` 舵机持续旋转的圈数，单位：圈

**使用示例**

```c
uint8_t servoId = 0; 	// 连接在转接板上的串口舵机ID号
uint16_t speed = 200; 	// 舵机的旋转方向 单位°/s
uint8_t is_cw = 0; 		// 舵机的旋转方向
uint16_t nCircle = 1; 	// 舵机旋转的圈数
FSUS_WheelMoveNCircle(servoUsart, servoId, is_cw, speed, nCircle);
```



## `FSUS_ResetUserData`重置用户自定义数据

```c
// 重置舵机的用户资料
FSUS_STATUS FSUS_ResetUserData(Usart_DataTypeDef *usart, uint8_t servoId);
```

* `usart` 舵机控制对应的串口数据对象`Usart_DataTypeDef`
* `servoId` 舵机的ID

**使用示例 **

```c
uint8_t servoId = 0;  		// 连接在转接板上的串口舵机ID号
FSUS_ResetUserData(servoUsart, servoId);
```



## `FSUS_ResetUserData`读取用户自定义数

```c
// 读取数据
FSUS_STATUS FSUS_ReadData(Usart_DataTypeDef *usart, uint8_t servoId,  uint8_t address, uint8_t *value, uint8_t *size);
```

* `usart` 舵机控制对应的串口数据对象`Usart_DataTypeDef`
* `servoId` 舵机的ID
* `address` 用户自定义数据的ID号
* `value` 读取到的数值
* `size` 读取到的数值的长度

**使用示例**

```c
// 读取用户自定义数据
// 数据表里面的数据字节长度一般为1个字节/2个字节/4个字节
// 查阅通信协议可知,舵机角度上限的数据类型是有符号短整型(UShort, 对应STM32里面的int16_t),长度为2个字节
// 所以这里设置value的数据类型为int16_t
int16_t value; 
uint8_t dataSize; 
// 传参数的时候, 要将value的指针强行转换为uint8_t
FSUS_ReadData(servoUsart, servoId, FSUS_PARAM_ANGLE_LIMIT_HIGH, (uint8_t *)&value, &dataSize);
```



## `FSUS_WriteData`写入用户自定义数据

```c
// 写入数据
FSUS_STATUS FSUS_WriteData(Usart_DataTypeDef *usart, uint8_t servoId, uint8_t address, uint8_t *value, uint8_t size);
```

* `usart` 舵机控制对应的串口数据对象`Usart_DataTypeDef`
* `servoId` 舵机的ID
* `address`  地址位
* `value` 写入的数据
* `size` 写入的数据的长度

**使用示例**

```c
uint8_t servoId = 0;  		// 连接在转接板上的串口舵机ID号
float angleLimitLow = -90.0; 	// 舵机角度下限 (默认值-135)
value = (int16_t)(angleLimitLow*10); // 舵机角度下限 转换单位为0.1度
statusCode = FSUS_WriteData(servoUsart, servoId, FSUS_PARAM_ANGLE_LIMIT_LOW, (uint8_t *)&value, 2);
```

