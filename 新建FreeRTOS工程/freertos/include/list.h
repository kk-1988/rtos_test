#ifndef LIST_H
#define LIST_H

#include "FreeRTOS.h"

/* 节点结构体定义 */
struct xLIST_ITEM
{
	TickType_t xItemValue;	/* 辅助值,用于帮助节点做顺序排列(升序排列) */
	struct xLIST_ITEM *pxNext;	/* 指向链表下一个节点 */
	struct xLIST_ITEM *pxPrevious;	/* 指向链表前一个节点 */
	void *pvOwner;					/* 指向拥有该节点的内核对象，通常是TCB */
	void *pvContainer;			/* 指向该节点所在的链表 */
};
typedef struct xLIST_ITEM ListItem_t;	/* 节点数据类型重定义 */

/* mini节点结构体定义，作为双向链表的结尾
*  因为双向链表是首尾相连的，头即是尾，尾即是头
* */
struct xMINI_LIST_ITEM
{
	TickType_t xItemValue;              /* 辅助值,用于帮助节点做升序排列 */
	struct xLIST_ITEM	*pxNext;        /* 指向链表下一个节点 */
	struct xLIST_ITEM	*pxPrevious;    /* 指向链表前一个节点 */
};
typedef struct xMINI_LIST_ITEM MiniListItem_t;	/* 最小节点数据类型重定义 */

/* 链表结构体定义 */
typedef struct xLSIT
{
	UBaseType_t uxNumberOfItems;	/* 链表节点计数器 */
	ListItem_t * pxIndex;					/* 链表节点索引指针 */
	MiniListItem_t xListEnd;			/* 链表最后一个节点 */
}List_t;

/* 初始化节点的拥有者 */
#define listSET_LIST_ITEM_OWNER( pxListIte, pxOwner )	( ( pxListItem )->pvOwner = ( void * ) ( pxOwner ) )

/* 获取节点拥有者 */
#define listGET_LIST_ITEM_OWNER( pxListItem ) ( ( pxListItem )->pvOwner )

/* 初始化节点排序辅助值 */
#define listSET_LIST_ITEM_VALUE( pxListItem, xValue ) ( ( pxListItem )->xItemValue = ( xValue ) )

/* 获取链表根节点的节点计数器的值 */
#define listGET_ITEM_VALUE_OF_HEAD_ENTRY( pxList ) ( ( ( pxList )->xListEnd ).pxNext->xItemValue )

/* 获取链表的入口节点 */
#define listGET_HEAD_ENTRY( pxList )  ( ( (pxList)->xListEnd ).pxNext )

/* 获取链表的抵押给节点 */
#define listGET_NEXT( pxListItem ) ( (pxListItem )->pxNext )

/* 获取链表的最后一个节点 */
#define listGET_END_MARKER( pxList ) ( (ListItem_t const *) (& ( ( pxList )->xListEnd ) 0 ))

/* 获取链表的节点数 */
#define listCURRENT_LIST_LENGTH( pxList ) (( pxList )->uxNumberOfItems )

/* 获取链表节点的OWNER，即TCB */
#define listGET_OWNER_OF_NEXT_ENTRY( pxTCB, pxList )
{
	List_t * const pxConstList = ( pxList );	\
	/* 节点索引指向链表第一个节点调整节点索引指针，指向下一个节点，
		如果当前链表有N个节点，当第N次调用该函数时，pxIndex则指向第N个节点
	 */
	( pxConstList )->pxIndex = ( pxConstList )->pxIndex->pxNext;	\
	/* 当前链表为空 */
	if( ( void * ) ( pxConstList )->pxIndex == ( void * )&( ( pxConstList )->xListEnd ) )	\
	{
		( pxConstList )->pxIndex = ( pxConstList )->pxIndex->pxNext;		
	}
	
	/* 获取节点的OWNER,即TCB */
	( pxTCB ) = ( pxConstList )->pxIndex->pvOwner; 
}

void vListInitialise(List_t * const pxList);
void vListInitialiseItem(ListItem_t * const pxItem);
void vListInsert(List * const pxList, ListItem_t * const pxNewListItem);

#endif		/* LIST_H */