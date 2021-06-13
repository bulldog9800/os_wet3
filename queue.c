//
// Created by Sari Abed on 13/06/2021.
//

#include "queue.h"


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

    err = pthread_cond_init(&queue->full_cond, NULL);
    if (err != 0) {
        free(queue);
        fprintf(stderr, "pthread_cond_init error: %s\n", strerror(err));
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

void queuePush(Queue* queue, void* data) {
    Node* node = Malloc(sizeof(Node));
    node->data = data;

    pthread_mutex_lock(&queue->lock);
    if (queue->size==0) {
        queue->head = node;
        queue->tail = node;
    }
    else {
        while (queue->size >= queue->max_size) {
            pthread_cond_wait(&queue->full_cond, &queue->lock);
        }
        queue->tail->next = node;
        queue->tail = node;
    }
    queue->size++;
    pthread_cond_signal(&queue->empty_cond);
    pthread_mutex_unlock(&queue->lock);
}

void* queuePop(Queue* queue) {
    pthread_mutex_lock(&queue->lock);
    while (queue->size == 0) {
        pthread_cond_wait(&queue->empty_cond);
    }
    Node* node = queue->head;
    queue->head = queue->head->next;
    queue->size--;
    pthread_cond_signal(&queue->full_cond);
    pthread_mutex_unlock(&queue->lock);

    void* temp = node->data;
    free(node);

    return temp;
}

