#ifndef __AP_LIST__
#define __AP_LIST__

#include <pthread.h>

/*Возвращаемые значения функций*/
#define ERROR -1
#define VALID 0

/*Информация о ТД*/
typedef struct ap_info_s
{
  pthread_t ap_thread;
  pthread_mutex_t ap_lock;

  time_t last_conn;     // Время последней связи
  void *ap_message;     // Указатель на присланное сообщение

  char *ap_id;    // MAC-адрес ТД
  char *ap_name;  // Уникальный идентификатор локации ТД
  //TODO Добавить данные конфигурации
  struct ap_info_s *next;
}ap_info_t;

typedef struct ap_list_s
{
  ap_info_t *head;
}ap_list_t;

ap_list_t *ap_list;

/*[Действие] Инициализация стека для хранения списка ТД и их данных*/
int init_ap_list(void);
/*[Действие] Авторизация ТД в системе или регистрация новой*/
ap_info_t* auth_ap(void *message);
/*[Действие] Очистка стека записей о ТД*/
int destroy_ap_list(void);

#endif
