#ifndef __M2H_CONVERT__
#define __M2H_CONVERT__

#include "M2H_base.h"
#include "M2H_main.h"
#include "M2H_loadDIR.h"

#define BUFFER_SIZE 512

#define MAX_TAG_LEN 1024
#define MAX_TXT_LEN (1024 * 4)
#define STACK_DEPTH 50
#define PREFIX 0
#define SUFFIX 1


/*
*     @func 转换MD为HTML
*     @pram md_file_path ：文件名
*/
int M2H_convet( char *md_file_name);

/*
*     @func 复制文件
*     @pram org_file_path ：原文件的路径
*     @pram org_file_path ：目标文件路径
*/
int M2H_copyFile(char *org_file_path, char *tar_file_path);


#endif
