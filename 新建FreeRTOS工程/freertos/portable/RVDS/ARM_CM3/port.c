#include "FreeRTOS.h"
#include "task.h"
#include "ARMCM3.h"


/*
* 参考资料《STM32F10XX Cortex-M3 programming manual》4.4.3，百度搜索“PM056”即可找到这个文档
* 在Cortex-M中，内核外设SCB中SHPR3寄存器用于设置SysTick和PendSV的异常优先级
* System handler priority register 3 (SCB_SHPR3) SCB_SHPR3: 0xE000 ED20
* Bits 31:24 PRI_15[7:0]:Priority of system handler 15,SysTick exception
* Bits 23:16 PRI_14[7:0]:Priority of system handler 14,PendSV 
*
*/
#define portNVIC_SYSPRI2_REG		( * ( ( volatile uint32_t * ) 0xe000ed20))

#define portNVIC_PENDSV_PRI			( ( ( uint32_t ) configKERNEL_INTERRUPT_PRIORITY ) << 16ul )
#define portNVIC_SYSTICK_PRI	 	( ( ( uint32_t ) configKERNEL_INTERRUPT_PRIORITY ) << 24ul )

/*
*************************************************************
*												函数声明
*************************************************************
*/
void prvStartFirstTask( void );
void xPortSVCHandler( void );
void xPortPendSVHandler( void );

/*
*************************************************************
*						任务初始化函数
*************************************************************
*/
static void prvTaskExitError( void )
{
	/* 函数停止在这里 */
	for(;;);
}

StackType_t *pxPortInitialiseStack( StackType_t *pxTopOfStack, TaskFunction_t pxCode, void *pvParameters )
{
	/* 异常发生时，自动加载到CPU寄存器的内容 */
	pxTopOfStack--;
	*pxTopOfStack = portINITIAL_XPSR;	/* xPSR的bit24必须置1 */
	pxTopOfStack--;
	*pxTopOfStack = ( ( StackType_t ) pxCode ) & portSTART_ADDRESS_MASK;	/* PC,即任务入口函数 */
	pxTopOfStack--;
	*pxTopOfStack = ( StackType_t )prvTaskExitError;	/* LR,函数返回地址 */
	pxTopOfStack -= 5;	/* R12,R3,R2 and R1 默认初始化为0 */
	*pxTopOfStack = ( StackType_t )pvParameters;		/* R0,任务形参 */
	
	/* 异常发生时，手动加载到CPU寄存器的内容 */
	pxTopOfStack -= 8;	/* R11,R10,R9,R8,R7,R6,R5 and R4默认初始化为0 */
	
	/* 返回栈顶指针，此时pxTopOfStack指向空闲栈 */
	return pxTopOfStack;
}

/*
* 		调度器启动函数
*/
BaseType_t xPortStartScheduler( void )
{
	/* 配置PendSV和SysTick的中断优先级最低 */
	portNVIC_SYSPRI2_REG |= portNVIC_PENDSV_PRI;
	portNVIC_SYSPRI2_REG |= PORTNVIC_YSSTICK_PRI;
	
	/* 启动第一个任务，不再返回 */
	prvStartFirstTask();
	
	/* 不应该运行到这里 */
	return 0; 	
}