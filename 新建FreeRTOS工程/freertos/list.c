#include "list.h"

/* ������ڵ��ʼ�� */
void vListInitialise(List_t * const pxList)
{
	/* ����������ָ��ָ�����һ���ڵ� */
	pxList->pxIndex = (ListItem_t *)&(pxList->xListEnd);
	/* ���������һ���ڵ�ĸ��������ֵ����Ϊ��� */
	pxList->xListEnd.xItemValue = portMAX_DELAY;
	
	/* ��pxNext��pxPreviousָ�붼ָ��������,��ʾ����Ϊ�� */
	pxList->xListEnd.pxNext = ( ListItem_t * )&(pxList->xListEnd);
	pxList->xListEnd.pxPrevious = (ListItem_t *)&(pxList->xListEnd);
	
	/* ��ʼ������ڵ��������ֵΪ0����ʾ����Ϊ�� */
	pxList->uxNumberOfItems = (UBaseType_t) 0U;
}

/* �ڵ��ʼ�� */
void vListInitialiseItem(ListItem_t * const pxItem)
{
	/* �������κ�һ������ */
	pxItem->pvContainer = NULL;
}

/* ��������ڵ㵽ĩβ */
void vListInsertEnd( List_t *const pxList, ListItem_t * const pxNewListeItem)
{
		ListItem_t * const pxIndex = pxList->pxIndex;		//end
		
		pxNewListItem->pxNext = pxIndex;
		pxNewListItem->pxPrevious = pxIndex->pxPrevious;
		pxIndex->pxOrevious->pxNext = pxNewListItem;
		pxIndex->pxPrevious = pxNewListItem;
		
		/* ��ס�ýڵ����ڵ����� */
		pxNewListItem->pvContainer = (void *) pxList;
		
		/* ����ڵ������++ */
		( pxList->uxNumberOfItems )++;
}