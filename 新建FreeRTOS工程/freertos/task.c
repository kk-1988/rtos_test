#include "task.h"

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