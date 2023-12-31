#ifndef __HYPRINTF_H
#define __HYPRINTF_H

// �����޷������͵Ķ���
#include "stm32f10x.h"
// ���ڿɱ����͵Ķ���
#include <stdarg.h>
// ��������ӿں���
#include "bsp_usart.h"
// ���ڶ�Ӧ����ϵͳ�ĺ���
#include "rtthread.h"



// �������Ŀɱ�������ͼ�����
#define hy_va_list 								va_list
#define hy_va_start(p_args,arg)		va_start(p_args,arg)
#define hy_va_arg(p_args,arg)			va_arg(p_args,arg)
#define hy_va_end(p_args)					va_end(p_args)

// ����CPU�Ĵ�����int�ֳ�
#define HY_INT_SIZE 					sizeof(int)		// ������Ϊ 4
#define hy_float							double

// ����� �޷�������
#ifndef HY_UNSIGNED_INT
	#define HY_UNSIGNED_INT 	  // Ĭ�������޷�������
#endif
#ifdef HY_UNSIGNED_INT				// ��������޷������ͣ�����д��Ӧ����
	#define hy_uint							unsigned int	
#else
	#define hy_uint							int	
#endif

// �����ѡ�ռ估����
#define HYPRINTF_PARAMETER_SIZE 			12				// '%'���Я������󳤶ȣ��� hyprintf("\n%-10.3f",3) �պôﵽ��󳤶�,��洢����Ϊchar(<=255)
#define HYPRINTF_FLOAT_FIXSIZE				8					// ������Ĭ�������С���������ͬʱҲ��Ϊ��Чλ�����ɴ�ӡ����λ��������Խ��ֵ����洢����Ϊchar(<=255)
#define HYPRINTF_FLOAT_NUMERATION     10				// �����������Ϊ 10������


	
	

// Ĭ�ϲ��������� hyprintf(),�����Ҫ�� hyprintf() ���������ģ��ĵ�����Ϣ���벻Ҫ����˺�
#ifndef HYPRINTF_PROTECT
	//#define HYPRINTF_PROTECT
#endif

// ʹ��Խ���������û�ͨ�� hyprintf_create_group() �� HYPRINTF_JUMP Խ����ʶ���󶨣��Ӷ����Խ��
#ifndef HYPRINTF_JUMP_FLAG 
	#define HYPRINTF_JUMP_FLAG
#endif

// �Ƿ������߳���ռ
#ifndef HYPRINTF_IN_OS
 #define HYPRINTF_IN_OS
#endif

#ifdef HYPRINTF_IN_OS
// �Խ� �����ٽ����Լ��˳��ٽ����ĺ���
	#define hy_enter_critical()			rt_enter_critical()		// ���߸��ݵ�ǰ������STM32F10xʹ�� __set_PRIMASK(1) ��ֹȫ���ж�
	#define	hy_exti_critical()			rt_exit_critical()		// ���߸��ݵ�ǰ������STM32F10xʹ�� __set_PRIMASK(0) ʹ��ȫ���ж�
#else
	#define hy_enter_critical()			__set_PRIMASK(1)
	#define	hy_exti_critical()			__set_PRIMASK(0)
#endif



// ���ڸ�ʽ����־λ - ���� int��λ��Ϊ32λ������ʹ�� 32�ֹ���
#define HYPRINTF_CASE_FLAG 				0x00000001		// ʹ���ַ���д
#define HYPRINTF_LEFT_FLAG				0x00000002		// ʹ������룬Ĭ���ұ߶�
#define HYPRINTF_ZERO_FLAG 				0x00000004		// �˱�ʶ�����ڱ�ʾ���ٽ�������ַ�'0',���׸�����Ϊ'0'���ܽ���'0'���
#define HYPRINTF_SIGN_FLAG				0x00000008		// ʹ��ǿ�����������
#define HYPRINTF_NUM2_FLAG				0x00000010		// ʹ�ܽ��յڶ�������
#define HYPRINTF_WELL_FLAG				0x00000020		// ʹ��ָ�����



// ���ڶ��庯�����ؽ��������
#define HY_RET_T 									char
	
// ��û��ö�����ͣ������� ���� ��ö�����飬����Ϊ #define ���壬ͬ��ı�ʶ�������ظ�����
// ���磺�������ؽ��(ͬ��ı�ʶ�����ɶ�����ͬ����)
// #define	HY_RET_FALSE		0
// #define	HY_RET_TRUE			1
// �������ؽ��
typedef enum{
	
	HY_RET_FALSE=0,
	HY_RET_TRUE=1,
	
	HY_RET_INT_ERR,								// ���ʹ���
	HY_RET_RAM_ERR,							  // RAM����
	HY_RET_ENUM_ERR,							// ö�ٴ���
	HY_RET_PARAMETER_SIZE_ERR,		// ������С����
	HY_RET_FLOAT_SIZE_ERR,				// ��������С����
	HY_RET_FLOAT_NUM_ERR,					// ��������ֵ����
	HY_RET_ARG_ERR,								// ��������
	HY_RET_LOCKED,								// ������
	
}HYPRINTF_RET_TYPE;



/**
 * @brief   ע���û���ʶ��ö�����͡�
 *
 * @details ��ö�����Ͷ��������ڱ�ʶ��ͬ�û��ӿڵı�ʶ������Щ��ʶ������������ hyprintf_api() �еĽӿں������Ա������Ҫѡ���ض�������ӿڡ�
 *
 * @note
 *   - ��ͬ�ı�ʶ������ָ����ͬ������ӿڡ��� hyprintf_api() �У��������Щ��ʶ��������Ӧ�Ľӿں�����
 * 	 - ͬ���ģ����� hyprintf() �и��� hyprintf() �Ľӿڱ�ʶ����
 *   - ������� HYPRINTF_JUMP_FLAG �꣬HYPRINTF_JUMP ��ʶ����ʾ������Խ����ʶ����ռ��һλ��
 *   - HYPRINTF_SEND_TYPE_SIZE ��ʶ�����ڱ�ʾö������ĸ�����Ӧ�����޸ġ�
 */
typedef enum{
	
	HYPRINTF_UART1,
	HYPRINTF_UART2,
	
	
	
	/* ***
		���б�ʶ�������������ã��ɰ�����ʹ�����б�ʶ����������޸�
	*** */
	
#ifdef HYPRINTF_JUMP_FLAG
	HYPRINTF_JUMP,			// ������Խ����ʶ����ռ��һλ
#endif
	
	HYPRINTF_SEND_TYPE_SIZE, // �˱�ʶ�����ڱ�ʾö������ĸ���
}HYPRINTF_SEND_TYPE;



// ��ʼ������
HY_RET_T hyprintf_init(void);
HY_RET_T hyprintf_set_group(int *group,int mutex_num,...);
HY_RET_T hyprintf_leave_group(int *group,int group_num,...);

// �û�����
HY_RET_T hyprintf(const char *format,...);
HY_RET_T hyprintf_echo(int str_size,char *ret_str,const char *format,...);
HY_RET_T hyprintf_by(int send_type,const char *format,...);



#endif // __HYPRINTF_H
