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
	
	
}