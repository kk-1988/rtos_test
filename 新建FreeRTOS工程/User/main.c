#include "list.h"

int main(void)
{
	/* ���ڵ����� */
	List_t list;	
	ListItem_t node1,node2,node3;
	
	/* �����ʼ�� */
	vListInitialise(&list);
	/* �ڵ��ʼ�� */
	vListInitialiseItem(&node1);
	vListInitialiseItem(&node2);
	vListInitialiseItem(&node3);
	
	/* ���� */
	vListInsert(&list, &node1);
	vListInsert(&list, &node2);
	vListInsert(&list, &node3);
	
	for(;;)
	{
		/* ɶ�¶����� */	
	}
	
	return 0;
}