#include "phase1.c"
#include <conio.h>

#define HEIGHT 20
#define WIDTH  80

int** positions;

enum colors
{
    DARK_BLUE = 1,
    DARK_GREEN = 2,
    CYAN = 11,
    RED = 4,
    PURPLE = 5,
    YELLOW = 6,
    WHITE = 7,
    GRAY = 8,
    BLUE = 9,
    GREEN = 10,
    ORANGE = 12,
    GRAY_BG = 135
};

enum modes
{
    INSERT_MODE,
    VISUAL_MODE,
    NORMAL_MODE
};

int last_mode = NORMAL_MODE;
int last_l = 0;

enum render
{
    _UP = -1,
    _DOWN = 1,
    _NOCHANGE = 10
};

void cPrint(int color, const char* text)
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    SetConsoleTextAttribute(hConsole, color);
    printf("%s", text);
    SetConsoleTextAttribute(hConsole, 7);
}

void SetWindow(int Width, int Height)
{
    COORD coord;
    coord.X = Width;
    coord.Y = Height + 2;

    SMALL_RECT Rect;
    Rect.Top = 0;
    Rect.Left = 0;
    Rect.Bottom = Height + 2;
    Rect.Right = Width;

    HANDLE Handle = GetStdHandle(STD_OUTPUT_HANDLE);      // Get Handle
    SetConsoleScreenBufferSize(Handle, coord);            // Set Buffer Size
    SetConsoleWindowInfo(Handle, TRUE, &Rect);            // Set Window Size
}

void SetCurser(int x, int y)
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    COORD pos = {x, y};
    SetConsoleCursorPosition(hConsole, pos);
}

COORD GetConsoleCursorPosition()
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO cbsi;
    if (GetConsoleScreenBufferInfo(hConsole, &cbsi))
    {
        return cbsi.dwCursorPosition;
    }
    else
    {
        // The function failed. Call GetLastError() for details.
        COORD invalid = { 0, 0 };
        return invalid;
    }
}

void initPositions()
{
    positions = (int**) calloc(SIZE, sizeof(int*));
    for(int i = 0; i < SIZE; i++)
    {
        *(positions + i) = (int*) calloc(2, sizeof (int));
    }
}

int Navigator(char c, int l, const char* text)
{
    void ShowText(const char *text, int startline);

    COORD curr_pos = GetConsoleCursorPosition();
    int x = curr_pos.X;
    int y = curr_pos.Y;
    switch (c)
    {
        case 'k':
        {
            if(y == 0)
            {
                if(l > 0)
                    return _UP;
                break;
            }
            else if(y == HEIGHT - 4 && l > 0)
            {
                l--;
                ShowText(text, l);
                return _UP;
            }
            else if(x >= positions[y-1][1] || x <= positions[y-1][0])
            {
                SetCurser(positions[y-1][1], y-1);
            }
            else
            {
                SetCurser(x, y-1);
            }
            break;
        }
        case 'j':
        {
            if(y == HEIGHT)
            {
                break;
            }
            else if(y == HEIGHT - 4 && (l <= HEIGHT + charNumToLine(text,strlen(text) - 1)))
            {
                l++;
                ShowText(text, l);
                return _DOWN;
            }
            else if(x >= positions[y+1][1] || x <= positions[y+1][0])
            {
                SetCurser(positions[y+1][1], y+1);
            }
            else
            {
                SetCurser(x, y+1);
            }
            break;
        }
        case 'h':
        {
            if(y == 0 && x == positions[0][0] && l > 0)
            {
                l--;
                ShowText(text, l);
                return _UP;
            }
            else if(y == 0 && x == positions[0][0])
            {
                break;
            }
            else if(x == positions[y][0])
            {
                SetCurser(positions[y-1][1], y-1);
            }
            else
            {
                SetCurser(x-1, y);
            }
            break;
        }
        case 'l':
        {
            if(y == HEIGHT && x == positions[HEIGHT][1] && l <= HEIGHT + charNumToLine(text,strlen(text) - 1))
            {
                l++;
                ShowText(text, l);
                SetCurser(positions[y][0], y);
                return _DOWN;
            }
            else if(y == HEIGHT && x == positions[HEIGHT][1])
            {
                break;
            }
            else if(x == positions[y][1])
            {
                SetCurser(positions[y+1][0], y+1);
            }
            else
            {
                SetCurser(x+1, y);
            }
            break;
        }
        default:
            return _NOCHANGE;
    }
    return 0;
}

char CursorCharRead()
{
    char buf[BUFSIZ];
    CONSOLE_SCREEN_BUFFER_INFO csbiInfo;
    HANDLE hConsole= GetStdHandle(STD_OUTPUT_HANDLE);
    GetConsoleScreenBufferInfo(hConsole, &csbiInfo);
    COORD pos = csbiInfo.dwCursorPosition; //set pos to current cursor location
    TCHAR strFromConsole[1];    //need space to only one char
    DWORD dwChars;
    ReadConsoleOutputCharacter(
            hConsole,
            strFromConsole, // Buffer where store symbols
            1, // Read 1 char to strFormConsole
            pos, // Read from current cursor position
            &dwChars); // How many symbols stored
    char c = strFromConsole[0];
    return c;
}

