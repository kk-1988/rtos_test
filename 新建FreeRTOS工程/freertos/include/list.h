

struct xLIST_ITEM
{
	TickType_t xItemValue;	/* 辅助值,用于帮助节点做顺序排列 */
	struct xLIST_ITEM *pxNext;	/* 指向链表下一个节点 */
	
	
};
typedef struct xLIST_ITEM ListItem_t;