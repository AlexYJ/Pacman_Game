/*
�а� : ����IT�̵����а�
�й� : 13184394
�̸� : ������
���� : 2014. 06. 12
���� : Pacman Game 
*/

#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <time.h>
#include <Windows.h>
#include <MMSystem.h> //�Ҹ� ���� ���

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

//�ܼ� �۾� �� ����
#define COL						  GetStdHandle(STD_OUTPUT_HANDLE)             // �ܼ�â�� ���� �޾ƿ�
#define SETTEXTCOLOR_RED          SetConsoleTextAttribute(COL, 0x000c)        // ������
#define SETTEXTCOLOR_BLUE		  SetConsoleTextAttribute(COL, 0x0009)        // �Ķ���
#define SETTEXTCOLOR_ORIGINAL     SetConsoleTextAttribute(COL, 0x000f)        // ���
#define SETTEXTCOLOR_YELLOW       SetConsoleTextAttribute(COL, 0x000e);       // �����
#define SETTEXTCOLOR_HIGH_GREEN   SetConsoleTextAttribute(COL, 0x000a);       // ���λ�
#define SETTEXTCOLOR_SKY_BLUE     SetConsoleTextAttribute(COL, 0x000b);       // �ϴû�
#define SETTEXTCOLOR_PLUM         SetConsoleTextAttribute(COL, 0x000d);       // ���ֻ�


// position
typedef struct _POS{
	int row;
	int col;
} POS;


// state of pacman
POS pacman;
int direction = RIGHT;
const int REAL_Speed = 100;
int Speed = 100;
int pacmanface = 1;

const int hot6 = 20;
const int wake_up_time = 2000; //��ȿ ���� �ð�. msec.
int Converted = 0;
clock_t Partytime;

void init();					// initialize
void turn(int key);				// turn
void move();					// move pacman
int check();					// check
POS rand_pos();					// random position

void draw_maze();				// draw maze
BOOL IsItFood(POS pos);         // check food or not
BOOL IsItWall(int i, int j);    // check wall or not
void draw_pacman();				// draw pacman
void About_wakeup();	
void clear_pacman();			// clear pacman
void show_instruction();		// instruction

void goto_pos(POS pos);
void gotoxy(int x, int y);		// move cursor
void showCursor(BOOL bVisible);
void print_comp();	
void delay(unsigned int n);		// ms timer
void Whattime(struct tm** pToday); //print present time
void luminescence();

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
	int ch, a;
	clock_t tic;
	REPLAY:
	print_comp();

	// initialize the game
	init();

	// instruction
	show_instruction();
	turn(_getch());

	// timer start
	tic = clock();

	PlaySound(TEXT("Skrillex-Kill Everybody.wav"), NULL, SND_LOOP | SND_NOSTOP | SND_ASYNC); //�Ҹ� ����

	// play
	while (ch = check())
	{	
		if (_kbhit())
			turn(_getch());
		show_instruction();
		clear_pacman();
		move(); //�Ѹ� �̵��Լ� ȣ��
		draw_pacman(); //�Ѹ� ����Լ� ȣ��

		// timer
		gotoxy(54, 8);
		SETTEXTCOLOR_ORIGINAL; //�Ϲ� �ܼ� ������ 
		printf("%7.1f sec", (clock()-tic)/1000.0);  //�ð� ���

		About_wakeup(); //����Ÿ�� �Լ� ȣ��

		Sleep(Speed); 
	}

	// exit
	gotoxy(54, 10);
	puts("Done!");
	gotoxy(0, MAZE_H);
	gotoxy(54, 16);
	SETTEXTCOLOR_RED;
	puts("0.Exit 1.Restart"); //�ý��� ���� �� �����
	gotoxy(54, 17);
	SETTEXTCOLOR_ORIGINAL;
	puts(" + press Enter");
	gotoxy(54, 16);
	RETRY : 
	scanf("%d",&a); //�Է¹ް�

	if(a==1) //�ٽ� �ϰڴٰ� �ϸ�
	{
		system("cls"); //�ý��� Ŭ���� ��Ű��
		goto REPLAY; //����� ���̺���������
	}
	else if(a==0) //�����ڴٰ� �ϸ�
		return 0;  //��������.
	else 
		gotoxy(54,19); //1�̳� 0 �̿� �ٸ� ���� �Է��ϸ�
		puts("input 1 or 0"); //1�̳� 0�� ������� ��
		goto RETRY;;

}


void Whattime(struct tm** pToday) //����ð� ǥ���Լ� ����
{
    time_t ltime;
	time(&ltime);
    *pToday = localtime(&ltime);
}

