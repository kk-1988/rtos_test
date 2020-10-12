#include "list.h"
#include "portmacro.h"

/*
* 全局
*/
portCHAR flag1;
portCHAR flag2;

//定义两个任务的任务栈(task1,task2)
//TaskHandle_t Task1_Handle;
#define TASK1_STACK_SIZE	20
StackType_t Task1Stack[TASK1_STACK_SIZE];

//TaskHandle_t Task2_Handle;
#define TASK2_STACK_SIZE	20
StackType_t Task2Stack[TASK2_STACK_SIZE];

TCB_t Task1TCB;		//任务控制块1(包括任务的栈指针，任务名称，任务的形参等)
TCB_t Task2TCB;		//任务控制块2

//定义一个任务函数
void delay (uint32_t count);
void Task1_Entry(void *p_arg);
void Task2_Entry(void *p_arg);

int main(void)
{
		
	for(;;)
	{
		/* 啥事都不干 */	
	}
	
	return 0;
}

//延时函数
void delay(uint32_t count)
{
	for(; count!=0;count--);	
}

//定义一个任务函数
void Task1_Entry(void *p_arg)
{
	while(1)
	{
		//todo
		flag1 = 1;
		delay(100);
		flag1 = 0;
		delay(100);
	}
}

//定义一个任务函数
void Task2_Entry(void *p_arg)
{
	while(1)
	{
		//todo
		flag2 = 1;
		delay(100);
		flag2 = 0;
		delay(100);
	}
}