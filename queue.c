#include "queue.h"

queue_t *queue = NULL;

int open_queue(void)
{
  queue = calloc(1, sizeof(queue_t));
  if(ap_list == NULL)
    return ERROR;

  return VALID;
}
