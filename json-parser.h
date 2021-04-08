#ifndef __JSON_PARSER__
#define __JSON_PARSER__

#include <json.h>

/* Функция, считывающая данные из файла и записывающая в объект json
 * для хранения в памяти*/
json_object* json_get_table(const char *filename);

/*Функция, создающая пустой json-массив*/
json_object* json_get_empty_array(void);

/*Функция для освобождения выделеной памяти под объекты json*/
void json_destroy(json_object *context);

/* Функция для получения строки значения из связки ключ-значение,
 * включенной в json-объект, по указанному ключу*/
const char* json_get_string_from_object(json_object *obj, const char *key);

/* Функция для получения значения из связки ключ-значение,
 * включенной в json-массив, по указанному ключу и индексу*/
const char* json_get_string_from_array(json_object *array, int index, const char *key);

/* Функция, возвращающая связку ключ-значение из элемента массива
 * по указанным индексу и ключу*/
json_object* json_get_object_from_array(json_object *array, int index, const char *key);

/*Функция для добавления нового элемента-объекта в массив*/
void json_add_new_elem_in_array(json_object *array, const char *key, void *value);

const char* json_get_serialize(json_object* context);

void json_add_obj_by_keys_to_array(json_object *array, json_object *obj,
                                   int num_keys, ...);

int json_get_int_from_array(json_object *array, int index, const char *key);

/*Функция для записи объекта json в создаваемый файл в "красивом" виде*/
int json_save_to_file(const char* filename, json_object* contect);

#endif
