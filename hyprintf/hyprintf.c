#include "hyprintf.h"



// 文件内部调用，不作为接口引出，但是给出了引出示例
HY_RET_T hyprintf_centre(\
	int send_type,int ret_str_size,char *ret_str,\
		const char *format,hy_va_list *p_args);
HY_RET_T hyprintf_handle(\
	int send_flag,int ret_str_size,char *ret_str,\
	const char *format,hy_va_list *p_args);
void hyprintf_send(int *send_flag,int *send_size,const char *send_str);



/**
 * @brief   用户API，根据指定的发送类型将字符串发送至相应目的。
 *
 * @details 该函数用于根据指定的发送类型将格式化结果发送至相应的目的。
 *          支持多种发送类型，用户可根据需要编写相应的接口函数。
 *
 * @param[in]  send_type      发送类型，表示不同的发送目的。使用预定义的发送类型常量，如HYPRINTF_USART1、HYPRINTF_USART2等。
 * @param[in]	 send_size			所发送数据的字节数
 * @param[in]  send_str       指向待发送字符串的指针。
 *
 * @return    函数执行结果，返回错误码（HY_RET_T类型），成功返回HY_RET_TRUE。
 *
 * @code
 *   int send_type = HYPRINTF_USART1;
 *   int err = hyprintf_api(send_type, "Hello, USART1!");
 *   // 根据发送类型将字符串发送至相应目的。
 * @endcode
 *
 * @note
 *   - 支持多种发送类型，用户可根据需要编写相应的接口函数。
 *   - 通过预定义的发送类型常量表示不同的发送目的，如HYPRINTF_USART1、HYPRINTF_USART2等。
 */
