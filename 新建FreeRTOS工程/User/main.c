
void delay(uint32_t count)
{
	for(; count!=0; count--);
}

//defined a task
void Task1_Entry(void *p_arg)
{
	for(;;)
	{
		flag1 = 1;
		vTaskDelay( 2 );
		flag = 0;
		vTaskDelay( 2 );
		taskYIELD();
	}
}

void Task2_Entry(void *p_arg)
{
	for(;;)
	{
		flag2 = 1;
		taskYIELD();
	}
}

StackType_t IdleTaskStack[configMINIMAL_STACK_SIZE];
TCB_t IdleTaskTCB;
void vApplicationGertIdleTaskMemory( TCB_t **ppxIdleTaskTCBBuffer,
																		); 

//vTaskDelay(2)
int main(int argc,char *argv[])
{
	return 0;
}