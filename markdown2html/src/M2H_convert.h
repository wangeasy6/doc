#ifndef __M2H_CONVERT__
#define __M2H_CONVERT__

#include "M2H_base.h"

#define BUFFER_SIZE 512

#define MAX_TAG_LEN 1024
#define STACK_DEPTH 50
#define PREFIX 0
#define SUFFIX 1

int M2H_convet( char *md_file_name);
int M2H_copyFile(char *org_path, char *tar_path);



#endif
