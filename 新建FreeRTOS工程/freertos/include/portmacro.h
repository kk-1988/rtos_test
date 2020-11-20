#ifndef PORTMACRO_H
#define PORTMACRO_H

#include "stdint.h"
#include "stddef.h"

/* ���������ض��� */
#define portCHAR	char
#define portFLOAT	float
#define portDOUBLE	double
#define portLONG	long
#define portSHORT	short
#define portSTACK_TYPE	uint32_t
#define	portBASE_TYPE	long
	
typedef portSTACK_TYPE StackType_t��
typedef long BaseType_t
typedef unsigned long UBaseType_t;

#if( configUSE_16_BIT_TICKS == 1)
	typedef uint16_t TickType_t;
	#define portMAX_DELAY( TickType_t ) 0xffff
#else
	typedef uint32_t TickType_t;
	#define portMAX_DELAY( TickType_t )0xffffffffUL
#endif

/*
* �жϿ���״̬�Ĵ�����0xe000ed04
* Bit 28 PENDSVSET��PendSV ����λ
*/
#define portNVIC_INT_CTRL_REG	( * ( ( volatile uint32_t * ) 0xe000ed04 ) )
#define portNVIC_PENDSVSET_BIT	( 1UL << 28UL )

#define portSY_FULL_READ_WRITE	(15)

#define portYIELD()
{
	/* ����PendSV,�����������л� */
	portNVIC_INT_CTRL_REG = portNVIC_PENDSVSET_BIT;
	__dsb( portSY_FULL_READ_WRITE );
	__isb( portSY_FULL_READ_WRITE );	
}

/* �ٽ������� */
extern void vPortEnterCritical( void );
extern void vPortExitCritical( void );

#define portDISABLE_INTERRUPTS()	vPortRaiseBASEPRI()
#define portENABLE_INTERRUPTS()		vPortSetBASEpri( 0 )

#define portSET_INTERRUPT_MASK_FROM_ISR()	ulPortRaiseBasePRI()	//��FROM_ISR��β�ĺ������ߺ궨�壬���������ж���ʹ�õ�
#define portCLEAR_INTERRUPT_MASK_FROM_ISR(x)	vPortSetBASEPRI(x)	

#define portINLINE __inline

#ifndef portFORCE_INLINE
	#define portFORCE_INLINE __forceinline
#endif	

static portFORCE_INLINE void vPortSetBaseSEPRI( uint32_t ulBASEPRI )
{
	uint32_t ulNewBASEPRI = configMAX_SYSCALL_INTERRUPT_PRIORITY;

	__asm
	{
		/* Set BASEPRI to the max syscall priority to effct a critical
		section. */
		msr basepri, ulNewBASEPRI
		dsb
		isb	
	}
}

#endif