void BackSpace()
{
    cPrint(WHITE, " ");
    Navigator('h', 0, NULL);
}

void SetLine(int color)
{
    COORD coor = GetConsoleCursorPosition();
    for(int i = coor.X; i <= WIDTH; i++)
    {
        cPrint(color," ");
    }
    SetCurser(coor.X, coor.Y);
}

void SetName(const char* file_name, int saved)
{
    char unsaved[10] = {'\0'};
    if(saved == 1)
    {
        strcat(unsaved, "   +");
    }
    SetCurser(8,HEIGHT + 1);
    cPrint(GRAY_BG, "   ");
    cPrint(GRAY_BG,  file_name);
    cPrint(GRAY_BG, unsaved);
    SetLine(GRAY_BG);
    SetCurser(0, 0);
}

void SetMode(int mode)
{
    char mode_name[10] = {'\0'};
    switch (mode)
    {
        case INSERT_MODE:
        {
            strcat(mode_name, " INSERT ");
            break;
        }
        case NORMAL_MODE:
        {
            strcat(mode_name, " NORMAL ");
            break;
        }
        case VISUAL_MODE:
        {
            strcat(mode_name, " VISUAL ");
            break;
        }
        default:
            return;
    }
    SetCurser(0,HEIGHT + 1);
    cPrint(144,  mode_name);
    SetCurser(0, 0);
}

void ClearText()
{
    SetCurser(0,0);
    for(int i = 0; i <= HEIGHT; i++)
    {
        SetLine(WHITE);
        printf("\n");
    }
    SetCurser(0,0);
}

void hideCursor()
{
    HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO info;
    info.dwSize = 100;
    info.bVisible = FALSE;
    SetConsoleCursorInfo(consoleHandle, &info);
}

void showCursor()
{
    HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO info;
    info.dwSize = 100;
    info.bVisible = TRUE;
    SetConsoleCursorInfo(consoleHandle, &info);
}

void ShowText(const char *text, int startline)
{
    hideCursor();
    initPositions();
    COORD curr_cor = GetConsoleCursorPosition();
    ClearText();

    int position = 2;
    int line = 0;
    char line_ps[10] = {'\0'};
    int l = 3, k;

    int temp_startline = startline;
    for(k = 0;text[k] != '\0' && temp_startline; k++)
    {
        if(text[k] == '\n')
        {
            temp_startline--;
        }
        if(!temp_startline)
            break;
    }
    if(k != strlen(text))
    {
        sprintf(line_ps,"%d  ", line + 1 + startline);
        cPrint(GRAY,line_ps);
        l = strlen(line_ps);
    }
    for(int i = k + 1; text[i] != '\0'; i++)
    {
        if(text[i] == '\n')
        {
            positions[line][0] = l;
            positions[line][1] = position;
            char line_p[10] = {'\0'};
            line++;
            if(line == HEIGHT + 1)
                break;
            if(line < 10)
                sprintf(line_p,"%d  ", line + 1 + startline);
            else
                sprintf(line_p,"%d ", line + 1 + startline);

            SetLine(GRAY);
            cPrint(GRAY,"\n");
            cPrint(GRAY, line_p);
            l = strlen(line_p);
            positions[line][0] = l;
            position = strlen(line_p);
        }
        else
        {
            positions[line][0] = l;
            position++;
            char s[10] = {'\0'};
            strncat(s, text + i, 1);
            cPrint(WHITE, s);
        }
        if(text[i+1] == '\0')
        {
            positions[line][1] = position;
        }
//        SetLine(128)
    }
    printf("\n");
    for(int a = line; a < HEIGHT; a++)
    {
        cPrint(BLUE,"~\n");
    }
    curr_cor.X = (curr_cor.X >= 3) ? curr_cor.X : 3;
    curr_cor.X = (curr_cor.Y >= line) ? 0 : curr_cor.X;
    SetCurser(curr_cor.X,curr_cor.Y);
    showCursor();
}

char InsertMode(const char* address)
// to be developed:
{
    last_mode = VISUAL_MODE;

    COORD first_cor = GetConsoleCursorPosition();

    char *text = (char*) calloc(SIZE, sizeof(char));
    char ch;
    _read_(address + 1, text);

    SetMode(INSERT_MODE);
    SetName(file_name(address + 1), 0);
    ShowText(text, 0);

    SetCurser(first_cor.X, first_cor.Y);

    while((ch = getch()) != ':' && ch != '/')
    {

    }

}

