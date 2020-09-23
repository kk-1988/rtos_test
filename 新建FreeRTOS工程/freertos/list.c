#include "list.h"

/* 链表根节点初始化 */
void vListInitialise(List_t * const pxList)
{
	/* 将链表索引指针指向最后一个节点 */
	pxList->pxIndex = (ListItem_t *)&(pxList->xListEnd);
	/* 将链表最后一个节点的辅助排序的值设置为最大 */
	pxList->xListEnd.xItemValue = portMAX_DELAY;
	
	/* 将pxNext和pxPrevious指针都指向其自身,表示链表为空 */
	pxList->xListEnd.pxNext = ( ListItem_t * )&(pxList->xListEnd);
	pxList->xListEnd.pxPrevious = (ListItem_t *)&(pxList->xListEnd);
	
	/* 初始化链表节点计数器的值为0，表示链表为空 */
	pxList->uxNumberOfItems = (UBaseType_t) 0U;
}

/* 节点初始化 */
void vListInitialiseItem(ListItem_t * const pxItem)
{
	/* 不属于任何一个链表 */
	pxItem->pvContainer = NULL;
}

/* 插入链表节点到末尾 */
void vListInsertEnd( List_t *const pxList, ListItem_t * const pxNewListeItem)
{
		ListItem_t * const pxIndex = pxList->pxIndex;		//end
		
		pxNewListItem->pxNext = pxIndex;		//新节点的下一个节点就是end节点本身
		pxNewListItem->pxPrevious = pxIndex->pxPrevious;	//新节点的上一个节点就是原本end节点的上一个节点
		pxIndex->pxOrevious->pxNext = pxNewListItem;	//end上一个节点的下一个指向新节点
		pxIndex->pxPrevious = pxNewListItem;	//end上一个节点指向新节点
		
		/* 记住该节点所在的链表 */
		pxNewListItem->pvContainer = (void *) pxList;
		
		/* 链表节点计数器++ */
		( pxList->uxNumberOfItems )++;
}

void vListInsert(List * const pxList, ListItem_t * const pxNewListItem)
{
	
}