//ASCgen2 �� �̿��Ͽ� �׸��� �ƽ�Ű ���ڷ� ��ȯ
void print_comp()
{
	//���ڿ� �迭 ����
	char* comp[22] = { 
"S P R I N G   SEMESTER     Xe9999999999eG                           ",
"       F I N A L           Xe9999999999eG                           ",
"                           Xe9999999999eG                           ",
" P A C M A N   G A M E     Xe999Geee999eG                           ",
"13184394 Youn Hyun Jin     Xe99eeK ee99eG                           ",
"                           5eeee    eeeee                           ",
"                           XDD#      #DzD                           ",
"              eeeeeeeeeeeeey             Ezzzzzzzzzz9E              ",
"              EeEEEEEEEEEEey             #yyyyyy5z9EGG              ",
"              EeEEEEEEEEEEey             #yyyyyyX                   ",
"              eeeeeGEEEEEEey             #yyyyyyD                   ",
"              #eEzzeEEEEEEey             #yyyyyyX                   ",
"                   eEEEEEEey             #yyyyyy5Xz#9E              ",
"                   KeeeeeeeE             eEEEEEEEEEEGG              ",
"                                                                    ",
"                                                                    ",
" ,#GE, yy5Dzy   WE9u   zz    9  5K    55u,uWD KUuW5y    5zz, WD   5u",
"Xe   # ee yXy ueeKyee  ze    e  ee    GeeeeGe #eE9eE  eeeEe5 ee   ee",
"Xeu    #e     e#    eG ye    e  e9       eu   ye     ee      ee   ez",
"  #eey #e5ee  ey    ee ye    e  e9       eX   yeGee  ee      eeEe9ez",
"    ee #e     ee    eX ue   Ke  e5       eX   5e     GeD     ee   e#",
"Geee9  DeEeeG  XeeeeK   Xeeee   Eeeeeu   ey   Weeeee  ,9eee9 9e   ez"
	};

	int i,j;
	//��� �� �κ����� ������ ���� �ܼ� ��Ʈ ���� ����
	for(i=0;i<=6;i++){
		for(j=0;j<=67;j++) {
			if(j<27) { 
				SETTEXTCOLOR_ORIGINAL;
				putchar(comp[i][j]);
			}
			else { 
				SETTEXTCOLOR_RED;
				putchar(comp[i][j]);
			}
		}
		putchar('\n');
	}

	for(i=7;i<=13;i++){
		for(j=0;j<=67;j++) {
			if(j<40) {
				SETTEXTCOLOR_BLUE;
				putchar(comp[i][j]);
			}
			else {
				SETTEXTCOLOR_ORIGINAL;
				putchar(comp[i][j]);
			}
		}
		putchar('\n');
	}

	for(i=14;i<=21;i++){
		for(j=0;j<=67;j++) {
			if(j<=37) {
				SETTEXTCOLOR_ORIGINAL;
				putchar(comp[i][j]);
			}
			else {
				SETTEXTCOLOR_ORIGINAL;
				putchar(comp[i][j]);
			}
		}
		putchar('\n');
	}
	
	SETTEXTCOLOR_PLUM;
	printf("Loading");
	//�ε���
	for(i=0;i<=30;i++) {
		delay(20); //�ӵ�����
		putchar('.');
	}
	for(i=31;i<=53;i++) {
		delay(80);
		putchar('.');
	}
	for(i=54;i<=60;i++) {
		delay(270);
		putchar('!');
	}
	
	delay(1000);
	system("cls"); //�ε� ���ȭ������
}


void delay(unsigned int n) //�б� �ΰ� �ܼ� ȭ�鿡 �� �ִ� �ð�
{ 
	unsigned o = GetTickCount(); //���ú��� �ҿ�� �ð�[ms]
	while((GetTickCount()-o)< n);
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

	pacman = rand_pos(); //�Ѹ��� ������ ��ҿ� ��ġ��Ű��
	maze[pacman.row][pacman.col]=0;  //�Ѹ���ġ�� maze���� 0���� ����
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
	int goto_x=0; //�����Ϸ� �ϴ� ���� X��ǥ
	int goto_y=0; //�����Ϸ� �ϴ� ���� Y��ǥ
	POS Go_Pacman;

	switch(direction) 
	{
	case LEFT: goto_x=-1; //�� �� ĭ �տ��� ����
		break;
	case RIGHT: goto_x=1; //�� �� ĭ �ڿ��� ����
		break;
	case UP: goto_y=-1;  //�� �� ĭ �տ��� ����
		break;
	case DOWN: goto_y=1;  //�� �� ĭ �ڿ��� ����
		break;
	}

	//�����ϴ� �Ѹ� ��ǥ
	Go_Pacman.row = pacman.row + goto_y;
	Go_Pacman.col = pacman.col + goto_x;

	//���� �Ѹ��� ������ �ϴ� ��ǥ ���� ���� �ƴ϶��
	if (!IsItWall(Go_Pacman.row, Go_Pacman.col)) 
	{
		pacman = Go_Pacman;	//�״�� �����δ�
		if(maze[pacman.row][pacman.col] == PILL) //�˾��� ���� �Ŀ���
		{ 
			Converted = 1;  //���� ���� �ٲ��
			Partytime = clock();  //�ӵ��� ������
		}
		maze[pacman.row][pacman.col] = 0; 
	}
}

