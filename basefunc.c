#include "basefunc.h"
#include "json-parser.h"
#include "aplist.h"

/*Файл, в который будет сохраняться база уникальных адресов со всех ТД*/
#define GLOBAL_DB_FILE "./radmac_database.json"

//==============================================================================

/*[Дейтвие] Инициализация главного объекта для хранения полученных MAC-адресов*/
int open_global_db(void)
{
  /*Считываем имеющиеся данные или создаем новый объект*/
  global_db = json_object_from_file(GLOBAL_DB_FILE);
  if(global_db == NULL)
    global_db = json_object_new_array();

  if(global_db == NULL)
    return ERROR;

  return VALID;
}

/**[Дейтвие] Сохранение объекта с данными в файл и очистка его блока памяти*/
int close_save_global_db(void)
{
  if(global_db == NULL)
    return VALID;

  json_save_to_file(GLOBAL_DB_FILE, global_db);
  json_destroy(global_db);

  return VALID;
}

//==============================================================================

