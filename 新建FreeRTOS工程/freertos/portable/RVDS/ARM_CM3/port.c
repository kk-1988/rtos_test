#include "FreeRTOS.h"
#include "task.h"
#include "ARMCM3.h"

static UBaseType_t uxCriticalNesting = 0xaaaaaaaa;

/*
* 宏定义
*/
#define portINITIAL_XPSR	( 0x01000000 )
#define portSTART_ADDRESS_MASK ( ( StackType_t ) 0xfffffffeUL )

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
	extern pxCurrentTCB;					//外部变量
	extern vTaskSwitchContext;		//外部函数
	
	PRESERVE8		//8字节对齐
	
	/*
	* 当进入PendSVC Handler时，上一个任务运行的环境即：
	* xPSR,PC(任务入口地址)，R14,R12,R3,R2,R0(任务的形参)
	* 这些CPU寄存器的值会自动保存到任务的栈中，剩下的R4-R11需要手动保存 
	* 
	*/
	/* 获取任务栈指针到r0 */
	mrs r0, psp
	isb
	
	ldr r3, = pxCurrentTCB	/* 加载pxCurrentTCB的地址到r3 */	
	ldr r2, [r3]					/* 加载pxCurrentTCB到r2 */
	
	stmdb r0!, {r4-r11}		/* 将CPU寄存器r4~r11的值存储到r0指向的地址 */
	str r0,[r2]						/* 将任务栈的新的栈顶指针存储到当前任务的TCB的第一个成员，即栈顶指针 */
	
	stmdb sp!,{r3,r14}		/* 将R13和R14临时压入堆栈，因为即将调用很函数vTaskSwitchContext,
													调用函数时，返回地址自动保存到R14中，所以一旦调用发生，R14的值会被覆盖，因为需要入栈保护;
													R3保存的当前激活的任务TCB指针(pxCurrentTCB)地址，函数调用后会用到，因此也要入栈保护 */
	mov r0, #configMAX_SYSCALL_INTERRUPT_PRIORITY	/* 进入临界段 */
	msr basepri, r0
	isb
	bx r14								/* 异常发生时，R14中保存异常返回标志，包括返回后进入线程模式还是处理器模式、使用
													PSP堆栈指针还是MSP堆栈指针，当调用bx r14指令后，硬件会知道要从异常返回，
													然后出栈，这个时候堆栈指针PSP已经指向了新任务堆栈的正确位置，
													当新任务的运行地址被出栈到PC寄存器后，新的任务也会被执行。*/
	
	nop
}

/*
**************************************************************
* 临界段相关函数
**************************************************************
*/
void vPortEnterCritical( void )
{
	portDISABLE_INTERRUPTS();
	uxCriticalNesting++;
	
	/* This is not the interrupt safe version of the enter critical function sopen
	*assert() if it is being called from an interrupt context. Only API functions 
	* that end in "FromISR" can be used in an interrupt. Only assert if the critical nesting
	* count is 1 to protect against recursive calls if the assert function also uses
	* a critical section. */
	if( uxCriticalNesting == 1 )
	{
		//configASSERT( ( portNVIC_INT_CTRL_REG & portVECTACTIVE_MASK ) == 0 );
	}
}

void vPortExitCritical( void )
{
	//configASSERT( uxCriticalNesting );
	uxCriticalNesting--;
	
	if( uxCriticalNesting == 0 )
	{
		portENABLE_INTERRUPTS();
	}
}

/* 在中断场合，临界段可以嵌套 */
{
	uint32_t ulReturn;
	/* 进入临界段，临界段可以嵌套 */
	ulReturn = taskENTER_CRITICAL_FROM_ISR();
	
	/* 临界段代码 */
	
	/* 退出临界段 */
	taskEXIT_CRITICAL_FROM_ISR( ulReturn );
	
}

/* 在非中断场合，临界段不能嵌套 */
{
	/* 进入临界段 */
	taskENTER_CIRTICAL();
	
	/* 临界段代码 */
	
	/* 退出临界段 */
	taskEXIT_CRITICAL();
} 