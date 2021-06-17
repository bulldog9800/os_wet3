//
// Created by Sari Abed on 13/06/2021.
//

#ifndef OS_WET3_HELPERS_H
#define OS_WET3_HELPERS_H

#include <pthread.h>
#include "segel.h"
#include <sys/time.h>
#include <string.h>

typedef struct timeval timeval_t;

extern int active_threads;
pthread_mutex_t active_threads_lock;
pthread_mutex_t full_queue_lock;
pthread_cond_t full_cond;

timeval_t master_start_time;


typedef struct Node_t {
    void* data;
    int fd;
    struct Node_t* next;
    timeval_t tv;
} Node;

typedef struct Queue_t {
    Node* head;
    Node* tail;
    int size;
    int max_size;
    pthread_mutex_t lock;
    pthread_cond_t empty_cond;
} Queue;

typedef struct thread_t {
    int id;
    int static_count;
    int dynamic_count;
    int count;
    timeval_t arrival_time;
    timeval_t finish_time;
}  Thread;

Thread* thread_creat (int id);

Thread* thread_infos;
Queue* pending_requests_queue;

Queue* queueCreate(int max_size);
void queuePush(Queue* queue, int fd);
int queuePop(Queue* queue);
Node* queueTop(Queue* queue);

#endif //OS_WET3_HELPERS_H
