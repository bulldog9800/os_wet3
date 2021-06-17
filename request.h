#ifndef __REQUEST_H__

#include "helpers.h"

void requestHandle(int fd, Thread* thread_info);
void* threadRequestHandle(void* queue);

#endif
