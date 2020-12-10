#include "task.h"

/* 任务就绪列表 */
List_t pxReadyTasksLists[ configMAX_PRIORITIES ];

TCB_t * volatile pxCurrentTCB = NULL;

/*
**************************************************
*					函数声明
**************************************************
*/
static void prvInitialiseNewTask(	TaskFunction_t pxTaskCode,	/* 任务入口 */
								const char * const pcName,		/* 任务名称，字符串形式 */
								const uint32_t ulStackDepth,	/* 任务栈大小，单位为字 */
								void * const pvParameters,		/* 任务形参 */
								TaskHandle_t * const pxCreatedTask,	/* 任务句柄 */
								TCB_t *pxNewTCB )				/* 任务控制块指针 */

#if( configSUPPORT_STATIC_ALLOCATION == 1 )
	
TaskHandle_t xTaskCreateStatic( TaskFunction_t pxTaskCode,		/* 任务入口 */
							const char * const pcName,			/* 任务名称，字符串形式 */
							const uint32_t ulStackDepth,		/* 任务栈大小，单位为字 */
							void * const pvParameters,			/* 任务形参 */
							StackType_t * const puxStackBuffer,	/* 任务栈起始地址 */
							TCB_t * const pxTaskBuffer )		/* 任务控制块指针 */
{
	TCB_t	*pxNewTCB;
	TaskHandle_t xReturn;
	
	if( ( pxTaskBuffer != NULL ) && ( puxStackBuffer != NULL ))
	{
		pxNewTCB = ( TCB_t * )pxTaskBuffer;
		pxNewTCB->pxStack = ( StackType_t * ) puxStackBuffer;
		
		/* 创建新的任务 */
		prvInitialiseNewTask( pxTaskCode,		/* 任务入口 */
								pcName,			/* 任务名称，字符串形式 */
								ulStackDepth,	/* 任务栈大小，单位为字 */
								pvParameters,	/* 任务形参 */
								&xReturn,		/* 任务句柄 */
								pxNewTCB);		/* 任务栈起始地址 */
	}
	else
	{
		xReturn = NULL;		
	}
	
	/* 返回任务句柄，如果任务创建成功，此时xReturn应该指向任务控制块 */
	
	
	
}							

/*
* 初始化新任务接口
*/
static void prvInitialiseNewTask(	TaskFunction_t pxTaskCode,	/* 任务入口 */
								const char * const pcName,		/* 任务名称，字符串形式 */
								const uint32_t ulStackDepth,	/* 任务栈大小，单位为字 */
								void * const pvParameters,		/* 任务形参 */
								TaskHandle_t * const pxCreatedTask,	/* 任务句柄 */
								TCB_t *pxNewTCB )				/* 任务控制块指针 */
{
	StackType_t *pxTopOfStack;
	UBaseType_t x;
	
	/* 获取栈顶指针 */
	pxTopOfStack = pxNewTCB->pxStack + ( ulStackDepth - ( uint32_t ) 1 );
	/* 向下做8字节对齐 */
	pxTopOfStack = ( StackType_t * )(((uint32_t) pxTopOfStack ) & (~((uint32_t) 0x0007 )));
	
	/* 将任务的名称存储在TCB中 */
	for( x= (UBaseType_t )0; x < (uBaseType_t ) configMAX_TASK_NAME_LEN; x++)
	{
		pxNewTCB->pcTaskName[ x ] = pcName[ x ];
		
		if( pcName[ x ] = 0x00 )
		{
			break;
		}
	}
	
	/* 任务名字的长度不能超过configMAX_TASK_NAME_LEN */
	pxNewTCB->pcTaskName[ configMAX_TASK_NAME_LEN - 1 ] = '\0';
	
	/* 初始化TCB中的xStateListItem节点 */
	vListInitialiseItem( &( pxNewTCB->xStateListItem ));
	/* 设置xStateListItem节点的拥有者 */
	listSET_LIST_ITEM_OWNER( &( pxNewTCB->xStateListItem ), pxNewTCB ); 
	
	/* 初始化任务栈 */
	pxNewTCB->pxTopOfStack = pxPortInitialiseStack( pxTopOfStack, pxTaskCode, pvParameters );
	
	/* 让任务句柄指向任务控制块 */
	if( ( void * )pxCreatedTask != NULL)
	{
		*pxCreatedTask = ( TaskHandle_t ) pxNewTCB;
	}
}

/*
* 就绪列表初始化
*/
void prvInitialiseTaskLists( void )
{
	UbaseType_t uxPriority;
	
	for( uxPriority = ( UBaseType_t ) 0U; uxPriority < ( UBaseType_t )configMAX_PRIORITIES; uxPriority++)
	{
		vListInitialise( &( pxReadyTasksLists[ uxPriority ] ));
	}
}

extern TCB_t Task1TCB;
extern TCB_t Task2TCB;
void vTaskStartScheduler( void )
{
	/*=============================================创建空闲任务start==========*/
	TCB_t *pxIdleTaskTCBBuffer = NULL;	/* 用于指向空闲任务控制块 */
	StackType_t *pxIdleTaskStackBuffer = NULL;	/* 用于空闲任务栈起始地址 */
	uint32_t ulIdleTaskStackSize;
	
	/* 获取空闲任务的内存：任务栈和任务TCB */
	vApplicationGetIdleTaskMemory( &pxIdleTaskTCBBuffer,
								   &pxIdleTaskStackBuffer,
								   &ulIdleTaskStackSize );
	
	/* 手动指定第一个运行的任务，
	 * pxCurrentTCB为一个全局变量,
	 * 指向当前运行的任务。
	 */
	pxCurrentTCB = &Task1TCB;
	
	/* 启动调度器 */
	if( xPortStartScheduler() != pdFALSE )
	{
		/* 调度器启动成功，则不会返回，即不会来到这里 */
			
	}
	
}

void vTaskSwitchContext( void )
{
	/* 两个任务轮流切换 */
	if( pxCurrentTCB = &Task1TCB)
	{
		pxCurrentTCB = &Task2TCB;
	}
	else
	{
		pxCurrentTCB = &Task1TCB;	
	}
}

static portTASK_FUNCTION( prvIdleTask, pvParameters )
{
	/* 防止编译器的警告 */
	( void )pvParameters;
	
	for(;;)
	{
		/* 空闲任务暂时什么都不做 */	
	}
}