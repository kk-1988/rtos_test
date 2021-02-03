#include "FreeRTOS.h"
#include "task.h"

/* 任务就绪列表 */
List_t pxReadyTasksLists[ configMAX_PRIORITIES ];

static TaskHandle_t xIdleTaskHandle		= NULL;

static List_t xDelayedTaskList1;
static List_t xDelayedTaskList2;
static List_t * volatile pxDelayedTaskList;
static List_t * volatile pxOverflowDelayedTaskList;

static volatile UBaseType_t uxCurrentNumberOfTasks = ( UBaseType_t ) 0U;
static UBaseType_t uxTaskNumber		= ( UBaseType_t ) 0U;

TCB_t * volatile pxCurrentTCB = NULL;

static volatile TickType_t xTickCount	= ( TickType_t ) 0U;
static volatile UBaseType_t uxTopReadyPriority = tskIDLE_PRIORITY;


/*
**************************************************
* 										宏定义
**************************************************
*/
/* 将任务添加到就绪列表 */
#define prvAddTaskToReadyList( pxTCB )
	taskRECORD_READY_PRIORITY( ( pxTCB )->uxPriority );
	vListInsertEnd( &( pxReadyTaskLists[ ( pxTCB )->uxPriority ], &( ( pxTCB )->xStateListItem ) );	\
	
/* 查找最高优先级的就绪任务：通用方法 */
#if ( configUSE_PORT_OPTIMISED_TASK_SELECTION == 0 )
	/* uxTopReadyPriority 存的是就绪任务的最高优先级 */
	#define taskRECORD_READY_PRIORITY( uxPriority )					\
	{																								\
		if( ( uxPriority ) > uxTopReadyPriority )							\
		{																							\
			uxTopReadyPriority = ( uxPriority );
		}																							\
	}/* taskRECORD_READY_PRIORITY */																							\
	
	#define taskSELECT_HIGHEST_PRIORITY_TASK()
	{																								\	
		UBaseType_t uxTopPriority = uxTopReadyPriority；
		
		/* 寻找包含就绪任务的最高优先级的队列 */
		while( listLIST_IS_EMPTY( &( pxReadyTasksLists[ uxTopPriority ] ) ) )
		{																							\
			--uxTopPriority;																\
		}																							\																							\
	
	/* 获取优先级最高的就绪任务的TCB，然后更新到pxCurrentTCB */
	listGET_OWNER_OF_NEXT_ENTRY( pxCurrentTCB, &( pxReadyTasksLists[ uxTopPriority ] ) );		\
	/* 更新uxTopReadyPriority */
	uxTopReadyPriority = uxTopPriority;
	}	/* taskSELECT_HIGHEST_PRIORITY_TASK */
	
	/*-----------------------------------------------*/
	
	/* 这两个宏定义只有在选择优化方法时采用，这里定义位空 */
	#define taskRESET_READY_PRIORITY( uxPriority )
	#define portRESET_READY_PRIORITY( uxPriority, uxTopReadyPriority )

/* 查找最高优先级的就绪任务：根据处理器架构优化后的方法 */
#else	/* configUSE_PORT_OPTIMISED_TASK_SELECTION */	
	#define taskRECORD_READY_PRIORITY( uxPriority ) portRECORD_READY_PRIORITY( uxPriority, uxTopReadyPriority )
	
	/*-----------------------------------------------------*/
	
	#define taskSELECT_HIGHEST_PRIORITY_TASK()
	{
		UBaseType_t uxTopPriority;
	
		/* 寻找最高优先级 */
		portGET_HIGHEST_PRIORITY( uxTopPriority, uxTopReadyPriority );
		/* 获取优先级最高的就绪任务的TCB，然后更新到pxCurrentTCB */
		listGET_OWNER_OF_NEXT_ENTRY( pxCurrentTCB, &( pxReadyTasksLists[ uxTopPriority ] ) );
	}/* taskSELECT_HIGHEST_PRIORITY_TASK() */
	
	)
#endif /* configUSE_PORT_OPTIMISED_TASK_SELECTION */