void hyprintf_api(int send_type,int send_size,const char *send_str){
	// 只做一次判断，请使用 if(...)else if(...)else(...) 的结构以便省去多余的判断时间
	
	// 请编写接口函数
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
 * @brief   格式化并发送字符串至默认目的，如USART1。
 *
 * @details 该函数用于格式化字符串并将结果发送至默认目的，例如USART1。用户可以通过调整send_type的值来选择不同的默认目的。
 *          使用预定义的发送类型常量，如HYPRINTF_USART1、HYPRINTF_USART2等。
 *					若要保护接口，可定义宏HYPRINTF_PROTECT，以使用hyprintf_centre函数替代hyprintf_handle。
 *
 * @param[in]  format            指向需进行格式化的字符串的指针。
 * @param[in]  ...            变长参数列表，为格式化字符串的可变参数。
 *
 * @return    函数执行结果，返回错误码（HY_RET_T类型），成功返回HY_RET_TRUE。
 *
 * @code
 *   int err = hyprintf("Formatted message: %d", 42);
 *   // 格式化并发送字符串至默认目的，如USART1。
 * @endcode
 *
 * @note
 *   - 用户可以通过调整send_type的值来选择不同的默认发送目的，使用预定义的发送类型常量，如HYPRINTF_UART1、HYPRINTF_UART2等。
 */
HY_RET_T hyprintf(const char *format,...){
	HY_RET_T err;
	
	hy_va_list p_args;
	hy_va_start(p_args,format);

	
	
	// 设定的发送标识符
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
 * @brief   初始化hyprintf库，用于检查部分宏是否正确被设置。
 *
 * @details 该函数用于初始化hyprintf库，检查各项参数和配置是否合法，确保hyprintf库能够正常工作。
 *          在初始化过程中，会检查整型字节数定义、可用的数据空间大小、Send类型个数、%后可携带参数个数、FLOAT类型的默认输出个数、
 *          以及FLOAT类型的进制数等参数，确保它们的取值在合法范围内。
 *
 * @return    函数执行结果，返回错误码（HY_RET_T类型），成功返回HY_RET_TRUE。
 *
 * @code
 *   int err = hyprintf_init();
 *   // 初始化hyprintf库，检查各项参数和配置是否合法。
 * @endcode
 *
 * @note
 *   - hyprintf库初始化时会检查整型字节数定义、可用的数据空间大小、Send类型个数、%后可携带参数个数、FLOAT类型的默认输出个数、
 *     以及FLOAT类型的进制数等参数，确保它们的取值在合法范围内。
 */
HY_RET_T hyprintf_init(void){
	
	// 整型的字节数定义错误
	if( (int)HY_INT_SIZE <= 0){return HY_RET_INT_ERR;}
	
	// 可选的Send类型个数超过整型的位数-1，因为整型的符号位固定用于组的构建，其余位用于表示不同的接口,(越锁标识符也占用一个位)
	if( ((int)HYPRINTF_SEND_TYPE_SIZE > (int)HY_INT_SIZE*8-1) || ((int)HYPRINTF_SEND_TYPE_SIZE<0) ){return HY_RET_ENUM_ERR;}
	
	// 查看 %后 可携带参数个数 是否大于等于1
	if( (int)HYPRINTF_PARAMETER_SIZE <= 0){ return HY_RET_PARAMETER_SIZE_ERR; }
	
	// 查看FLOAT类型的默认输出个数是否合法
	if( (int)HYPRINTF_FLOAT_FIXSIZE <= 0 ){ return HY_RET_FLOAT_SIZE_ERR; }
	
	// 查看FLOART类型的进制数是否合法
	if( (int)HYPRINTF_FLOAT_NUMERATION <= 0 ){ return HY_RET_FLOAT_NUM_ERR; }
	
	return HY_RET_TRUE;
}



/**
 * @brief   将格式化后的字符串返回至指定的字符串缓冲区。
 *
 * @details 该函数用于将格式化后的字符串返回至指定的字符串缓冲区，通过传入的参数将格式化结果写入到指定的缓冲区中。
 *          内部调用hyprintf_handle函数处理格式化，并将结果存储到ret_str中。
 *
 * @param[in]  ret_str_size   存放格式化结果的数据空间大小。
 * @param[out] ret_str        指向存放格式化结果的字符串的指针。
 * @param[in]  format            指向需进行格式化的字符串的指针。
 * @param[in]  ...            变长参数列表，为格式化字符串的可变参数。
 *
 * @return    函数执行结果，返回错误码（HY_RET_T类型），成功返回HY_RET_TRUE。
 *
 * @code
 *   char ret_str[256];
 *   int err = hyprintf_echo(256, ret_str, "Formatted message: %d", 42);
 *   // 将格式化后的字符串返回至ret_str中。
 * @endcode
 *
 * @note
 *   - 通过传入的参数将格式化结果写入到指定的缓冲区中。
 *   - 内部调用hyprintf_handle函数处理格式化，并将结果存储到ret_str中。
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
 * @brief   创建一个hyprintf组，可将一份数据轮流发出，节省内存资源并提升发送效率。
 *
 * @details 该函数用于创建一个hyprintf组，将一份数据轮流发送至组内的不同成员。组内任一成员未通过锁都将无法发送。
 *          若使能越锁功能，组可与越锁标识符绑定，此时组内所有成员都可无条件越锁。
 *
 * @param[in, out] group     指向表示hyprintf组的整数指针，用于存储组成员的发送类型标志位。
 *                            若传入的值为负数，函数将取其绝对值作为待加入的hyprintf组。
 *                            调用后，该指针的值将被置为组成员的发送类型标志位。
 * @param[in]       group_num 传递的参数个数，表示待创建的hyprintf组成员个数，待加入的成员可以是组。
 * @param[in]       ...        变长参数列表，为待创建的hyprintf组成员的发送类型。每个参数应为整数，表示发送类型的索引。
 *
 * @return    函数执行结果，返回错误码（HY_RET_T类型），成功返回HY_RET_TRUE。
 *
 * @code
 *   int printf_group = 0; // 创建hyprintf组
 *   int err = hyprintf_set_group(&printf_group, 2, 1, -6);
*   // 创建hyprintf组，其中成员1为发送类型1，成员2为发送类型2和发送类型3，此时组内成员有发送类型1、2、3。
 * @endcode
 *
 * @note
 *   - hyprintf组成员使用位掩码表示，确保使用正确的位掩码值。
 *   - 若传入的hyprintf组值为负数，函数将取其绝对值作为待加入的hyprintf组。
 *   - 组内任一成员未通过锁都将无法发送，使能越锁功能时，组可与越锁标识符绑定，使组内所有成员无条件越锁。
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
 * @brief   将指定发送组中的某些发送类型标志清零的函数。
 *
 * @details 该函数用于在指定的发送组中将某些发送类型的标志位清零，以便控制具体哪些类型的信息需要发送。
 *          支持变长参数，用户可传递多个整数表示要清零的发送类型。
 *
 * @param[in, out] group     指向表示发送组的整数指针，用于存储发送类型标志位。调用后，该指针将存储更新后的发送组标志。
 *                            若传入的值为负数，函数将取其绝对值作为待清零的发送组。
 *                            调用成功后，该指针的值将被置为更新后的发送组。
 * @param[in]       group_num 传递的参数个数，表示待清零的发送类型个数。
 * @param[in]       ...        变长参数列表，为待清零的发送类型。每个参数应为整数，表示发送类型的索引。
 *
 * @return    函数执行结果，返回错误码（HY_RET_T类型），成功返回HY_RET_TRUE。
 *
 * @code
 *   int send_group = 0xF;	// 表示此组中包含了发送类型1、2、3、4
 *   int err = hyprintf_leave_group(&send_group, 2, 1, -6);
 *   // 1-(0b)1-发送类型为1、-6->6-(0b)110-发送类型为2和3,将发送类型1、2、3从组中脱离
 * @endcode
 *
 * @note
 *   - 发送组使用位掩码表示，确保使用正确的位掩码值。
 *   - 若传入的发送组值为负数，函数将取其绝对值作为待清零的发送组。
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
 * @brief   根据指定发送类型，将格式化后的字符串发送至目的函数。
 *
 * @details 该函数用于根据指定的发送类型将格式化结果发送至目的函数。
 *          内部调用hyprintf_centre函数处理格式化，并将结果发送至指定的目的函数。
 *
 * @param[in]  send_type      发送类型，使用位掩码表示，传递给 hyprintf_centre() 函数。
 * @param[in]  format            指向需进行格式化的字符串的指针。
 * @param[in]  ...            变长参数列表，为格式化字符串的可变参数。
 *
 * @return    函数执行结果，返回错误码（HY_RET_T类型），成功返回HY_RET_TRUE。
 *
 * @code
 *   int send_type = HYPRINTF_SEND_TYPE_1 | HYPRINTF_SEND_TYPE_3; // 启用第1和第3种发送类型
 *   int err = hyprintf_by(send_type, "Formatted message: %d", 42);
 *   // 根据发送类型将格式化后的字符串发送至目的函数。
 * @endcode
 *
 * @note
 *   - 发送类型使用位掩码表示，确保使用正确的位掩码值。
 *   - 内部调用hyprintf_centre函数处理格式化，并将结果发送至指定的目的函数。
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
 * @brief   该文件包含了一个居中处理的格式化输出函数。
 *
 * @details 该函数用于格式化输出字符串，并根据指定的发送类型将结果居中处理后发送至不同的接口。
 *          支持互斥锁功能，防止同时发送相同类型的信息。可选地启用越锁标识符功能以支持特殊处理。
 *
 * @param[in]  send_type      发送类型，使用位掩码表示，传递给 hyprintf_handle() 函数。
 * @param[in]  ret_str_size   存放格式化结果的数据空间大小。
 * @param[out] ret_str        指向存放格式化结果的字符串的指针。
 * @param[in]  format            指向需进行格式化的字符串的指针。
 * @param[in]  p_args         指向可变参数类型的指针。
 *
 * @return    函数执行结果，返回错误码（HY_RET_T类型），成功返回HY_RET_TRUE。
 *
 * @code
 *   int send_type = HYPRINTF_SEND_TYPE_1 | HYPRINTF_SEND_TYPE_3; // 启用第1和第3种发送类型
 *   char ret_str[256];
 *   int err = hyprintf_centre(send_type, 256, ret_str, "Centered message: %d", 42);
 *   // 居中处理后发送至启用的发送类型。
 * @endcode
 *
 * @note
 *   - 发送类型使用位掩码表示，确保使用正确的位掩码值。
 *   - 互斥锁功能可防止同时发送相同类型的信息。
 *	 - 如果使用在线程抢占的运行环境中，请确保正确配置进入临界区和退出临界区的函数
 *   - 如果启用越锁标识符功能，。
 */
HY_RET_T hyprintf_centre(int send_type,int ret_str_size,char *ret_str,const char *format,hy_va_list *p_args){
	
	// 将负数类型转为对应的正数位掩码
	if( send_type<0 ){ send_type = -send_type; }
	else{ send_type = 1<<send_type; }
	
	// 静态变量，表示互斥锁的状态
	// 由于用整型的符号位表示此发送类型为组，因此32位至多可配备31个锁
	// 倘若使能了越锁功能，则至多配备30个锁
	static int mutex_flag = 0;
	
	// 如果启用越锁功能
	#ifdef HYPRINTF_JUMP_FLAG
	if( ( send_type&(1<<HYPRINTF_JUMP) )==0 ){
	#endif
		
		// 进入临界区
		hy_enter_critical();
		// 如果互斥，返回上锁错误码
		if( send_type & mutex_flag ){ 
			#ifndef HYPRINTF_PROTECT
			hyprintf("\nThe current channel is locked, and the current lock value is %#b",mutex_flag);
			#endif
			hy_exti_critical();
			return HY_RET_LOCKED;
		}
		// 此处存在时间窗口，如果判别后出现线程抢占，将使互斥锁失效，因此需配置进入临界区和退出临界区的函数
		// 上锁
		mutex_flag |= send_type; 
		// 退出临界区
		hy_exti_critical();
		
	#ifdef HYPRINTF_JUMP_FLAG
	}
	#endif
	
	// 执行格式化处理
	HY_RET_T err = hyprintf_handle(send_type,ret_str_size,ret_str,format,p_args);
	
	// 如果启用越锁功能
	#ifdef HYPRINTF_JUMP_FLAG
	if( ( send_type&(1<<HYPRINTF_JUMP) )==0 ){
	#endif
	
		// 解锁
		mutex_flag &= ~send_type;
	
	#ifdef HYPRINTF_JUMP_FLAG
	}
	#endif
	
	// 成功返回HY_RET_TRUE(1)，错误返回错误类型
	return err;
}



/**
 * @file    hyprintf.c
 * @brief   该文件包含了一个类似于hyprintf的C函数，根据发送类型指向不同的接口，将格式化结果hyprintf至不同的接口中。
 *
 * @details 该函数根据发送类型指向不同的接口，将格式化结果hyprintf至不同的接口中。
 * 
 * @param[in]  send_flag      发送类型，使用位掩码表示，传递给 hyprintf_send() 函数。
 * @param[in]  ret_str_size   存放格式化结果的数据空间大小。
 * @param[in]  ret_str        指向存放格式化结果的字符串的指针。
 * @param[in]  format            指向需进行格式化的字符串的指针。
 * @param[in]  p_args         指向可变参数类型的指针。
 *
 * @return   固定返回 HY_RET_TRUE(1)。
 *
 * @code
 *   int send_flag = 0;
 *   char ret_str[3];
 *   int err = hyprintf_handle(send_flag, 3, ret_str, ":\n%d", 123456789);
 *   //  send_flag为0，表示此整型里没有任何一个位为1。
 *   //  分为5次发送，"12" + "34" + "56" + "78" + "9"。
 * @endcode
 *
 * @note
 *   - HYPRINTF_PARAMETER_SIZE('%'后可携带的最大信息个数)不得小于等于0。
 *   - ret_str_size不得小于等于1，因为数组空间里的最后一个字节固定填充'\0'。
 *   - 当前索引值等于可用数据空间减一时，将调用 hyprintf_send() 执行一次发送。
 *   - 当没有定义 HY_UNSIGNED_INT(使能无符号整型的标识符)时，替换为用 int 接收参数。
 */
HY_RET_T hyprintf_handle(\
	int send_flag,int ret_str_size,char *ret_str,\
	const char *format,hy_va_list *p_args){
	
	// 可用的数组空间不得小于等于1，否则将返回错误，错误类型为HY_RET_ARG_ERR。
	if( ret_str_size<=1 ){ return HY_RET_RAM_ERR; }
	// 先执行减一操作，以便后续判断不需要此计算过程
	ret_str_size = ret_str_size-1;
	
	// 指向存放格式化结果的字符数组指针
	char *ret_str_ptr = ret_str;
	// 用于表示当前索引值
	int	ret_str_idx=0;
	
	// 指向需进行格式化的字符串指针
	const char *str_ptr = format;
	
	// 当 当前指针指向的值不为空字符时
	while( *str_ptr!='\0' ){
		
		// 如果当前指针指向的值为'%'
		if( *str_ptr == '%' ){
			
			char parameter_len = HYPRINTF_PARAMETER_SIZE;		// '%'后可携带的最大信息个数
			int 	parameter_flag=0;													// 带参标志位
			char 	parameter_sign=' ';												// 默认填充,带参符号部分
			int 	parameter_num=0;													// 带参数字1
			int	 	parameter_num2=0;													// 带参数字2

			// 可退出循环的方式有
			// *str_ptr == '\0'											, 指向空字符，将直接goto出大循环
			// *str_ptr == 'c/s/d/f/x/X/f/p/u/b/o'	, 打印类型
			// *str_ptr == '%'											, 指向'%'
			// *str_ptr == other										, 直接退出
			while( parameter_len-- ){
				
				// 指向后一个参数
				str_ptr++;
				
				if( *str_ptr == '\0' ){goto strover;}
				
				// 如果为打印类型
				if( *str_ptr == 'c' | *str_ptr == 's' | *str_ptr == 'S' |\
						*str_ptr == 'd' | *str_ptr == 'f' | *str_ptr == 'x' | *str_ptr == 'X' |\
						*str_ptr == 'p' | *str_ptr == 'u' | *str_ptr == 'b' | *str_ptr == 'o' ){
					
					// 存放进制数、无符号整型、浮点类型以及整数的阶数(若接收的整数为123456，则此值为100000)
					char numeration=0;
					hy_uint temp_u=0;
					hy_float temp_f=0;
					hy_uint int_power=1;

					// 如果为数字转字符的打印类型
					if( *str_ptr != 'c' && *str_ptr != 's' && *str_ptr != 'S' ){
						
						// 填写进制数，使能对应的标识符
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

						// 根据打印类型，接收参数
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
						
						// 若无法正常使用无符号整型
						#ifndef HY_UNSIGNED_INT
							temp_d = temp_u;
						#endif
						
						// int_power临时用于累计格式化后字符大小，后将此临时值传给temp_d
						// 默认为1是因为整数个数一定会占用1个
						int_power=1;
						
						// 如果该数为负数，则打印负号并将其值的翻转为正数传给无符号整型，
						// 否则如果使能了强制输出符号并且接收值大于0，则填充正号
						// 在此判断中，整数部分最终都将用temp_u表达
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
						
						// 此时将int_power存放的临时值传给temp_d
						temp_d=int_power;
						
						// 累计前缀的数量
						if( parameter_flag&HYPRINTF_WELL_FLAG ){
							if( numeration==16 || numeration==2 ){
								temp_d=temp_d+2;
							}
							else if( numeration==8 ){
								temp_d++;
							}
						}
						
						// 累计整数的个数，并算出整数部分的阶数存储在int_power( 若接收的整数为123456，则此值为100000 )
						int_power=1;
						while( temp_u/int_power>=numeration ){
							temp_d++;
							int_power = int_power*numeration;
						}
						
						// 累计浮点数的个数
						if( *str_ptr == 'f' ){
							// 当打印类型为浮点数时，接收的带参数据2表示为需打印的小数个数
							// 当小数个数大于最大有效个数，则用最大有效个数表示
							if( parameter_num2>(int)HYPRINTF_FLOAT_FIXSIZE || parameter_num2==0 ){ parameter_num2=(int)HYPRINTF_FLOAT_FIXSIZE; }
							temp_d = temp_d+1+parameter_num2;
						}
						
						// 带参数字1：若此次'%'格式化后的字符个数大于等于此值，则无效；
						// 反之，空余部分将用特定字符填充，
						if( parameter_num>temp_d ){
							parameter_num = parameter_num-temp_d; 
						}
						else{
							parameter_num = 0;
						}
					} // *str_ptr != 'c'/'s'
					
					// 默认靠右，进行左填充
					if( (parameter_flag&HYPRINTF_LEFT_FLAG)==0 && *str_ptr!='S' ){
						// 若为打印类型为字符类型，则只需留存一个字节空间
						if( *str_ptr=='c' ){ if(parameter_num>=1)parameter_num--; }
						while(parameter_num--){
							ret_str_idx++;*(ret_str_ptr++) = parameter_sign;
							if( ret_str_idx>=ret_str_size )
								{ *(ret_str_ptr)='\0';hyprintf_send(&send_flag,&ret_str_idx,ret_str);ret_str_idx=0;ret_str_ptr=ret_str; }
						}
					} // if( (parameter_flag&HYPRINTF_LEFT_FLAG)==0 ){

					// 填充字符类型
					if( *str_ptr == 'c' ){
						ret_str_idx++;*(ret_str_ptr++)=(char)hy_va_arg(*p_args,int);
						if( ret_str_idx>=ret_str_size )
							{ *(ret_str_ptr)='\0';hyprintf_send(&send_flag,&ret_str_idx,ret_str);ret_str_idx=0;ret_str_ptr=ret_str; }
					}
					// 填充字符串，若给定有带参数字，
					else if( *str_ptr == 's'){
						const char *temp_s = hy_va_arg(*p_args,const char *);
						while( *temp_s != '\0' ){
							ret_str_idx++;*(ret_str_ptr++)=*(temp_s++);
							if( ret_str_idx>=ret_str_size )
								{ *(ret_str_ptr)='\0';hyprintf_send(&send_flag,&ret_str_idx,ret_str);ret_str_idx=0;ret_str_ptr=ret_str; }
							// 如果parameter_num不为默认值0，进行减一，然后再次判断是否为0
							// 则表示此次格式化只需输出前parameter_num个字符
							if( parameter_num2!=0 ){
								parameter_num2--;
								if( parameter_num2==0 ){ break; }
							}
						}
					}
					// 填充字符数据，输出数组的大小由 parameter_num2 决定
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
					// 本次的打印类型为数字转字符的类型
					else{
						// 打印前缀,(二进制前缀："0b",八进制前缀:"0",十六进制前缀："0x"\"0X"(大写))
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
						
						// 打印整数部分
						int temp_d;
						if( parameter_flag & HYPRINTF_CASE_FLAG ){ temp_d=55; }
						else{ temp_d=87;} // 默认小写
						while( int_power ){
							if(temp_u/int_power >= 10){
								ret_str_idx++;*(ret_str_ptr++)= temp_u/int_power+temp_d;
							}else{
								ret_str_idx++;*(ret_str_ptr++)= temp_u/int_power+48;
							}
							temp_u = temp_u - int_power*(temp_u/int_power); //其中 (temp_d/t_len) 为约数
							int_power = int_power/numeration;
							if( ret_str_idx>=ret_str_size )
								{ *(ret_str_ptr)='\0';hyprintf_send(&send_flag,&ret_str_idx,ret_str);ret_str_idx=0;ret_str_ptr=ret_str; }
						} // while( int_power ){
						
						// 如果为浮点数，则直接加点以及根据浮点数量添加小数
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
					
					// 使能靠左，进行右填充
					if( parameter_flag&HYPRINTF_LEFT_FLAG && *str_ptr!='S' ){
						if( *str_ptr=='c' ){ if(parameter_num>=1)parameter_num--; }
						while(parameter_num--){
							ret_str_idx++;*(ret_str_ptr++)=parameter_sign;
							if( ret_str_idx>=ret_str_size )
								{ *(ret_str_ptr)='\0';hyprintf_send(&send_flag,&ret_str_idx,ret_str);ret_str_idx=0;ret_str_ptr=ret_str; }
						}
					} // if( parameter_flag&HYPRINTF_LEFT_FLAG ){

					// 此次'%'格式化完成，退出 while( parameter_len-- )
					break;
				} // if( *str_ptr == 'c'/'s'/...dxXpfubo
				else if( *str_ptr == '-' ){
					// 数据置左，右填充
					parameter_flag |= HYPRINTF_LEFT_FLAG;
				}
				else if( *str_ptr == '+' ){
					// 强制输出正负号
					parameter_flag |= HYPRINTF_SIGN_FLAG;
				}
				else if( *str_ptr == '.' ){
					// 用于接收带参数字2
					parameter_flag |= HYPRINTF_NUM2_FLAG;
				}
				else if( *str_ptr == '#' ){
					// 用于输出二进制、八进制、十六进制的前缀
					parameter_flag |= HYPRINTF_WELL_FLAG;
				}
				else if( *str_ptr >= '0' && *str_ptr <= '9' ){
					// 如果第一个接收的数字字符为0，则表示用'0'进行空位填充
					if( *str_ptr == '0' && (parameter_flag&HYPRINTF_ZERO_FLAG)==0 ){
						parameter_sign = *str_ptr;
					}
					// 如果接收到小数点'.'，则后续读的数据将作为带参数字2
					else if( parameter_flag & HYPRINTF_NUM2_FLAG ){
						if( parameter_num2==0 ){ parameter_num2 = (*str_ptr-48); }
						else{ parameter_num2 = parameter_num2*10+(*str_ptr-48); }
					}
					// 在接收到小数点前，前面读的数字均作为带参数据1
					else{
						if( parameter_num==0 ){ parameter_num = (*str_ptr-48); }
						else{ parameter_num = parameter_num*10+(*str_ptr-48); }
					}
					// 使能不用'0'填充空位
					parameter_flag |= HYPRINTF_ZERO_FLAG;
				} // else if( *temp_str >= '0' && *temp_str <= '9' ){
				else if( *str_ptr == '%' ){
					// 如果接收到'%'，则直接输出'%'，并退出此次格式化记录
					ret_str_idx++;*(ret_str_ptr++)='%';
					if( ret_str_idx>=ret_str_size )
						{ *(ret_str_ptr)='\0';hyprintf_send(&send_flag,&ret_str_idx,ret_str);ret_str_idx=0;ret_str_ptr=ret_str; }
					break;
				} // else if( *str_ptr == '%' ){
				else{
					// 其他的数据都将直接退出此次格式化，并且不会输出任何内容
					break;
				} // *str_ptr == other
			}	// while( parameter_len-- ){ 出循环条件 parameter_len递减为0 或 if(*temp_str == d/x/%/'\0' ) 或 越界
		}	// if( *str_ptr == '%' ){
		else{
			// 指向的字符串不为'%'/'\0'，而是其他字符，将直接填充进结果数组内
			ret_str_idx++;*(ret_str_ptr++)=*str_ptr;
			if( ret_str_idx>=ret_str_size )
				{ *(ret_str_ptr)='\0';hyprintf_send(&send_flag,&ret_str_idx,ret_str);ret_str_idx=0;ret_str_ptr=ret_str; }
		} // if( *str_ptr ！= '%'/'\0' ){	

		// 指向下一个字符
		str_ptr++;
	} // while( *str_ptr!='\0' ){
	
	// 此标识符用于循环中指向的字符为'\0'，直接跳出循环
	strover:
	// 若当前索引值不为0，则表示仍有数据未发送
	if( ret_str_idx!=0 ){
		*(ret_str_ptr)='\0';
		hyprintf_send(&send_flag,&ret_str_idx,ret_str);
	}
	
	// 固定返回HY_RET_TRUE(1)
	return HY_RET_TRUE;
}



/**
 * @file    hyprintf.c
 * @brief   该文件包含了发送格式化字符串的通用函数实现。
 *
 * @details 该函数用于发送格式化字符串，支持通过不同的发送类型选择发送目标。
 *           当多个发送类型同时被激活时，将按位顺序发送至对应的API，如果所有发送类型均未激活，
 *           则默认通过串口1（USART1）发送字符串。
 *
 * @param   send_flag  发送类型标志，使用位掩码表示，每一位对应一种发送类型。
 *                      例如，若想同时激活第1和第3种发送类型，将send_flag设置为：(1 << 0) | (1 << 2)。
 * @param   send_size  待发送的格式化字符串
 * @param   send_str   待发送的格式化字符串
 *
 * @return  函数执行结果，返回错误码（HY_RET_T类型），成功返回HY_SUCCESS。
 *
 * @code
 *   int send_flag = (1 << 0) | (1 << 2); // 激活第1和第3种发送类型
 *   const char *message = "Hello, World!";
 *   HY_RET_T result = hyprintf_send(&send_flag, message);
 *   // 处理函数执行结果
 * @endcode
 *
 * @note    注意事项：
 *          - 该函数依赖于hyprintf_api和Usart_SendString函数，确保这两个函数的正确实现和可用性。
 *          - send_flag参数使用位掩码表示不同的发送类型，确保使用正确的位掩码值。
 */
void hyprintf_send(int *send_flag,int *send_size,const char *send_str){
	for( int temp_i=0; temp_i<(int)HYPRINTF_SEND_TYPE_SIZE ;temp_i++ ){
		if( *send_flag & (1<<temp_i) ){
			hyprintf_api(temp_i,*send_size,send_str);		// 将数据传至API
		}
	}
}
