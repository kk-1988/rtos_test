#include "list.h"
#include "portmacro.h"
#include "task.h"

/*
* ȫ��
*/
portCHAR flag1;
portCHAR flag2;

//�����������������ջ(task1,task2)
//TaskHandle_t Task1_Handle;
#define TASK1_STACK_SIZE	20
StackType_t Task1Stack[TASK1_STACK_SIZE];

//TaskHandle_t Task2_Handle;
#define TASK2_STACK_SIZE	20
StackType_t Task2Stack[TASK2_STACK_SIZE];

TCB_t Task1TCB;		//������ƿ�1(���������ջָ�룬�������ƣ�������βε�)
TCB_t Task2TCB;		//������ƿ�2

//����������
TaskHandle_t Task1_Handle;
TaskHandle_t Task2_Handle;

//����һ��������
void delay (uint32_t count);
void Task1_Entry(void *p_arg);
void Task2_Entry(void *p_arg);

extern List_t pxReadyTasksLists[ configMAX_PRIORITIES ];

int main(void)
{
	prvInitialiseTaskLists();	
	
	//��������1
	Task1_Handle = xTaskCreateStatic(Task1_Entry, 
									  "Task1", 
									  TASK1_STACK_SIZE, 
									  NULL,
									  Task1Stack,		/* ����ջ��ʼ��ַ */
									  &Task1TCB);
	//���ĺ�����vListInsert����������ƿ���б����������б���  
	vListInsert(&pxReadyTasksLists[1], &Task1TCB.xStateListItem);								  
									  
	//��������2						
	Task2_Handle = xTaskCreateStatic(Task2_Entry, 
									  "Task2", 
									  TASK2_STACK_SIZE, 
									  NULL,
									  Task2Stack,		/* ����ջ��ʼ��ַ */
									  &Task2TCB);								  
	vListInsert(&pxReadyTasksLists[2], &Task2TCB.xStateListItem);
	
	//����������
	vTaskStartScheduler();
	
	for(;;)
	{
		/* ɶ�¶����� */	
	}
	
	return 0;
}

//��ʱ����
void delay(uint32_t count)
{
	for(; count!=0;count--);	
}

//����һ��������
void Task1_Entry(void *p_arg)
{
	while(1)
	{
		//todo
		flag1 = 1;
		delay(100);
		flag1 = 0;
		delay(100);
		taskYIELD();
	}
}

//����һ��������
void Task2_Entry(void *p_arg)
{
	while(1)
	{
		//todo
		flag2 = 1;
		delay(100);
		flag2 = 0;
		delay(100);
		taskYIELD();
	}
}

StackType_t IdleTaskStack[configMINIMAL_STACK_SIZE];
TCB_t IdleTaskTCB;
