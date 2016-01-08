/*
�а� : ����IT�̵����а�
�й� : 13184394
�̸� : ������
���� : Pacman Game 
*/

#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <time.h>
#include <Windows.h>

#define KEY_ESC		0x1B
#define KEY_8		'8'
#define KEY_2		'2'
#define KEY_4		'4'
#define KEY_6		'6'
#define KEY_W		'w'
#define KEY_X		'x'
#define KEY_A		'a'
#define KEY_D		'd'
// direction ����
#define UP			1
#define DOWN		2
#define LEFT		3
#define RIGHT		4
// maze �̷� ũ��
#define MAZE_H		25
#define MAZE_W		26
// tile type Ÿ�� ����
#define WALL		32
#define FOOD		1
#define PILL		2
#define NUM_PILL	4


// position
typedef struct _POS{
	int row;
	int col;
} POS;


// state of pacman
POS pacman;
int direction = RIGHT;
int speed = 100;

void init();					// initialize
void turn(int key);				// turn
void move();					// move pacman
int check();					// check
POS rand_pos();					// random position

void draw_maze();				// draw maze
BOOL IsItFood(POS pos);         // check food or not
BOOL IsItWall(int i, int j);    // check wall or not
void draw_pacman();				// draw pacman
void clear_pacman();			// clear pacman
void show_instruction();		// instruction

void goto_pos(POS pos);
void gotoxy(int x, int y);		// move cursor
void showCursor(BOOL bVisible);



// wall         32,   33,   34,   35,   36,   37,   38,   39,   40,   41,   42,   43
char* tile[] = {"  ", "��", "��", "��", "��", "��", "��", "��", "��", "��", "��", "��"};


int maze[MAZE_H][MAZE_W] = 
{
	{33,38,38,38,38,38,38,41,38,38,38,38,38,38,38,38,38,38,41,38,38,38,38,38,38,34},
	{37, 1, 1, 1, 1, 1, 1,37, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,37, 1, 1, 1, 1, 1, 1,37},
	{37, 1,33,38,38,34, 1,37, 1,33,38,38,38,38,38,38,34, 1,37, 1,33,38,38,34, 1,37},
	{37, 1,35,38,38,36, 1,37, 1,35,38,38,38,38,38,38,36, 1,37, 1,35,38,38,36, 1,37},
	{37, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,37},
	{35,38,34, 1,33,34, 1,38,38,38,38, 1,33,34, 1,38,38,38,38, 1,33,34, 1,33,38,36},
	{33,38,36, 1,37,37, 1, 1, 1, 1, 1, 1,37,37, 1, 1, 1, 1, 1, 1,37,37, 1,35,38,34},
	{37, 1, 1, 1,37,35,38,34, 1,33,38,38,36,35,38,38,34, 1,33,38,36,37, 1, 1, 1,37},
	{37, 1, 1, 1,35,38,38,36, 1,35,38,38,38,38,38,38,36, 1,35,38,38,36, 1, 1, 1,37},
	{35,38,34, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,33,38,36},
	{32,32,37, 1,33,38,38,34, 1,33,38,38,38,38,38,38,34, 1,33,38,38,34, 1,37,32,32},
	{32,32,37, 1,37,33,38,36, 1,37, 7, 3,15, 3, 3, 3,37, 1,35,38,34,37, 1,37,32,32},
	{32,32,37, 1,37,37, 1, 1, 1,37, 3,11, 3,19, 3, 3,37, 1, 1, 1,37,37, 1,37,32,32},
	{33,38,36, 1,35,36, 1,37, 1,35,38,38,38,38,38,38,36, 1,37, 1,35,36, 1,35,38,34},
	{37, 1, 1, 1, 1, 1, 1,37, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,37, 1, 1, 1, 1, 1, 1,37},
	{35,38,34, 1,38,38,38,42,38,38,38, 1,33,34, 1,38,38,38,42,38,38,38, 1,33,38,36},
	{32,32,37, 1, 1, 1, 1, 1, 1, 1, 1, 1,37,37, 1, 1, 1, 1, 1, 1, 1, 1, 1,37,32,32},
	{32,32,37, 1,33,38,38,34, 1,33,38,38,36,35,38,38,34, 1,33,38,38,34, 1,37,32,32},
	{33,38,36, 1,35,38,38,36, 1,35,38,38,38,38,38,38,36, 1,35,38,38,36, 1,35,38,34},
	{37, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,37},
	{37, 1,33,38,34, 1,33,41,38,38,38, 1,33,34, 1,38,38,38,41,34, 1,33,38,34, 1,37},
	{37, 1,37,32,37, 1,37,37, 1, 1, 1, 1,37,37, 1, 1, 1, 1,37,37, 1,37,32,37, 1,37},
	{37, 1,35,38,36, 1,35,36, 1,38,38,38,42,42,38,38,38, 1,35,36, 1,35,38,36, 1,37},
	{37, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,37},
	{35,38,38,38,38,38,38,38,38,38,38,38,38,38,38,38,38,38,38,38,38,38,38,38,38,36},
};


