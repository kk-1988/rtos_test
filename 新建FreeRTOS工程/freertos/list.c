#include "list.h"

/* ������ڵ��ʼ�� */
void VListInitialise(ListItem_t * const pxList)
{
	/* ����������ָ��ָ�����һ���ڵ� */
	pxList->pxIndex = (ListItem_t *)&(pxList->xListEnd);
	
}

/* �ڵ��ʼ�� */
void vListInitialiseItem(ListItem_t * const pxItem)
{
	pxItem->pvContainer = NULL;
}