#include "hyprintf.h"



// �ļ��ڲ����ã�����Ϊ�ӿ����������Ǹ���������ʾ��
HY_RET_T hyprintf_centre(\
	int send_type,int ret_str_size,char *ret_str,\
		const char *format,hy_va_list *p_args);
HY_RET_T hyprintf_handle(\
	int send_flag,int ret_str_size,char *ret_str,\
	const char *format,hy_va_list *p_args);
void hyprintf_send(int *send_flag,int *send_size,const char *send_str);



/**
 * @brief   �û�API������ָ���ķ������ͽ��ַ�����������ӦĿ�ġ�
 *
 * @details �ú������ڸ���ָ���ķ������ͽ���ʽ�������������Ӧ��Ŀ�ġ�
 *          ֧�ֶ��ַ������ͣ��û��ɸ�����Ҫ��д��Ӧ�Ľӿں�����
 *
 * @param[in]  send_type      �������ͣ���ʾ��ͬ�ķ���Ŀ�ġ�ʹ��Ԥ����ķ������ͳ�������HYPRINTF_USART1��HYPRINTF_USART2�ȡ�
 * @param[in]	 send_size			���������ݵ��ֽ���
 * @param[in]  send_str       ָ��������ַ�����ָ�롣
 *
 * @return    ����ִ�н�������ش����루HY_RET_T���ͣ����ɹ�����HY_RET_TRUE��
 *
 * @code
 *   int send_type = HYPRINTF_USART1;
 *   int err = hyprintf_api(send_type, "Hello, USART1!");
 *   // ���ݷ������ͽ��ַ�����������ӦĿ�ġ�
 * @endcode
 *
 * @note
 *   - ֧�ֶ��ַ������ͣ��û��ɸ�����Ҫ��д��Ӧ�Ľӿں�����
 *   - ͨ��Ԥ����ķ������ͳ�����ʾ��ͬ�ķ���Ŀ�ģ���HYPRINTF_USART1��HYPRINTF_USART2�ȡ�
 */
void hyprintf_api(int send_type,int send_size,const char *send_str){
	// ֻ��һ���жϣ���ʹ�� if(...)else if(...)else(...) �Ľṹ�Ա�ʡȥ������ж�ʱ��
	
	// ���д�ӿں���
	if( send_type==HYPRINTF_UART1 ){
		Usart_SendArray(USART1,send_str,send_size);
	}
	else if( send_type==HYPRINTF_UART2 ){
		Usart_SendArray(USART2,send_str,send_size);
	}
	else{
		
	}
}



/**
 * @brief   ��ʽ���������ַ�����Ĭ��Ŀ�ģ���USART1��
 *
 * @details �ú������ڸ�ʽ���ַ����������������Ĭ��Ŀ�ģ�����USART1���û�����ͨ������send_type��ֵ��ѡ��ͬ��Ĭ��Ŀ�ġ�
 *          ʹ��Ԥ����ķ������ͳ�������HYPRINTF_USART1��HYPRINTF_USART2�ȡ�
 *					��Ҫ�����ӿڣ��ɶ����HYPRINTF_PROTECT����ʹ��hyprintf_centre�������hyprintf_handle��
 *
 * @param[in]  format            ָ������и�ʽ�����ַ�����ָ�롣
 * @param[in]  ...            �䳤�����б�Ϊ��ʽ���ַ����Ŀɱ������
 *
 * @return    ����ִ�н�������ش����루HY_RET_T���ͣ����ɹ�����HY_RET_TRUE��
 *
 * @code
 *   int err = hyprintf("Formatted message: %d", 42);
 *   // ��ʽ���������ַ�����Ĭ��Ŀ�ģ���USART1��
 * @endcode
 *
 * @note
 *   - �û�����ͨ������send_type��ֵ��ѡ��ͬ��Ĭ�Ϸ���Ŀ�ģ�ʹ��Ԥ����ķ������ͳ�������HYPRINTF_UART1��HYPRINTF_UART2�ȡ�
 */
HY_RET_T hyprintf(const char *format,...){
	HY_RET_T err;
	
	hy_va_list p_args;
	hy_va_start(p_args,format);

	
	
	// �趨�ķ��ͱ�ʶ��
	int send_type=HYPRINTF_UART1;
	
	
	
	char send_str[ 32 ];send_str[0] = '\0';
	
	#ifdef HYPRINTF_PROTECT
	err = hyprintf_centre( send_type, 32, send_str, format, &p_args);
	#else
	err = hyprintf_handle( 1<<send_type, 32, send_str, format, &p_args);
	#endif
	hy_va_end(p_args);
	return err;
}



/**
 * @brief   ��ʼ��hyprintf�⣬���ڼ�鲿�ֺ��Ƿ���ȷ�����á�
 *
 * @details �ú������ڳ�ʼ��hyprintf�⣬����������������Ƿ�Ϸ���ȷ��hyprintf���ܹ�����������
 *          �ڳ�ʼ�������У����������ֽ������塢���õ����ݿռ��С��Send���͸�����%���Я������������FLOAT���͵�Ĭ�����������
 *          �Լ�FLOAT���͵Ľ������Ȳ�����ȷ�����ǵ�ȡֵ�ںϷ���Χ�ڡ�
 *
 * @return    ����ִ�н�������ش����루HY_RET_T���ͣ����ɹ�����HY_RET_TRUE��
 *
 * @code
 *   int err = hyprintf_init();
 *   // ��ʼ��hyprintf�⣬����������������Ƿ�Ϸ���
 * @endcode
 *
 * @note
 *   - hyprintf���ʼ��ʱ���������ֽ������塢���õ����ݿռ��С��Send���͸�����%���Я������������FLOAT���͵�Ĭ�����������
 *     �Լ�FLOAT���͵Ľ������Ȳ�����ȷ�����ǵ�ȡֵ�ںϷ���Χ�ڡ�
 */
