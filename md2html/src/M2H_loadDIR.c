#include "M2H_loadDIR.h"
#include "M2H_convert.h"

#define FILE_DEPTH 200                                      //路径最大深度
char g_file_path[FILE_DEPTH][MAX_PATH_LEN] = {'\0'};        //保存当前路径
unsigned int index_file_depth = 0;                          //记录当前路径

char *get_file_path(const char *file_name, int suffix)
{
      static int i = 0;
      static char file_path[MAX_PATH_LEN] = {'\0'};
      memset(file_path, '\0', MAX_PATH_LEN);

      switch(suffix)                                              //路径前缀
      {
            case 0:
                  strncat(file_path, g_config.md_root_path, MAX_PATH_LEN);
                  break;
            case 1:
                  strncat(file_path, g_config.html_root_path, MAX_PATH_LEN);
                  break;
            default:
                  break;
      }
      strncat(file_path, "/\0", 2);
      
      for (i = 1; i < index_file_depth; i++)                      //路径拼接
      {
            strncat(file_path, g_file_path[i], MAX_PATH_LEN);
            strncat(file_path, "/\0", 2);
      }
      
      if (file_name != NULL)                                      //文件
      {
            strncat(file_path, file_name, MAX_PATH_LEN);
      }
      
      return file_path;
}

static int loop_loadDIR(const char *path)
{
      int ret = TRUE;
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
            }
            else if (S_ISDIR(dir_stat.st_mode))
            {     
                  if (g_config.is_load_private == 0 && NULL != strstr(dirp->d_name, "private"))
                  {
                        printf("Skip the private folder\r\n");
                        continue;
                  }
                  
                  strncpy(g_file_path[index_file_depth], dirp->d_name, MAX_PATH_LEN);
                  index_file_depth++;
                  if ( FALSE == M2H_checkDIR(get_file_path(NULL, 1)))
                  {
                        printf("check DIR error:%s\r\n", get_file_path(NULL, 1));
                        ret = FALSE;
                        break;
                  }
                  if ( FALSE == loop_loadDIR(get_file_path(NULL, 0)))
                  {
                        printf("load DIR error:%s", get_file_path(NULL, 0));
                        index_file_depth--;
                        ret = FALSE;
                        break;
                  }
                  memset(g_file_path[index_file_depth], '\0', MAX_PATH_LEN);
                  index_file_depth--;
            }
            else if (g_config.is_load_private == 0 && NULL != strstr(dirp->d_name, "_p.md"))
            {
                  printf("%s is private,jump over\r\n",dirp->d_name);
                  continue;
            }
            else if ( NULL != strstr(dirp->d_name, ".md"))
            {
                  printf("%s\r\n",get_file_path(dirp->d_name, 0));
                  
                  if (TRUE == M2H_convet(dirp->d_name))
                  {
                        printf("                                                                        Conveted\r\n");
                  }
                  else
                  {
                        printf("                                                                        Error\r\n");
                        ret = FALSE;
                        break;
                  }
            }
            else
            {
                  char org_file_temp[MAX_PATH_LEN];
                  strncpy(org_file_temp, get_file_path(dirp->d_name, 0), MAX_PATH_LEN);
                  if ( FALSE == M2H_copyFile(org_file_temp, get_file_path(dirp->d_name, 1)))
                  {
                        printf("Copy file failed\r\n");
                        ret = FALSE;
                        break;
                  }
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

      if ( FALSE == loop_loadDIR(g_config.md_root_path))
            return FALSE;
      
      return TRUE;
}