char VisualMode(const char* address)
{
    last_mode = VISUAL_MODE;

    COORD first_cor = GetConsoleCursorPosition();
    char *text = (char*) calloc(SIZE, sizeof(char));
    _read_(address + 1, text);

    SetMode(VISUAL_MODE);
    SetName(file_name(address + 1), 0);
    ShowText(text, last_l);

    SetCurser(first_cor.X, first_cor.Y);

    char b_f_flag;
    char ch, c;
    int l = last_l, size = 0;

    c = CursorCharRead();
    char s[10] = {'\0'};
    strncat(s, &c, 1);
    cPrint(16, s);
    Navigator('h', l, text);

    while((ch = getch()) != 'd' && ch != 'y' && ch != 'v' && ch != 'v')
    {
        if(ch == 'k' || ch == 'j')
        {
            size = 0;
            first_cor = GetConsoleCursorPosition();
            ShowText(text, l);
        }
        int render = Navigator(ch, l, text);
        if(render != _NOCHANGE)
        {
            l += render;
            c = CursorCharRead();
            char s[10] = {'\0'};
            strncat(s, &c, 1);

            if(ch == 'l')
            {
                size++;
                if(size < 0)
                {
//                    Navigator('h', l, text);
                    cPrint(WHITE, s);
                }
                else
                    cPrint(16, s);
                Navigator('h', l, text);
            }
            else if(ch == 'h')
            {
                size--;
                if(size > 0)
                    cPrint(WHITE, s);
                else
                    cPrint(16, s);
                Navigator('h', l, text);
            }
            else
            {
                c = CursorCharRead();
                char s[10] = {'\0'};
                strncat(s, &c, 1);
                cPrint(16, s);
                Navigator('h', l, text);
            }
        }
    }

    b_f_flag = size < 0 ? 'b' : 'f';
    size = size < 0 ? -1 * size : size;

    free(text);

    if(ch == 'y')
    {
        copy_str(address, first_cor.Y + 1 + l, first_cor.X - positions[first_cor.Y][0], size,b_f_flag);
    }
    else if(ch == 'd')
    {
        cut_str(address, first_cor.Y + 1 + l, first_cor.X - positions[first_cor.Y][0], size,b_f_flag);
    }
    last_l = l;
    return ch;
}

char NormalMode(const char* address)
{
    last_mode = NORMAL_MODE;

    char *text = (char*) calloc(SIZE, sizeof(char));
    char ch;
    _read_(address + 1, text);
    int l = last_l;
    COORD first_cor = GetConsoleCursorPosition();

    SetMode(NORMAL_MODE);
    SetName(file_name(address + 1), 0);
    ShowText(text, last_l);
    if(first_cor.Y > HEIGHT)
    {
        first_cor.Y = 0;
        first_cor.X = 3;
    }
    SetCurser(first_cor.X, first_cor.Y);

    while((ch = getch()) != ':' && ch != '/' && ch != 'i' && ch != 'v')
    {
        int render = Navigator(ch, l, text);
        if(render != _NOCHANGE)
            l += render;
    }
    last_l = l;
    return ch;
}

char ModeChanger(char ch, const char* address)
{
    void BarCommand();
    switch (last_mode)
    {
        case NORMAL_MODE:
        {
            if(ch == 'i')
                ch = InsertMode(address);
            else if(ch == 'v')
                ch =VisualMode(address);
            else if(ch == ':' || ch == '/')
                BarCommand();
            break;
        }
        case VISUAL_MODE:
        {
            if(ch == 'd' || ch == 'y' || ch == 'v')
            {
                ch = NormalMode(address);
            }
            else if(ch == 'i')
            {
                ch = InsertMode(address);
            }
            else if(ch == ':' || ch == '/')
                BarCommand();
        }
    }
    return ch;
}

void Open(const char* address)
{
    char ch;
    last_l = 0;
    if(!fileExists(address + 1))
    {
        errorOutput("ERROR: File with this name does not exists.");
        return;
    }
    ch = NormalMode(address);
    while (1)
    {
        ch = ModeChanger(ch, address);
        if(ch == ':' || ch == '/')
            return;
    }
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
        case OPEN:
        {
            address = file_input();
            Open(address);
            break;
        }
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

void ClearBC()
{
    SetCurser(0,HEIGHT + 2);
    SetLine(WHITE);
}

void BarCommand()
{
    char ch, order[WIDTH] = {'\0'};
    SetCurser(0,HEIGHT + 2);
    while((ch = getche()) != '\r' && ch != '\n')
    {
        if(ch == '\b')
        {
            BackSpace();
        }
        else if(ch == ':')
        {
//            cPrint(WHITE, ":");
//            SetCurser(0,HEIGHT :::://;sdfakljl+ 2);
//            SetLine(WHITE);
            functioncaller(NULL);
            ClearBC();
        }
    };
    SetCurser(0,HEIGHT + 2);
    SetLine(WHITE);
    SetCurser(positions[0][0],0);
}


int main()
{
    HANDLE Handle = GetStdHandle(STD_OUTPUT_HANDLE);

    SetConsoleTitle("Vim");
    SetWindow(WIDTH, HEIGHT);
    char ch;

    char address[] = "/root/=Dtest.txt\0";
    ch = NormalMode(address);

    while(1)
    {
        ch = ModeChanger(ch, address);
    }
}
