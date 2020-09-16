#include "list.h"

/* 链表根节点初始化 */
void VListInitialise(List_t * const pxList)
{
	/* 将链表索引指针指向最后一个节点 */
	pxList->pxIndex = (ListItem_t *)&(pxList->xListEnd);
	/* 将链表最后一个节点的辅助排序的值设置为最大 */
	pxList->xListEnd.xItemValue = portMAX_DELAY;
	
	/* 将pxNext和pxPrevious指针都指向其自身,表示链表为空 */
	
	
}

/* 节点初始化 */
void vListInitialiseItem(ListItem_t * const pxItem)
{
	/* 不属于任何一个链表 */
	pxItem->pvContainer = NULL;
}