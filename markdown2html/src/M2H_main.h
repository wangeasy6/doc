#ifndef __M2H_MAIN__
#define __M2H_MAIN__

#include "M2H_base.h"

#define CONFIG_PATH "../config.ini"

#define MAX_READ_LEN 512

/*
* @func 加载配置文件
* @return 成功 1，失败 0
*/
int M2H_loadConfig();

#endif
