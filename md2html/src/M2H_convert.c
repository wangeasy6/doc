#include "M2H_convert.h"

FILE *g_fd_output;

char g_tag_stack[2][STACK_DEPTH][MAX_TAG_LEN];  //以栈结构存储tag
unsigned char g_stack_index = 0;                //计数缩进
unsigned char g_same_tag = 0;                   //数是否有相同tag，使相同tag有相同的缩进

unsigned int g_l1 = 0;       //用于标题计数
unsigned int g_l2 = 0;       //用于行计数
unsigned int g_l3 = 0;       //用于子行计数

extern char **g_file_path;

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
      int i = 0;
      char prefix[MAX_TAG_LEN] = {0};
      char suffix[MAX_TAG_LEN] = {0};
      char txt[MAX_TXT_LEN] ={0};
      
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

                  if (3 == i)
                        snprintf(txt, MAX_TXT_LEN, "%d、%s", g_l1++, &sentence[i]);
                  else
                        snprintf(txt, MAX_TXT_LEN, "%s", &sentence[i]);

                  stack_up(prefix, suffix);
                  fputs(txt, g_fd_output);
                  stack_down();

                  g_l2 = 0;
                  break;
            }
            case '*':
            { 
                  while (1)
                  {
                        if (*sentence == '*' && *(sentence+1) == ' ')
                        {
                              i++;
                              sentence += 2;
                        }
                        else
                              break;
                  }

                  snprintf(prefix, MAX_TAG_LEN, "<div class=\"text-indent%d\">",i);
                  snprintf(suffix, MAX_TAG_LEN, "</div>\r\n");
                  
                  if ( i == 1)
                  {
                        snprintf(txt, MAX_TXT_LEN, "%d、%s", g_l2++, sentence);
                        g_l3 = 0;
                  }
                  if ( i == 2)
                  {
                        snprintf(txt, MAX_TXT_LEN, "%d、%s", g_l3++, sentence);
                  }

                  stack_up(prefix, suffix);
                  fputs(txt, g_fd_output);
                  stack_down();
                  
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

      if ( FALSE == M2H_checkDIR(get_file_path(NULL, 1)))
            return FALSE;
      
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
      
      if ( FALSE == M2H_copyFile("../template/header.html", get_file_path(html_file_name, 1)))
      {
            printf("%s,%d: copy file failed\r\n", __FUNCTION__, __LINE__);
            return FALSE;
      }

      FILE *fd_md = fopen( get_file_path(md_file_name, 0), "r" );
      if ( fd_md == NULL )
      {
            perror("open md");
            return FALSE;
      }

      FILE *fd_html = fopen( get_file_path(html_file_name, 1), "a" );
      if ( fd_html == NULL )
      {
            perror("open html");
            fclose(fd_md);
            return FALSE;
      }
      g_fd_output = fd_html;
      g_l1 = 0;

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


int M2H_copyFile(char *org_file_path, char *tar_file_path)
{
      char buffer[BUFFER_SIZE] = {0};
      printf("%s: Copy %s to %s\r\n",__FUNCTION__, org_file_path, tar_file_path);
      
      FILE *fd_org = fopen( org_file_path, "r" );
      if ( fd_org == NULL )
      {
            perror("open org");
            return FALSE;
      }

      char test_file_path[MAX_PATH_LEN] = {"\0"};
      strncat(test_file_path, tar_file_path, MAX_PATH_LEN);
      //FILE *fd_tar = fopen( tar_file_path, "w" );
      FILE *fd_tar = fopen( test_file_path, "w" );
      if ( fd_tar == NULL )
      {
            perror("open tar");
            fclose(fd_org);
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
