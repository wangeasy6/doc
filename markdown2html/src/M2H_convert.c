#include "M2H_convert.h"

int M2H_convet( char *md_file_name)
{
      
      char md_path[MAX_PATH_LEN] = {0};
      char html_path[MAX_PATH_LEN] = {0};
      
      char html_file_name[MAX_PATH_LEN] = {0};
      char suffix[] = "html";
      char buffer[BUFFER_SIZE] = {0};
      int i,j;
      
      for(i = 0; i < strlen(md_file_name); i++)
      {
            html_file_name[i] = md_file_name[i];
            if ( '.' == md_file_name[i] )
            {
                  i++;
                  for( j = 0; i < MAX_PATH_LEN && j < strlen(suffix); i++,j++)
                  {
                        html_file_name[i] = suffix[j];
                  }
                  break;
            }
      }

      snprintf( md_path, MAX_PATH_LEN, "%s/%s", g_config.md_path, md_file_name);
      snprintf( html_path, MAX_PATH_LEN, "%s/%s", g_config.html_path, html_file_name);
      
      FILE *fd_header = fopen( "../template/header.html", "r" );
      if ( fd_header == NULL )
      {
            perror("open header");
            return FALSE;
      }
      FILE *fd_html = fopen( html_path, "w+" );
      if ( fd_html == NULL )
      {
            perror("open html");
            return FALSE;
      }

      while( NULL != fgets(buffer, BUFFER_SIZE, fd_header) )
      {
            if ( EOF ==  fputs(buffer, fd_html) )
            {
                  printf("write fd_html error\r\n");
                  return FALSE;
            }
            memset(buffer, 0, sizeof(buffer));
      }

      FILE *fd_md = fopen( md_path, "r" );
      if ( fd_md == NULL )
      {
            perror("open md");
            return FALSE;
      }

      char body_start[] = "<body>\r\n";
      char body_eof[] = "</body>\r\n";
      char html_eof[] = "</html>\r\n";
      fputs( body_start, fd_html);
      while( NULL != fgets(buffer, BUFFER_SIZE, fd_md) )
      {
            break;
      }
     fputs( body_eof, fd_html);
     fputs( html_eof, fd_html);

      fclose(fd_header);
      fclose(fd_html);
      fclose(fd_md);
      return TRUE;
}