static void prvAddNewTaskToReadyList( TCB_t *pxNewTCB )
{
	/* 进入临界段 */
	taskENTER_CRITICAL();
	{
			/* 全局任务计数器加一操作 */
			uxCurrentNumberOfTasks++;
		
			/* 如果pxCurrentTCB为空，则将pxCurrentTCB指向新创建的任务 */
			if( pxCurrentTCB == NULL )
			{
					pxCurrentTCB = pxNewTCB;
				
				/* 如果是第一次创建任务，则需要初始化任务相关的列表 */
				if( uxCurrentNumberOfTasks == ( UBaseType_t ) 1)
				{
						/* 初始化任务相关的列表 */
						prvInitialiseTaskLists();
				}
			}
			else	/* 如果pxCurrentTCB不为空，则根据任务的优先级将pxCurrentTCB指向最高优先任务的TCB */
			{
				if( pxCurrentTCB->uxPriority <= pxNewTCB->uxPriority )
				{
					pxCurrentTCB = pxNewTCB;
				}
			}
			
			uxTaskNumber++;
			
			/* 将任务添加到就绪列表 */
			prvAddTaskToReadyList( pxNewTCB );
		}
		/* 退出临界段 */
		taskEXIT_CRITICAL();
}

/*
**************************************************
*											函数声明
**************************************************
*/
static void prvInitialiseNewTask(	TaskFunction_t pxTaskCode,	/* 任务入口 */
								const char * const pcName,		/* 任务名称，字符串形式 */
								const uint32_t ulStackDepth,	/* 任务栈大小，单位为字 */
								void * const pvParameters,		/* 任务形参 */
								TaskHandle_t * const pxCreatedTask,	/* 任务句柄 */
								TCB_t *pxNewTCB );				/* 任务控制块指针 */

#if( configSUPPORT_STATIC_ALLOCATION == 1 )
	
