#include "FreeRTOS.h"

struct xLIST_ITEM
{
	TickType_t xItemValue;	/* ����ֵ,���ڰ����ڵ���˳������ */
	struct xLIST_ITEM *pxNext;	/* ָ��������һ���ڵ� */
	struct xLIST_ITEM *pxPrevious;	/* ָ������ǰһ���ڵ� */
	void *pvOwner;					/* ָ��ӵ�иýڵ���ں˶���ͨ����TCB */
	void *pvContainer;			/* ָ��ýڵ����ڵ����� */
};
typedef struct xLIST_ITEM ListItem_t;	/* �ڵ����������ض��� */

struct xMINI_LIST_ITEM
{
	TickType_t xItemValue;
	struct xLIST_ITEM	*pxNext;
	struct xLIST_ITEM	*pxPrevious;
};
typedef struct xMINI_LIST_ITEM MiniListItem_t;	/* ��С�ڵ����������ض��� */

/* ����ṹ�嶨�� */
typedef struct xLSIT
{
	UBaseType_t uxNumberOfItems;	/* ����ڵ������ */
	ListItem_t * pxIndex;					/* ����ڵ�����ָ�� */
	MiniListItem_t xListEnd;			/* �������һ���ڵ� */
}List_t;