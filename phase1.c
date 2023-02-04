#include "phase1Functions.c"

enum ORDERS
        {
    OPEN,
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
    ARMAN,
    EXIT
};

int whichFunction(const char* order)
{
    if(!strcmp(order, "create"))
    {
        return CREATE;
    }
    else if(!strcmp(order, "open"))
    {
        return OPEN;
    }
    else if(!strcmp(order, "insert"))
    {
        return INSERT;
    }
    else if(!strcmp(order, "remove"))
    {
        return REMOVE;
    }
    else if(!strcmp(order, "cat"))
    {
        return CAT;
    }
    else if(!strcmp(order, "copy"))
    {
        return COPY;
    }
    else if(!strcmp(order, "cut"))
    {
        return CUT;
    }
    else if(!strcmp(order, "paste"))
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
    else if(!strcmp(order, "exit"))
    {
        return EXIT;
    }
    else
        return -1;
}

char *grep_files_input()
{
    char* address = (char*) calloc(SIZE, sizeof(char));
    char c;
    scanf("%c", &c);
    while(c == ' ')
    {
        scanf("%c", &c);
    }

    if(c == '/')
    {
        address[0] = '/';
        scanf("%s", address + 1);
    }
    else if(c == '\n')
    {
        return NULL;
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
                errorOutput("ERORR: Input size limit exceeded.\n");
                return NULL;
            }
        }
        address[i] = '\0';
    }
    else
    {
        errorOutput("ERROR: Invalid file address\n");
        return NULL;
    }

    return address;
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
                errorOutput("ERORR: Input size limit exceeded.\n");
                return NULL;
            }
        }
        address[i] = '\0';
    }
    else
    {
        errorOutput("ERROR: Invalid file address\n");
        while(c != '\n')
        {
            c = getchar();
        }
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
        int it = 0;
        while(!(isSeperator(c)))
        {
            string[it] = c;
            c = getchar();
            it++;
        }
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
                        if(c == '"')
                        {
                            if (string[i - 1] != '\\')
                            {
                                return string;
                            }
                        }
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
            errorOutput("ERORR: Input size limit exceeded.\n");
            return NULL;
        }
    }
    else
    {
        errorOutput("ERROR: Invalid string\n");
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

char b_f_flag_input()
{
    char c;
    scanf(" -%c", &c);
    return c;

}

int* find_flags()
{
    int *flags = (int*) calloc(2, sizeof (int));
    int flag = 0, space = 0, at = 0;
    char at_string[SIZE] = {'\0'};
    char* flag_string = (char*) calloc(SIZE, sizeof(char));
    scanf(" %[^\n]s", flag_string);
//    getchar();
    for(int i = 0; flag_string[i] != '\0'; i++)
    {
        if(flag_string[i] == ' ' || flag_string[i+1] == '\0')
        {
            if(!strncmp(flag_string + space, "-all", 4))
            {
                flag |= ALL;
            }
            else if(!strncmp(flag_string + space, "-count", 6))
            {
                flag |= COUNT;
            }
            else if(!strncmp(flag_string + space, "-byword", 7))
            {
                flag |= BYWORD;
            }
            else if(!strncmp(flag_string + space, "-at", 3))
            {
                flag |= AT;
                i++;
                for(i + 1; flag_string[i] != ' ' && flag_string[i] != '\0'; i++)
                {
                    strncat(at_string, &flag_string[i], 1);
                }
                at = atoi(at_string);
            }
            space = i + 1;
        }
    }
    flags[0] = flag;
    flags[1] = at;
    return flags;
}

//int main()
//{
//    while(1)
//    {
//        functioncaller(NULL);
//    }
//}
