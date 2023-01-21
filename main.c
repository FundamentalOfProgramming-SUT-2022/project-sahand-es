#include <stdio.h>
#include <string.h>
#include <conio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <direct.h>
#include <windows.h>


#define SIZE 1000

enum find_type{COUNT = 1, AT = 1 << 1, BYWORD = 1 << 2, ALL = 1 << 3};

int file_exists(const char address[])
{
    FILE *does_exist = fopen(address, "r");
    if(does_exist == NULL)
    {
        fclose(does_exist);
        return 0;
    }
    fclose(does_exist);
    return 1;
}

void create_file(char address[])
{
    char dirname[SIZE] = {'\0'};
    int first_slash = 0;
    int iterate = 1;

    while (address[iterate] != '\0')
    {
        while(address[iterate] != '/' && address[iterate] != '\0')
        {
            iterate++;
        }
        if(address[iterate] == '\0')
        {
            break;
        }
        strncpy(dirname, address + first_slash + 1, iterate - 1);
        mkdir(dirname);
        iterate++;
    }

    strncpy(dirname, address + first_slash + 1, iterate - 1);

    if(file_exists(dirname))
    {
        printf("file with this name exists.\n");
        return;
    }
    else
    {
        FILE  *new_file = fopen(dirname, "a+");
        fclose(new_file);
    }

}

void insert_str(const char address[],const char string[], int line, int position)
{
    line--;
    int count = strlen(string), i = 0, j = 0, place = 0;
    char c, *name = (char*) address + 1;
    char final_string[SIZE] = {'\0'};

    if(!file_exists(name))
    {
        printf("file with this name does not exists.\n");
        return;
    }

    FILE *file_to_read = fopen(name, "r");
    int iter = 0, line_counter = 0, position_counter = 0, flag = 1;

    while ((c = fgetc(file_to_read)) != EOF)
    {
        final_string[iter] = c;
        if(c == '\n')
        {
            if(line_counter == line && flag)
            {
                for(; position_counter < position; position_counter++)
                {
                    final_string[iter] = ' ';
                    iter++;
                }
                strcat(final_string, string);
                iter += count;

                final_string[iter] = '\n';
                iter++;
                flag = 0;
            }
            else
                iter++;

            line_counter++;
            position_counter = 0;
            continue;

        }
        else if(position_counter == position && line_counter == line && flag)
        {
            final_string[iter] = '\0';
            strcat(final_string, string);
            iter += count;
            final_string[iter] = c;
            flag = 0;
        }
        position_counter++;
        iter++;
    }



    for(i = 0; i < line - line_counter && flag; i++)
    {
        final_string[iter] = '\n';
        iter++;
    }
    for(j = 0; j < position - position_counter && flag; j++)
    {
        final_string[iter] = ' ';
        iter++;
    }
    if(flag)
        strcat(final_string, string);

    FILE *file_to_write = fopen(name, "w");

    fprintf(file_to_write, "%s", final_string);

    fclose(file_to_write);
    fclose(file_to_read);
}

void cat(const char address[])
{
    char *name = (char*) address + 1, c;
    if(!file_exists(name))
    {
        printf("file with this name does not exists.\n");
        return;
    }

    FILE *file_to_read = fopen(name, "r");

    while ((c = fgetc(file_to_read)) != EOF)
    {
        printf("%c", c);
    }
}

void remove_str(const char address[], int line, int position, int size, char b_f_flag)
{
    line--;
    char final_string[SIZE] = {'\0'};
    char*name = (char*) address + 1, c;
    if(!file_exists(name))
    {
        printf("file with this name does not exists.\n");
        return;
    }
    int line_counter = 0, position_counter = 0, iter = 0, flag = 0;

    FILE *file_to_read = fopen(name, "r");

    while (c != EOF)
    {
        while(line_counter != line)
            {
                c = getc(file_to_read);
                strncat(final_string, &c, 1);
                if(c == '\n')
                {
                    line_counter++;
                }
                if(c == EOF)
                {
                    printf("ERORR: Empty line.\n");
                    return;
                }
            }
        while(position_counter != position)
        {
            position_counter++;
            c = getc(file_to_read);
            if(c == EOF)
            {
                break;
            }
            strncat(final_string, &c, 1);
        }
        if(!flag)
        {
            flag = 1;
            switch (b_f_flag)
            {
                case 'f':
                {
                    for (int i = 0; i < size; i++)
                    {
                        c = getc(file_to_read);
                    }
                    break;
                }
                case 'b':
                {
                    int l = strlen(final_string) - 1;
                    for (int i = 0; i < size; i++)
                    {
                        final_string[l - i] = '\0';
                    }
                    break;
                }
            }
        }
        c = getc(file_to_read);
        if(c == EOF)
        {
            break;
        }
        strncat(final_string, &c, 1);
    }

    FILE *file_to_write = fopen(name, "w");

    fprintf(file_to_write, "%s", final_string);

    fclose(file_to_write);
    fclose(file_to_read);

}