HY_RET_T hyprintf_init(void){
	
	// ���͵��ֽ����������
	if( (int)HY_INT_SIZE <= 0){return HY_RET_INT_ERR;}
	
	// ��ѡ��Send���͸����������͵�λ��-1����Ϊ���͵ķ���λ�̶�������Ĺ���������λ���ڱ�ʾ��ͬ�Ľӿ�,(Խ����ʶ��Ҳռ��һ��λ)
	if( ((int)HYPRINTF_SEND_TYPE_SIZE > (int)HY_INT_SIZE*8-1) || ((int)HYPRINTF_SEND_TYPE_SIZE<0) ){return HY_RET_ENUM_ERR;}
	
	// �鿴 %�� ��Я���������� �Ƿ���ڵ���1
	if( (int)HYPRINTF_PARAMETER_SIZE <= 0){ return HY_RET_PARAMETER_SIZE_ERR; }
	
	// �鿴FLOAT���͵�Ĭ����������Ƿ�Ϸ�
	if( (int)HYPRINTF_FLOAT_FIXSIZE <= 0 ){ return HY_RET_FLOAT_SIZE_ERR; }
	
	// �鿴FLOART���͵Ľ������Ƿ�Ϸ�
	if( (int)HYPRINTF_FLOAT_NUMERATION <= 0 ){ return HY_RET_FLOAT_NUM_ERR; }
	
	return HY_RET_TRUE;
}



/**
 * @brief   ����ʽ������ַ���������ָ�����ַ�����������
 *
 * @details �ú������ڽ���ʽ������ַ���������ָ�����ַ�����������ͨ������Ĳ�������ʽ�����д�뵽ָ���Ļ������С�
 *          �ڲ�����hyprintf_handle���������ʽ������������洢��ret_str�С�
 *
 * @param[in]  ret_str_size   ��Ÿ�ʽ����������ݿռ��С��
 * @param[out] ret_str        ָ���Ÿ�ʽ��������ַ�����ָ�롣
 * @param[in]  format            ָ������и�ʽ�����ַ�����ָ�롣
 * @param[in]  ...            �䳤�����б�Ϊ��ʽ���ַ����Ŀɱ������
 *
 * @return    ����ִ�н�������ش����루HY_RET_T���ͣ����ɹ�����HY_RET_TRUE��
 *
 * @code
 *   char ret_str[256];
 *   int err = hyprintf_echo(256, ret_str, "Formatted message: %d", 42);
 *   // ����ʽ������ַ���������ret_str�С�
 * @endcode
 *
 * @note
 *   - ͨ������Ĳ�������ʽ�����д�뵽ָ���Ļ������С�
 *   - �ڲ�����hyprintf_handle���������ʽ������������洢��ret_str�С�
 */
HY_RET_T hyprintf_echo(int ret_str_size,char *ret_str,const char *format,...){
	HY_RET_T err;
	
	hy_va_list p_args;
	hy_va_start(p_args,format);
	
	err = hyprintf_handle(0,ret_str_size,ret_str,format,&p_args);
	
	hy_va_end(p_args);
	return err;
}



/**
 * @brief   ����һ��hyprintf�飬�ɽ�һ������������������ʡ�ڴ���Դ����������Ч�ʡ�
 *
 * @details �ú������ڴ���һ��hyprintf�飬��һ�������������������ڵĲ�ͬ��Ա��������һ��Աδͨ���������޷����͡�
 *          ��ʹ��Խ�����ܣ������Խ����ʶ���󶨣���ʱ�������г�Ա����������Խ����
 *
 * @param[in, out] group     ָ���ʾhyprintf�������ָ�룬���ڴ洢���Ա�ķ������ͱ�־λ��
 *                            �������ֵΪ������������ȡ�����ֵ��Ϊ�������hyprintf�顣
 *                            ���ú󣬸�ָ���ֵ������Ϊ���Ա�ķ������ͱ�־λ��
 * @param[in]       group_num ���ݵĲ�����������ʾ��������hyprintf���Ա������������ĳ�Ա�������顣
 * @param[in]       ...        �䳤�����б�Ϊ��������hyprintf���Ա�ķ������͡�ÿ������ӦΪ��������ʾ�������͵�������
 *
 * @return    ����ִ�н�������ش����루HY_RET_T���ͣ����ɹ�����HY_RET_TRUE��
 *
 * @code
 *   int printf_group = 0; // ����hyprintf��
 *   int err = hyprintf_set_group(&printf_group, 2, 1, -6);
*   // ����hyprintf�飬���г�Ա1Ϊ��������1����Ա2Ϊ��������2�ͷ�������3����ʱ���ڳ�Ա�з�������1��2��3��
 * @endcode
 *
 * @note
 *   - hyprintf���Աʹ��λ�����ʾ��ȷ��ʹ����ȷ��λ����ֵ��
 *   - �������hyprintf��ֵΪ������������ȡ�����ֵ��Ϊ�������hyprintf�顣
 *   - ������һ��Աδͨ���������޷����ͣ�ʹ��Խ������ʱ�������Խ����ʶ���󶨣�ʹ�������г�Ա������Խ����
 */
