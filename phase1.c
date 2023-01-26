#include "phase1Functions.c"
enum ORDERS
        {
    CREATE,
    INSERT,
    REMOVE,
    CAT,
    COPY,
    CUT,
    PASTE,
    FIND,
    REPLACE,
    GREP,
    UNDO,
    AUTO_INDENT,
    COMPARE,
    TREE,
    ARMAN
};

int whichFunction(const char* order)
{
    if(!strcmp(order, "createfile"))
    {
        return CREATE;
    }
    else if(!strcmp(order, "insertstr"))
    {
        return INSERT;
    }
    else if(!strcmp(order, "removestr"))
    {
        return REMOVE;
    }
    else if(!strcmp(order, "cat"))
    {
        return CAT;
    }
    else if(!strcmp(order, "copystr"))
    {
        return COPY;
    }
    else if(!strcmp(order, "cutstr"))
    {
        return CUT;
    }
    else if(!strcmp(order, "pastestr"))
    {
        return PASTE;
    }
    else if(!strcmp(order, "replace"))
    {
        return REPLACE;
    }
    else if(!strcmp(order, "find"))
    {
        return FIND;
    }
    else if(!strcmp(order, "grep"))
    {
        return GREP;
    }
    else if(!strcmp(order, "undo"))
    {
        return UNDO;
    }
    else if(!strcmp(order, "auto-indent"))
    {
        return AUTO_INDENT;
    }
    else if(!strcmp(order, "compare"))
    {
        return COMPARE;
    }
    else if(!strcmp(order, "tree"))
    {
        return TREE;
    }
    else if(!strcmp(order, "=D"))
    {
        return ARMAN;
    }
    else
        return -1;
}

char *file_input()
{
    char* address = (char*) calloc(SIZE, sizeof(char));
    char c;
    scanf(" --file ");
    scanf("%c", &c);

    if(c == '/')
    {
        address[0] = '/';
        scanf("%s", address + 1);
    }
    else if(c == '"')
    {
        int i = 0;
        while(i <= SIZE)
        {
            c = getchar();
            if(c == '"')
            {
                if(address[i - 1] != '\\')
                {
                    break;
                }
                else
                {
                    address[i - 1] = c;
                }
            }
            else
            {
                address[i] = c;
                i++;
            }
            if(i == SIZE)
            {
                perror("ERORR: Input size limit exceeded.\n");
                return NULL;
            }
        }
        address[i] = '\0';
    }
    else
    {
        perror("ERROR: Invalid file address\n");
        return NULL;
    }

    return address;
}

char *string_input()
{
    char* string = (char*) calloc(SIZE, sizeof(char));
    char c;
    scanf(" --str ");
    scanf("%c", &c);

    if(c != '"')
    {
        string[0] = c;
        scanf("%s", string + 1);
    }
    else if(c == '"')
    {
        int i = 0;
        while(i <= SIZE)
        {
            c = getchar();
            if (c == '"')
            {
                if (string[i - 1] != '\\')
                {
                    break;
                }
            }
            if(c == '\\')
            {
                char next_slash;
                next_slash = getchar();
                if(next_slash == '\\')
                {
                    string[i] = '\\';
                    i++;
                    do
                    {
                        c = getchar();
                        string[i] = c;
                        i++;
                    }
                    while(c != ' ' && c != '\0');
                }
                else if(next_slash == 'n')
                {
                    string[i] = '\n';
                    i++;
                }
                else if(next_slash == 't')
                {
                    string[i] = '\t';
                    i++;
                }
                else if(next_slash == '"')
                {
                    string[i] = '"';
                    i++;
                }
            }
            else
            {
                string[i] = c;
                i++;
            }
        }
        if(i == SIZE)
        {
            perror("ERORR: Input size limit exceeded.\n");
            return NULL;
        }
    }
    else
    {
        perror("ERROR: Invalid string\n");
        return NULL;
    }
    return string;
}

int* pos_input()
{
    int *line_position = (int*) calloc(2, sizeof (int));
    scanf(" --pos ");
    scanf("%d:%d", &line_position[0], &line_position[1]);
    return line_position;
}

int size_input()
{
    int x;
    scanf(" -size ");
    scanf("%d", &x);
    return x;
}

int flag_input()
{
    char c;
    scanf(" -%c", &c);
    return c;

}

void functioncaller()
{
    char *order = (char*) calloc(SIZE, sizeof(char));
    char *address, *string, flag = '\0';
    int *line_position, size = 0;

    scanf("%s", order);
    switch (whichFunction(order))
    {
        case CREATE:
        {
            create_file(file_input());
            break;
        }
        case INSERT:
        {
            address = file_input();
            string = string_input();
            line_position = pos_input();
            insert_str(address, string, line_position[0], line_position[1]);

            break;
        }
        case CAT:
        {
            cat(file_input(),NULL);

            break;
        }
        case REMOVE:
        {
            address = file_input();
            line_position = pos_input();
            size = size_input();
            flag = flag_input();
            remove_str(address, line_position[0], line_position[1], size, flag);

            break;
        }
        case COPY:
        {
            address = file_input();
            line_position = pos_input();
            size = size_input();
            flag = flag_input();
            copy_str(address, line_position[0], line_position[1], size, flag);

            break;
        }
        case CUT:
        {
            address = file_input();
            line_position = pos_input();
            size = size_input();
            flag = flag_input();
            cut_str(address, line_position[0], line_position[1], size, flag);

            break;
        }
        case PASTE:
        {
            address = file_input();
            line_position = pos_input();
            paste(address, line_position[0], line_position[1]);

            break;
        }
        case FIND:
        {
            string = string_input();
            address = file_input();
            break;
        }
    }

    free(order);
}

