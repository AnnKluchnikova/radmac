#ifndef __RADAR__
#define __RADAR__

#include <time.h>

/*Возвращаемые значения функций*/
#define ERROR -1
#define VALID 0

/*Флаг завершения работы программы*/
volatile char exit_flag = 0;

/*Объект общей базы с уникальными MAC-адресами*/
void *global_db = NULL;

#endif