TaskHandle_t xTaskCreateStatic( TaskFunction_t pxTaskCode,		/* 任务入口 */
							const char * const pcName,			/* 任务名称，字符串形式 */
							const uint32_t ulStackDepth,		/* 任务栈大小，单位为字 */
							void * const pvParameters,			/* 任务形参 */
							UBaseType_t  uxPriority,				/* 任务优先级，数值越大，优先级越高 */
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
								uxPriority,		/* 任务优先级 */
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
								UBaseType_t uxPriority,				/* 任务优先级，数值越大，优先级越高 */
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
	
	/* 初始化优先级 */
	if( uxPriority >= ( UBaseType_t ) configMAX_PRIORITIES )
	{
		uxPriority = ( UBaseType_t ) configMAX_PRIORITIES - ( UBaseType_t )1U;
	}
	pxNewTCB->uxPriority = uxPriority;
	
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

static portTASK_FUNCTION( prvIdleTask, pvParameters )
{
	/* 防止编译器的警告 */
	( void )pvParameters;
	
	for(;;)
	{
		/* 空闲任务暂时什么都不做 */	
	}
}

extern void vApplicationGetIdleTaskMemory( TCB_t **ppxIdleTaskTCBBuffer,
									StackType_t **ppxIdleTaskStackBuffer,
									uint32_t *pulIdleTaskStackSize );

void vTaskStartScheduler( void )
{
	/*=============================================创建空闲任务start==========*/
	TCB_t *pxIdleTaskTCBBuffer = NULL;	/* 用于指向空闲任务控制块 */
	StackType_t *pxIdleTaskStackBuffer = NULL;	/* 用于空闲任务栈起始地址 */
	uint32_t ulIdleTaskStackSize;
	
	xIdleTaskHandle = xTaskCreateStatic( (TaskFunction_t)prvIdleTask,			/* 任务入口 */
																			 (char *)"IDLE"										/* 任务名称，字符串形式*/
																			 (uint32_t)ulIdleTaskStackSize,		/* 任务栈大小，单位为字 */
																			 (void *) NULL,										/* 任务形参 */
																			 (UBaseType_t) tskIDLE_PRIORITY,	/* 任务优先级，数值越大，优先级越高 */
																			 (StackType_t *)pxIdleTaskStackBuffer,	/* 任务栈起始地址 */
																			 (TCB_t *)pxIdleTaskTCBBuffer );	/* 任务控制块 */
																			 
	/* 将任务添加到就绪列表 */
																			 
	
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

//void vTaskSwitchContext( void )
//{
	/* 两个任务轮流切换 */
//	if( pxCurrentTCB = &Task1TCB)
//	{
//		pxCurrentTCB = &Task2TCB;
//	}
//	else
//	{
//		pxCurrentTCB = &Task1TCB;	
//	}
//}

void vTaskDelay( const TickType_t xTicksToDelay )
{
	TCB_t *pxTCB = NULL;
	
	/* 获取当前任务的TCB */
	pxTCB = pxCurrentTCB;
	
	/* 设置延时时间 */
	pxTCB->xTicksToDelay = xTicksToDelay;
	
	/* 任务切换 */
	taskYIELD();
}

void vTaskSwitchContext( void )
{
	/* 如果当前线程是空闲线程，那么就去尝试执行线程1或者线程2，
		看看他们的延时时间是否结束，如果线程的延时时间均没有到期，
		那就返回继续执行空闲线程 */
	if( pxCurrentTCB == &IdleTaskTCB )
	{
		if(Task1TCB.xTicksToDelay == 0)
		{
			pxCurrentTCB = &Task1TCB;
		}
		else if(Task2TCB.xTicksToDelay == 0)
		{
			pxCurrentTCB = &Task2TCB;
		}
		else
		{
			return;		/* 线程延时均没有到期则返回 */
		}
	}
	else
	{
		/* 如果当前线程是线程1或者线程2，检查下另外一个线程，
		* 如果另外的线程不在延时中，就切换到该线程，否则，判断下
		* 当前线程是否应该进入延时状态，如果是的话，就切换到空闲线程。否则就不进行任何切换
		*/
		if(pxCurrentTCB == &Task1TCB)
		{
			if(Task2TCB.xTicksToDelay == 0)
			{
				pxCurrentTCB = &Task2TCB;
			}
			else if(pxCurrentTCB->xTicksToDelay != 0)
			{
				pxCurrentTCB = &IdleTaskTCB;
			}
			else
			{
				return;	/* 返回，不进行切换，因为两个线程都处于延时中 */
			}
		}
		else if(pxCurrentTCB == &Task2TCB)
		{
			if(Task1TCB.xTicksToDelay == 0)
			{
				pxCurrentTCB  = &Task1TCB;
			}
			else if(pxCurrentTCB->xTicksToDelay != 0)
			{
				pxCurrentTCB = &IdleTaskTCB;
			}
			else
			{
				return;	/* 返回，不进行切换，因为两个线程都处于延时中 */
			}
		}
	}
}

void xTaskIncrementTick( void )
{
	TCB_t *pxTCB = NULL;
	BaseType_t i = 0;
	
	/* 更新系统时基计数器xTickCount,xTickCount是一个在port.c中定义的全局变量 */
	const TickType_t xConstTickCount = xTickCount + 1;
	xTickCount = xConstTickCount;
	
	/* 扫描就绪列表中所有线程的xTicksToDelay,如果不为0,则减1 */
	for(i = 0; i < configMAX_PRIORITIES; i++)
	{
		pxTCB = ( TCB_t * )listGET_OWNER_OF_HEAD_ENTRY( (&pxReadyTasksLists[i]) );
		if(pxTCB->xTicksToDelay > 0)
		{
			pxTCB->xTicksToDelay--;
			
			/* 延时时间到，将任务就绪 */
			if( pxTCB->xTicksToDelay == 0 )
			{
				taskRECORD_READY_PRIORITY( pxTCB->uxPriority );
			}
		}
	}
	
	portYIELD();
}

#define portTASK_FUNCTION( vFunction, pvParameters ) void vFunction( void *pvParameters )