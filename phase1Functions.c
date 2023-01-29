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
#include <dirent.h>


#define SIZE 999999

enum FIND_TYPES {COUNT = 1, AT = 1 << 1, BYWORD = 1 << 2, ALL = 1 << 3};

void errorOutput(char *ERORR)
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    SetConsoleTextAttribute(hConsole, FOREGROUND_RED);
    printf("%s", ERORR);
    SetConsoleTextAttribute(hConsole, 7);

}

void _read_(const char* name, char *text)
{
    char c;
    FILE *file_to_read = fopen(name, "r");

    while((c = getc(file_to_read)) != EOF)
    {
        strncat(text, &c, 1);
    }

    fclose(file_to_read);
}

void _hidden_read_(const char* name, char *text)
{
    SetFileAttributesA(name, 32);

    char c;
    FILE *file_to_read = fopen(name, "r");

    while((c = getc(file_to_read)) != EOF)
    {
        strncat(text, &c, 1);
    }

    fclose(file_to_read);

    SetFileAttributesA(name, 32 + FILE_ATTRIBUTE_HIDDEN);
}

void _write_(const char* name, char *text)
{

    FILE *file_to_write = fopen(name, "w");

    fprintf(file_to_write, "%s", text);

    fclose(file_to_write);
}

void _hidden_write_(const char* name, char *text)
{
    SetFileAttributesA(name, 32);

    FILE *file_to_write = fopen(name, "w");
    fprintf(file_to_write, "%s", text);
    fclose(file_to_write);

    SetFileAttributesA(name, 32 + FILE_ATTRIBUTE_HIDDEN);
}

int charNumToLine(const char *text, int char_num)
{
    int line = 1;
    for(int i = 0; text[i] != '\0' && i < char_num - 1; i++)
    {
        if(text[i] == '\n')
        {
            line++;
        }
    }
    return line;
}

int lineToCharNum(const char *text, int line)
{
    int line_count = 1;
    for(int i = 0; text[i] != '\0'; i++)
    {
        if(line_count == line)
            return i + 1;
        if(text[i] == '\n')
        {
            line_count++;
        }
    }
    return 0;
}

int posTocCharNum(const char* text, int line, int position)
{
    int char_num = lineToCharNum(text, line);
    return char_num + position;
}

int charNumToPos(const char* text, int char_num)
{
    int position = 0;
    int line = charNumToLine(text, char_num);
    int line_count = 1;
    for(int i = 0; text[i] != '\0'; i++)
    {
        if(line_count == line)
        {
            return char_num - 1 - i;
        }
        if(text[i] == '\n')
        {
            line_count++;
        }
    }
    return 0;

}

int fileExists(const char name[])
{
    FILE *does_exist = fopen(name, "r");
    if(does_exist == NULL)
    {
        fclose(does_exist);
        return 0;
    }
    fclose(does_exist);
    return 1;
}

char* hiddenFileName(const char address[])
{
    char* hidden_name = (char*) calloc(SIZE, sizeof (char));
    int last_slash = 0;

    for(int i = 0; address[i] != '\0'; i++)
    {
        if(address[i] == '/')
        {
            last_slash = i;
        }
    }

    strncpy(hidden_name, address + 1, last_slash);
    strcat(hidden_name, ".");
    strcat(hidden_name,address + 1 + last_slash);

    return hidden_name;
}

void copyToHiddenFile(const char* address)
{
    char *text = (char*) calloc(SIZE, sizeof (char));

    _read_(address + 1, text);
    _hidden_write_(hiddenFileName(address), text);

    free(text);

}

void create_file(char address[])
{
    char dirname[SIZE] = {'\0'};
    int first_slash = 0;
    int last_slash = 0;
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
        last_slash = iterate;
        strncpy(dirname, address + first_slash + 1, iterate - 1);
        mkdir(dirname);
        iterate++;
    }

    strncpy(dirname, address + first_slash + 1, iterate - 1);

    if(fileExists(dirname))
    {
        errorOutput("ERROR: file with this name exists.\n");
        return;
    }
    else
    {
        FILE  *new_file = fopen(dirname, "a+");
        fclose(new_file);

        for(int i = 0; i < iterate; i++)
        {
            dirname[i]  = '\0';
        }

        char* hidden_name = hiddenFileName(address);

        new_file = fopen(hidden_name, "a+");
        DWORD attributes = GetFileAttributes(hidden_name);
        SetFileAttributesA(hidden_name, attributes + FILE_ATTRIBUTE_HIDDEN);
        fclose(new_file);
        free(hidden_name);
    }

}

