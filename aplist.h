#ifndef __AP_LIST__
#define __AP_LIST__

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
  struct ap_info_s *next;
}ap_info_t;

typedef struct ap_list_s
{
  ap_info_t *head;
}ap_list_t;

extern ap_list_t *ap_list;

/*[Действие] Инициализация стека для хранения списка ТД и их данных*/
int init_ap_list(void);
/*[Действие] Регистрация ТД в системе или обновление ее данных*/
ap_info_t* register_ap(void *message);
/*[Действие] Очистка стека записей о ТД*/
int destroy_ap_list(void);

#endif