HY_RET_T hyprintf_set_group(int *group,int group_num,...){
	
	if( *group<0 ){
		*group=-*group;
	}else{
		*group=0;
	}
	
	hy_va_list p_args;
	hy_va_start(p_args,group_num);
	
	int temp_i = 0;
	for(int i=0;i<group_num;i++){
		temp_i = hy_va_arg(p_args,int);
		if( temp_i >= (int)(HYPRINTF_SEND_TYPE_SIZE) ){ continue; }
		
		if( temp_i<0 ){ 
			*group |= -temp_i;
		}
		else{
			*group |= 1<<temp_i;
		}
	}
	*group=-*group;
	
	hy_va_end(p_args);
	return HY_RET_TRUE;
}



/**
 * @brief   ��ָ���������е�ĳЩ�������ͱ�־����ĺ�����
 *
 * @details �ú���������ָ���ķ������н�ĳЩ�������͵ı�־λ���㣬�Ա���ƾ�����Щ���͵���Ϣ��Ҫ���͡�
 *          ֧�ֱ䳤�������û��ɴ��ݶ��������ʾҪ����ķ������͡�
 *
 * @param[in, out] group     ָ���ʾ�����������ָ�룬���ڴ洢�������ͱ�־λ�����ú󣬸�ָ�뽫�洢���º�ķ������־��
 *                            �������ֵΪ������������ȡ�����ֵ��Ϊ������ķ����顣
 *                            ���óɹ��󣬸�ָ���ֵ������Ϊ���º�ķ����顣
 * @param[in]       group_num ���ݵĲ�����������ʾ������ķ������͸�����
 * @param[in]       ...        �䳤�����б�Ϊ������ķ������͡�ÿ������ӦΪ��������ʾ�������͵�������
 *
 * @return    ����ִ�н�������ش����루HY_RET_T���ͣ����ɹ�����HY_RET_TRUE��
 *
 * @code
 *   int send_group = 0xF;	// ��ʾ�����а����˷�������1��2��3��4
 *   int err = hyprintf_leave_group(&send_group, 2, 1, -6);
 *   // 1-(0b)1-��������Ϊ1��-6->6-(0b)110-��������Ϊ2��3,����������1��2��3����������
 * @endcode
 *
 * @note
 *   - ������ʹ��λ�����ʾ��ȷ��ʹ����ȷ��λ����ֵ��
 *   - ������ķ�����ֵΪ������������ȡ�����ֵ��Ϊ������ķ����顣
 */
HY_RET_T hyprintf_leave_group(int *group,int group_num,...){
	
	if( *group<0 ){
		*group=-*group;
	}else{
		return HY_RET_ARG_ERR;
	}
	
	hy_va_list p_args;
	hy_va_start(p_args,group_num);
	
	int temp_i = 0;
	for(int i=0;i<group_num;i++){
		temp_i = hy_va_arg(p_args,int);
		if( temp_i >= (int)(HYPRINTF_SEND_TYPE_SIZE) ){ continue; }
			
		if( temp_i<0 ){
			*group &= ~(-temp_i);
		}
		else{
			*group &= ~(1<<temp_i);
		}
	}
	*group=-*group;
	
	hy_va_end(p_args);
	return HY_RET_TRUE;
}



/**
 * @brief   ����ָ���������ͣ�����ʽ������ַ���������Ŀ�ĺ�����
 *
 * @details �ú������ڸ���ָ���ķ������ͽ���ʽ�����������Ŀ�ĺ�����
 *          �ڲ�����hyprintf_centre���������ʽ�����������������ָ����Ŀ�ĺ�����
 *
 * @param[in]  send_type      �������ͣ�ʹ��λ�����ʾ�����ݸ� hyprintf_centre() ������
 * @param[in]  format            ָ������и�ʽ�����ַ�����ָ�롣
 * @param[in]  ...            �䳤�����б�Ϊ��ʽ���ַ����Ŀɱ������
 *
 * @return    ����ִ�н�������ش����루HY_RET_T���ͣ����ɹ�����HY_RET_TRUE��
 *
 * @code
 *   int send_type = HYPRINTF_SEND_TYPE_1 | HYPRINTF_SEND_TYPE_3; // ���õ�1�͵�3�ַ�������
 *   int err = hyprintf_by(send_type, "Formatted message: %d", 42);
 *   // ���ݷ������ͽ���ʽ������ַ���������Ŀ�ĺ�����
 * @endcode
 *
 * @note
 *   - ��������ʹ��λ�����ʾ��ȷ��ʹ����ȷ��λ����ֵ��
 *   - �ڲ�����hyprintf_centre���������ʽ�����������������ָ����Ŀ�ĺ�����
 */
HY_RET_T hyprintf_by(int send_type,const char *format,...){
	HY_RET_T err;
	
	hy_va_list p_args;
	hy_va_start(p_args,format);
	
	char ret_str[ 32 ];ret_str[0] = '\0';
	err = hyprintf_centre( send_type, 32, ret_str, format, &p_args );
	
	hy_va_end(p_args);
	return err;
}



