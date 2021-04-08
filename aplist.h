#ifndef __AP_INFO__
#define __AP_INFO__

/*TODO Добавить библиотеку с тредами*/

/*Информация о ТД*/
typedef struct ap_info_s
{
  pthread_t ap_thread;
  pthread_mutex_t ap_lock;

  time_t last_conn;     // Время последней связи
  const char *ap_id;    // MAC-адрес ТД
  const char *ap_name;  // Уникальный идентификатор локации ТД
  //TODO Добавить данные конфигурации
  struct wap_info_s *next;
}wap_info_t;

extern ap_info_t *ap_head;

#endif
