#ifndef __BASIC__
#define __BASIC__

#include "radmac.h"

/*[Дейтвие] Инициализация главного объекта для хранения полученных MAC-адресов*/
int open_global_db(void);
/**[Дейтвие] Сохранение объекта с данными в файл и очистка его блока памяти*/
int close_save_global_db(void);

/*[Действие] Проверка пришедшего адрес на совпадение с элементами общего списка
 *           Если совпадений нет, то в список сохраняется адрес с некоторыми
 *           атребутами*/
void save_new_unique_mac(void* context, void* element);

#endif