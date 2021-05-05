#include "malloc.h"	
#include "queue.h"

/****************************************************************
*函数名称:	InitQueue
*函数功能:	初始化一个队列,将该队列初始化为空队列
*入口参数:	pQueue:	指向队列的指针
*返 回 值:	无
*****************************************************************/
void InitQueue(Queue *pQueue)
{
	pQueue->front = NULL;
	pQueue->rear = NULL;
	pQueue->items = 0;
}

/****************************************************************
*函数名称:	QueueIsFull
*函数功能:	检查队列是否已满
*入口参数:	pQueue:	指向一个已初始化的队列
*返 回 值:	true:	队列已满
*			false:	队列未满
*****************************************************************/
bool QueueIsFull(const Queue *pQueue)
{
	return pQueue->items == MAXQUEUE;
}

/****************************************************************
*函数名称:	QueueIsEmpty
*函数功能:	检查队列是否为空
*入口参数:	pQueue:	指向一个已初始化的队列
*返 回 值:	true:	队列为空
*			false:	队列不为空
*****************************************************************/
bool QueueIsEmpty(const Queue *pQueue)
{
	return pQueue->items == 0;
}

/****************************************************************
*函数名称:	QueueItemCount
*函数功能:	确定队列中项目的个数
*入口参数:	pQueue:	指向一个已初始化的队列
*返 回 值:	队列中项目的个数
*****************************************************************/
int QueueItemCount(const Queue *pQueue)
{
	return pQueue->items;
}

/****************************************************************
*函数名称:	EnQueue
*函数功能:	向队列尾端添加项目
*入口参数:	item:	被添加的项目
*			pQueue:	指向一个已初始化的队列
*返 回 值:	true:	添加项目成功
*			false:	添加项目失败
*****************************************************************/
bool EnQueue(Item item, Queue *pQueue)
{
	Node *newNode;

	if (QueueIsFull(pQueue))
	{
		return false;
	}

	newNode = (Node *)mymalloc(sizeof(Node));
	if (NULL == newNode)
	{
		return false;
	}

	newNode->item = item;
	newNode->next = NULL;

	if (QueueIsEmpty(pQueue))
	{
		pQueue->front = newNode;
		pQueue->rear = newNode;
		pQueue->items++;
	}
	else
	{
		pQueue->rear->next = newNode;
		pQueue->rear = newNode;
		pQueue->items++;
	}

	return true;
}

/****************************************************************
*函数名称:	DeQueue
*函数功能:	从队列首端删除项目,如果队列非空,队列首端项目被
*			复制到pitem,并被从队列中删除
*入口参数:	pQueue:	指向一个已初始化的队列
*返 回 值:	true:	添加项目成功
*			false:	添加项目失败
*****************************************************************/
bool DeQueue(Queue *pQueue)
{
	Node *temp;

	if (QueueIsEmpty(pQueue))
	{
		return false;
	}

	temp = pQueue->front;
	pQueue->front = pQueue->front->next;
	myfree(temp);
	pQueue->items--;

	if (0 == pQueue->items)
	{
		pQueue->rear = NULL;
	}

	return true;
}

/****************************************************************
*函数名称:	EmptyTheQueue
*函数功能:	清空队列
*入口参数:	pQueue:	指向一个已初始化的队列
*返 回 值:	无
*****************************************************************/
void EmptyTheQueue(Queue *pQueue)
{
	while (!QueueIsEmpty(pQueue))
	{
		DeQueue(pQueue);
	}
}
