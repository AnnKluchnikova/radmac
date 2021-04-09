#include <stdio.h>
#include <signal.h> // signal()
#include <pthread.h>
#include <time.h>
#include <unistd.h>

#include "queue.h"
#include "radmac.h"
#include "aplist.h"
#include "basefunc.h"
#include "json-parser.h"

/*Файл, в который пишется таблица MAC-адресов от ТД*/
#define POST_FILE "/var/www/html/radmac/test.json"

//------------------------------------------------------------------------------

/*[Действие] Перехват сигнала выхода Ctrl+C*/
static void sigint_handler(int signo)
{
  exit_flag = 1;
}

//------------------------------------------------------------------------------

void *parse_ap_message(void* data)
{
  int index = 0;         // Счетчик индексов массива
  int length = 0;        // Размер массива
  json_object* array = NULL;    // json-массив, взятый из объекта

  void *mac_list = NULL;          // Индивидуальный список уникальных адресов

  ap_info_t *ap = (ap_info_t*)data;
  void *message = ap->ap_message; // Анализируемое сообщение

  /*TODO Пишем в лог о начале работы потока*/

  json_object_object_get_ex(message, "probe_requests", &array);
  length = json_object_array_length(array);

  if(length == 0)
    pthread_exit(0);

  mac_list = json_object_new_array();

  for(; index < length; index++)
  {
    save_new_unique_mac( mac_list, json_object_array_get_idx(array, index));
  }

  pthread_mutex_lock(&ap->ap_lock);
  put_queue(mac_list);
  pthread_mutex_unlock(&ap->ap_lock);

  json_destroy(mac_list);
  /*TODO Пишем в лог о конце работы потока*/
  pthread_exit(0);
}

void main_loop(void)
{
  void *ap_message = NULL;
  ap_info_t *ap = NULL;

//  while(exit_flag != 1)
  {
    if((ap_message = json_read_obj_from_file(POST_FILE)) == NULL)
    {
      return;
      //continue;
    }

    ap = auth_ap(ap_message);
    pthread_create(&ap->ap_thread, NULL, (void*(*)(void*))parse_ap_message, ap);
    pthread_join(ap->ap_thread, NULL);

    json_destroy(ap_message);

    pthread_mutex_lock(&ap->ap_lock);
    show_queue();
    pthread_mutex_unlock(&ap->ap_lock);

    sleep(5);
  }
}

int main(int argc, char const *argv[])
{
  exit_flag = 0;

  signal(SIGINT, sigint_handler);

  /*TODO Создаем/открываем файл логирования*/

  /*Инициализируем объект общей базы*/
  if(open_global_db() == ERROR)
  {
    /*TODO Сообщение об ошибке*/
    return ERROR;
  }

  /*Инициализация стека очереди*/
  if(open_queue() == ERROR)
  {
    /*TODO Сообщение об ошибке*/
    return ERROR;
  }

  /*Инициализация списка ТД*/
  if(init_ap_list() == ERROR)
  {
    /*TODO Сообщение об ошибке*/
    return ERROR;
  }

/* Циклы в потоках
  keep_statistics();  // Читает данные из очереди и ведет статистику
  gtk_display();*/

  main_loop();

  close_queue();
  /* BAG При использовании free() для очистки указателя
   * в функции close_queue(), память не освобождается.
   * Но при этом в main() сробатывает*/
  free(queue);

  destroy_ap_list();
  close_save_global_db();
  return VALID;
}