void copy_str(const char address[], int line, int position, int size, char b_f_flag)
{
    line--;
    char c;
    char temp[SIZE] = {'\0'};
    char text[SIZE] = {'\0'}, *name = (char*) address + 1;
    int line_counter = 0, position_counter = 0;
    if(!file_exists(name))
    {
        printf("file with this name does not exists.\n");
        return;
    }

    FILE *file_to_read = fopen(name, "r");

    while(line_counter != line)
    {
        c = getc(file_to_read);
        strncat(temp, &c, 1);
        if(c == '\n')
        {
            line_counter++;
        }
        if(c == EOF)
        {
            printf("ERORR: Empty line.\n");
            return;
        }
    }
    switch (b_f_flag)
    {
        case 'f':
        {
            for(int i = 0; i < size; i++)
            {
                while(position_counter != position)
                {
                    position_counter++;
                    c = getc(file_to_read);
                }
                c = getc(file_to_read);
                if(c == EOF)
                {
                    break;
                }
                text[i] = c;
            }
            break;
        }
        case 'b':
        {
            while(position_counter != position)
            {
                position_counter++;
                c = getc(file_to_read);
                strncat(temp, &c, 1);
            }
            int ii = 0;
            for(int i = size - 1; i >= 0; i--)
            {
                int l = strlen(temp);
                text[ii] = temp[strlen(temp) - i - 1];
                ii++;
            }

            break;
        }
    }

    //clipboard:

    const size_t len = strlen(text) + 1;
    HGLOBAL hMem =  GlobalAlloc(GMEM_MOVEABLE, len);
    memcpy(GlobalLock(hMem), text, len);
    GlobalUnlock(hMem);
    OpenClipboard(0);
    EmptyClipboard();
    SetClipboardData(CF_TEXT, hMem);
    CloseClipboard();
}

void cut_str(const char address[], int line, int position, int size, char b_f_flag)
{
    copy_str(address, line, position, size, b_f_flag);
    remove_str(address, line, position, size, b_f_flag);
}

void paste(const char address[], int line, int position)
{
    char text[SIZE] = {'\0'};
    OpenClipboard(0);
    strcpy(text ,(char*) GetClipboardData(CF_TEXT));
    CloseClipboard();
    insert_str(address, text, line, position);
}

char* regex_build(const char* searchfor)
{
    char* expression = calloc(SIZE, sizeof (char));
    const char wildcard[5] = "\\w*";
    const char star = '*';
    if(searchfor[0] == '*')
    {
        strcat(expression, wildcard);
    }
    else
    {
        strncat(expression, &searchfor[0], 1);
    }
    for(int i = 1; i < strlen(searchfor); i++)
    {
        if(searchfor[i] == '*')
        {
            if(searchfor[i-1] != '\\')
            {
                strcat(expression, wildcard);
            }
            if(searchfor[i-1] == '\\')
            {
                strncat(expression, &star, 1);
            }
        }
        else  if(searchfor[i] == '\\')
        {
            if(i == strlen(searchfor) - 1 || searchfor[i+1] != '*')
            {
                strncat(expression, &searchfor[i], 1);
            }
        }
        else
        {
            strncat(expression, &searchfor[i], 1);
        }
    }
    return expression;
}

int is_wildcard(const char* expression)
{
    const char wildcard[5] = "\\w*";
    for(int i = 0; i < 3; i++)
    {
        if(expression[i] != wildcard[i])
            return 0;
    }
    return 1;
}

int byword(const char text[], int offset)
{
    int count = 0;
    for(int i = 0; i < offset; i++)
    {
        if ((text[i] == ' ' || text[i] == '\n')&& text[i+1] != ' ')
            count++;
    }
    return count + 1;
}

int is_seperator(char c)
{
    if(c == ' ' || c == EOF || c == '\n')
        return 1;
    return 0;
}

int **regex(const char address[], const char pattern[])
{
    char *name = (char*) address + 1;
    char c;
    char text[SIZE] = {'\0'};
    int nmatch = 1, regex_iter = 0, regex_len = 0, match_start = 0;
    char regex[SIZE + 1] = {'\0'};
    strcpy(regex, regex_build(pattern));
    regex_len = strlen(regex);

    int **result = (int**) calloc(SIZE, sizeof(int*));
    for(int i = 0; i < SIZE; i++)
    {
        *(result + i) = (int*) calloc(3, sizeof (int));
    }

    FILE *file_to_read = fopen(name, "r");

    while((c = getc(file_to_read)) != EOF)
    {
        strncat(text, &c, 1);
    }

    fclose(file_to_read);

    for(int i = 0;text[i] != '\0'; i++)
    {

        if(regex_iter == regex_len)
        {
           result[nmatch][0] = match_start + 1;
            result[nmatch][1] = i;
            result[nmatch][2] = byword(text,match_start);
            nmatch++;
            regex_iter = 0;
            match_start = i;
        }
        if(!is_wildcard(regex + regex_iter))
        {
            if(text[i] == regex[regex_iter])
            {
                regex_iter++;
            }
            else
            {
                regex_iter = 0;
                match_start = i + 1;
            }
        }
        else if(is_wildcard(regex + regex_iter))
        {
            if(regex_iter == regex_len - 3 && is_seperator(text[i]))
            {
                regex_iter += 3;
                i--;
                continue;
            }
            else if(text[i] != regex[regex_iter + 3])
            {
                if(is_seperator(text[i]))
                {
                        regex_iter = 0;
                        match_start = i + 1;
                }
                if(i == strlen(text) - 1)
                {
                    if(regex_iter == regex_len - 3)
                    {
                        result[nmatch][0] = match_start + 1;
                        result[nmatch][1] = i + 1;
                        result[nmatch][2] = byword(text,match_start);
                        nmatch++;
                        regex_iter = 0;
                    }
                }
                continue;
            }
            else if(text[i] == regex[regex_iter + 3])
            {
                regex_iter += 4;
            }
        }
    }
    result[0][0] = nmatch - 1;
    return result;
}