void About_wakeup() //����Ÿ�ӿ�����
{
	int i,j;
	if(Converted == 1)  //����ġ�� on�Ǹ鼭
	{
		Speed = hot6;  //�����Ⱑ ������ ������� ����
		SETTEXTCOLOR_PLUM; //���� �� ����
	} 
	else 
	{
		Speed = REAL_Speed; //�̿��� ��쿡�� ������ �������.
	}
	if (clock()-Partytime > wake_up_time) //������ �����ð��� ������
	{
		Converted = 0; //��� ����ġ�� ���� �������.
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
	if (maze[pos.row][pos.col]==FOOD) //���� ��ǥ�� ������ ������
		return TRUE; //�� ����
	else  //�� ���� ���
		return FALSE;  //���� ����
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
		pos.row = rand() % MAZE_H; //�࿡���� ��ġ ��������
		pos.col = rand() % MAZE_W; //�������� ��ġ ��������
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
	luminescence();
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

void luminescence() //�Ѹ��� ���� ������ �߱��ϰ� �� ��
{
	if(pacmanface == 1 && About_wakeup == 0) //���� ������ ����
	{
		SETTEXTCOLOR_YELLOW; //���������
		pacmanface = 2;
	} 
	else if (pacmanface == 2 && Converted == 1) //���� �Ծ��� ����
	{
		SETTEXTCOLOR_PLUM; //���ֻ���
		pacmanface = 1;
	} 
	else 
	{
		SETTEXTCOLOR_YELLOW; //������� �����ư��� �߱��ϵ��� ��Ÿ����.
		pacmanface = 2;
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
				SETTEXTCOLOR_SKY_BLUE; //���� ���� �ϴû�����
				printf(". ");
				break;
			case PILL:
				SETTEXTCOLOR_RED; //�˾� ���� ����������
				printf("+ ");
				break;
			default:
				if(IsItWall(i,j)) 
				{
					SETTEXTCOLOR_HIGH_GREEN; //�⺻ �� �� ���λ�
					printf(tile[maze[i][j]-WALL]);
				} else 
				{
					printf("  ");
				}
			}
		}
		if(i!=MAZE_H-1)  //�ܼ�â�� �Ʒ� ���� ���� ����
		{
			printf("\n");
		}
	}
}


BOOL IsItWall(int i, int j)   //������ �ƴ��� �Ǵ� �Լ�
{
	if(32 <= maze[i][j] && maze[i][j] <= 43) //������ ������ �� �� ���ڿ� �� ���� ���� ������
	{
		return TRUE; //���̴�.
	} 
	else  //�� ���� ���
	{
		return FALSE;  //���� �ƴϴ�.
	}
}


// show instruction
void show_instruction()
{
	struct tm* today; //��¥ ��¿� ����ü ����
	SETTEXTCOLOR_ORIGINAL;
	gotoxy(54,1);		puts("Left(4, A), Right(6, D)");
	gotoxy(54,2);		puts("Down(2, X), Up(8, W)");
	gotoxy(54,3);		puts("Quit(ESC)");
	gotoxy(54,5);		puts("Press any key to start...");
    //�ð� ���. ���������� �������� ���� ��, ��, ��, ��, ��, �ʰ� ��µ�. ���� ���ϰ� press ESC to exit�� ��µ� �� �� �ڸ��� ����.
	Whattime(&today);
	gotoxy(54,14);       printf("%04d-%02d-%02d %02d:%02d:%02d",
		today->tm_year + 1900,   // tm_year�� 1900�� ���ؾ� ���� ������ ��
		today->tm_mon + 1, // tm_mon�� 1���� 0���� �Է¹޾� 1�� ������� ���� ǥ���� �� ����
		today->tm_mday,
		today->tm_hour,
		today->tm_min,
		today->tm_sec);

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