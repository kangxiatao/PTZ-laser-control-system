#include "malloc.h"	
#include "queue.h"

/****************************************************************
*��������:	InitQueue
*��������:	��ʼ��һ������,���ö��г�ʼ��Ϊ�ն���
*��ڲ���:	pQueue:	ָ����е�ָ��
*�� �� ֵ:	��
*****************************************************************/
void InitQueue(Queue *pQueue)
{
	pQueue->front = NULL;
	pQueue->rear = NULL;
	pQueue->items = 0;
}

/****************************************************************
*��������:	QueueIsFull
*��������:	�������Ƿ�����
*��ڲ���:	pQueue:	ָ��һ���ѳ�ʼ���Ķ���
*�� �� ֵ:	true:	��������
*			false:	����δ��
*****************************************************************/
bool QueueIsFull(const Queue *pQueue)
{
	return pQueue->items == MAXQUEUE;
}

/****************************************************************
*��������:	QueueIsEmpty
*��������:	�������Ƿ�Ϊ��
*��ڲ���:	pQueue:	ָ��һ���ѳ�ʼ���Ķ���
*�� �� ֵ:	true:	����Ϊ��
*			false:	���в�Ϊ��
*****************************************************************/
bool QueueIsEmpty(const Queue *pQueue)
{
	return pQueue->items == 0;
}

/****************************************************************
*��������:	QueueItemCount
*��������:	ȷ����������Ŀ�ĸ���
*��ڲ���:	pQueue:	ָ��һ���ѳ�ʼ���Ķ���
*�� �� ֵ:	��������Ŀ�ĸ���
*****************************************************************/
int QueueItemCount(const Queue *pQueue)
{
	return pQueue->items;
}

/****************************************************************
*��������:	EnQueue
*��������:	�����β�������Ŀ
*��ڲ���:	item:	����ӵ���Ŀ
*			pQueue:	ָ��һ���ѳ�ʼ���Ķ���
*�� �� ֵ:	true:	�����Ŀ�ɹ�
*			false:	�����Ŀʧ��
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
*��������:	DeQueue
*��������:	�Ӷ����׶�ɾ����Ŀ,������зǿ�,�����׶���Ŀ��
*			���Ƶ�pitem,�����Ӷ�����ɾ��
*��ڲ���:	pQueue:	ָ��һ���ѳ�ʼ���Ķ���
*�� �� ֵ:	true:	�����Ŀ�ɹ�
*			false:	�����Ŀʧ��
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
*��������:	EmptyTheQueue
*��������:	��ն���
*��ڲ���:	pQueue:	ָ��һ���ѳ�ʼ���Ķ���
*�� �� ֵ:	��
*****************************************************************/
void EmptyTheQueue(Queue *pQueue)
{
	while (!QueueIsEmpty(pQueue))
	{
		DeQueue(pQueue);
	}
}
