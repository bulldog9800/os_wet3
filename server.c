#include "segel.h"
#include "request.h"
#include "helpers.h"

// 
// server.c: A very, very simple web server
//
// To run:
//  ./server <portnum (above 2000)>
//
// Repeatedly handles HTTP requests sent to this port number.
// Most of the work is done within routines written in request.c
//

// HW3: Parse the new arguments too
void getargs(int *port, int *thread_num, int* buffer_size, char* policy,int argc, char *argv[])
{
    if (argc < 4) {
	fprintf(stderr, "Usage: %s <port>\n", argv[0]);
	exit(1);
    }
    *port = atoi(argv[1]);
    *thread_num = atoi(argv[2]);
    *buffer_size = atoi(argv[3]);
    strlcpy(policy, argv[4], sizeof(policy));
}


int main(int argc, char *argv[])
{
    int listenfd, connfd, port, clientlen, thread_num, buffer_size;
    struct sockaddr_in clientaddr;
    char policy[7];

    getargs(&port, &thread_num, &buffer_size, policy,argc, argv);

    // 
    // HW3: Create some threads...
    //
    pthread_t* threads = (pthread_t*) Malloc(sizeof(pthread_t)*thread_num);
    thread_infos = (Thread*) Malloc(sizeof(Thread) * thread_num );

    pending_requests_queue = queueCreate(thread_num);
    pthread_mutex_init(&active_threads_lock, NULL);

    for (int i=0; i<thread_num; i++){
        thread_infos[i].id = i;
        thread_infos[i].count = 0;
        thread_infos[i].dynamic_count = 0;
        thread_infos[i].static_count = 0;
        pthread_create(&threads[i], NULL, threadRequestHandle, &thread_infos[i]);
    }

    gettimeofday(&master_start_time, NULL);
    listenfd = Open_listenfd(port);
    while (1) {
	clientlen = sizeof(clientaddr);
	connfd = Accept(listenfd, (SA *)&clientaddr, (socklen_t *) &clientlen);

	// 
	// HW3: In general, don't handle the request in the main thread.
	// Save the relevant info in a buffer and have one of the worker threads 
	// do the work. 
	//
	pthread_mutex_lock(&active_threads_lock);
	if (pending_requests_queue->size + active_threads >= buffer_size) {
        if (!strcmp("block", policy)) {
            while (pending_requests_queue->size + active_threads >= buffer_size) {
                pthread_cond_wait(&full_cond, &full_queue_lock);
            }
            pthread_mutex_unlock(&active_threads_lock);
            queuePush(pending_requests_queue, connfd);
        } else if (!strcmp("dt", policy)) {
            close(connfd);
            continue;
        } else if (!strcmp("dh", policy)) {
            queuePop(pending_requests_queue);
            queuePush(pending_requests_queue, connfd);
        }
    }
	else{
        queuePush(pending_requests_queue, connfd);
	}



    }

}


    


 
