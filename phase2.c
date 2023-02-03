#include "phase1.c"
#include <conio.h>

#define HEIGHT 35
#define WIDTH  100


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

void navigator(char c)
{
    COORD curr_pos = GetConsoleCursorPosition();
    int x = curr_pos.X;
    int y = curr_pos.Y;
    switch (c)
    {
        case 'k':
        {
            if(y >= 1)
            {
                SetCurser(x, y-1);
            }
            break;
        }
        case 'j':
        {
            if(y <= HEIGHT - 1)
            {
                SetCurser(x, y+1);
            }
            break;
        }
        case 'h':
        {
            if(x >= 1)
            {
                SetCurser(x-1, y);
            }
            else if(x == 0)
            {
                if(y >= 1)
                    SetCurser(WIDTH, y-1);
            }
            break;
        }
        case 'l':
        {
            if(x <= WIDTH - 1)
            {
                SetCurser(x+1, y);
            }
            else if(x == WIDTH)
            {
                if(y < HEIGHT)
                {
                    SetCurser(0, y+1);
                }
            }
            break;
        }
        default:
            return;
    }
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
void BarCommand()
{
    char ch;
    SetCurser(0,HEIGHT + 2);
    while((ch = getche()) != '\r' && ch != '\n');
    SetCurser(0,HEIGHT + 2);
    SetLine(WHITE);
    SetCurser(0,0);
}
int main()
{
    SetConsoleTitle("Vim");
    SetWindow(WIDTH, HEIGHT);
    char ch;
    SetMode(INSERT_MODE);
    SetName("GOOZ",1);
    while((ch = getch()) != 'Q')
    {
        navigator(ch);
        if(ch == '/' || ch == ':')
        {
            BarCommand();
        }
        if(ch == 'f')
        {
            SetName("KOS NANE ALAEI", 1);
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