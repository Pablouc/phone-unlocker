#ifndef ARDLIB_H
#define ARDLIB_H

#include <semaphore.h>

typedef struct  {
  sem_t driver_sem;
} ardlib_ctx_t;

int press_button(ardlib_ctx_t *ctx, int button);
int init_ardlib(ardlib_ctx_t *context);
int change_size(ardlib_ctx_t *ctx, int size);

#endif
