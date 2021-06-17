//
// Created by Sari Abed on 13/06/2021.
//

#include "helpers.h"

int active_threads = 0;


Queue* queueCreate(int max_size) {
    Queue* queue = (Queue*) Malloc(sizeof(Queue));
    queue->max_size = max_size;
    queue->size = 0;
    int err = pthread_mutex_init(&queue->lock, NULL);
    if (err != 0) {
        free(queue);
        fprintf(stderr, "pthread_mutex_init error: %s\n", strerror(err));
        exit(0);
    }


    err = pthread_cond_init(&queue->empty_cond, NULL);
    if (err != 0) {
        free(queue);
        fprintf(stderr, "pthread_cond_init error: %s\n", strerror(err));
        exit(0);
    }
    queue->head = NULL;
    queue->tail = NULL;
    return queue;
}

void queuePush(Queue* queue, int fd) {
    Node* node = Malloc(sizeof(Node));
    node->fd = fd;
    gettimeofday(&node->tv, NULL);


    pthread_mutex_lock(&queue->lock);
    if (queue->size==0) {
        queue->head = node;
        queue->tail = node;
    }
    else {
        queue->tail->next = node;
        queue->tail = node;
    }
    queue->size++;
    pthread_cond_signal(&queue->empty_cond);
    pthread_mutex_unlock(&queue->lock);
}

int queuePop(Queue* queue) {
    pthread_mutex_lock(&queue->lock);
    pthread_mutex_lock(&active_threads_lock);
    while (queue->size == 0) {
        pthread_cond_wait(&queue->empty_cond, &queue->lock);
    }
    Node* node = queue->head;
    queue->head = queue->head->next;
    queue->size--;

    active_threads++;

    pthread_mutex_unlock(&queue->lock);
    pthread_mutex_unlock(&active_threads_lock);

    int temp = node->fd;
    free(node);

    return temp;
}

Thread* thread_creat(int id){
    Thread* thread =(Thread*) Malloc(sizeof(Thread));
    thread->id=id;
    thread->static_count=0;
    thread->dynamic_count=0;
    thread->count = 0;
    return  thread ;
}

Node* queueTop(Queue* queue) {
    return queue->head;
}
