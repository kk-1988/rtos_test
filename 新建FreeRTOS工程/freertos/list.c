#include "list.h"

/* 链表根节点初始化 */
void VListInitialise(ListItem_t * const pxList)
{
	/* 将链表索引指针指向最后一个节点 */
	pxList->pxIndex = (ListItem_t *)&(pxList->xListEnd);
	
}

/* 节点初始化 */
void vListInitialiseItem(ListItem_t * const pxItem)
{
	pxItem->pvContainer = NULL;
}