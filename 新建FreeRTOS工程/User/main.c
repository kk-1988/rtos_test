#include "list.h"

int main(void)
{
	/* 根节点链表 */
	List_t list;	
	ListItem_t node1,node2,node3;
	
	/* 链表初始化 */
	vListInitialise(&list);
	/* 节点初始化 */
	vListInitialiseItem(&node1);
	vListInitialiseItem(&node2);
	vListInitialiseItem(&node3);
	
	/* 插入 */
	vListInsert(&list, &node1);
	
	return 0;
}