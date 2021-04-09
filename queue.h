#ifndef __DATA_QUEUE__
#define __DATA_QUEUE__

#include "aplist.h"

/*Возвращаемые значения функций*/
#define ERROR -1
#define VALID 0

typedef struct qdata_s
{
  void* data;
  struct qdata_s *next;
  struct qdata_s *prev;
}qdata_t;

typedef struct queue_s
{
  qdata_t *head;
  qdata_t *tail;
}queue_t;

queue_t *queue;

/*[Действие] Выделение памяти под стек очереди*/
int open_queue(void);
/*[Действие] Постановка данных в очередь обработки по принципу FIFO*/
void put_queue(void* data);
/*[Действие] Полное удаление списка очереди с очисткой содержимого*/
int close_queue(void);

/*Только для дебага*/
void show_queue(void);

#endif