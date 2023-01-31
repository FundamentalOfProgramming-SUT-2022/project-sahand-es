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
    ARMAN,
    EXIT
};

int whichFunction(const char* order)
{
    if(!strcmp(order, "create"))
    {
        return CREATE;
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

void functioncaller(char *arman)
{
    int *find_flags_arr;
    char *order = (char*) calloc(SIZE, sizeof(char));
    char *address = NULL, *string = NULL, flag = '\0', *string1;
    int *line_position, size = 0;

    if(arman != NULL)
    {
        if(*arman != '\0')
        {
            string = arman;
            arman = NULL;
        }
    }

    scanf("%s", order);
    switch (whichFunction(order))
    {
        case CREATE:
        {
            address = file_input();
            if(address == NULL)
                break;
            create_file(address);
            break;
        }
        case INSERT:
        {
            address = file_input();
            if(address == NULL)
                break;
            if(string == NULL)
                string = string_input();
            line_position = pos_input();
            insert_str(address, string, line_position[0], line_position[1]);

            break;
        }
        case CAT:
        {
            address = file_input();
            if(address == NULL)
                break;

            cat(address,arman);

            break;
        }
        case REMOVE:
        {
            address = file_input();
            if(address == NULL)
                break;

            line_position = pos_input();
            size = size_input();
            flag = b_f_flag_input();
            remove_str(address, line_position[0], line_position[1], size, flag);

            break;
        }
        case COPY:
        {
            address = file_input();
            if(address == NULL)
                break;

            line_position = pos_input();
            size = size_input();
            flag = b_f_flag_input();
            copy_str(address, line_position[0], line_position[1], size, flag);

            break;
        }
        case CUT:
        {
            address = file_input();
            if(address == NULL)
                break;

            line_position = pos_input();
            size = size_input();
            flag = b_f_flag_input();
            cut_str(address, line_position[0], line_position[1], size, flag);

            break;
        }
        case PASTE:
        {
            address = file_input();
            if(address == NULL)
                break;

            line_position = pos_input();
            paste(address, line_position[0], line_position[1]);

            break;
        }
        case FIND:
        {
            if(string == NULL)
                string = string_input();
            address = file_input();
            if(address == NULL)
                break;

            find_flags_arr = find_flags();

            find(address, string, find_flags_arr[0], find_flags_arr[1], arman);
            break;
        }
        case REPLACE:
        {
            string = string_input();
            string1 = string_input();
            address = file_input();
            if(address == NULL)
                break;

            find_flags_arr = find_flags();

            replace_str(address, string, string1, find_flags_arr[0], find_flags_arr[1]);
            break;
        }
        case GREP:
        {
            int files_count = 0;
            scanf(" -%c ", &flag);
            if(flag == ' ')
                flag = '\0';
            if(string == NULL)
                string = string_input();
            scanf(" --files ");
            char** files = (char **) calloc(100, sizeof (char*));
            for(int i = 0; i < 100; i++)
            {
                *(files + i) = (char *) calloc(SIZE, sizeof(char));
            }
            while(1)
            {
                files[files_count] = grep_files_input();
                if(files[files_count] == NULL)
                {
                    break;
                }
                files_count++;
            }
            grep(files_count, files, string, flag, arman);

            free(files);
            break;
        }
        case UNDO:
        {
            address = file_input();

            undo(address);
            break;
        }
        case AUTO_INDENT:
        {
            address = file_input();
            if(address == NULL)
                return;

            auto_indent(address);
            break;
        }
        case COMPARE:
        {
            string = file_input();
            if(string == NULL)
                break;

            string1 = file_input();
            if(string1 == NULL)
                break;

            text_comparator(string, string1, arman);
            break;
        }
        case TREE:
        {
            chdir("root");
            int depth = 0;
            scanf("%d", &depth);

            tree(depth, depth, arman);
            chdir("..");
            break;
        }
        case ARMAN:
        {
            char *arman_arr = (char *) calloc(SIZE, sizeof(char));
            functioncaller(arman_arr);

            free(arman_arr);
            return;
        }
        case EXIT:
        {
            exit(0);
        }
        default:
        {
            char c;
            while ((c = getchar()) != '\n');
            errorOutput("ERROR: Invalid command.\n");
            return;
        }
        
    }
    if(arman != NULL)
    {
        functioncaller(arman);
    }

    free(order);
}

int main()
{
    while(1)
    {
        functioncaller(NULL);
    }
}