/**
 * @file    hyprintf.c
 * @brief   ���ļ�������һ�����д���ĸ�ʽ�����������
 *
 * @details �ú������ڸ�ʽ������ַ�����������ָ���ķ������ͽ�������д����������ͬ�Ľӿڡ�
 *          ֧�ֻ��������ܣ���ֹͬʱ������ͬ���͵���Ϣ����ѡ������Խ����ʶ��������֧�����⴦��
 *
 * @param[in]  send_type      �������ͣ�ʹ��λ�����ʾ�����ݸ� hyprintf_handle() ������
 * @param[in]  ret_str_size   ��Ÿ�ʽ����������ݿռ��С��
 * @param[out] ret_str        ָ���Ÿ�ʽ��������ַ�����ָ�롣
 * @param[in]  format            ָ������и�ʽ�����ַ�����ָ�롣
 * @param[in]  p_args         ָ��ɱ�������͵�ָ�롣
 *
 * @return    ����ִ�н�������ش����루HY_RET_T���ͣ����ɹ�����HY_RET_TRUE��
 *
 * @code
 *   int send_type = HYPRINTF_SEND_TYPE_1 | HYPRINTF_SEND_TYPE_3; // ���õ�1�͵�3�ַ�������
 *   char ret_str[256];
 *   int err = hyprintf_centre(send_type, 256, ret_str, "Centered message: %d", 42);
 *   // ���д�����������õķ������͡�
 * @endcode
 *
 * @note
 *   - ��������ʹ��λ�����ʾ��ȷ��ʹ����ȷ��λ����ֵ��
 *   - ���������ܿɷ�ֹͬʱ������ͬ���͵���Ϣ��
 *	 - ���ʹ�����߳���ռ�����л����У���ȷ����ȷ���ý����ٽ������˳��ٽ����ĺ���
 *   - �������Խ����ʶ�����ܣ���
 */
HY_RET_T hyprintf_centre(int send_type,int ret_str_size,char *ret_str,const char *format,hy_va_list *p_args){
	
	// ����������תΪ��Ӧ������λ����
	if( send_type<0 ){ send_type = -send_type; }
	else{ send_type = 1<<send_type; }
	
	// ��̬��������ʾ��������״̬
	// ���������͵ķ���λ��ʾ�˷�������Ϊ�飬���32λ������䱸31����
	// ����ʹ����Խ�����ܣ��������䱸30����
	static int mutex_flag = 0;
	
	// �������Խ������
	#ifdef HYPRINTF_JUMP_FLAG
	if( ( send_type&(1<<HYPRINTF_JUMP) )==0 ){
	#endif
		
		// �����ٽ���
		hy_enter_critical();
		// ������⣬��������������
		if( send_type & mutex_flag ){ 
			#ifndef HYPRINTF_PROTECT
			hyprintf("\nThe current channel is locked, and the current lock value is %#b",mutex_flag);
			#endif
			hy_exti_critical();
			return HY_RET_LOCKED;
		}
		// �˴�����ʱ�䴰�ڣ�����б������߳���ռ����ʹ������ʧЧ����������ý����ٽ������˳��ٽ����ĺ���
		// ����
		mutex_flag |= send_type; 
		// �˳��ٽ���
		hy_exti_critical();
		
	#ifdef HYPRINTF_JUMP_FLAG
	}
	#endif
	
	// ִ�и�ʽ������
	HY_RET_T err = hyprintf_handle(send_type,ret_str_size,ret_str,format,p_args);
	
	// �������Խ������
	#ifdef HYPRINTF_JUMP_FLAG
	if( ( send_type&(1<<HYPRINTF_JUMP) )==0 ){
	#endif
	
		// ����
		mutex_flag &= ~send_type;
	
	#ifdef HYPRINTF_JUMP_FLAG
	}
	#endif
	
	// �ɹ�����HY_RET_TRUE(1)�����󷵻ش�������
	return err;
}



/**
 * @file    hyprintf.c
 * @brief   ���ļ�������һ��������hyprintf��C���������ݷ�������ָ��ͬ�Ľӿڣ�����ʽ�����hyprintf����ͬ�Ľӿ��С�
 *
 * @details �ú������ݷ�������ָ��ͬ�Ľӿڣ�����ʽ�����hyprintf����ͬ�Ľӿ��С�
 * 
 * @param[in]  send_flag      �������ͣ�ʹ��λ�����ʾ�����ݸ� hyprintf_send() ������
 * @param[in]  ret_str_size   ��Ÿ�ʽ����������ݿռ��С��
 * @param[in]  ret_str        ָ���Ÿ�ʽ��������ַ�����ָ�롣
 * @param[in]  format            ָ������и�ʽ�����ַ�����ָ�롣
 * @param[in]  p_args         ָ��ɱ�������͵�ָ�롣
 *
 * @return   �̶����� HY_RET_TRUE(1)��
 *
 * @code
 *   int send_flag = 0;
 *   char ret_str[3];
 *   int err = hyprintf_handle(send_flag, 3, ret_str, ":\n%d", 123456789);
 *   //  send_flagΪ0����ʾ��������û���κ�һ��λΪ1��
 *   //  ��Ϊ5�η��ͣ�"12" + "34" + "56" + "78" + "9"��
 * @endcode
 *
 * @note
 *   - HYPRINTF_PARAMETER_SIZE('%'���Я���������Ϣ����)����С�ڵ���0��
 *   - ret_str_size����С�ڵ���1����Ϊ����ռ�������һ���ֽڹ̶����'\0'��
 *   - ��ǰ����ֵ���ڿ������ݿռ��һʱ�������� hyprintf_send() ִ��һ�η��͡�
 *   - ��û�ж��� HY_UNSIGNED_INT(ʹ���޷������͵ı�ʶ��)ʱ���滻Ϊ�� int ���ղ�����
 */
