#include <stdio.h>
#include <windows.h>
#include <time.h>
#include <stdlib.h>
#include <conio.h>
#include <cstring>


using namespace std;

const int x_max = 40;
const int y_max = 25;

void GoToXY(int x, int y);
void CursorVisible(bool visible);

enum Rotation
{
    left,
    right,
    up,
    down
};

struct Position
{
    int x;
    int y;
};

class Field
{
public:
    char field[y_max][x_max + 1];
    void CreateBoundaries(char ch);
    void Show();
    void SpownPrice();
};

class Snake
{
private:

    Position snakePos[1000];
    Rotation snakeRot;

public:
    int length;
    Snake(int _length, Rotation _snakeRot) :
        length(_length), snakeRot(_snakeRot) {};

    void ToStartValues();
    void Move(char field[y_max][x_max + 1]); \
        void AddTail();
    bool IsGameOver();
};


//Работа с полем
void Field::CreateBoundaries(char ch)
{
    for (int y = 0; y < y_max; y++)
    {
        for (int x = 0; x < x_max; x++)
        {
            if (y == 0 || y == y_max - 1 ||
                x == 0 || x == x_max - 1)
            {
                field[y][x] = ch;
            }
            else
            {
                field[y][x] = ' ';
            }
        }
        field[y][x_max] = '\0';
    }
}

void Field::Show()
{
    GoToXY(0, 0);

    for (int y = 0; y < y_max; y++)
    {
        printf("%s\n", field[y]);
    }
}

void Field::SpownPrice()
{
    int x = rand() % (x_max - 2) + 1;
    int y = rand() % (y_max - 2) + 1;

    bool isSpown = false;
    while (true)
    {
        if (x % 2 == 0) isSpown = true;

        if (field[y][x] == 'O' || field[y][x] == '*') isSpown = false;

        if (isSpown) break;

        x = rand() % (x_max - 2) + 1;
    }

    field[y][x] = '$';

}

//Змейка
void Snake::ToStartValues()
{
    for (int i = 0; i < length; i++)
    {
        snakePos[i].x = 0;
        snakePos[i].y = 0;
    }

    length = 1;

    snakePos[0] = { x_max / 2, y_max / 2 };

    GoToXY(snakePos[0].x, snakePos[0].y);
    putchar('O');
}


void Snake::AddTail()
{
    snakePos[length].x = snakePos[length - 1].x;
    snakePos[length].y = snakePos[length - 1].y;

    length++;
}

bool Snake::IsGameOver()
{
    bool isGameOver = false;
    int* score;
    if (snakePos[0].x <= 0 || snakePos[0].x >= x_max - 1 ||
        snakePos[0].y <= 0 || snakePos[0].y >= y_max - 1)
    {
        isGameOver = true;
    }

    for (int i = 2; i < length; i++)
    {
        if (snakePos[0].x == snakePos[i].x &&
            snakePos[0].y == snakePos[i].y)
        {
            isGameOver = true;
        }
    }
    score = &length;
    return isGameOver;
}
void Snake::Move(char field[y_max][x_max + 1])
{


    if (_kbhit())
    {
        switch (_getch())
        {
        case 'w':
            snakeRot = up;
            break;
        case 'a':
            snakeRot = left;
            break;
        case 's':
            snakeRot = down;
            break;
        case 'd':
            snakeRot = right;
            break;
        }
    }
    field[snakePos[length - 1].y][snakePos[length - 1].x] = ' ';

    for (int i = length; i > 0; i--)
    {
        snakePos[i] = snakePos[i - 1];
    }

    switch (snakeRot)
    {
    case up:
        snakePos[0].y = snakePos[0].y - 1;
        break;

    case down:
        snakePos[0].y = snakePos[0].y + 1;
        break;

    case left:
        snakePos[0].x = snakePos[0].x - 2;
        break;

    case right:
        snakePos[0].x = snakePos[0].x + 2;
        break;
    }

    if (field[snakePos[0].y][snakePos[0].x] == '$') AddTail();

    field[snakePos[0].y][snakePos[0].x] = 'O';

    if (length > 1)
    {
        field[snakePos[1].y][snakePos[1].x] = '*';
    }
}


//Функции курсора
void GoToXY(int x, int y)
{
    COORD pos = { x, y };
    HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleCursorPosition(handle, pos);
}

void CursorVisible(bool visible)
{
    HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO structCursorInfo;
    GetConsoleCursorInfo(handle, &structCursorInfo);
    structCursorInfo.bVisible = visible;
    SetConsoleCursorInfo(handle, &structCursorInfo);
}

void ShowLose()
{
    system("cls");
    GoToXY(50, 11);
    printf("You lose!");
    Sleep(1000);
}

void ShowWin()
{
    system("cls");
    GoToXY(50, 11);
    printf("You win!");
    Sleep(1000);
}

void SnakeMain()
{
    srand(time(NULL));

    CursorVisible(false);

    Field gameField;
    Snake snake(1, up);

    gameField.CreateBoundaries(219);
    snake.ToStartValues();
    for (int i = 0; i < 40; i++)
    {
        gameField.SpownPrice();
    }

    while (true)
    {
        if (snake.IsGameOver()) break;
        snake.Move(gameField.field);
        gameField.Show();
        Sleep(60);
    }

}

int main()
{
    printf("Welcome to my project called the <UNUSUAL SNAKE>\n");
    system("pause");
    int choice;
    do
    {

        system("cls");
        printf("Choose what you want to do\n");
        printf("1 - Start new game\n");
        printf("2 - Manual\n");
        printf("3 - Where to get the source code?\n");
        printf("4 - Exit\n");
        scanf_s("%d", &choice);
        switch (choice)
        {
        case 1:
            system("cls");
            printf("The speed is high, so get ready");
            Sleep(3000);
            SnakeMain();
            Sleep(500);
            break;
        case 2:
            system("cls");
            printf("You play as a snake, to win you need to collect all the fruits without crashing into walls or your tail\nThe snake is controlled via 'W' 'A' 'S' 'D', respectively 'W' -up, 'A'- left, 'S'- down,'D'- right\n");
            system("pause");
            break;
        case 3:
            system("cls");
            printf("Double click on the link to select and copy\nhttps://github.com/fliprayn/AnUnusualSnake\n");
            system("pause");
            break;
        case 4:
            break;
        }
    } while (choice != 4);
}
