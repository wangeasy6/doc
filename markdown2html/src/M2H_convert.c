#include "M2H_convert.h"

FILE *g_fd_output;

char g_tag_stack[2][STACK_DEPTH][MAX_TAG_LEN];
unsigned char g_stack_index = 0;
unsigned char g_same_tag = 0;

//unsigned int g_

static void stack_up(char *prefix, char *suffix)
{
      if(g_stack_index >= STACK_DEPTH)
      {
            printf("stack spill up\r\n");
            return;
      }

      unsigned int i;
      unsigned int indent = 4 * g_stack_index;
      char indent_str[MAX_PATH_LEN] = {0};
      for (i = 0; i < indent ; i++)
      {
            indent_str[i] = ' ';
      }
      indent_str[i] = '\0';

      snprintf(g_tag_stack[PREFIX][g_stack_index], MAX_TAG_LEN, "%s%s", indent_str, prefix);
      strncpy(g_tag_stack[SUFFIX][g_stack_index], suffix, MAX_TAG_LEN);
      fputs( g_tag_stack[PREFIX][g_stack_index], g_fd_output);

      if ( NULL != strstr(g_tag_stack[PREFIX][g_stack_index-1], prefix)|| NULL != strstr(g_tag_stack[SUFFIX][g_stack_index-1], suffix))
      {
            g_same_tag +=1;
      }else{
            g_stack_index += 1;
      }

      return;
}
static void stack_down()
{

      if(g_stack_index > 0)
            {
                  if (g_same_tag > 0)
                        g_same_tag -= 1;
                  else
                        g_stack_index -= 1;
                  
                  fputs( g_tag_stack[SUFFIX][g_stack_index], g_fd_output);

                  memset(g_tag_stack[PREFIX][g_stack_index], 0, MAX_TAG_LEN);
                  memset(g_tag_stack[SUFFIX][g_stack_index], 0, MAX_TAG_LEN);
            }
      else
            printf("stack spill down\r\n");
      return;
}

static void analysis_md(char *sentence)
{
      int i;
      char prefix[MAX_TAG_LEN] = {0};
      char suffix[MAX_TAG_LEN] = {0};
      switch(sentence[0])
      {
            case '#':
            {
                  for(i = 0; i < strlen(sentence) ; i++)
                  {
                        if( '#' != sentence[i] )
                              break;
                  }
                  snprintf(prefix, MAX_TAG_LEN, "<h%d>", i);
                  snprintf(suffix, MAX_TAG_LEN, "</h%d>\r\n", i);
                  
                  stack_up(prefix, suffix);
                  fputs(&sentence[i], g_fd_output);
                  stack_down();
                  
                  break;
            }
            case '*':
            {
                  break;
            }
            case '%':
            {
                  break;
            }
            default:
                  break;
     }

      return;
}

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
      
      if ( FALSE == M2H_copyFile("../template/header.html", html_path))
      {
            printf("%s,%d: copy file failed\r\n", __FUNCTION__, __LINE__);
            return FALSE;
      }

      FILE *fd_md = fopen( md_path, "r" );
      if ( fd_md == NULL )
      {
            perror("open md");
            return FALSE;
      }

      FILE *fd_html = fopen( html_path, "a" );
      if ( fd_html == NULL )
      {
            perror("open html");
            return FALSE;
      }
      g_fd_output = fd_html;

      stack_up("<body>\r\n", "</body>\r\n");
      
      while( NULL != fgets(buffer, BUFFER_SIZE, fd_md) )
      {
            analysis_md(buffer);
            memset(buffer, BUFFER_SIZE, 0);
      }
      
      stack_down();
      
      char html_eof[] = "</html>\r\n";
      fputs( html_eof, fd_html);

      fclose(fd_html);
      fclose(fd_md);
      return TRUE;
}


int M2H_copyFile(char *org_path, char *tar_path)
{
      char buffer[BUFFER_SIZE] = {0};
      printf("%s: Copy %s to %s\r\n",__FUNCTION__, org_path, tar_path);
      
      FILE *fd_org = fopen( org_path, "r" );
      if ( fd_org == NULL )
      {
            perror("open org");
            return FALSE;
      }
      FILE *fd_tar = fopen( tar_path, "w+" );
      if ( fd_tar == NULL )
      {
            perror("open tar");
            return FALSE;
      }

      while( NULL != fgets(buffer, BUFFER_SIZE, fd_org) )
      {
            if ( EOF ==  fputs(buffer, fd_tar) )
            {
                  printf("write fd_tar error\r\n");
                  return FALSE;
            }
            memset(buffer, 0, sizeof(buffer));
      }

      fclose(fd_org);
      fclose(fd_tar);

      return TRUE;
}
