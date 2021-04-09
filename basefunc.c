#include <string.h>

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

/*[Действие] Проверка пришедшего адрес на совпадение с элементами общего списка
 *           Если совпадений нет, то в список сохраняется адрес с некоторыми
 *           атребутами*/
void save_new_unique_mac(void* context, void* element)
{
  int index = 0, length = 0;
  char not_unique = 0;
  const char *unique_mac = NULL, *new_mac = NULL;

  if((context == NULL)||(element == NULL))
    return;

  length = json_object_array_length(context);
  new_mac = json_get_string_from_object(element, "mac");

  for(; index < length; index++)
  {
    unique_mac = json_get_string_from_array(context, index, "mac");
    if(strcmp(unique_mac, new_mac) == 0)
      not_unique = 1;
  }

  if(not_unique == 0)
  {
    json_add_obj_by_keys_to_array(context, element, 3,
                                  "mac", "timestamp", "type_bitmask");
  }
}
