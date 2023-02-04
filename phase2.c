#include "phase1.c"
#include <conio.h>

#define HEIGHT 10
#define WIDTH  100

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
            else if(y == HEIGHT - 4 && l <= HEIGHT + 2)
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
            if(y == 0 && x == positions[0][0])
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
            if(y == HEIGHT && x == positions[HEIGHT][1])
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

void SetLine(int color)
{
    COORD coor = GetConsoleCursorPosition();
    for(int i = coor.X; i <= WIDTH; i++)
    {
        cPrint(color," ");
    }
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
    cPrint(BLUE * 6 + WHITE + 2,  mode_name);
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

void ShowText(const char *text, int startline)
{
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
}

//void ScrollText(char render, const char *text)
//{
//    }
//}

void BarCommand()
{
    char ch, order[WIDTH] = {'\0'};
    SetCurser(0,HEIGHT + 2);
    while((ch = getche()) != '\r' && ch != '\n')
    {
//        if(ch == ':')
//        {
////            scanf("%s", order);
//            functioncaller(NULL);
////            printf("%s", order);
//        }
    };
    SetCurser(0,HEIGHT + 2);
    SetLine(WHITE);
    SetCurser(positions[0][0],0);
}

//void InsertMode(const char* address)
//{
//    COORD first_cor = GetConsoleCursorPosition();
//
//    void NormalMode(const char* address);
//    char *text = (char*) calloc(SIZE, sizeof(char));
//    char ch;
//    _read_(address + 1, text);
//
//    SetMode(INSERT_MODE);
//    SetName(file_name(address + 1), 0);
//    ShowText(text, 0);
//
//    SetCurser(first_cor.X, first_cor.Y);
//
//    while((ch = getch()) != ':' && ch != '/')
//    {
//
//    }
//
//}

void VisualMode(const char* address)
{
    void NormalMode(const char* address);
    COORD first_cor = GetConsoleCursorPosition();
    char *text = (char*) calloc(SIZE, sizeof(char));
    _read_(address + 1, text);

    SetMode(VISUAL_MODE);
    ShowText(text, 0);

    SetCurser(first_cor.X, first_cor.Y);

    char b_f_flag;
    char ch, c;
    int l = 0, size = 0;
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
        NormalMode(address);
    }
    else if(ch == 'd')
    {
        cut_str(address, first_cor.Y + 1 + l, first_cor.X - positions[first_cor.Y][0], size,b_f_flag);
        NormalMode(address);
    }
    else if(ch == 'v')
    {
        NormalMode(address);
    }
    else if(ch == 'i')
    {
        InsertMode(address);
    }

}

void NormalMode(const char* address)
{
    char *text = (char*) calloc(SIZE, sizeof(char));
    char ch;
    _read_(address + 1, text);
    int l = 0;
    COORD first_cor = GetConsoleCursorPosition();

    SetMode(NORMAL_MODE);
    SetName(file_name(address + 1), 0);
    ShowText(text, 0);
    SetCurser(first_cor.X, first_cor.Y);

    while((ch = getch()) != ':' && ch != '/' && ch != 'i' && ch != 'v')
    {
        int render = Navigator(ch, l, text);
        if(render != _NOCHANGE)
            l += render;
    }
    if(ch == ':' || ch == '/')
        BarCommand();
    else if(ch == 'i')
        InsertMode(address);
    else if(ch == 'v')
        VisualMode(address);

}

int main()
{
    HANDLE Handle = GetStdHandle(STD_OUTPUT_HANDLE);

    SetConsoleTitle("Vim");
    SetWindow(WIDTH, HEIGHT);
    char ch;
    SetMode(VISUAL_MODE);
    SetName("GOOZ",1);
//    ShowText("a\nsa");
//    ShowText("");

//    VisualMode("/root/=Dtest.txt");
    NormalMode("/root/=Dtest.txt");

    char text[] = "sahand1\nsahand2\nsdf as\ndf asdf\n sadf asdfa\ndf asdf s\nadf\nsahand5\nsdf a\nsdf asdf\na asdf as\nsahnad6'\0";
    ShowText(text, 0);
    int l = 0;
    while((ch = getch()) != 'Q')
    {
        int render = Navigator(ch, l, text);
        if(render != _NOCHANGE)
            l += render;
        if(ch == '/' || ch == ':')
        {
            BarCommand();
        }
        if(ch == 'f')
        {
            SetName("YOU SURE?", 1);
            ch = getch();
            if(ch == 'y')
            {
                SetName("KOS NANE ALAEI XD.", 0);
            }
            else
            {
                SetName("GOOZ",1);
            }
        }
    }

}
//int main()
//{
//    SetConsoleTitle("Vim");
//    SetWindow(WIDTH, HEIGHT);
//    char ch;
//    printf("some god\nfddsflma\nasdfasdf s          \nadsf         asd\f\n");
//    SetCurser(0,0);
////printf("\n");
//    while((ch = getch()) != 'Q')
//    {
//        navigator(ch);
//        char c = CursorCharRead();
//        if(ch == '\b')
//        {
//            printf(" ");
//            navigator('h');
//        }
//    }
//}