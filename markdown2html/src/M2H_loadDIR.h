#ifndef __M2H_LOAD_DIR__
#define __M2H_LOAD_DIR__

#include "M2H_base.h"
#include <sys/types.h>
#include <dirent.h>

//#define DATA_LEN 48

typedef struct ALL_MD_
{
      char file_path[MAX_PATH_LEN];
      //char file_last_change[DATA_LEN];
      struct ALL_MD_ *next;
}ALL_MD;

/*
* @函数功能：加载和保存MD文档目录结构
* @return 成功 1，失败 0
*/
int M2H_LoadDIR();

#endif

