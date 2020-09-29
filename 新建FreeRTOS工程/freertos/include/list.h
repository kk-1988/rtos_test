#include "FreeRTOS.h"

/* �ڵ�ṹ�嶨�� */
struct xLIST_ITEM
{
	TickType_t xItemValue;	/* ����ֵ,���ڰ����ڵ���˳������(��������) */
	struct xLIST_ITEM *pxNext;	/* ָ��������һ���ڵ� */
	struct xLIST_ITEM *pxPrevious;	/* ָ������ǰһ���ڵ� */
	void *pvOwner;					/* ָ��ӵ�иýڵ���ں˶���ͨ����TCB */
	void *pvContainer;			/* ָ��ýڵ����ڵ����� */
};
typedef struct xLIST_ITEM ListItem_t;	/* �ڵ����������ض��� */

/* mini�ڵ�ṹ�嶨�壬��Ϊ˫������Ľ�β
*  ��Ϊ˫����������β�����ģ�ͷ����β��β����ͷ
* */
struct xMINI_LIST_ITEM
{
	TickType_t xItemValue;              /* ����ֵ,���ڰ����ڵ����������� */
	struct xLIST_ITEM	*pxNext;        /* ָ��������һ���ڵ� */
	struct xLIST_ITEM	*pxPrevious;    /* ָ������ǰһ���ڵ� */
};
typedef struct xMINI_LIST_ITEM MiniListItem_t;	/* ��С�ڵ����������ض��� */

/* ����ṹ�嶨�� */
typedef struct xLSIT
{
	UBaseType_t uxNumberOfItems;	/* ����ڵ������ */
	ListItem_t * pxIndex;					/* ����ڵ�����ָ�� */
	MiniListItem_t xListEnd;			/* �������һ���ڵ� */
}List_t;

/* ��ȡ�������ڽڵ� */
#define listGET_HEAD_ENTRY( pxList )  ( ( (pxList)->xListEnd ).pxNext )

/* ��ȡ����ĵ�Ѻ���ڵ� */
#define listGET_NEXT( pxListItem ) ( (pxListItem )->pxNext )

/* ��ȡ��������һ���ڵ� */
#define listGET_END_MARKER( pxList ) ( (ListItem_t const *) (& ( ( pxList )->xListEnd ) 0 ))

/* ��ȡ����Ľڵ��� */
#define listCURRENT_LIST_LENGTH( pxList ) (( pxList )->uxNumberOfItems )

/* ��ȡ����ڵ��OWNER����TCB */
#define listGET_OWNER_OF_NEXT_ENTRY( pxTCB, pxList )
{
	List_t * const pxConstList = ( pxList );
	/* �ڵ�����ָ�������һ���ڵ�����ڵ�����ָ�룬ָ����һ���ڵ㣬
		�����ǰ������N���ڵ㣬����N�ε��øú���ʱ��pxIndex��ָ���N���ڵ�
	 */
	( pxConstList )->pxIndex = ( pxConstList )->pxIndex->pxNext;
	/* ��ǰ����Ϊ�� */
}

void vListInitialise(List_t * const pxList);
void vListInitialiseItem(ListItem_t * const pxItem);
void vListInsert(List * const pxList, ListItem_t * const pxNewListItem);