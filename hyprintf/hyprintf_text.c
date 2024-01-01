// if set HYPRINTF_USART1 and HYPRINTF_USART1 in typedef enum{...}HYPRINTF_SEND_TYPE;



void hyprintf_text_base(void){

	hyprintf("Hello,World!");

	hyprintf("\nhyprintf init!");
	
	HY_RET_T err = hyprintf_init();
	if(err!=HY_RET_TRUE){ hyprintf("\nerr is %d",err);while(1); }
	
	
	
	hyprintf("\n************************");
	
	hyprintf("\n:%d:",30);
	hyprintf("\n:%d:",-30);
	hyprintf("\n:%+d:",30);
	hyprintf("\n:%10d:",30);
	hyprintf("\n:%3d:",3000);
	hyprintf("\n:%-10d:",30);
	hyprintf("\n:%010d:",30);
	hyprintf("\n:%+-010d:",30);
	
	hyprintf("\n************************");
	
	hyprintf("\n:%b:",30);
	hyprintf("\n:%o:",30);
	hyprintf("\n:%x:",30);
	hyprintf("\n:%X:",30);
	
	hyprintf("\n************************");
	
	hyprintf("\n:%#b:",30);
	hyprintf("\n:%#o:",30);
	hyprintf("\n:%#x:",30);
	hyprintf("\n:%#X:",30);
	
	hyprintf("\n************************");
	
	hyprintf("\n:%u:",0xFFFFFFFF);
	hyprintf("\n:%u:",-30);
	hyprintf("\n:%+u:",-30);
	
	hyprintf("\n************************");
	
	hyprintf("\n:%f:",123456.123456789);
	hyprintf("\n:%.5f:",123456.123456789);
	
	hyprintf("\n************************");
	
	int temp_p=0;
	hyprintf("\n:%p:",&temp_p);
	hyprintf("\n:%08p:",&temp_p);
	
	hyprintf("\n************************");
	
	hyprintf("\n:%c:",'a');
	hyprintf("\n:%-10c:",'a');
	
	hyprintf("\n************************");
	
	hyprintf("\n:%s:","09AZaz");
	hyprintf("\n:%10s:","09AZaz");
	hyprintf("\n:%.3s:","09AZaz");
	hyprintf("\n:%-010.3s:","09AZaz");
	
	hyprintf("\n************************");

	int32_t i32[2] = {0x31323334,0x35363738};
	hyprintf("\n:%.8S:",i32);
	hyprintf("\n:%4.2S:",i32);
	int16_t i16[3] = {0x3132,0x3334,0x3536};
	hyprintf("\n:%2.2S:",&i16[1]);

	hyprintf("\n************************");
	
}


	
void hyprintf_text_echo(void){

	hyprintf("\n************************");
	
	char ret_str[16];
	hyprintf_echo(16,ret_str,"%s","Hello World!");
	hyprintf("\n:%s:",ret_str);
	
	char ret_str2[4];
	hyprintf_echo(4,ret_str2,"%s","Hello World!");
	hyprintf("\n:%s:",ret_str2);

	hyprintf("\n************************");

}



void hyprintf_text_group(void){

	hyprintf("\n************************");
	
	hyprintf_by(HYPRINTF_UART1,"\nU1:%s:","Hello World!");
	hyprintf_by(HYPRINTF_UART2,"\nU2:%s:","Hello World!");
	
	int HYPRINTF_UART12=0;
	err = hyprintf_set_group(&HYPRINTF_UART12, 2, HYPRINTF_UART1, HYPRINTF_UART2);
	if(err!=HY_RET_TRUE){ hyprintf("\nerr is %d",err);while(1); }
	else{ hyprintf("\ngroup is %d=-%#b",HYPRINTF_UART12,-HYPRINTF_UART12); }

	hyprintf_by(HYPRINTF_UART12,"\nset U1+U2:%s:","Hello World!");
	
	err = hyprintf_leave_group(&HYPRINTF_UART12, 1, HYPRINTF_UART2);
	if(err!=HY_RET_TRUE){ hyprintf("\nerr is %d",err);while(1); }
	else{ hyprintf("\ngroup is %d=-%#b",HYPRINTF_UART12,-HYPRINTF_UART12); }

	hyprintf_by(HYPRINTF_UART12,"\nleave U2:%s:","Hello World!");
	
	hyprintf("\n************************");

}



void hyprintf_text_jump(void){

	// 若在 hyprintf_centre() 中注释解锁程序 "mutex_flag &= ~send_type;" ，会在在完成第一次发送后，无法进行第二次发送

	hyprintf("\n************************");
	
	hyprintf_by(HYPRINTF_UART1,"\nU1-1:%s:","Hello World!");
	hyprintf_by(HYPRINTF_UART1,"\nU1-2:%s:","Hello World!");
	
	int HYPRINTF_UART1J=0;
	err = hyprintf_set_group(&HYPRINTF_UART1J, 2, HYPRINTF_UART1, HYPRINTF_JUMP);
	/*
		// this way is error.
		int HYPRINTF_USART1J = HYPRINTF_USART1;
		hyprintf_set_group(&HYPRINTF_USART1J, 1, HYPRINTF_JUMP);
	*/
	hyprintf_by(HYPRINTF_UART1J,"\nU1-3:%s:","Hello World!");
	
	// 默认不保护hyprintf的接口，因此可以完成发送
	hyprintf("\nU1-4:%s:","Hello World!");
	
	int HYPRINTF_UART12=0;
	hyprintf_set_group(&HYPRINTF_UART12, 2, HYPRINTF_UART1, HYPRINTF_UART2);
	hyprintf_by(HYPRINTF_UART12,"\nU1+U2-5:%s:","Hello World!");
	
	hyprintf_set_group(&HYPRINTF_UART12, 1, HYPRINTF_JUMP);
	hyprintf_by(HYPRINTF_UART12,"\nU1+U2-6:%s:","Hello World!");
	
	hyprintf_leave_group(&HYPRINTF_UART12,1,HYPRINTF_JUMP);
	hyprintf_by(HYPRINTF_UART12,"\nU1+U2-7:%s:","Hello World!");
	
	hyprintf("\n************************");

}