HY_RET_T hyprintf_handle(\
	int send_flag,int ret_str_size,char *ret_str,\
	const char *format,hy_va_list *p_args){
	
	// ���õ�����ռ䲻��С�ڵ���1�����򽫷��ش��󣬴�������ΪHY_RET_ARG_ERR��
	if( ret_str_size<=1 ){ return HY_RET_RAM_ERR; }
	// ��ִ�м�һ�������Ա�����жϲ���Ҫ�˼������
	ret_str_size = ret_str_size-1;
	
	// ָ���Ÿ�ʽ��������ַ�����ָ��
	char *ret_str_ptr = ret_str;
	// ���ڱ�ʾ��ǰ����ֵ
	int	ret_str_idx=0;
	
	// ָ������и�ʽ�����ַ���ָ��
	const char *str_ptr = format;
	
	// �� ��ǰָ��ָ���ֵ��Ϊ���ַ�ʱ
	while( *str_ptr!='\0' ){
		
		// �����ǰָ��ָ���ֵΪ'%'
		if( *str_ptr == '%' ){
			
			char parameter_len = HYPRINTF_PARAMETER_SIZE;		// '%'���Я���������Ϣ����
			int 	parameter_flag=0;													// ���α�־λ
			char 	parameter_sign=' ';												// Ĭ�����,���η��Ų���
			int 	parameter_num=0;													// ��������1
			int	 	parameter_num2=0;													// ��������2

			// ���˳�ѭ���ķ�ʽ��
			// *str_ptr == '\0'											, ָ����ַ�����ֱ��goto����ѭ��
			// *str_ptr == 'c/s/d/f/x/X/f/p/u/b/o'	, ��ӡ����
			// *str_ptr == '%'											, ָ��'%'
			// *str_ptr == other										, ֱ���˳�
			while( parameter_len-- ){
				
				// ָ���һ������
				str_ptr++;
				
				if( *str_ptr == '\0' ){goto strover;}
				
				// ���Ϊ��ӡ����
				if( *str_ptr == 'c' | *str_ptr == 's' | *str_ptr == 'S' |\
						*str_ptr == 'd' | *str_ptr == 'f' | *str_ptr == 'x' | *str_ptr == 'X' |\
						*str_ptr == 'p' | *str_ptr == 'u' | *str_ptr == 'b' | *str_ptr == 'o' ){
					
					// ��Ž��������޷������͡����������Լ������Ľ���(�����յ�����Ϊ123456�����ֵΪ100000)
					char numeration=0;
					hy_uint temp_u=0;
					hy_float temp_f=0;
					hy_uint int_power=1;

					// ���Ϊ����ת�ַ��Ĵ�ӡ����
					if( *str_ptr != 'c' && *str_ptr != 's' && *str_ptr != 'S' ){
						
						// ��д��������ʹ�ܶ�Ӧ�ı�ʶ��
						if( *str_ptr == 'd' ){
							numeration = 10;
						}else if( *str_ptr == 'f' ){
							numeration = HYPRINTF_FLOAT_NUMERATION;
						}else if( *str_ptr == 'x' ){
							numeration = 16;
						}else if( *str_ptr == 'X' || *str_ptr == 'p' ){
							numeration = 16;
							parameter_flag |= HYPRINTF_CASE_FLAG;
						}else if( *str_ptr == 'u'){
							numeration = 10;
						}else if( *str_ptr == 'b'){
							numeration = 2;
						}else if( *str_ptr == 'o' ){
							numeration = 8;
						}

						// ���ݴ�ӡ���ͣ����ղ���
						int temp_d=0;
						if( *str_ptr == 'f' ){
							temp_f = hy_va_arg(*p_args,hy_float);
							temp_d = temp_f;
							temp_u = temp_d;
						}
						else if( *str_ptr == 'd' ){
							temp_d = hy_va_arg(*p_args,int);
							temp_u = temp_d;
						}
						else if( *str_ptr == 'p' ){
							temp_u = (hy_uint)( hy_va_arg(*p_args,hy_uint *) );
						}
						else{
							temp_u = hy_va_arg(*p_args,hy_uint);
						}
						
						// ���޷�����ʹ���޷�������
						#ifndef HY_UNSIGNED_INT
							temp_d = temp_u;
						#endif
						
						// int_power��ʱ�����ۼƸ�ʽ�����ַ���С���󽫴���ʱֵ����temp_d
						// Ĭ��Ϊ1����Ϊ��������һ����ռ��1��
						int_power=1;
						
						// �������Ϊ���������ӡ���Ų�����ֵ�ķ�תΪ���������޷������ͣ�
						// �������ʹ����ǿ��������Ų��ҽ���ֵ����0�����������
						// �ڴ��ж��У������������ն�����temp_u���
						if(temp_d<0){
							int_power++;
							ret_str_idx++;*(ret_str_ptr++)='-';
							if( ret_str_idx>=ret_str_size )
								{*(ret_str_ptr)='\0';hyprintf_send(&send_flag,&ret_str_idx,ret_str);ret_str_idx=0;ret_str_ptr=ret_str; }
							temp_u = -temp_d;
							if( *str_ptr == 'f' ){temp_f = -temp_f;}
						}
						else if( parameter_flag&HYPRINTF_SIGN_FLAG && (temp_d>0||temp_u>0) ){
							int_power++;
							ret_str_idx++;*(ret_str_ptr++)='+';
							if( ret_str_idx>=ret_str_size )
								{*(ret_str_ptr)='\0';hyprintf_send(&send_flag,&ret_str_idx,ret_str);ret_str_idx=0;ret_str_ptr=ret_str; }
						}
						
						// ��ʱ��int_power��ŵ���ʱֵ����temp_d
						temp_d=int_power;
						
						// �ۼ�ǰ׺������
						if( parameter_flag&HYPRINTF_WELL_FLAG ){
							if( numeration==16 || numeration==2 ){
								temp_d=temp_d+2;
							}
							else if( numeration==8 ){
								temp_d++;
							}
						}
						
						// �ۼ������ĸ�����������������ֵĽ����洢��int_power( �����յ�����Ϊ123456�����ֵΪ100000 )
						int_power=1;
						while( temp_u/int_power>=numeration ){
							temp_d++;
							int_power = int_power*numeration;
						}
						
						// �ۼƸ������ĸ���
						if( *str_ptr == 'f' ){
							// ����ӡ����Ϊ������ʱ�����յĴ�������2��ʾΪ���ӡ��С������
							// ��С���������������Ч���������������Ч������ʾ
							if( parameter_num2>(int)HYPRINTF_FLOAT_FIXSIZE || parameter_num2==0 ){ parameter_num2=(int)HYPRINTF_FLOAT_FIXSIZE; }
							temp_d = temp_d+1+parameter_num2;
						}
						
						// ��������1�����˴�'%'��ʽ������ַ��������ڵ��ڴ�ֵ������Ч��
						// ��֮�����ಿ�ֽ����ض��ַ���䣬
						if( parameter_num>temp_d ){
							parameter_num = parameter_num-temp_d; 
						}
						else{
							parameter_num = 0;
						}
					} // *str_ptr != 'c'/'s'
					
					// Ĭ�Ͽ��ң����������
					if( (parameter_flag&HYPRINTF_LEFT_FLAG)==0 && *str_ptr!='S' ){
						// ��Ϊ��ӡ����Ϊ�ַ����ͣ���ֻ������һ���ֽڿռ�
						if( *str_ptr=='c' ){ if(parameter_num>=1)parameter_num--; }
						while(parameter_num--){
							ret_str_idx++;*(ret_str_ptr++) = parameter_sign;
							if( ret_str_idx>=ret_str_size )
								{ *(ret_str_ptr)='\0';hyprintf_send(&send_flag,&ret_str_idx,ret_str);ret_str_idx=0;ret_str_ptr=ret_str; }
						}
					} // if( (parameter_flag&HYPRINTF_LEFT_FLAG)==0 ){

					// ����ַ�����
					if( *str_ptr == 'c' ){
						ret_str_idx++;*(ret_str_ptr++)=(char)hy_va_arg(*p_args,int);
						if( ret_str_idx>=ret_str_size )
							{ *(ret_str_ptr)='\0';hyprintf_send(&send_flag,&ret_str_idx,ret_str);ret_str_idx=0;ret_str_ptr=ret_str; }
					}
					// ����ַ������������д������֣�
					else if( *str_ptr == 's'){
						const char *temp_s = hy_va_arg(*p_args,const char *);
						while( *temp_s != '\0' ){
							ret_str_idx++;*(ret_str_ptr++)=*(temp_s++);
							if( ret_str_idx>=ret_str_size )
								{ *(ret_str_ptr)='\0';hyprintf_send(&send_flag,&ret_str_idx,ret_str);ret_str_idx=0;ret_str_ptr=ret_str; }
							// ���parameter_num��ΪĬ��ֵ0�����м�һ��Ȼ���ٴ��ж��Ƿ�Ϊ0
							// ���ʾ�˴θ�ʽ��ֻ�����ǰparameter_num���ַ�
							if( parameter_num2!=0 ){
								parameter_num2--;
								if( parameter_num2==0 ){ break; }
							}
						}
					}
					// ����ַ����ݣ��������Ĵ�С�� parameter_num2 ����
					else if( *str_ptr == 'S' ){
						const char *temp_i = hy_va_arg(*p_args,const char *);
						if( parameter_num!=0 ){
							char temp_d;
							while( parameter_num2-- ){
								temp_d = parameter_num;
								while(temp_d--){
									ret_str_idx++;*(ret_str_ptr++)=*(temp_i+temp_d);
									if( ret_str_idx>=ret_str_size )
										{ *(ret_str_ptr)='\0';hyprintf_send(&send_flag,&ret_str_idx,ret_str);ret_str_idx=0;ret_str_ptr=ret_str; }
								}
								temp_i=temp_i+parameter_num;
							}
						} // if( parameter_num!=0 ){
						else{
							while( parameter_num2-- ){
								ret_str_idx++;*(ret_str_ptr++)=*temp_i;
								temp_i++;
								if( ret_str_idx>=ret_str_size )
									{ *(ret_str_ptr)='\0';hyprintf_send(&send_flag,&ret_str_idx,ret_str);ret_str_idx=0;ret_str_ptr=ret_str; }
							}
						}
					}
					// ���εĴ�ӡ����Ϊ����ת�ַ�������
					else{
						// ��ӡǰ׺,(������ǰ׺��"0b",�˽���ǰ׺:"0",ʮ������ǰ׺��"0x"\"0X"(��д))
						if( parameter_flag&HYPRINTF_WELL_FLAG ){
							if( numeration==16 || numeration==2 ){
								ret_str_idx++;*(ret_str_ptr++)='0';
								if( ret_str_idx>=ret_str_size )
									{*(ret_str_ptr)='\0';hyprintf_send(&send_flag,&ret_str_idx,ret_str);ret_str_idx=0;ret_str_ptr=ret_str; }
								if( numeration==16 ){
									if( parameter_flag&HYPRINTF_CASE_FLAG ){
										ret_str_idx++;*(ret_str_ptr++)='X';
									}else{
										ret_str_idx++;*(ret_str_ptr++)='x';
									}
								}else if( numeration==2 ){
									ret_str_idx++;*(ret_str_ptr++)='b';
								}
								if( ret_str_idx>=ret_str_size )
									{*(ret_str_ptr)='\0';hyprintf_send(&send_flag,&ret_str_idx,ret_str);ret_str_idx=0;ret_str_ptr=ret_str; }
							}
							else if( numeration==8 ){
								ret_str_idx++;*(ret_str_ptr++)='0';
								if( ret_str_idx>=ret_str_size )
									{*(ret_str_ptr)='\0';hyprintf_send(&send_flag,&ret_str_idx,ret_str);ret_str_idx=0;ret_str_ptr=ret_str; }
							}
						}
						
						// ��ӡ��������
						int temp_d;
						if( parameter_flag & HYPRINTF_CASE_FLAG ){ temp_d=55; }
						else{ temp_d=87;} // Ĭ��Сд
						while( int_power ){
							if(temp_u/int_power >= 10){
								ret_str_idx++;*(ret_str_ptr++)= temp_u/int_power+temp_d;
							}else{
								ret_str_idx++;*(ret_str_ptr++)= temp_u/int_power+48;
							}
							temp_u = temp_u - int_power*(temp_u/int_power); //���� (temp_d/t_len) ΪԼ��
							int_power = int_power/numeration;
							if( ret_str_idx>=ret_str_size )
								{ *(ret_str_ptr)='\0';hyprintf_send(&send_flag,&ret_str_idx,ret_str);ret_str_idx=0;ret_str_ptr=ret_str; }
						} // while( int_power ){
						
						// ���Ϊ����������ֱ�Ӽӵ��Լ����ݸ����������С��
						if( *str_ptr == 'f' ){
							ret_str_idx++;*(ret_str_ptr++)='.';
							if( ret_str_idx>=ret_str_size )
								{ *(ret_str_ptr)='\0';hyprintf_send(&send_flag,&ret_str_idx,ret_str);ret_str_idx=0;ret_str_ptr=ret_str; }
							while( parameter_num2-- ){
								temp_f = temp_f-(int)temp_f;
								ret_str_idx++;*(ret_str_ptr++)=temp_f*numeration+48;
								temp_f = temp_f*numeration;
								if( ret_str_idx>=ret_str_size )
									{ *(ret_str_ptr)='\0';hyprintf_send(&send_flag,&ret_str_idx,ret_str);ret_str_idx=0;ret_str_ptr=ret_str; }
							} // while( parameter_num2-- ){
						} // if( *str_ptr == 'f' ){
					} // if(	*str_ptr == 'd' | *str_ptr == 'f' | *str_ptr == 'x' | *str_ptr == 'X' |\...
					
					// ʹ�ܿ��󣬽��������
					if( parameter_flag&HYPRINTF_LEFT_FLAG && *str_ptr!='S' ){
						if( *str_ptr=='c' ){ if(parameter_num>=1)parameter_num--; }
						while(parameter_num--){
							ret_str_idx++;*(ret_str_ptr++)=parameter_sign;
							if( ret_str_idx>=ret_str_size )
								{ *(ret_str_ptr)='\0';hyprintf_send(&send_flag,&ret_str_idx,ret_str);ret_str_idx=0;ret_str_ptr=ret_str; }
						}
					} // if( parameter_flag&HYPRINTF_LEFT_FLAG ){

					// �˴�'%'��ʽ����ɣ��˳� while( parameter_len-- )
					break;
				} // if( *str_ptr == 'c'/'s'/...dxXpfubo
				else if( *str_ptr == '-' ){
					// �������������
					parameter_flag |= HYPRINTF_LEFT_FLAG;
				}
				else if( *str_ptr == '+' ){
					// ǿ�����������
					parameter_flag |= HYPRINTF_SIGN_FLAG;
				}
				else if( *str_ptr == '.' ){
					// ���ڽ��մ�������2
					parameter_flag |= HYPRINTF_NUM2_FLAG;
				}
				else if( *str_ptr == '#' ){
					// ������������ơ��˽��ơ�ʮ�����Ƶ�ǰ׺
					parameter_flag |= HYPRINTF_WELL_FLAG;
				}
				else if( *str_ptr >= '0' && *str_ptr <= '9' ){
					// �����һ�����յ������ַ�Ϊ0�����ʾ��'0'���п�λ���
					if( *str_ptr == '0' && (parameter_flag&HYPRINTF_ZERO_FLAG)==0 ){
						parameter_sign = *str_ptr;
					}
					// ������յ�С����'.'��������������ݽ���Ϊ��������2
					else if( parameter_flag & HYPRINTF_NUM2_FLAG ){
						if( parameter_num2==0 ){ parameter_num2 = (*str_ptr-48); }
						else{ parameter_num2 = parameter_num2*10+(*str_ptr-48); }
					}
					// �ڽ��յ�С����ǰ��ǰ��������־���Ϊ��������1
					else{
						if( parameter_num==0 ){ parameter_num = (*str_ptr-48); }
						else{ parameter_num = parameter_num*10+(*str_ptr-48); }
					}
					// ʹ�ܲ���'0'����λ
					parameter_flag |= HYPRINTF_ZERO_FLAG;
				} // else if( *temp_str >= '0' && *temp_str <= '9' ){
				else if( *str_ptr == '%' ){
					// ������յ�'%'����ֱ�����'%'�����˳��˴θ�ʽ����¼
					ret_str_idx++;*(ret_str_ptr++)='%';
					if( ret_str_idx>=ret_str_size )
						{ *(ret_str_ptr)='\0';hyprintf_send(&send_flag,&ret_str_idx,ret_str);ret_str_idx=0;ret_str_ptr=ret_str; }
					break;
				} // else if( *str_ptr == '%' ){
				else{
					// ���������ݶ���ֱ���˳��˴θ�ʽ�������Ҳ�������κ�����
					break;
				} // *str_ptr == other
			}	// while( parameter_len-- ){ ��ѭ������ parameter_len�ݼ�Ϊ0 �� if(*temp_str == d/x/%/'\0' ) �� Խ��
		}	// if( *str_ptr == '%' ){
		else{
			// ָ����ַ�����Ϊ'%'/'\0'�����������ַ�����ֱ���������������
			ret_str_idx++;*(ret_str_ptr++)=*str_ptr;
			if( ret_str_idx>=ret_str_size )
				{ *(ret_str_ptr)='\0';hyprintf_send(&send_flag,&ret_str_idx,ret_str);ret_str_idx=0;ret_str_ptr=ret_str; }
		} // if( *str_ptr ��= '%'/'\0' ){	

		// ָ����һ���ַ�
		str_ptr++;
	} // while( *str_ptr!='\0' ){
	
	// �˱�ʶ������ѭ����ָ����ַ�Ϊ'\0'��ֱ������ѭ��
	strover:
	// ����ǰ����ֵ��Ϊ0�����ʾ��������δ����
	if( ret_str_idx!=0 ){
		*(ret_str_ptr)='\0';
		hyprintf_send(&send_flag,&ret_str_idx,ret_str);
	}
	
	// �̶�����HY_RET_TRUE(1)
	return HY_RET_TRUE;
}