void find(const char address[], const char pattern[], int flag, int at)
{
    int **result = regex(address, pattern);
    int nmatch = result[0][0];
    if(flag == COUNT)
    {
        printf("%d", nmatch);
        return;
    }
    if(nmatch == 0)
    {
        printf("-1\n");
        return;
    }
    switch (flag)
    {
        case ALL:
        {
            for(int i = 1; i <= result[0][0]; i++)
            {
                if(i != nmatch)
                    printf("%d, ", result[i][0]);
                else
                    printf("%d", result[i][0]);
            }
            printf("\n");
            break;
        }
        case ALL | BYWORD:
        {
            for(int i = 1; i <= result[0][0]; i++)
            {
                if(i != nmatch)
                    printf("%d, ", result[i][2]);
                else
                    printf("%d", result[i][2]);
            }
            printf("\n");
            break;
        }
        case AT:
        {
            if(nmatch < at)
            {
                printf("-1\n");
                return;
            }
            printf("%d\n", result[at][0]);
            break;
        }
        case AT | BYWORD:
        {
            if(nmatch < at)
            {
                printf("-1\n");
                return;
            }
            printf("%d\n", result[at][2]);
            break;
        }
        default:
        {
            printf("Error: Invalid flags for find function.\n");
            return;
        }

    }

}

void replace(const char address[], const char pattern[], const char replace[],int flag, int at)
{
    char *name = (char*) address + 1;
    char c;
    char text[SIZE] = {'\0'};
    char final_string[SIZE] = {'\0'};
    int **result = regex(address, pattern);
    int res = 1, nmatch = result[0][0];
    FILE *file_to_read = fopen(name, "r");

    while((c = getc(file_to_read)) != EOF)
    {
        strncat(text, &c, 1);
    }
    fclose(file_to_read);

    if(nmatch == 0)
    {
        printf("Nothing to replace.\n");
        return;
    }

    switch (flag)
    {
        case ALL:
        {
                for(int j = 0; j < strlen(text); j++)
                {
                    if(res <= nmatch && j == result[res][0] - 1)
                    {
                        strcat(final_string, replace);
                        j = result[res][1] - 1;
                        res++;
                    }
                    else
                    {
                        strncat(final_string, &text[j], 1);
                    }
                }
            break;
        }
        case AT:
        {
            if(nmatch < at)
            {
                printf("Error: \'AT\' index is bigger than occurrence of pattern.\n");
                return;
            }
            for(int j = 0; j < strlen(text); j++)
            {
                if(j == result[at][0] - 1)
                {
                    strcat(final_string, replace);
                    if(result[at][0] != result[at][1])
                        j = result[at][1] - 1;
                    else
                        j++;
                }
                else
                {
                    strncat(final_string, &text[j], 1);
                }
            }
            break;
        }
        case AT | ALL:
        {
            printf("Error: Invalid flags, don't use \'ALL\' & \'AT\' flags together.\n");
            return;
        }
        default:
        {
            printf("Error: Invalid flags for replace function.\n");
            return;
        }
    }
    FILE *file_to_write = fopen(name, "w");

    fprintf(file_to_write, "%s", final_string);

    fclose(file_to_write);

}

//int main()
//{
//    char address[SIZE] = "/root/file1.txt";
//    create_file(address);
////    remove_str(address, 1, 0, 500, 'f');
////    insert_str(address, "RegExr was created by gskinner.com.\nEdit the Expression & Text to see matches. Roll over matches or the expression for details. PCRE & JavaScript flavors of RegEx are supported. Validate your expression with Tests mode.", 1, 0);
////    cut_str(address, 1, 0, 20, 'f');
////    paste(address, 1, 0);
//    cat(address);
////    regex_compiler("sdaf\\*\\a*\\");
////    find( address, "*he*", ALL, 1);
////    replace(address, "*he", "koon",ALL, 5);
//}
