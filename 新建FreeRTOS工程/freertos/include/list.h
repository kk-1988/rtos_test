

struct xLIST_ITEM
{
	TickType_t xItemValue;	/* ����ֵ,���ڰ����ڵ���˳������ */
	struct xLIST_ITEM *pxNext;	/* ָ��������һ���ڵ� */
	struct xLIST_ITEM *pxPrevious;	/* ָ������ǰһ���ڵ� */
	void *pvOwner;					/* ָ��ӵ�иýڵ���ں˶���ͨ����TCB */
	void *pvContainer;			/* ָ��ýڵ����ڵ����� */
};
typedef struct xLIST_ITEM ListItem_t;	/* �ڵ����������ض��� */

