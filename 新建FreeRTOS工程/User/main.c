#include "list.h"
#include "portmacro.h"
#include "task.h"

portCHAR flag1;
portCHAR flag2;

//TaskHandle_t Task1_Handle;
#define TASK1_STACK_SIZE	20
StackType_t Task1Stack[TASK1_STACK_SIZE];

//TaskHandle_t Task2_Handle;
#define TASK2_STACK_SIZE	20
StackType_t Task2Stack[TASK2_STACK_SIZE];

TCB_t Task1TCB;	
TCB_t Task2TCB;	

TaskHandle_t Task1_Handle;
TaskHandle_t Task2_Handle;

void delay (uint32_t count);
void Task1_Entry(void *p_arg);
void Task2_Entry(void *p_arg);

extern List_t pxReadyTasksLists[ configMAX_PRIORITIES ];

int main(void)
{
	Task1_Handle = xTaskCreateStatic(Task1_Entry, 
									  "Task1", 
									  TASK1_STACK_SIZE, 
									  NULL,
									  Task1Stack,	
									  &Task1TCB);

	vListInsert(&pxReadyTasksLists[1], &Task1TCB.xStateListItem);								  
									  			
	Task2_Handle = xTaskCreateStatic(Task2_Entry, 
									  "Task2", 
									  TASK2_STACK_SIZE, 
									  NULL,
									  Task2Stack,		/* ??????? */
									  &Task2TCB);								  
	vListInsert(&pxReadyTasksLists[2], &Task2TCB.xStateListItem);
	
	for(;;)
	{

	}
	
	return 0;
}

void delay(uint32_t count)
{
	for(; count!=0;count--);	
}

void Task1_Entry(void *p_arg)
{
	while(1)
	{
		//todo
		flag1 = 1;
		vTaskDelay(2);
		flag1 = 0;
		vTaskDelay(2);
	}
}

void Task2_Entry(void *p_arg)
{
	while(1)
	{
		//todo
		flag2 = 1;
		vTaskDelay(2);
		flag2 = 0;
		vTaskDelay(2);
	}
}