void insert_str(const char address[],const char string[], int line, int position)
{
    line--;
    int count = strlen(string), i = 0, j = 0, place = 0;
    char c, *name = (char*) address + 1, *text;
    char final_string[SIZE] = {'\0'};

    if(!fileExists(name))
    {
        errorOutput("ERORR: file with this name does not exists.\n");
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

    copyToHiddenFile(address);

    FILE *file_to_write = fopen(name, "w");

    fprintf(file_to_write, "%s", final_string);

    fclose(file_to_write);
    fclose(file_to_read);
}

void cat(const char address[], char *arman)
{
    char *name = (char*) address + 1, c;
    if(!fileExists(name))
    {
        errorOutput("ERROR: File with this name does not exists.\n");
        return;
    }

    FILE *file_to_read = fopen(name, "r");

    if(arman == NULL)
    {
        while ((c = fgetc(file_to_read)) != EOF)
        {
            printf("%c", c);
        }
        printf("\n");
    }
    else
    {
        _read_(address + 1, arman);
        strcat(arman, "\n");
    }
}

void remove_str(const char address[], int line, int position, int size, char b_f_flag)
{
    line--;
    char final_string[SIZE] = {'\0'};
    char*name = (char*) address + 1, c;
    if(!fileExists(name))
    {
        errorOutput("ERROR: File with this name does not exists.\n");
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
                    errorOutput("ERORR: Empty line.\n");
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

    copyToHiddenFile(address);

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
    if(!fileExists(name))
    {
        errorOutput("ERROR: File with this name does not exists.\n");
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
            errorOutput("ERORR: Empty line.\n");
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

int isWildcard(const char* expression)
{
    const char wildcard[5] = "\\w*";
    for(int i = 0; i < 3; i++)
    {
        if(expression[i] != wildcard[i])
            return 0;
    }
    return 1;
}

int isSeperator(char c)
{
    if(c == ' ' || c == EOF || c == '\n' || c == '\0')
        return 1;
    return 0;
}

int byWord(const char text[], int offset)
{
    int count = 0;
    for(int i = 0; i < offset; i++)
    {
        if ((text[i] == ' ' || text[i] == '\n')&& text[i+1] != ' ')
            count++;
    }
    return count + 1;
}

int **regex(const char* address, const char* pattern)
{
    if(!fileExists(address + 1))
    {
        errorOutput("ERROR: File with this name does not exists.\n");
        return NULL;
    }
    char *name = (char*) address + 1;
    char c;
    char text[SIZE] = {'\0'};
    int nmatch = 1, regex_iter = 0, regex_len = 0, match_start = 0,flag = 1, i = 0;
    char regex[SIZE + 1] = {'\0'};
    strcpy(regex, regex_build(pattern));
    regex_len = strlen(regex);

    int **result = (int**) calloc(SIZE, sizeof(int*));
    for(int i = 0; i < SIZE; i++)
    {
        *(result + i) = (int*) calloc(5, sizeof (int));
    }

    _read_(name, text);

    do
    {
            if(regex_iter == regex_len)
            {
                result[nmatch][0] = match_start + 1;
                result[nmatch][1] = i;
                result[nmatch][2] = byWord(text, match_start);
                result[nmatch][3] = charNumToLine(text, result[nmatch][0]);
                result[nmatch][4] = charNumToPos(text, result[nmatch][0]);
                nmatch++;
                regex_iter = 0;
                match_start = i;
            }
            if(!isWildcard(regex + regex_iter))
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
            else if(isWildcard(regex + regex_iter))
            {
                if(regex_iter == regex_len - 3 && isSeperator(text[i]))
                {
                    regex_iter += 3;
                    i--;
                    continue;
                }
                else if(text[i] != regex[regex_iter + 3])
                {
                    if(isSeperator(text[i]))
                    {
                        regex_iter = 0;
                        match_start = i + 1;
                    }
                    continue;
                }
                else if(text[i] == regex[regex_iter + 3])
                {
                    for(int j = i; !isSeperator(text[j]); j++)
                    {
                        if(text[j + 1] == regex[regex_iter + 3])
                        {
                            flag = 0;
                            break;
                        }
                    }
                    if(!flag)
                    {
                        flag = 1;
                        continue;
                    }
                    regex_iter += 4;
                }
            }
    } while (text[i] != '\0' && ++i);

    result[0][0] = nmatch - 1;
    return result;
}

void find(const char address[], const char pattern[], int flag, int at, char *arman)
{
    if(!fileExists(address + 1))
    {
        errorOutput("ERROR: File with this name does not exists.\n");
        return;
    }
    int **result = regex(address, pattern);
    int nmatch = result[0][0];
    if(flag == COUNT)
    {
        if(arman == NULL)
            printf("%d\n", nmatch);
        else
            sprintf(arman + strlen(arman),"%d\n", nmatch);
            free(result);
        return;
    }
    if(nmatch == 0)
    {
        if(arman == NULL)
        {
            printf("-1\n");
            free(result);
            return;
        }
        else
        {
            sprintf(arman + strlen(arman),"-1\n");
            free(result);
            return;
        }
    }
    switch (flag)
    {
        case ALL:
        {
            for(int i = 1; i <= result[0][0]; i++)
            {
                if (arman == NULL)
                {
                    if(i != nmatch)
                        printf("%d, ", result[i][0]);
                    else
                        printf("%d", result[i][0]);
                }
                else
                {
                    if(i != nmatch)
                        sprintf(arman + strlen(arman),"%d, ", result[i][0]);
                    else
                        sprintf(arman + strlen(arman),"%d", result[i][0]);
                }
            }
            if(arman == NULL)
                printf("\n");
            else
                sprintf(arman + strlen(arman),"\n");
            break;
        }
        case ALL | BYWORD:
        {
            for(int i = 1; i <= result[0][0]; i++)
            {
                if (arman == NULL)
                {
                    if(i != nmatch)
                        printf("%d, ", result[i][2]);
                    else
                        printf("%d", result[i][2]);
                }
                else
                {
                    if(i != nmatch)
                        sprintf(arman + strlen(arman),"%d, ", result[i][2]);
                    else
                        sprintf(arman + strlen(arman),"%d", result[i][2]);
                }
            }
            if(arman == NULL)
                printf("\n");
            else
                sprintf(arman + strlen(arman),"\n");
            break;
        }
        case AT:
        {
            if(nmatch < at)
            {
                if(arman == NULL)
                {
                    printf("-1\n");
                    free(result);
                    return;
                }
                else
                {
                    sprintf(arman + strlen(arman),"-1\n");
                    free(result);
                    return;
                }
            }
            if(arman == NULL)
                printf("%d\n", result[at][0]);
            else
                sprintf(arman + strlen(arman),"%d\n", result[at][0]);
            break;
        }
        case AT | BYWORD:
        {
            if(nmatch < at)
            {
                if(arman == NULL)
                {
                    printf("-1\n");
                    free(result);
                    return;
                }
                else
                {
                    sprintf(arman + strlen(arman),"-1\n");
                    free(result);
                    return;
                }
            }
            if(arman == NULL)
                printf("%d\n", result[at][2]);
            else
                sprintf(arman + strlen(arman),"%d\n", result[at][2]);
            break;
        }
        default:
        {
            errorOutput("ERROR: Invalid flags for find function.\n");
            free(result);
            return;
        }

    }

}

void replace_str(const char address[], const char pattern[], const char replace[], int flag, int at)
{

    char *name = (char*) address + 1;
    char c;
    char text[SIZE] = {'\0'};
    char final_string[SIZE] = {'\0'};
    int **result;
    result = regex(address,pattern);
    int res = 1, nmatch = result[0][0];

    if(!fileExists(name))
    {
        errorOutput("ERROR: File with this name does not exists.\n");
        return;
    }

    _read_(name, text);

    if(nmatch == 0)
    {
        printf("Nothing to replace_str.\n");
        free(result);
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
                errorOutput("ERROR: \'AT\' index is bigger than occurrence of pattern.\n");
                free(result);
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
            errorOutput("ERROR: Invalid flags, don't use \'ALL\' & \'AT\' flags together.\n");
            free(result);
            return;
        }
        default:
        {
            errorOutput("ERROR: Invalid flags for replace_str.\n");
            free(result);
            return;
        }
    }

    copyToHiddenFile(address);

    FILE *file_to_write = fopen(name, "w");

    fprintf(file_to_write, "%s", final_string);

    fclose(file_to_write);
}

char *file_name(char* name)
{
    int last_slash = 0;
    for(int i = 0; name[i] !='\0'; i++)
    {
        if(name[i] == '/')
        {
            last_slash = i;
        }
    }
    return name + last_slash + 1;
}

void grep(int files_count, const char **address, char* pattern, char l_c_flag, char *arman)
{

    int **result = NULL, nmatch = 0, count = 0;
    char *grep_string = (char*) calloc(SIZE * files_count, sizeof (char));
    char *found_names = (char*) calloc(SIZE * files_count, sizeof (char));
    for(int i = 0; i < files_count; i++)
    {

        char *text = (char*) calloc(SIZE * files_count, sizeof (char));
        char *name = (char*) address[i] + 1;
        int last_line = 0;

        if(!fileExists(name))
        {
            errorOutput("ERROR: File with this name does not exists.\n");
            return;
        }
        _read_(name, text);

        result = regex(*(address+i), pattern);
        nmatch = result[0][0];

        for(int ii = 1; ii <= nmatch; ii++)
        {
            if(result[ii][3] == last_line)
            {
                continue;
            }
            count++;
            if(ii == 1)
            {
                strcat(found_names, file_name(name));
                strcat(found_names, "\n");
            }
            strcat(grep_string, file_name(name));
            strcat(grep_string, ": ");
            for(int j = lineToCharNum(text,result[ii][3]) - 1; text[j] != '\n' && text[j] != '\0'; j++)
            {
                strncat(grep_string, &text[j], 1);
            }
            strcat(grep_string, "\n");
            last_line = result[ii][3];
        }
    }

    switch (l_c_flag)
    {
        case '\0':
        {
            if(arman == NULL)
            {
                printf("%s\n",grep_string);
                break;
            }
            else
            {
                sprintf(arman + strlen(arman),"%s\n",grep_string);
                break;
            }
        }
        case 'l':
        {
            if(arman == NULL)
            {
                printf("%s\n", found_names);
                break;
            }
            else
            {
                sprintf(arman + strlen(arman), "%s\n", found_names);
                break;
            }
        }
        case 'c':
        {
            if(arman == NULL)
            {
                printf("%d\n", count);
                break;
            }
            else
            {
                sprintf(arman + strlen(arman),"%d\n", count);
                break;
            }
        }
        default:
        {
            errorOutput("ERROR: Invalid flags for grep.\n");
            break;
        }
    }

    free(grep_string);
    free(found_names);
    free(result);
}

void undo(const char* address)
{
    char *text = (char*) calloc(SIZE, sizeof (char));
    _hidden_read_(hiddenFileName(address), text);
    copyToHiddenFile(address);
    _write_(address + 1, text);
    free(text);
}

void auto_indent(const char* address)
{
    if(!fileExists(address + 1))
    {
        errorOutput("ERROR: File with this name does not exists.\n");
        return;
    }
    int open_count = 0, open_flag = 0, close_flag = 0;
    char *text = (char*) calloc(SIZE, sizeof (char));
    char *final_string = (char*) calloc(SIZE, sizeof (char));
    _read_(address + 1,text);
    int i , j;

    for(j = 0; isSeperator(text[j]); j++)
    {
        strcat(final_string, " ");
        open_count++;
    }

    for(i = j; text[i] != '\0'; i++)
    {
        if (text[i] != '{' && text[i] != '}')
        {

            if(open_flag && isSeperator(text[i]))
            {
                continue;
            }
            if(open_flag)
            {
                strcat(final_string, "\n");
                for(int k = 0; k < open_count; k++)
                {
                    strcat(final_string, " ");
                }
            }
            else if(!isSeperator(text[i]))
            {
                open_flag = 0;
            }
            if(text[i] == ' ')
            {
                int k = i;
                while(text[k] != '{' && isSeperator(text[k]))
                {
                    k++;
                }
                if(text[k] == '{')
                {
                    strcat(final_string, " ");
                    i = k - 1;
                    continue;
                }
            }

            strncat(final_string, &text[i], 1);
            if(text[i+1] == '{')
            {
                strcat(final_string, " ");
            }
            open_flag = 0;
            close_flag = 0;
        }
        else if (text[i] == '{')
        {
            char correction[SIZE] = {'\0'};
            if(open_flag || close_flag)
            {
                correction[0] = '\n';
                for (int jj = 1; jj < open_count + 1; jj++)
                {
                    correction[jj] = ' ';
                }
            }

            strcat(correction, "{\0");
            strcat(final_string, correction);
            open_count += 4;
            open_flag = 1;
            close_flag = 0;
        }
        else if (text[i] == '}')
        {
            open_count -= 4;
            char correction[SIZE] = {'\0'};
            correction[0] = '\n';
            for(int k = 1; k < open_count + 1; k++)
            {
                correction[k] = ' ';
            }
            strcat(final_string, correction);
            strcat(final_string, "}");
            open_flag = 0;
            close_flag = 1;
        }
    }

    _write_(address + 1, final_string);


    free(text);
    free(final_string);
}

int oneDiffrencePrint(const char* text1_line, const char* text2_line, int line, char *arman)
{
    char** words1 = (char **) calloc(SIZE, sizeof (char*));
    char** words2 = (char **) calloc(SIZE, sizeof (char*));
    for(int i = 0; i < SIZE; i++)
    {
        *(words1 + i) = (char*) calloc(SIZE, sizeof (char));
        *(words2 + i) = (char*) calloc(SIZE, sizeof (char));
    }
    int start = 0;
    int c1 = 0, c2 = 0, diff = 0, diff_count = 0;
    for(int i = 0; text1_line[i] != '\0'; i++)
    {
        if(byWord(text1_line,i) != byWord(text1_line, start))
        {
            strncpy(words1[c1], text1_line + start, i - start - 1);
            c1++;
            start = i;
        }
        else if(byWord(text1_line,i) == byWord(text1_line ,strlen(text1_line) - 1))
        {
            strncpy(words1[c1], text1_line + start, strlen(text1_line) - start);
            c1++;
            start = i;
            break;
        }
    }
    start = 0;
    for(int i = 0; text2_line[i] != '\0'; i++)
    {
        if(byWord(text2_line,i) != byWord(text2_line, start))
        {
            strncpy(words2[c2], text2_line + start, i - start - 1);
            c2++;
            start = i;
        }
        else if(byWord(text2_line,i) == byWord(text2_line ,strlen(text2_line) - 1))
        {
            strncpy(words2[c2], text2_line + start, strlen(text2_line) - start);
            c2++;
            start = i;
            break;
        }
    }
    if(c1 != c2)
        return 0;

    for(int i = 0; i < c1; i++)
    {
        if(diff_count > 1)
        {
            free(words1);
            free(words2);
            return 0;
        }
        if(strcmp(words1[i], words2[i]) != 0)
        {
            diff = i;
            diff_count++;
        }
    }
    printf("============ #%d ============\n", line);
    for(int i = 0; i < c1; i++)
    {
        if(arman == NULL)
        {
            if(i != diff)
                printf("%s ", words1[i]);
            else
            {
                printf(">>%s<< ", words1[i]);
            }
        }
        else
        {
            if(i != diff)
                sprintf(arman + strlen(arman),"%s ", words1[i]);
            else
            {
                sprintf(arman + strlen(arman),">>%s<< ", words1[i]);
            }
        }
    }
    if(arman == NULL)
        printf("\n");
    else
        sprintf(arman + strlen(arman),"\n");

    for(int i = 0; i < c1; i++)
    {
        if(arman == NULL)
        {
            if(i != diff)
                printf("%s ", words2[i]);
            else
            {
                printf(">>%s<< ", words2[i]);
            }
        }
        else
        {
            if(i != diff)
                sprintf(arman + strlen(arman),"%s ", words2[i]);
            else
            {
                sprintf(arman + strlen(arman),">>%s<< ", words2[i]);
            }
        }
    }
    if(arman == NULL)
        printf("\n");
    else
        sprintf(arman + strlen(arman),"\n");
    free(words1);
    free(words2);
    return 1;
}

void text_comparator(const char* address1, const char* address2, char *arman)
{
    if(!fileExists(address1 + 1))
    {
        errorOutput("ERROR: First file: file with this name does not exists.\n");
        return;
    }
    if(!fileExists(address2 + 1))
    {
        errorOutput("ERROR: Second file: file with this name does not exists.\n");
        return;
    }
    char *text1 = (char*) calloc(SIZE, sizeof (char));
    char *text2 = (char*) calloc(SIZE, sizeof (char));
    int line_counter1 = 0, line_counter2 = 0, flag1 = 1, flag2 = 1, line_start1 = 0, line_start2 = 0;
    _read_(address1 + 1, text1);
    _read_(address2 + 1, text2);
    while(flag1 || flag2)
    {
        char text1_line[SIZE] = {'\0'};
        char text2_line[SIZE] = {'\0'};
        for(int i = line_start1; text1[i] != '\0'; i++)
        {
            if(text1[i] == '\n')
            {
                strncpy(text1_line, text1 + line_start1, i - line_start1);
                line_start1 = i + 1;
                line_counter1++;
                if(text1[i+1] == '\0')
                    flag1 = 0;
                break;
            }
            if(text1[i+1] == '\0')
            {
                flag1 = 0;
                strncpy(text1_line, text1 + line_start1, i + 1 - line_start1);
                line_start1 = i + 1;
                line_counter1++;
            }
        }
        for(int i = line_start2; text2[i] != '\0'; i++)
        {
            if(text2[i] == '\n')
            {
                strncpy(text2_line, text2 + line_start2, i - line_start2);
                line_start2 = i + 1;
                line_counter2++;
                if(text2[i+1] == '\0')
                    flag2 = 0;
                break;
            }
            if(text2[i+1] == '\0')
            {
                flag2 = 0;
                strncpy(text2_line, text2 + line_start2, i + 1 - line_start2);
                line_start2 = i + 1;
                line_counter2++;
            }
        }
        if(strcmp(text1_line, text2_line) != 0)
        {
            if(!strcmp(text1_line, "\0"))
            {
                if(arman == NULL)
                {
                    printf("<<<<<<<<<<<<< #%d - #%d >>>>>>>>>>>>>\n", line_counter2, line_counter1 + 1);
                    printf("%s\n", text2_line);
                }
                else
                {
                    sprintf(arman + strlen(arman),"<<<<<<<<<<<<< #%d - #%d >>>>>>>>>>>>>\n", line_counter2, line_counter1 + 1);
                    sprintf(arman + strlen(arman),"%s\n", text2_line);
                }
            }
            else if(!strcmp(text2_line, "\0"))
            {
                if(arman == NULL)
                {
                    printf("<<<<<<<<<<<<< #%d - #%d >>>>>>>>>>>>>\n", line_counter1, line_counter2 + 1);
                    printf("%s\n", text1_line);
                }
                else
                {
                    sprintf(arman + strlen(arman),"<<<<<<<<<<<<< #%d - #%d >>>>>>>>>>>>>\n", line_counter1, line_counter2 + 1);
                    sprintf(arman + strlen(arman),"%s\n", text1_line);
                }
            }
            else if(!oneDiffrencePrint(text1_line, text2_line,line_counter1, NULL))
            {
                if(arman == NULL)
                {
                    printf("============ #%d ============\n", line_counter1);
                    printf("%s\n%s\n",text1_line, text2_line);
                }
                else
                {
                    sprintf(arman + strlen(arman),"============ #%d ============\n", line_counter1);
                    sprintf(arman + strlen(arman),"%s\n%s\n",text1_line, text2_line);
                }
            }
        }
    }

    free(text2);
    free(text1);
}
void tree(int depth, int first_depth, char *arman)
{
    if(first_depth < -1)
    {
        errorOutput("ERROR: Invalid depth for tree.\n");
        return;
    }
    struct dirent *entry;
    struct stat filestat;
    if(depth == 0)
        return;

    DIR *folder;
    folder = opendir(".");
    if(folder == NULL)
    {
        errorOutput("ERORR: Unable to read directory\n");
        return;
    }

    while( (entry=readdir(folder)) )
    {
        char *arrow = (char*) calloc(SIZE, sizeof(char));
        if(first_depth  - depth > 0 && depth)
        {
            for(int i = 0; i < first_depth - depth - 1; i++)
            {
                strcat(arrow, "     ");
            }
                strcat(arrow, "|--->");
        }
        stat(entry->d_name,&filestat);
        if( S_ISDIR(filestat.st_mode) )
        {
            if(entry->d_name[0] != '.')
            {
                if(arman == NULL)
                {
                    printf("%s", arrow);
                    printf("%s:\n",entry->d_name);
                }
                else
                {
                    sprintf(arman + strlen(arman),"%s", arrow);
                    sprintf(arman + strlen(arman),"%s:\n",entry->d_name);
                }
                chdir(entry->d_name);

                tree(depth - 1, first_depth,arman);
                chdir("..");
            }
        }
        else if(entry->d_name[0] != '.')
        {
            if(arman == NULL)
            {
                printf("%s", arrow);
                printf("%s\n",entry->d_name);
            }
            else
            {
                sprintf(arman + strlen(arman),"%s", arrow);
                sprintf(arman + strlen(arman),"%s\n",entry->d_name);
            }
        }
    }
    closedir(folder);
}

