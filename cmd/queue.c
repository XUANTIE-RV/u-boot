// SPDX-License-Identifier: GPL-2.0-only
/*
 * Copyright(C) 2021 Alibaba Communications Inc.
 * Author: David Li <liyong.li@alibaba-inc.com>
 */
/*
 * queue management
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include "queue.h"
/**
	*  queue init
	*	parameter：
	*		queueIncreasedEvent：event
	*	ret：queue_ptr to be successful，NULL to be failed
	*/
Queue* Queue_Init(QueueIncreased queueIncreasedEvent)
{
	Queue* queue = NULL;
	queue = (Queue*)malloc(sizeof(Queue));
	if(queue == NULL)
	{
		return NULL;
	}
	queue->length = 0;
	queue->head = NULL;
	queue->tail = NULL;
	queue->onQueueIncreased = queueIncreasedEvent;
	return queue;
}
 
/**
	*  insert data to head
	*	parameter：
	*		queue：queue ptr
	*		data：data to be inserted
	*	ret：0 to be successful，-1 to be failed
	*/
int Queue_AddToHead(Queue* queue, char data)
{
	QueueNode* node = NULL;
	if(queue == NULL)
		return -1;
	// create node
	node = (QueueNode*)malloc(sizeof(QueueNode));
	if(node == NULL)
		return -1;
	node->data = data;
	// in first time
	if(queue->tail == NULL && queue->head == NULL)
	{
		queue->tail = node;
		queue->head = node;
		node->next = NULL;
		node->prior = NULL;
	}
	else
	{
		// add to head
		queue->head->prior = node;
		node->next = queue->head;
		node->prior = NULL;
		queue->head = node;
	}
	queue->length++;
	//callback event
	if(queue->onQueueIncreased != NULL)
		return queue->onQueueIncreased(queue,data);
	return 0;
}
 
/**
	*  insert data to taol
	*	parameter：
	*		queue：queue ptr
	*		data：data to be inserted
	*	ret：0 to be successful，-1 to be failed
	*/
int Queue_AddToTail(Queue* queue, char data)
{
	QueueNode* node = NULL;
	if(queue == NULL)
		return -1;
	// create node
	node = (QueueNode*)malloc(sizeof(QueueNode));
	if(node == NULL)
		return -1;
	node->data = data;
	// in first time
	if(queue->tail == NULL && queue->head == NULL)
	{
		queue->tail = node;
		queue->head = node;
		node->next = NULL;
		node->prior = NULL;
	}
	else
	{
		//add to tail
		queue->tail->next = node;
		node->prior = queue->tail;
		node->next = NULL;
		queue->tail = node;
	}
	queue->length++;
	// callback event
	if(queue->onQueueIncreased != NULL)
		return queue->onQueueIncreased(queue,data);
	return 0;
}
 
/**
	*  get data from queue head
	*	parameter：
	*		queue：queue ptr
	*	ret：valid data to be successful，-1 to be failed
	*/
char Queue_GetFromHead(Queue* queue)
{
	char data = -1;
	QueueNode* node = NULL; 
	if(queue == NULL || queue->head == NULL)
	{
		return -1;
	}
	node = queue->head;
	queue->head = node->next;
	if(queue->head != NULL)
	{
		queue->head->prior = NULL;
	}
	else
	{
		//empty queue
		queue->tail = NULL;
		queue->head = NULL;
	}
	data = node->data;
	free(node);
	queue->length--;
	return data;
}
 
 
/**
	*  get data from queue tail
	*	parameter：
	*		queue：queue ptr
	*	ret：valid data to be successful，-1 to be failed
	*/
char Queue_GetFromTail(Queue* queue)
{
	char data = -1;
	QueueNode* node = NULL; 
	if(queue == NULL || queue->tail == NULL)
	{
		return -1;
	}
	node = queue->tail;
	queue->tail = node->prior;
	if(queue->tail != NULL)
	{
		queue->tail->next = NULL;
	}
	else
	{
		// empty queue
		queue->tail = NULL;
		queue->head = NULL;
	}
	data = node->data;
	free(node);
	queue->length--;
	return data;
}

char Queue_QueryAt(Queue* queue, unsigned long index)
{
	QueueNode* node = NULL; 
	if(queue == NULL || queue->head == NULL)
	{
		return -1;
	}
	
	node = queue->head;
	while (index > 0) {
		if (node == queue->tail) {
			printf("Queue QueryAt is out of range");
			return -1;
		}
		node = node->next;
		index--;
	};
	return node->data;
}
 
/**
	*  free queue
	*	parameter：
	*		queue：queue ptr
	*		isFreeData: flag to indicator free data automatically
	*/
void Queue_Free(Queue* queue, bool isFreeData)
{
	if(isFreeData) {
		while (queue->tail != NULL)
			Queue_GetFromTail(queue);
	}
	free(queue);
 
}
 
// new event
int OnQueueIncreasedEvent(void* queue, char data)
{
        Queue* q = (Queue*)queue;
        char p = data;
	return 0;
}
 
