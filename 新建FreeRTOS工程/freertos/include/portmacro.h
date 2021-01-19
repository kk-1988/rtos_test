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

#define potENTER_CRITICAL()		vPortEnterCritical()
#define portDISABLE_CRITICAL()	vPortExitCritical()

#define portSET_INTERRUPT_MASK_FROM_ISR()		ulPortRaiseBasePRI()	//��FROM_ISR��β�ĺ������ߺ궨�壬���������ж���ʹ�õ�
#define portCLEAR_INTERRUPT_MASK_FROM_ISR(x)	vPortSetBASEPRI(x)		//����x=5����ô����5���ж��������ǲ��ᱻ��Ӧ��

#define portINLINE __inline

#ifndef portFORCE_INLINE
	#define portFORCE_INLINE __forceinline
#endif	

static portFORCE_INLINE void vPortSetBASEPRI( void )	//��������ֵ,����Ƕ��
{
	uint32_t ulNewBASEPRI = configMAX_SYSCALL_INTERRUPT_PRIORITY;

	__asm
	{
		/* Set BASEPRI to the max syscall priority to effct a critical
		section. */
		msr basepri, ulNewBASEPRI	//11,����11 ���ܱ���Ӧ�ģ�С��11����Ա���Ӧ
		dsb
		isb	
	}
}

static portFORCE_INLINE uint32_t vPortSetBASEPRI( void )	//����Ƕ��,�������ж���ʹ��
{
	uint32_t ulReturn, ulNewBASEPRI = configMAX_SYSCALL_INTERRUPT_PRIORITY;

	__asm
	{
		/* Set BASEPRI to the max syscall priority to effct a critical
		section. */
		mrs ulReturn,basepri		//״̬�Ĵ�����ͨ�üĴ����Ĵ���ָ��(basepri��ֵ������ulReturn��)
		msr basepri,ulNewBASEPRI	//ͨ�üĴ�����״̬�Ĵ����Ĵ���ָ��(������basepri��ֵ)
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
		msr basepri, ulBASEPRI			//basepri������Ϊ0���򲻹ر��κ��ж�(���ж�)
	}
}

/*
* ���ж�
* 1.ulPortRaiseBASEPRI	//11,���ص���0��5��6��7��8����Ӧ
* 2.ulPortRaiseBASEPRI	//5,���ص���11(֮ǰbasepri��ֵ)��5��6��7��8���ᱻ��Ӧ
* 
* ���ж�
* 3.ulPortRaiseBASEPRI(11)	//��ʱbasepri�����ó�11
* 4.vPortSetBASEPRI(0)		//�����ж϶��ᱻ��Ӧ
*/


#endif