int main(void)
{
	int ch;
	clock_t tic;

	// initialize the game
	init();

	// instruction
	show_instruction();
	turn(_getch());

	// timer start
	tic = clock();

	// play
	while (ch = check())
	{
		if (_kbhit())
			turn(_getch());

		clear_pacman();
		move();
		draw_pacman();

		// timer
		gotoxy(54, 8);
		printf("%7.1f sec", (clock()-tic)/1000.0);  //�ð� ���

		Sleep(speed);
	}

	// exit
	gotoxy(54, 10);
	puts("Done!");
	gotoxy(0, MAZE_H);
	return 0;
}


// initialize
void init()
{
	POS pos;
	int i;

	//random seed
	srand((int)time(NULL));

	// items
	/*  TODO
	�˾�(PILL)�� NUM_PILL��ŭ �����ؼ�
	����(FOOD)��ſ� ��ġ��Ų��.
	*/
	for(i=0;i<NUM_PILL;i++) 
	{
		pos = rand_pos();
		maze[pos.row][pos.col] = PILL;  //NUM_PILL�� �� ������ �������� ���� �ϳ��� �ʿ� ��ġ��
	}


	// place packman 
	/*  TODO
	�Ѹ�(pacman)�� ������ ��ҿ� ��ġ��Ų��.
	����!
	�Ѹ��� �ڽ��� ��ġ�� ������ �ڵ� �����ϹǷ�
	�Ѹ� ��ġ(packman)�� maze���� 0���� ����
	*/ 

	pacman = rand_pos();
	maze[pacman.row][pacman.col]=0; 
	// clear screen
	system("cls");

	// init window
	showCursor(FALSE);

	// draw
	draw_maze();
	draw_pacman();
}


// turn the direction of pacman
void turn(int key)
{
	switch(key)
	{
	case KEY_4:  // left
	case KEY_A:
		direction = LEFT;
		break;
	case KEY_6:  // right
	case KEY_D:
		direction = RIGHT;
		break;
	case KEY_2:  // down
	case KEY_X:
		direction = DOWN;
		break;
	case KEY_8:  // up
	case KEY_W:
		direction = UP;
		break;
	case KEY_ESC:	// quit
		gotoxy(54, 10);
		puts("Exit!\n");
		gotoxy(0, MAZE_H);
		exit(0);
		break;
	}
}

// move pacman in the current direction
void move()
{

	/*  TODO
	���� ���� ����(direction)�� ����
	�Ѹ��� �̵����� ��ġ(pacman)�� ���� 
	���� ��ġ�� �ִ� ������ �ڵ����� �����ϹǷ�
	�Ѹ� ��ġ(packman)�� maze���� 0���� ����

	������⿡ ���� ������ ���ڸ���*/

	POS pos;
	int goto_dx=0; //�����Ϸ� �ϴ� ���� X��ǥ ��ȭ ũ��
	int goto_dy=0; //�����Ϸ� �ϴ� ���� Y��ǥ ��ȭ ũ��
	POS Go_Pacman;

	switch(direction) {
	case LEFT: goto_dx=-1; //�� �� ĭ �տ��� ����
		break;
	case RIGHT: goto_dx=1; //�� �� ĭ �ڿ��� ����
		break;
	case UP: goto_dy=-1;  //�� �� ĭ �տ��� ����
		break;
	case DOWN: goto_dy=1;  //�� �� ĭ �ڿ��� ����
		break;
	}

	//�����ϴ� �Ѹ� ��ǥ
	Go_Pacman.row = pacman.row + goto_dy;
	Go_Pacman.col = pacman.col + goto_dx;

	//���� �����Ϸ� �ϴ� �Ѹ� ��ǥ�� ���� �ƴ� ���
	if (!IsItWall(Go_Pacman.row, Go_Pacman.col)) 
	{
		pacman = Go_Pacman;	//�״�� �����δ�
		maze[pacman.row][pacman.col] = 0;
	}

}

