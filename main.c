#include <stdio.h>
#include <string.h>
#include <conio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <direct.h>

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

void insert_str(char address[], char string[SIZE], int line, int position)
{
    line--;
    int count = strlen(string), i = 0, j = 0, place = 0;
    int address_len = strlen(address);
    address[strlen(address)] = '\0';
    char c, *name = address + 1;
    char *final_string = (char*) calloc(SIZE, sizeof (char));

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
        else if(position_counter == position - 1 && line_counter == line && flag)
        {
            strcat(final_string, string);
            iter += count;
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
