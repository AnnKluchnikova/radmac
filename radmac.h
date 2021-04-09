#ifndef __RADAR__
#define __RADAR__

/*Возвращаемые значения функций*/
#define ERROR -1
#define VALID 0

/*Флаг завершения работы программы*/
volatile char exit_flag;

/*Объект общей базы с уникальными MAC-адресами*/
void *global_db;

#endif
