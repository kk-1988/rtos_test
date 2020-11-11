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

/*
*  参考资料《STM32F10XX Cortex-M3 programming manual》4.4.3，百度搜索“PM056”即可找到这个文档
*  在Cortex-M中，内核外设SCB的地址范围为：0xE000ED000-0xE000ED3F
*  0xE000ED008为SCB外设中SCB_VTOR这个寄存器的地址，里面存放的是向量表的起始地址，即MSP的地址
*/
__asm prvStartFirstTask( void )
{
	PRESERVE8
	/*
	* 在Cortex-M中，0xE000ED08是SCB_VTRO这个寄存器的地址，
			里面存放的是向量表的起始地址，即MSP的地址 */
			ldr r0, = 0xE000ED08
			ldr r0, [r0]
			ldr r0, [r0]
			
			/* 设置主堆栈指针msp的值 */
			msr msp, r0		/* 主栈的栈顶指针 */
			
			/* 使能全局中断 */
			cpsie i
			cpsie f
			dsb
			isb
			
			/* 调用SVC去启动第一个任务 */
			svc 0
			nop
			nop
}	

__asm void xPortPendSVHandler( void )
{
	extern pxCurrentTCB;
	extern vTaskSwitchContext;
	
	PRESERVE8
	
	/*
	* 当进入PendSVC Handler时，上一个任务运行的环境即：
	* xPSR,PC(任务入口地址)，R14,R12,R3,R2,R0(任务的形参)
	* 这些CPU寄存器的值会自动保存到任务的栈中，剩下的R4-R11需要手动保存 
	* 
	*/
	/* 获取任务栈指针到r0 */
	mrs r0, psp
	isb
	
	ldr r3, = pxCurrentTCB	/* 加载oxCurrentTCB的地址到r3 */	
}