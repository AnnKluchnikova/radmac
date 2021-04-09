#include <string.h>
#include <stdlib.h>
#include <time.h>

#include "aplist.h"
#include "json-parser.h"

//==============================================================================

ap_list_t *ap_list = NULL;

//==============================================================================

/**/
ap_info_t* reg_new_ap(void *message)
{
  ap_info_t *new = calloc(1, sizeof(ap_info_t));

  new->last_conn = time(NULL);
  new->ap_id = strdup(json_get_string_from_object(message, "ap_id"));
  new->ap_name = strdup(json_get_string_from_object(message, "ap_name"));
  /*TODO Данные конфигурации*/
  pthread_mutex_init(&new->ap_lock, NULL);

  if(ap_list->head == NULL)
  {
    new->next = NULL;
    ap_list->head = new;
  }
  else
  {
    new->next = ap_list->head;
    ap_list->head = new;
  }

  return ap_list->head;
}

/**/
ap_info_t* get_link_to_id(const char* ap_id)
{
  ap_info_t *link = ap_list->head;

  while(link != NULL)
  {
    if(strcmp(link->ap_id, ap_id) == 0)
    {
      link->last_conn = time(NULL);
      break;
    }
    link = link->next;
  }

  return link;
}

/**/
void clear_auth_list(void)
{
  ap_info_t *link = NULL;
  ap_info_t *next = NULL;

  if(ap_list == NULL)
    return;

  link = ap_list->head;

  while(link != NULL)
  {
    next = link->next;

    free(link->ap_id);
    free(link->ap_name);
    pthread_mutex_destroy(&link->ap_lock);
    free(link);

    link = next;
  }
}

//==============================================================================

/*[Действие] Инициализация стека для хранения списка ТД и их данных*/
int init_ap_list(void)
{
  ap_list = calloc(1, sizeof(ap_info_t));
  if(ap_list == NULL)
    return ERROR;

  return VALID;
}

/*[Действие] Авторизация ТД в системе или регистрация новой*/
ap_info_t* auth_ap(void *message)
{
  ap_info_t *ap = get_link_to_id(json_get_string_from_object(message, "ap_id"));
  if(ap == NULL)
    ap = reg_new_ap(message);

  ap->ap_message = message;
  /*TODO Пишем в лог о соединении с ТД*/
  return ap;
}

/*[Действие] Очистка стека записей о ТД*/
int destroy_ap_list(void)
{
  if(ap_list == NULL)
    return ERROR;

  clear_auth_list();
  free(ap_list);

  return VALID;
}
