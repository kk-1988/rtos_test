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
		
		pxNewListItem->pxNext = pxIndex;		//�½ڵ����һ���ڵ����end�ڵ㱾��
		pxNewListItem->pxPrevious = pxIndex->pxPrevious;	//�½ڵ����һ���ڵ����ԭ��end�ڵ����һ���ڵ�
		pxIndex->pxOrevious->pxNext = pxNewListItem;	//end��һ���ڵ����һ��ָ���½ڵ�
		pxIndex->pxPrevious = pxNewListItem;	//end��һ���ڵ�ָ���½ڵ�
		
		/* ��ס�ýڵ����ڵ����� */
		pxNewListItem->pvContainer = (void *) pxList;
		
		/* ����ڵ������++ */
		( pxList->uxNumberOfItems )++;
}

void vListInsert(List * const pxList, ListItem_t * const pxNewListItem)
{
	
}