#include "list.h"

/* ������ڵ��ʼ�� */
void VListInitialise(List_t * const pxList)
{
	/* ����������ָ��ָ�����һ���ڵ� */
	pxList->pxIndex = (ListItem_t *)&(pxList->xListEnd);
	/* ���������һ���ڵ�ĸ��������ֵ����Ϊ��� */
	pxList->xListEnd.xItemValue = portMAX_DELAY;
	
}

/* �ڵ��ʼ�� */
void vListInitialiseItem(ListItem_t * const pxItem)
{
	pxItem->pvContainer = NULL;
}