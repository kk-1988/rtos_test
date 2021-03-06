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

/* 升序插入 */
void vListInsert(List * const pxList, ListItem_t * const pxNewListItem)
{
		ListItem_t *pxIterator;
		
		/* 获取节点的排序辅助值 */
		const TickType_t xValueOfInsertion = pxNewListItem->xItemValue;
		
		/* 寻找节点要插入的位置 */
		if( xValueOfInsertion == portMAX_DELAY )
		{
			pxIterator = pxList->xListEnd.pxPrevious;	
		}	
		else
		{
			for( pxIterator = ( ListItem_t * ) &(pxList->xListEnd );
					 pxIterator->pxNext->xItemValue <= xValueOfInsertion;
					 pxIterator = pxIterator->pxNext )
				{
					/* 没有事情可做，不断迭代只为了找到节点要插入的位置*/	
				}	
		}
		
		pxNewListItem->pxNext = pxIterator->pxNext;
		pxNewListItem->pxNext->pxPrevious = pxNewListItem;
		pxNewListItem->pxPrevious = pxIterator;
		pxIterator->pxNext = pxNewListItem;
		
		/* 记住该节点所在的链表 */
		pxNewListItem->pvContainer = (void *)pxList;		
		
		/* 链表节点计数器++ */
	  ( pxList->uxNumberOfItems )++;
}

/* 移除某个节点 */
UBaseType_t uxListRemove(ListItem_t * const pxItemToRemove)
{
	/* 获取节点所在的链表 */
	List_t * const pxList = ( List_t *) pxItemToRemove->pvContainer;
	
	pxItemToRemove->pxNext->pxPrevious = pxItemToRemove->pxPrevious;
	pxItemToRemove->pxPrevious->pxNext = pxItemToRemove->pxNext;
	
	/* Make sure the index is left pointing to a valid item .*/
	if( pxLit->pxIndex == pxItemToRemove )
	{
		pxList->pxIndex = pxItemToRemove->pxPrevious;		
	}
	
	/* 初始化该节点所在的链表为空，表示节点还没有插入到任何链表中 */
	pxItemToRemove->pvContainer = NULL;
	
	/* 链表节点计数器-- */
 ( pxList->uxNumberOfItems )--;
 
 /* 返回链表中剩余节点的个数 */
 return pxList->uxNumberOfItems;
}


