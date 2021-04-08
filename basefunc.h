#ifndef __BASIC__
#define __BASIC__

#include "radmac.h"

/*[Дейтвие] Инициализация главного объекта для хранения полученных MAC-адресов*/
int open_global_db(void);
/**[Дейтвие] Сохранение объекта с данными в файл и очистка его блока памяти*/
int close_save_global_db(void);



#endif