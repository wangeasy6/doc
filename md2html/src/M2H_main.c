#include "M2H_main.h"
#include "M2H_loadDIR.h"

S_CONFIG g_config;


#define CONFIG_PATH "../config.ini"

#define MAX_READ_LEN 512
static int analysis_config(const char *Sentence,void *Storage_addr, char types)
{
      int i,j,ret;
      if(Sentence == NULL || Storage_addr == NULL)
      {
            printf("prams is null\r\n");
            return FALSE;
      }
      switch(types)
      {
            case 's':
            {
                  for ( i = 0; i < strlen(Sentence) ; i++ )
                  {
                        if( '\"' == Sentence[i] )
                        {
                              i++;
                              for ( j = 0; i < strlen(Sentence) && '\"' != Sentence[i] ; i++,j++ )
                              {
                                    *((char *)Storage_addr + j) = Sentence[i];
                              }
                        }
                  }
                  break;
            }
            case 'i':
            {
                  for ( i = 0; i < strlen(Sentence) ; i++ )
                  {
                        if( '=' == Sentence[i] )
                        {
                              i++;
                              for ( ; i < strlen(Sentence); i++ )                   //支持中间有空格
                              {
                                    if ( ' ' != Sentence[i])
                                    {
                                          *(int *)Storage_addr = Sentence[i] - 48;
                                          break;
                                    }
                              }
                        }
                  }
                  break;
            }
            default:
                  break;
      }

      return TRUE;
}

/*
* @func 加载配置文件
* @return 成功 1，失败 0
*/
static int M2H_loadConfig()
{
      FILE *fd = fopen( CONFIG_PATH, "r" );
      if ( fd == NULL )
      {
            perror("open config");
            return FALSE;
      }

      int i,j;
      char read_str[MAX_READ_LEN];
      while( NULL != fgets( read_str, MAX_READ_LEN, fd ))
      {
            if ( NULL != strstr(read_str, "md_path"))
            {
                  analysis_config(read_str, g_config.md_root_path, 's');
            }
            
            if ( NULL != strstr(read_str, "html_path"))
            {
                  analysis_config(read_str, g_config.html_root_path, 's');
            }
             
            if ( NULL != strstr(read_str, "is_load_private"))
            {
                  analysis_config(read_str, &g_config.is_load_private, 'i');
            }
      }
      return TRUE;
}

int M2H_checkDIR(char *dir_path)
{
      struct dirent *dirp = NULL;
      struct stat dir_stat;
      
      if (lstat(dir_path, &dir_stat) < 0)
      {
            if ( 0 == mkdir(dir_path, (mode_t)0777))
            {
                  printf("mkidr:%s\r\n",dir_path);
                  return TRUE;
            }else
            {
                  perror("mkdir");
                  return FALSE;
            }
      }else
      if (S_ISDIR(dir_stat.st_mode))
      {
            return TRUE;
      }
      
      return FALSE;
}

int main(int argc, char *argv[])
{
      if ( FALSE == M2H_loadConfig() )
      {
            printf("load Config failed\r\n");
            return FALSE;
      }else{
            printf("load Config: md_path=%s html_path=%s is_load_private=%d\r\n", \
                  g_config.md_root_path, g_config.html_root_path, g_config.is_load_private);
      }
/*
      if ( FALSE == M2H_checkDIR(g_config.html_root_path))
      {
            return FALSE;
      }
      
      if ( FALSE == M2H_LoadDIR() )
      {
            printf("load DIR failed\r\n");
            return FALSE;
      }*/
      return TRUE;
}
