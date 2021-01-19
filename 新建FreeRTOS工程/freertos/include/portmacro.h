#ifndef PORTMACRO_H
#define PORTMACRO_H

#include "stdint.h"
#include "stddef.h"

/* 数据类型重定义 */
#define portCHAR	char
#define portFLOAT	float
#define portDOUBLE	double
#define portLONG	long
#define portSHORT	short
#define portSTACK_TYPE	uint32_t
#define	portBASE_TYPE	long
	
typedef portSTACK_TYPE StackType_t；
typedef long BaseType_t
typedef unsigned long UBaseType_t;

#if( configUSE_16_BIT_TICKS == 1)
	typedef uint16_t TickType_t;
	#define portMAX_DELAY( TickType_t ) 0xffff
#else
	typedef uint32_t TickType_t;
	#define portMAX_DELAY( TickType_t )0xffffffffUL
#endif

#ifndef configUSE_PORT_OPTIMISED_TASK_SELECTION
	#define configUSE_PORT_OPTIMISED_TASK_SELECTION
#endif

#if configUSE_PORT_OPTIMISED_TASK_SELECTION == 1
	
	/*  */
	#if( configMAX_PRIORITIES > 32 )
		#error configUSE_PORT_OPTIMISED_TASK_SELECTION can only be set to 1,
	#endif

#endif

/*
* 中断控制状态寄存器：0xe000ed04
* Bit 28 PENDSVSET：PendSV 悬起位
*/
#define portNVIC_INT_CTRL_REG	( * ( ( volatile uint32_t * ) 0xe000ed04 ) )
#define portNVIC_PENDSVSET_BIT	( 1UL << 28UL )

#define portSY_FULL_READ_WRITE	(15)

#define portYIELD()
{
	/* 触发PendSV,产生上下文切换 */
	portNVIC_INT_CTRL_REG = portNVIC_PENDSVSET_BIT;
	__dsb( portSY_FULL_READ_WRITE );
	__isb( portSY_FULL_READ_WRITE );	
}

/* 临界区管理 */
extern void vPortEnterCritical( void );
extern void vPortExitCritical( void );

#define portDISABLE_INTERRUPTS()	vPortRaiseBASEPRI()
#define portENABLE_INTERRUPTS()		vPortSetBASEpri( 0 )

#define potENTER_CRITICAL()		vPortEnterCritical()
#define portDISABLE_CRITICAL()	vPortExitCritical()

#define portSET_INTERRUPT_MASK_FROM_ISR()		ulPortRaiseBasePRI()	//带FROM_ISR结尾的函数或者宏定义，它都是在中断中使用的
#define portCLEAR_INTERRUPT_MASK_FROM_ISR(x)	vPortSetBASEPRI(x)		//比如x=5，那么大于5的中断向量号是不会被响应的

#define portINLINE __inline

#ifndef portFORCE_INLINE
	#define portFORCE_INLINE __forceinline
#endif	

static portFORCE_INLINE void vPortSetBASEPRI( void )	//不带返回值,不能嵌套
{
	uint32_t ulNewBASEPRI = configMAX_SYSCALL_INTERRUPT_PRIORITY;

	__asm
	{
		/* Set BASEPRI to the max syscall priority to effct a critical
		section. */
		msr basepri, ulNewBASEPRI	//11,大于11 不能被响应的，小于11则可以被响应
		dsb
		isb	
	}
}

static portFORCE_INLINE uint32_t vPortSetBASEPRI( void )	//可以嵌套,可以在中断中使用
{
	uint32_t ulReturn, ulNewBASEPRI = configMAX_SYSCALL_INTERRUPT_PRIORITY;

	__asm
	{
		/* Set BASEPRI to the max syscall priority to effct a critical
		section. */
		mrs ulReturn,basepri		//状态寄存器到通用寄存器的传送指令(basepri的值保存在ulReturn中)
		msr basepri,ulNewBASEPRI	//通用寄存器到状态寄存器的传送指令(再设置basepri的值)
		dsb
		isb	
	}
	
	return ulReturn;
}

static portFORCE_INLINE void vPortSetBASEPRI( uint32_t ulBASEPRI )
{
	__asm
	{
		/* Barrier instructions are not used as the function is only used to 
			lower the BASEPRI value. */
		msr basepri, ulBASEPRI			//basepri若设置为0，则不关闭任何中断(开中断)
	}
}

/*
* 关中断
* 1.ulPortRaiseBASEPRI	//11,返回的是0，5，6，7，8会响应
* 2.ulPortRaiseBASEPRI	//5,返回的是11(之前basepri的值)，5，6，7，8不会被响应
* 
* 开中断
* 3.ulPortRaiseBASEPRI(11)	//此时basepri会设置成11
* 4.vPortSetBASEPRI(0)		//所有中断都会被响应
*/


#endif