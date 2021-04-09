#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#include "queue.h"
#include "json-parser.h"

/*[Действие] Выделение памяти под стек очереди*/
int open_queue(void)
{
  queue = calloc(1, sizeof(queue_t));
  if(queue == NULL)
    return ERROR;

  return VALID;
}

/*[Действие] Полное удаление списка очереди с очисткой содержимого*/
int close_queue(void)
{
  qdata_t *link = NULL;

  if(queue->head == NULL)
    return ERROR;

  while(queue->head != NULL)
  {
    link = queue->head;
    json_destroy(queue->head->data);

    queue->head = link->next;
    free(link);
  }

  queue->head = queue->tail = NULL;
  /* BAG При использовании free() для очистки указателя
   * в функции close_queue(), память не освобождается.
   * Но при этом в main() сробатывает*/
//  free(queue);

  return VALID;
}

/*[Действие] Постановка данных в очередь.
 *[Примечание] Обработка осуществляется по принципу FIFO*/
void put_queue(void* data)
{
  qdata_t *new = calloc(1, sizeof(queue_t));
  new->data = json_object_get(data);

  if(queue->head == NULL)
  {
    queue->tail = new;
    new->next = NULL;
    new->prev = NULL;
  }
  else
  {
    new->prev = NULL;
    new->next = queue->head;
    (queue->head)->prev = new;
  }

  queue->head = new;
}

int take_queue(void)
{
}

/*[Действие] Удаление элемента в конце списка
 *
 *[Примечание] Передполагается, что в очередь будут поступать данные с начала
 *             списка, а забираться с конца. Поэтому после обработки данных
 *             должен удалаться именно конец.*/
int release_queue(void)
{
  qdata_t *link = queue->tail;

  if(link == NULL)
    return ERROR;

  (link->prev)->next = NULL;
  queue->tail = link->prev;

  json_destroy(link->data);
  free(link);

  return VALID;
}

void show_queue(void)
{
  static int count = 0;
  qdata_t *stack = queue->tail;

  while(stack != NULL)
  {
    printf("[#]: %d [QUEUE]: %s\n", count, json_object_get_string(stack->data));
    stack = stack->prev;
  }

  count++;
}
