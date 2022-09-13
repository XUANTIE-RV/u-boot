// SPDX-License-Identifier: GPL-2.0-only
/*
 * Copyright(C) 2021 Alibaba Communications Inc.
 * Author: David Li <liyong.li@alibaba-inc.com>
 */
/*
 * queue management
 *
 */

#pragma once
#ifndef _QUEUE_H
#define _QUEUE_H
#ifdef __cplusplus
extern "C" {
#endif
 
#ifndef bool
#define bool int
#endif
#ifndef true
#define true 1
#endif
#ifndef false
#define false 0
#endif
	
 
	typedef int(*QueueIncreased)(void* queue, char data);
 
	typedef struct _QueueNode{
		char data; 
		struct _QueueNode* next;
		struct _QueueNode* prior;
	}QueueNode;
 
	typedef struct _Queue{
		QueueNode* head;
		QueueNode* tail;
		unsigned long length;
		QueueIncreased onQueueIncreased;
	}Queue;
 
	
 
	Queue* Queue_Init(QueueIncreased queueIncreasedEvent);
 
	int Queue_AddToHead(Queue* queue, char data);
 
	int Queue_AddToTail(Queue* queue, char data);
 
	char Queue_GetFromHead(Queue* queue);
 
	char Queue_GetFromTail(Queue* queue);
	char Queue_QueryAt(Queue* queue, unsigned long index);
 
	void Queue_Free(Queue* queue,bool isFreeData);
 
	int OnQueueIncreasedEvent(void* queue, char data);
 
 
#ifdef __cplusplus
}
#endif
#endif
