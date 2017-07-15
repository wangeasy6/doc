#include "M2H_loadDIR.h"
#include "M2H_convert.h"

#define FILE_DEPTH 48

char g_file_path[FILE_DEPTH][MAX_PATH_LEN] = {'\0'};        //保存当前路径
unsigned int index_file_depth = 0;                          //记录当前路径

char *get_file_path(char *file_name, int suffix)
{
      static int i = 0;
      static char file_path[MAX_PATH_LEN] = {'\0'};
      memset(file_path, '\0', MAX_PATH_LEN);

      if (suffix == 0)
            strncat(file_path, g_config.md_root_path, MAX_PATH_LEN);
      if (suffix == 1)
            strncat(file_path, g_config.html_root_path, MAX_PATH_LEN);
      strncat(file_path, "/\0", 2);
      
      for (i = 1; i < index_file_depth; i++)
      {
            strncat(file_path, g_file_path[i], MAX_PATH_LEN);
            strncat(file_path, "/\0", 2);
      }
      if (file_name != NULL)
      {
            strncat(file_path, file_name, MAX_PATH_LEN);
      }
      return file_path;
}

static int loop_readdir(char *path)
{
      DIR *dp = NULL;
      struct dirent *dirp = NULL;
      struct stat dir_stat;

      if( path == NULL)
      {
            printf("%s, error path\n",__FUNCTION__);
            return FALSE;
      }
      
      if ( NULL == (dp = opendir(path)) )
      {
            printf("cat't open %s\r\n", path);
            return FALSE;
      }

      while ( NULL != ( dirp = readdir(dp) ))
      {
            if( strcmp(dirp->d_name, ".") == 0|| strcmp(dirp->d_name, "..") == 0)
                  continue;
            
            if (lstat(get_file_path(dirp->d_name, 0), &dir_stat) < 0)
            {
                  printf("lstat error:%s\n",get_file_path(dirp->d_name, 0));
                  continue;
            }else
            if (S_ISDIR(dir_stat.st_mode))
            {
                  strncpy(g_file_path[index_file_depth], dirp->d_name, MAX_PATH_LEN);
                  index_file_depth++;
                  if ( FALSE == M2H_checkDIR(get_file_path(NULL, 1)))
                        return FALSE;
                  loop_readdir(get_file_path(NULL, 0));
                  memset(g_file_path[index_file_depth], '\0', MAX_PATH_LEN);
                  index_file_depth--;
            }else
            if ( NULL != strstr(dirp->d_name, ".md"))
            {
                  printf("%s\r\n",get_file_path(dirp->d_name, 0));
                  
                  if (TRUE == M2H_convet(dirp->d_name))
                        printf("                                                                        Conveted\r\n");
                  else
                        printf("                                                                        Error\r\n");
            }else
            {
                  //原样Copy
            }
      }
      closedir(dp);
      return TRUE;
}

int M2H_LoadDIR()
{
      DIR *dp = NULL;

      if ( NULL == (dp = opendir(g_config.md_root_path)) )
      {
            printf("cat't open %s\r\n", g_config.md_root_path);
            return FALSE;
      }
      closedir(dp);

      strncpy(g_file_path[index_file_depth], g_config.md_root_path, MAX_PATH_LEN);
      index_file_depth++;

      loop_readdir(g_config.md_root_path);
      
      return TRUE;
}
