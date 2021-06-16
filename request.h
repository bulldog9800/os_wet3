#ifndef __REQUEST_H__

#include "queue.h"

void requestHandle(int fd);
void* threadRequestHandle(void* queue);

#endif
