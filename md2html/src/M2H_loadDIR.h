#ifndef __M2H_LOAD_DIR__
#define __M2H_LOAD_DIR__

#include "M2H_base.h"
#include <dirent.h>

/*
* @func 返回当前路径
* @pram 传入文件名，返回整个文件路径；传入NULL，返回当前目录
*/
char *get_file_path(char *file_name, int suffix);

/*
* @func 加载和保存MD文档目录结构
* @ret  成功 1，失败 0
*/
int M2H_LoadDIR();

#endif

