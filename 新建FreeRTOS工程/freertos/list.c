#include "list.h"

/* ������ڵ��ʼ�� */
void VListInitialise(List_t * const pxList)
{
	/* ����������ָ��ָ�����һ���ڵ� */
	pxList->pxIndex = (ListItem_t *)&(pxList->xListEnd);
	/* ���������һ���ڵ�ĸ��������ֵ����Ϊ��� */
	pxList->xListEnd.xItemValue = portMAX_DELAY;
	
	/* ��pxNext��pxPreviousָ�붼ָ��������,��ʾ����Ϊ�� */
	
	
}

/* �ڵ��ʼ�� */
void vListInitialiseItem(ListItem_t * const pxItem)
{
	/* �������κ�һ������ */
	pxItem->pvContainer = NULL;
}