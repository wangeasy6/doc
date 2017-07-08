#ifndef __M2H_BASE__
#define __M2H_BASE__

#include <stdio.h>
#include <string.h>

#define MAX_PATH_LEN 200

#define TRUE 1
#define FALSE 0


typedef struct S_CONFIG_
{
      char md_path[MAX_PATH_LEN];
      char html_path[MAX_PATH_LEN];
      unsigned char is_load_private;
}S_CONFIG;

extern S_CONFIG g_config;

#endif
