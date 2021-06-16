//
// Created by Sari Abed on 13/06/2021.
//

#ifndef OS_WET3_QUEUE_H
#define OS_WET3_QUEUE_H

#include <pthread.h>
#include "segel.h"

int active_threads = 0;
pthread_mutex_t active_threads_lock;

typedef struct Node_t {
    void* data;
    int fd;
    struct Node_t* next;
} Node;

typedef struct Queue_t {
    Node* head;
    Node* tail;
    int size;
    int max_size;
    pthread_mutex_t lock;
    pthread_cond_t full_cond;
    pthread_cond_t empty_cond;
} Queue;

Queue* queueCreate(int max_size);
void queuePush(Queue* queue, int fd);
int queuePop(Queue* queue);


#endif //OS_WET3_QUEUE_H
