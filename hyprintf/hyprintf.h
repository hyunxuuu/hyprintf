#ifndef __HYPRINTF_H
#define __HYPRINTF_H

// 用于无符号整型的定义
#include "stm32f10x.h"
// 用于可变类型的定义
#include <stdarg.h>
// 用于引入接口函数
#include "bsp_usart.h"
// 用于对应操作系统的函数
#include "rtthread.h"



// 编译器的可变变量类型及函数
#define hy_va_list 								va_list
#define hy_va_start(p_args,arg)		va_start(p_args,arg)
#define hy_va_arg(p_args,arg)			va_arg(p_args,arg)
#define hy_va_end(p_args)					va_end(p_args)

// 定义CPU寄存器的int字长
#define HY_INT_SIZE 					sizeof(int)		// 或者填为 4
#define hy_float							double

// 如果有 无符号整型
#ifndef HY_UNSIGNED_INT
	#define HY_UNSIGNED_INT 	  // 默认启用无符号整型
#endif
#ifdef HY_UNSIGNED_INT				// 如果存在无符号整型，请填写对应类型
	#define hy_uint							unsigned int	
#else
	#define hy_uint							int	
#endif

// 定义可选空间及参数
#define HYPRINTF_PARAMETER_SIZE 			12				// '%'后可携带的最大长度，如 hyprintf("\n%-10.3f",3) 刚好达到最大长度,其存储类型为char(<=255)
#define HYPRINTF_FLOAT_FIXSIZE				8					// 浮点数默认输出的小数点个数，同时也作为有效位数，可打印浮点位数不会逾越此值，其存储类型为char(<=255)
#define HYPRINTF_FLOAT_NUMERATION     10				// 浮点数的输出为 10进制数


	
	

// 默认不保护函数 hyprintf(),如果需要用 hyprintf() 输出本功能模块的调试信息，请不要定义此宏
#ifndef HYPRINTF_PROTECT
	//#define HYPRINTF_PROTECT
#endif

// 使能越锁操作，用户通过 hyprintf_create_group() 与 HYPRINTF_JUMP 越锁标识符绑定，从而完成越锁
#ifndef HYPRINTF_JUMP_FLAG 
	#define HYPRINTF_JUMP_FLAG
#endif

// 是否用于线程抢占
#ifndef HYPRINTF_IN_OS
 #define HYPRINTF_IN_OS
#endif

#ifdef HYPRINTF_IN_OS
// 对接 进入临界区以及退出临界区的函数
	#define hy_enter_critical()			rt_enter_critical()		// 或者根据当前处理器STM32F10x使用 __set_PRIMASK(1) 禁止全局中断
	#define	hy_exti_critical()			rt_exit_critical()		// 或者根据当前处理器STM32F10x使用 __set_PRIMASK(0) 使能全局中断
#else
	#define hy_enter_critical()			__set_PRIMASK(1)
	#define	hy_exti_critical()			__set_PRIMASK(0)
#endif



// 用于格式化标志位 - 基于 int的位长为32位，最多可使能 32种功能
#define HYPRINTF_CASE_FLAG 				0x00000001		// 使能字符大写
#define HYPRINTF_LEFT_FLAG				0x00000002		// 使能左对齐，默认右边对
#define HYPRINTF_ZERO_FLAG 				0x00000004		// 此标识符用于表示不再接收填充字符'0',需首个数字为'0'才能进行'0'填充
#define HYPRINTF_SIGN_FLAG				0x00000008		// 使能强制输出正负号
#define HYPRINTF_NUM2_FLAG				0x00000010		// 使能接收第二个数字
#define HYPRINTF_WELL_FLAG				0x00000020		// 使能指数填充



// 用于定义函数返回结果的类型
#define HY_RET_T 									char
	
// 若没有枚举类型，则将下列 各类 的枚举数组，更改为 #define 定义，同类的标识符不可重复定义
// 例如：函数返回结果(同类的标识符不可定义相同的数)
// #define	HY_RET_FALSE		0
// #define	HY_RET_TRUE			1
// 函数返回结果
typedef enum{
	
	HY_RET_FALSE=0,
	HY_RET_TRUE=1,
	
	HY_RET_INT_ERR,								// 整型错误
	HY_RET_RAM_ERR,							  // RAM错误
	HY_RET_ENUM_ERR,							// 枚举错误
	HY_RET_PARAMETER_SIZE_ERR,		// 参数大小错误
	HY_RET_FLOAT_SIZE_ERR,				// 浮点数大小错误
	HY_RET_FLOAT_NUM_ERR,					// 浮点数数值错误
	HY_RET_ARG_ERR,								// 参数错误
	HY_RET_LOCKED,								// 已锁定
	
}HYPRINTF_RET_TYPE;



/**
 * @brief   注册用户标识符枚举类型。
 *
 * @details 该枚举类型定义了用于标识不同用户接口的标识符。这些标识符可用于配置 hyprintf_api() 中的接口函数，以便根据需要选择特定的输出接口。
 *
 * @note
 *   - 不同的标识符用于指定不同的输出接口。在 hyprintf_api() 中，需根据这些标识符配置相应的接口函数。
 * 	 - 同样的，可在 hyprintf() 中更换 hyprintf() 的接口标识符。
 *   - 如果启用 HYPRINTF_JUMP_FLAG 宏，HYPRINTF_JUMP 标识符表示无条件越锁标识符，占用一位。
 *   - HYPRINTF_SEND_TYPE_SIZE 标识符用于表示枚举数组的个数，应谨慎修改。
 */
typedef enum{
	
	HYPRINTF_UART1,
	HYPRINTF_UART2,
	
	
	
	/* ***
		下列标识符具有特殊作用，可按需求使用下列标识符，请谨慎修改
	*** */
	
#ifdef HYPRINTF_JUMP_FLAG
	HYPRINTF_JUMP,			// 无条件越锁标识符，占用一位
#endif
	
	HYPRINTF_SEND_TYPE_SIZE, // 此标识符用于表示枚举数组的个数
}HYPRINTF_SEND_TYPE;



// 初始化函数
HY_RET_T hyprintf_init(void);
HY_RET_T hyprintf_set_group(int *group,int mutex_num,...);
HY_RET_T hyprintf_leave_group(int *group,int group_num,...);

// 用户函数
HY_RET_T hyprintf(const char *format,...);
HY_RET_T hyprintf_echo(int str_size,char *ret_str,const char *format,...);
HY_RET_T hyprintf_by(int send_type,const char *format,...);



#endif // __HYPRINTF_H
