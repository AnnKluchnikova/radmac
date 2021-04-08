#include <stdio.h>
#include <signal.h> // signal()

#include "radmac.h"
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

void *parse_ap_message(void* message)
{

}

void take_message(void)
{
  void *ap_message = NULL;
  ap_info_t *ap = NULL;

  while(exit_flag != 1)
  {
    if((ap_message = json_object_from_file(POST_FILE)) == NULL)
      continue;

    ap = register_ap(ap_message);
    pthread_create(&ap->ap_thread, NULL, (void*(*)(void*))parse_ap_message,
                                                          ap_message);

    pthread_join(&ap->ap_thread, NULL);
    json_destroy(ap_message);
  }
}

int main(int argc, char const *argv[])
{
  signal(SIGINT, sigint_handler);

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

  take_message();
  keep_statistics();
  gtk_display();

  destroy_ap_list();
  close_save_global_db();
  return VALID;
}
