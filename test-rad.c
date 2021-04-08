#include <stdio.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <time.h>
#include <stdlib.h>
#include "json-parser.h"

#define LOGFILE   "./test-rad-log"
//#define POST_FILE "./real-tab.json"
#define POST_FILE "/var/www/html/radmac/test.json"
#define MAC_FILE "./unique_mac.json"
#define GET_INTERVAL 5
#define PERIOD 10

//------------------------------------------------------------------------------

/*Список ТД*/
/*typedef struct ap_info_s
{
  time_t last_conn;
  const char *ap_id;
  const char *ap_name;
  //TODO Добавить данные конфигурации
  json_object *mac_list;
  struct ap_info_s *next;
}ap_info_t;

typedef struct list_s
{
  unsigned int len;
  ap_info_t *head;
}list_t;

list_t *ap_list;*/

/*Время для сбора статистики*/
/*typedef struct stat_time_s
{
  time_t start;
  int period;
}stat_time_t;*/

//------------------------------------------------------------------------------

struct
{
  unsigned int start;
  unsigned int finish;
  unsigned int counter;
  unsigned int period;
}stat_time_s;

time_t last_conn;         // Время последнего контакта ТД с сервером
const char *ap_id;     // MAC адрес ТД
const char *ap_name;   // Географический домен ТД (локация)
json_object *mac_list = NULL;

char exit_flag = 0;

//------------------------------------------------------------------------------

/*Функция для перевода unix-времени в понятное для человека*/
char* convert_utime(time_t utime)
{
  int ret = 0;
  char buff[32];
  char *strtime = NULL;
  struct tm *calendar = localtime(&utime);

  memset(buff, 0, 32);
  ret = strftime(buff, 20, "%d.%m.%Y %H:%M:%S", calendar);

  if(ret == 0)
    return NULL;

  strtime = calloc(1, ret + 1);
  strncpy(strtime, buff, ret);

  return strtime;
}

//------------------------------------------------------------------------------

void read_title(void *table)
{
  char *strtime = NULL;

  /*Находим и записываем информацию о ТД*/
  ap_id = json_get_string_from_object(table, "ap_id");
  ap_name = json_get_string_from_object(table, "ap_name");

  last_conn = time(NULL);
  strtime = convert_utime(last_conn);

  printf("ap_id: %s ap_name: %s time: %s\n", ap_id, ap_name, strtime);
  free(strtime);
}

/*==============ВЫЯВЛЕНИЕ УНИКАЛЬНЫХ АДРЕСОВ=======(НАЧАЛО)===================*/

void save_new_unique_mac(json_object *arr_elem)
{
  int index = 0;
  char not_unique = 0;
  char *unique_mac = NULL;
  char *new_mac = NULL;

  int len_mac_list = json_object_array_length(mac_list);
  new_mac = json_get_string_from_object(arr_elem, "mac");

  for(; index < len_mac_list; index++)
  {
    unique_mac = json_get_string_from_array(mac_list, index, "mac");
    if(strcmp(unique_mac, new_mac) == 0)
      not_unique = 1;
  }

  if(not_unique == 0)
  {
    json_add_obj_by_keys_to_array(mac_list, arr_elem, 3,
                                  "mac", "timestamp", "type_bitmask");
  }
}

void get_unique_mac_from_context(void *table)
{
  int index = 0;                 // Счетчик индексов массива
  int arr_len = 0;               // Размер массива
  json_object *array = NULL;     // json-массив, взятый из объекта

  json_object_object_get_ex(table, "probe_requests", &array);
  arr_len = json_object_array_length(array);

  for(; index < arr_len; index++)
  {
    save_new_unique_mac(json_object_array_get_idx(array, index));
  }

}
/*==============ВЫЯВЛЕНИЕ УНИКАЛЬНЫХ АДРЕСОВ===(КОНЕЦ)========================*/

/*==============ПОДСЧЕТ УНИКАЛЬНЫХ АДРЕСОВ ЗА ВРЕМЯ===(НАЧАЛО)================*/

void count_mac_for_period_from_last(int period, json_object *list)
{
  int regtime = 0;
  stat_time_s.counter = 0;
  int index = json_object_array_length(list) - 1;
  char *sstart, *sfinish;

  stat_time_s.finish = atoi(json_get_string_from_array(list, index - 1, "timestamp"));
  stat_time_s.start = stat_time_s.finish - period;

  for(; index > 0; index--)
  {
    regtime = atoi(json_get_string_from_array(list, index, "timestamp"));
    if(regtime < stat_time_s.start)
      break;

    if(regtime <= stat_time_s.finish)
      stat_time_s.counter++;
  }

  sstart = convert_utime(stat_time_s.start);
  sfinish = convert_utime(stat_time_s.finish);
  printf("From %s to %s counted %d MAC\n\n", sstart, sfinish,
                                             stat_time_s.counter);

  free(sstart);
  free(sfinish);
}

/*==============ПОДСЧЕТ УНИКАЛЬНЫХ АДРЕСОВ ЗА ВРЕМЯ===(КОНЕЦ)=================*/

void parse_table(void *table)
{
  /*Считываение обзей информации о ТД*/
  read_title(table);
  /*Выявление уникальных записей в пришедшей таблице*/
  get_unique_mac_from_context(table);
}

//==============================================================================

static void sigint_handler(int signo)
{
  exit_flag = 1;
}

//==============================================================================

int main(int argc, char const *argv[])
{
  json_object *table = NULL;

  signal(SIGINT, sigint_handler);

  mac_list = json_object_from_file(MAC_FILE);
  if(mac_list == NULL)
    mac_list = json_object_new_array();

/*LOOP 1*/
  //while(exit_flag != 1)
  {
    table = json_object_from_file(POST_FILE);
    if(table == NULL)
    {
      printf("[ERROR]: Failed to read file data\n"); //TODO Уточнить
      //continue;
    }

    parse_table(table);
    //printf("%s\n", json_get_serialize(mac_list));

    json_destroy(table);
    count_mac_for_period_from_last(PERIOD, mac_list);
    //sleep(GET_INTERVAL);
  }

/*LOOP 2*/
  /*TODO Статистика за n-e время*/

  json_save_to_file(MAC_FILE, mac_list);

  json_destroy(mac_list);
  return 0;
}