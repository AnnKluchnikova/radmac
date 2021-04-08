#ifndef __DATA_QUEUE__
#define __DATA_QUEUE__

/*Возвращаемые значения функций*/
#define ERROR -1
#define VALID 0

typedef struct qdata_s
{
  void* data;
}qdata_t;

typedef struct queue_s
{
  qdata_t *head;
}queue_t;

extern queue_t *queue;

#endif