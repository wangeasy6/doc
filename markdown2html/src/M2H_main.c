#include "M2H_main.h"
#include "M2H_loadDIR.h"

S_CONFIG g_config;

int main(int argc, char *argv[])
{
      if ( FALSE == M2H_loadConfig() )
      {
            printf("load Config failed\r\n");
            return FALSE;
      }else{
      printf("load Config: md_path=%s html_path=%s is_load_private=%d\r\n", \
            g_config.md_path, g_config.html_path, g_config.is_load_private);
      }

      if ( FALSE == M2H_LoadDIR() )
      {
            printf("load DIR failed\r\n");
            return FALSE;
      }
      return TRUE;
}

int M2H_loadConfig()
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
                  for ( i = 0; i < strlen(read_str) ; i++ )
                  {
                        if( '\"' == read_str[i] )
                        {
                              i++;
                              for ( j = 0; i < strlen(read_str) && '\"' != read_str[i] ; i++,j++ )
                              {
                                    g_config.md_path[j] = read_str[i];
                              }
                        }
                  }
            }
            
            if ( NULL != strstr(read_str, "html_path"))
            {
                  for ( i = 0; i < strlen(read_str) ; i++ )
                  {
                        if( '\"' == read_str[i] )
                        {
                              i++;
                              for ( j = 0; i < strlen(read_str) && '\"' != read_str[i] ; i++,j++ )
                              {
                                    g_config.html_path[j] = read_str[i];
                              }
                        }
                  }
            }
             
            if ( NULL != strstr(read_str, "is_load_private"))
            {
                  for ( i = 0; i < strlen(read_str) ; i++ )
                  {
                        if( '=' == read_str[i] )
                        {
                              i++;
                              for ( ; i < strlen(read_str) && '\"' != read_str[i] ; i++ )
                              {
                                    if ( ' ' != read_str[i])
                                    {
                                          g_config.is_load_private = read_str[i] - 48;
                                          break;
                                    }
                              }
                        }
                  }
            }
      }
      return TRUE;
}