/**
 * @file    hyprintf.c
 * @brief   ���ļ������˷��͸�ʽ���ַ�����ͨ�ú���ʵ�֡�
 *
 * @details �ú������ڷ��͸�ʽ���ַ�����֧��ͨ����ͬ�ķ�������ѡ����Ŀ�ꡣ
 *           �������������ͬʱ������ʱ������λ˳��������Ӧ��API��������з������;�δ���
 *           ��Ĭ��ͨ������1��USART1�������ַ�����
 *
 * @param   send_flag  �������ͱ�־��ʹ��λ�����ʾ��ÿһλ��Ӧһ�ַ������͡�
 *                      ���磬����ͬʱ�����1�͵�3�ַ������ͣ���send_flag����Ϊ��(1 << 0) | (1 << 2)��
 * @param   send_size  �����͵ĸ�ʽ���ַ���
 * @param   send_str   �����͵ĸ�ʽ���ַ���
 *
 * @return  ����ִ�н�������ش����루HY_RET_T���ͣ����ɹ�����HY_SUCCESS��
 *
 * @code
 *   int send_flag = (1 << 0) | (1 << 2); // �����1�͵�3�ַ�������
 *   const char *message = "Hello, World!";
 *   HY_RET_T result = hyprintf_send(&send_flag, message);
 *   // ������ִ�н��
 * @endcode
 *
 * @note    ע�����
 *          - �ú���������hyprintf_api��Usart_SendString������ȷ����������������ȷʵ�ֺͿ����ԡ�
 *          - send_flag����ʹ��λ�����ʾ��ͬ�ķ������ͣ�ȷ��ʹ����ȷ��λ����ֵ��
 */
void hyprintf_send(int *send_flag,int *send_size,const char *send_str){
	for( int temp_i=0; temp_i<(int)HYPRINTF_SEND_TYPE_SIZE ;temp_i++ ){
		if( *send_flag & (1<<temp_i) ){
			hyprintf_api(temp_i,*send_size,send_str);		// �����ݴ���API
		}
	}
}
