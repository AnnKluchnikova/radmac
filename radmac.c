#include <stdio.h>
#include <signal.h> // signal()

#include "radmac.h"
#include "json-parser.h"

/*Файл, в который пишется таблица MAC-адресов от ТД*/
#define POST_FILE "/var/www/html/radmac/test.json"
/*Файл, в который будет сохраняться база уникальных адресов со всех ТД*/
#define GLOBAL_DB_FILE "./radmac_database.json"

//------------------------------------------------------------------------------

/*[Действие] Перехват сигнала выхода Ctrl+C*/
static void sigint_handler(int signo)
{
  exit_flag = 1;
}

//------------------------------------------------------------------------------

int init_global_db(void)
{
  /*Считываем имеющиеся данные или создаем новый объект*/
  global_db = json_object_from_file(GLOBAL_DB_FILE);
  if(global_db == NULL)
    global_db = json_object_new_array();

  if(global_db == NULL)
    return ERROR;

  return VALID;
}

int main(int argc, char const *argv[])
{
  void *ap_message = NULL;
  const char *ap_id = NULL;

  signal(SIGINT, sigint_handler);

  /*Инициализируем объект общей базы*/
  if(init_global_db == ERROR)
  {
    /*TODO Сообщение об ошибке*/
    return ERROR;
  }

  while(exit_flag != 1)
  {
    if((ap_message = json_object_from_file(POST_FILE)) == NULL)
      continue;

    ap_id = which_ap(ap_message);

  }

  return VALID;
}
