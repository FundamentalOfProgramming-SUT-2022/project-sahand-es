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

#define SIZE 500

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

//int main()
//{
//    char address[SIZE] = "/root/dir1/temp/file1.txt";
//    create_file(address);
//    insert_str(address, "salam\nkhobi?", 1, 0);
//    remove_str(address, 1, 0, 15, 'f');
//    cut_str(address, 1, 0, 20, 'f');
//    paste(address, 1, 0);
//    cat(address);
//
//}
