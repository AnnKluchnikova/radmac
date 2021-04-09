#include <stdio.h>
#include <stdarg.h>
#include "json-parser.h"

/*TODO Переписать библиотеку*/

/* Функция, считывающая данные из файла и записывающая в объект json
 * для хранения в памяти*/
json_object* json_get_table(const char *filename)
{
  json_object* table = json_object_from_file(filename);
  return (json_object*)table;
}

/* Функция для освобождения выделеной памяти под объекты json
 * ПРИМЕЧАНИЕ: Внутри стандартной функции существует проверку на
 * пустоту объекта, НО она не срабатывает. Поэтому следите за тем,
 * чтобы повторно не использовать функцию для очистки того же блока*/
void json_destroy(json_object *context)
{
  json_object_put(context);
  context = NULL;
}

/* Функция для получения строки значения из связки ключ-значение,
 * включенной в json-объект, по указанному ключу*/
const char* json_get_string_from_object(json_object *obj, const char *key)
{
  json_object* value = NULL;

  json_object_object_get_ex(obj, key, &value);
  return json_object_get_string(value);
}

/*Функция для возврата числового значения из объекта*/
int json_get_int_from_object(json_object *obj, const char *key)
{
  json_object* value = NULL;

  json_object_object_get_ex(obj, key, &value);
  return json_object_get_int(value);
}

/* Функция для возврата числового значения из объекта,
 * включенного в массива*/
int json_get_int_from_array(json_object *array, int index, const char *key)
{
  json_object* array_elem = NULL; //Элемент json -массива: json-объект

  array_elem = json_object_array_get_idx(array, index);
  return json_get_int_from_object(array_elem, key);
}

/* Функция для получения значения из связки ключ-значение,
 * включенной в json-массив, по указанному ключу и индексу*/
const char* json_get_string_from_array(json_object *array, int index, const char *key)
{
  json_object* array_elem = NULL; //Элемент json -массива: json-объект

  array_elem = json_object_array_get_idx(array, index);
  return json_get_string_from_object(array_elem, key);
}

/* Функция, возвращающая связку ключ-значение из элемента массива
 * по указанным индексу и ключу*/
json_object* json_get_object_from_array(json_object *array, int index, const char *key)
{
  json_object* array_elem = NULL; //Элемент json -массива: json-объект
  json_object* value = NULL;      //Элемент json-объекта: ключ-значение

  array_elem = json_object_array_get_idx(array, index);
  json_object_object_get_ex(array_elem, key, &value);

  return value;
}

/*Функция для добавления нового элемента-объекта в массив*/
void json_add_new_elem_in_array(json_object *array, const char *key, void *value)
{
  json_object *new_elem = NULL;
  json_object *string = NULL;

  new_elem = json_object_new_object();
  string = json_object_new_string(value);
  json_object_object_add(new_elem, key, string);

  json_object_array_add(array, new_elem);
}

/* Функция создает объект и копирует в него данные из другого объекта
 * по указанным ключам, после чего добавляет объект в массив*/

/*TODO Хорошо подумать над копированием, возможно поможет json_object_get()*/
void json_add_obj_by_keys_to_array(json_object *array, json_object *obj,
                                   int num_keys, ...)
{
  int i = 0;
  char *key = NULL;
  json_object* value = NULL;
  json_object* new_obj = NULL;

  const char *string = NULL; // *
  int digit = 0; // *

  if(num_keys == 0)
    return;

  new_obj = json_object_new_object();

  va_list ap;
  va_start(ap, num_keys);

  for(; i < num_keys; i++)
  {
    key = va_arg(ap, char *);
    if(json_object_object_get_ex(obj, key, &value) == 0)
      return;
    /* Сложная схема копирования нужна, чтобы избежать ошибок при очистке памяти
     * Существует два параллельных ЭЛЕМЕНТА: таблица, считанная из файла,
     * и список уникальных адресов, если копировать ОБЪЕКТЫ из одного в другой,
     * то при очистке по хранимым указателям в одном из ЭЛЕМЕНТОВ не окажется
     * данных, из-за чего система падает*/
    switch((int)json_object_get_type(value))
    {
      case json_type_int:
        digit = json_object_get_int(value); // *
        value = json_object_new_int(digit); // *
        break;
      case json_type_string:
        string = json_object_get_string(value); // *
        value = json_object_new_string(string); // *
        break;
    }
    /**************************************************************************/
    json_object_object_add(new_obj, key, value);
  }

  va_end(ap);
  json_object_array_add(array, new_obj);
}

/*Функция для передачи "красивой" строки*/
const char* json_get_serialize(json_object* context)
{
  if (context == NULL)
    return NULL;
  return json_object_to_json_string_ext(context, JSON_C_TO_STRING_PRETTY);
}

/*Функция для записи объекта json в создаваемый файл в "красивом" виде*/
int json_save_to_file(const char* filename, json_object* contect)
{
  FILE *fd = fopen(filename, "w+");
  if(fd == NULL)
    return -1;

  fprintf(fd, "%s\n", json_get_serialize(contect));
  fclose(fd);

  return 0;
}

/*[Действие] Функция парсит файл, формируя объект json
 *
 *[Примечание] Используется стандартная функция json-c, которая может привести
 *             к падению проекта, если файл окажется пустым. Поэтому в функцию
 *             отдельно включена задача проверки файла на не пустоту*/
void* json_read_obj_from_file(const char* filename)
{
  int pos = 0;
  void *context = NULL;

  /*Проверка файла на пустоту*/
  FILE *fd = fopen(filename, "r");
  /*Ставим указатель в конец*/
  fseek(fd, 0, SEEK_END);

  /*Получаем номер позиции*/
  pos = ftell(fd);
  if(pos == 0)
  {
    fclose(fd);
    return NULL;
  }

  fclose(fd);
  context = json_object_from_file(filename);

  return context;
}
