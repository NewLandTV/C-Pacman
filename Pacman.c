#include <stdio.h>
#include <conio.h>
#include <time.h>
#include <stdlib.h>
#include <Windows.h>

#define SCREEN_WIDTH 60
#define SCREEN_HEIGHT 15

#define NONE 0
#define UP 72
#define LEFT 75
#define RIGHT 77
#define DOWN 80

// Structs
typedef struct _Vector2
{
	float x;
	float y;
} Vector2;

typedef struct _Pacman
{
	Vector2 position;
	
	char sprite;
	unsigned char direction;
	unsigned char life;
	unsigned int ateCoins;
} Pacman;

// Global variables
Pacman pacman;

/*
 * '.' is Coin
 * '#' is Wall
 * ' ' is Empty space
 * '!' is Teleportation point
 */
unsigned char map[(SCREEN_WIDTH - 2) * (SCREEN_HEIGHT - 2) + 1] = {
																      ".........................................................."
																      ".###.#######.#####.###.######.#####.###.#####.#######.###."
																      ".....#..............................................#....."
																      ".###...#######.###.#####.####.###.#####.###.#######...###."
																      ".............#.........#..........#.........#............."
																      "#####..#######.###.#####.####.###.#####.###.#######..#####"
																      "!    ................................................    !"
																      "#####..#######.###.#####.####.###.#####.###.#######..#####"
																      ".............#.........#..........#.........#............."
																      ".###...#######.###.#####.####.###.#####.###.#######...###."
																      ".....#..............................................#....."
																      ".###.#######.#####.###.######.#####.###.#####.#######.###."
																      ".........................................................."
																  };

// Functions
void Init();
void Update();

void DrawOutline();
void DrawMap();
void DrawPacman();
void ClearPacman();
void DrawPacmanState();
void EatCoin();
void CheckTeleportation();

void MovePacman();

void Gotoxy(unsigned short x, unsigned short y);

int main(void)
{
	Init();
	
	while (1)
	{
		Update();
		Sleep(100);
	}
	
	return 0;
}

void Init()
{
	int i;
	
	srand((unsigned int)time(NULL));
	
	// Setup console
	system("title Pacman");
	system("mode con cols=60 lines=18");
	
	// Hide cursor
	CONSOLE_CURSOR_INFO consoleCursorInfo;
	
	consoleCursorInfo.bVisible = 0;
	consoleCursorInfo.dwSize = 1;
	
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &consoleCursorInfo);
	
	// Initialize global variables
	pacman.position = (Vector2){ SCREEN_WIDTH >> 1, SCREEN_HEIGHT >> 1 };
	pacman.sprite = '@';
	pacman.direction = NONE;
	pacman.life = 3;
	pacman.ateCoins = 0;
	
	// Call at first functions
	DrawOutline();
	DrawMap();
	DrawPacman();
}

void Update()
{
	MovePacman();
	DrawPacmanState();
}

void DrawOutline()
{
	int i;
	int j;
	
	for (i = 0; i < SCREEN_WIDTH; i++)
	{
		for (j = 0; j < SCREEN_HEIGHT; j++)
		{
			if ((i == 0 || i == SCREEN_WIDTH - 1 || j == 0 || j == SCREEN_HEIGHT - 1) && j != (SCREEN_HEIGHT >> 1))
			{
				Gotoxy(i, j);
				printf("#");
			}
		}
	}
}

void DrawMap()
{
	int i;
	int line;
	
	Gotoxy(1, 1);
	
	for (i = 0, line = 1; i < (SCREEN_WIDTH - 2) * (SCREEN_HEIGHT - 2) + 1; i++)
	{
		if (i % (SCREEN_WIDTH - 2) == 0)
		{
			Gotoxy(1, line++);
		}
		
		if (map[i] == '!')
		{
			printf(" ");
			
			continue;
		}
		
		printf("%c", map[i]);
	}
}

