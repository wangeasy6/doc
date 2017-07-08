#include "M2H_loadDIR.h"
#include "M2H_convert.h"

ALL_MD g_head;

int M2H_LoadDIR()
{
      DIR *dp = NULL;
      struct dirent *dirp;

      if ( NULL == (dp = opendir(g_config.md_path)) )
      {
            printf("cat't open %s\r\n", g_config.md_path);
            return FALSE;
      }
      
      while ( NULL != ( dirp = readdir(dp) ))
      {
            printf("%s\r\n", dirp -> d_name);
            if ( NULL != strstr(dirp->d_name, ".md"))
            {
                  if ( NULL == g_head.next )
                  {
                        snprintf( g_head.file_path, MAX_PATH_LEN, "%s", dirp->d_name);
                        M2H_convet(g_head.file_path);
                        
                  }
            }
      }
      return TRUE;
}