// check
int check()
{
	/*  TODO
	�̷�(maze) �ȿ� �����ִ� 
	����(FOOD)�� �˾�(PILL)�� ������ return
	���� �Ծ����� 0�� return
	*/	
	int i,j, grounds = 0;
	for(i=0;i<MAZE_H;i++) 
	{
		for(j=0;j<MAZE_W;j++) 
		{
			if(maze[i][j]==FOOD || maze[i][j]==PILL) //�����̳� �˾��� ������
			{
				grounds++;  //�ܿ��� ���� ��������
			}
		}
	}
	return grounds;  //�ܿ��� �ִٰ� ��ȯ����
}


BOOL IsItFood(POS pos)  //�������� �ƴ��� �ľ�
{
	if (maze[pos.row][pos.col]==FOOD)
		return TRUE;
	else
		return FALSE;
}

// random position
POS rand_pos()
{
	/*  TODO
	maze ���� ��ġ�� �������� �����Ѵ�.
	����!
	FOOD�� �ִ� ��ġ������ ����
	*/
	POS pos;
	do
	{
		pos.row = rand() % MAZE_H;
		pos.col = rand() % MAZE_W;
	}
	while(!IsItFood(pos)); 
	return pos; //pos�� �ǵ�����
}


// clear pacman
void clear_pacman()
{
	// move cursor
	goto_pos(pacman);
	printf("  ");
}


// draw pacman
void draw_pacman()
{
	// move cursor
	goto_pos(pacman);

	/*  TODO
	packman�� �׸���.
	����(direction)�� ���� V ^ > < �������
	*/

	switch (direction)
	{
	case UP:
		printf("V ");
		break;
	case DOWN:
		printf("^ ");
		break;
	case LEFT:
		printf("> ");
		break;
	case RIGHT:
		printf("< ");
		break;
	}
}

// draw maze
void draw_maze()
{
	/*  TODO
	�̷�(maze)�� �׸���.
	FOOD  ". "
	PILL  "+ "
	WALL  tile �̿�
	*/

	int i, j;

	for(i=0; i<MAZE_H; i++)
	{
		for(j=0; j<MAZE_W; j++)
		{
			switch(maze[i][j]) 
			{
			case FOOD: 
				printf(". ");
				break;
			case PILL:
				printf("+ ");
				break;
			default:
				if(IsItWall(i,j)) 
				{
					printf(tile[maze[i][j]-WALL]);
				} else 
				{
					printf("  ");
				}
			}
		}
		if(i!=MAZE_H-1)  //�ܼ�â�� ���� ���� ����
		{
			printf("\n");
		}
	}
}

BOOL IsItWall(int i, int j)   //������ �ƴ��� �Ǵ� �Լ�
{
	if(32 <= maze[i][j] && maze[i][j] <= 43) 
	{
		return TRUE;
	} else 
	{
		return FALSE;
	}
}

// show instruction
void show_instruction()
{
	gotoxy(54,1);		puts("Left(4, A), Right(6, D)");
	gotoxy(54,2);		puts("Down(2, X), Up(8, W)");
	gotoxy(54,3);		puts("Quit(ESC)");
	gotoxy(54,5);		puts("Press any key to start...");
}


// move cursor to pos
void goto_pos(POS pos)
{
	gotoxy(pos.col*2, pos.row);
}


// move cursor to (x,y)
// upper left corner is (0,0)
void gotoxy(int x, int y)
{
	COORD Pos = {x, y};
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), Pos);
}


// show cursor
void showCursor(BOOL bVisible)
{
	CONSOLE_CURSOR_INFO CurInfo;
	CurInfo.dwSize = 100;
	CurInfo.bVisible = bVisible;
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &CurInfo);
}