void DrawPacman()
{
	Gotoxy(pacman.position.x, pacman.position.y);
	printf("%c", pacman.sprite);
}

void ClearPacman()
{
	Gotoxy(pacman.position.x, pacman.position.y);
	
	if (map[(int)((pacman.position.x - 1) + (pacman.position.y - 1) * (SCREEN_WIDTH - 2))] == '!')
	{
		printf(" ");
		
		return;
	}
	
	printf("%c", map[(int)((pacman.position.x - 1) + (pacman.position.y - 1) * (SCREEN_WIDTH - 2))]);
}

void DrawPacmanState()
{
	Gotoxy(2, SCREEN_HEIGHT + 1);
	printf("Life : %d", pacman.life);
	Gotoxy(14, SCREEN_HEIGHT + 1);
	printf("Ate Coins : %u", pacman.ateCoins);
	Gotoxy(38, SCREEN_HEIGHT + 1);
	printf("Position [%03.0f, %03.0f]", pacman.position.x - 1, pacman.position.y - 1);
}

void EatCoin()
{
	if (map[(int)((pacman.position.x - 1) + (pacman.position.y - 1) * (SCREEN_WIDTH - 2))] == '.')
	{
		pacman.ateCoins++;
		
		map[(int)((pacman.position.x - 1) + (pacman.position.y - 1) * (SCREEN_WIDTH - 2))] = ' ';
	}
}

void CheckTeleportation()
{
	if (map[(int)((pacman.position.x - 1) + (pacman.position.y - 1) * (SCREEN_WIDTH - 2))] == '!')
	{
		ClearPacman();
		
		if (pacman.position.x == 1)
		{
			pacman.position.x = SCREEN_WIDTH - 3;
		}
		else
		{
			pacman.position.x = 2;
		}
		
		DrawPacman();
	}
}

void MovePacman()
{
	switch (pacman.direction)
	{
	case UP:
		if (pacman.position.y > 1 && map[(int)((pacman.position.x - 1) + (pacman.position.y - 2) * (SCREEN_WIDTH - 2))] != '#')
		{
			ClearPacman();
			
			pacman.position.y--;
			
			DrawPacman();
			EatCoin();
			CheckTeleportation();
		}
		
		break;
	case LEFT:
		if (pacman.position.x > 1 && map[(int)((pacman.position.x - 2) + (pacman.position.y - 1) * (SCREEN_WIDTH - 2))] != '#')
		{	
			ClearPacman();
			
			pacman.position.x--;
			
			DrawPacman();
			EatCoin();
			CheckTeleportation();
		}
		
		break;
	case RIGHT:
		if (pacman.position.x < SCREEN_WIDTH - 2 && map[(int)((pacman.position.x) + (pacman.position.y - 1) * (SCREEN_WIDTH - 2))] != '#')
		{	
			ClearPacman();
			
			pacman.position.x++;
			
			DrawPacman();
			EatCoin();
			CheckTeleportation();
		}
		
		break;
	case DOWN:
		if (pacman.position.y < SCREEN_HEIGHT - 2 && map[(int)((pacman.position.x - 1) + (pacman.position.y) * (SCREEN_WIDTH - 2))] != '#')
		{	
			ClearPacman();
			
			pacman.position.y++;
			
			DrawPacman();
			EatCoin();
			CheckTeleportation();
		}
		
		break;
	}
	
	if (!_kbhit() || _getch() != 224)
	{
		return;
	}
	
	switch (_getch())
	{
	case UP:
		pacman.direction = UP;
		
		break;
	case LEFT:
		pacman.direction = LEFT;
		
		break;
	case RIGHT:
		pacman.direction = RIGHT;
		
		break;
	case DOWN:
		pacman.direction = DOWN;
		
		break;
	}
}

void Gotoxy(unsigned short x, unsigned short y)
{
	COORD position = { x, y };
	
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), position);
}
