/*
학과 : 전자IT미디어공학과
학번 : 13184394
이름 : 윤현진
제출 : 2014. 06. 12
내용 : Pacman Game 
*/

#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <time.h>
#include <Windows.h>
#include <MMSystem.h> //소리 삽입 헤더

#define KEY_ESC		0x1B
#define KEY_8		'8'
#define KEY_2		'2'
#define KEY_4		'4'
#define KEY_6		'6'
#define KEY_W		'w'
#define KEY_X		'x'
#define KEY_A		'a'
#define KEY_D		'd'
// direction 방향
#define UP			1
#define DOWN		2
#define LEFT		3
#define RIGHT		4
// maze 미로 크기
#define MAZE_H		25
#define MAZE_W		26
// tile type 타일 종류
#define WALL		32
#define FOOD		1
#define PILL		2
#define NUM_PILL	4

//콘솔 글씨 색 변경
#define COL						  GetStdHandle(STD_OUTPUT_HANDLE)             // 콘솔창의 정보 받아옴
#define SETTEXTCOLOR_RED          SetConsoleTextAttribute(COL, 0x000c)        // 빨간색
#define SETTEXTCOLOR_BLUE		  SetConsoleTextAttribute(COL, 0x0009)        // 파란색
#define SETTEXTCOLOR_ORIGINAL     SetConsoleTextAttribute(COL, 0x000f)        // 흰색
#define SETTEXTCOLOR_YELLOW       SetConsoleTextAttribute(COL, 0x000e);       // 노란색
#define SETTEXTCOLOR_HIGH_GREEN   SetConsoleTextAttribute(COL, 0x000a);       // 연두색
#define SETTEXTCOLOR_SKY_BLUE     SetConsoleTextAttribute(COL, 0x000b);       // 하늘색
#define SETTEXTCOLOR_PLUM         SetConsoleTextAttribute(COL, 0x000d);       // 자주색


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
const int wake_up_time = 2000; //약효 지속 시간. msec.
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
char* tile[] = {"  ", "┌", "┐", "└", "┘", "│", "─", "├", "┤", "┬", "┴", "┼"};


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

	PlaySound(TEXT("Skrillex-Kill Everybody.wav"), NULL, SND_LOOP | SND_NOSTOP | SND_ASYNC); //소리 삽입

	// play
	while (ch = check())
	{	
		if (_kbhit())
			turn(_getch());
		show_instruction();
		clear_pacman();
		move(); //팩맨 이동함수 호출
		draw_pacman(); //팩맨 출력함수 호출

		// timer
		gotoxy(54, 8);
		SETTEXTCOLOR_ORIGINAL; //일반 콘솔 색으로 
		printf("%7.1f sec", (clock()-tic)/1000.0);  //시간 출력

		About_wakeup(); //각성타임 함수 호출

		Sleep(Speed); 
	}

	// exit
	gotoxy(54, 10);
	puts("Done!");
	gotoxy(0, MAZE_H);
	gotoxy(54, 16);
	SETTEXTCOLOR_RED;
	puts("0.Exit 1.Restart"); //시스템 종료 및 재시작
	gotoxy(54, 17);
	SETTEXTCOLOR_ORIGINAL;
	puts(" + press Enter");
	gotoxy(54, 16);
	RETRY : 
	scanf("%d",&a); //입력받고

	if(a==1) //다시 하겠다고 하면
	{
		system("cls"); //시스템 클리어 시키고
		goto REPLAY; //선언된 레이블지점으로
	}
	else if(a==0) //나가겠다고 하면
		return 0;  //나가도록.
	else 
		gotoxy(54,19); //1이나 0 이외 다른 수를 입력하면
		puts("input 1 or 0"); //1이나 0을 넣으라고 함
		goto RETRY;;

}


void Whattime(struct tm** pToday) //현재시간 표시함수 정의
{
    time_t ltime;
	time(&ltime);
    *pToday = localtime(&ltime);
}

//ASCgen2 를 이용하여 그림을 아스키 문자로 변환
void print_comp()
{
	//문자열 배열 선언
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
	//행과 열 부분으로 나누어 각각 콘솔 폰트 색상 변경
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
	//로딩바
	for(i=0;i<=30;i++) {
		delay(20); //속도조절
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
	system("cls"); //로딩 대기화면지움
}


void delay(unsigned int n) //학교 로고가 콘솔 화면에 떠 있는 시간
{ 
	unsigned o = GetTickCount(); //부팅부터 소요된 시간[ms]
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
	알약(PILL)을 NUM_PILL만큼 생성해서
	음식(FOOD)대신에 위치시킨다.
	*/
	for(i=0;i<NUM_PILL;i++) 
	{
		pos = rand_pos();
		maze[pos.row][pos.col] = PILL;  //NUM_PILL이 될 때까지 랜덤으로 돌려 하나씩 맵에 배치함
	}


	// place packman 
	/*  TODO
	팩맨(pacman)을 임의의 장소에 위치시킨다.
	주의!
	팩맨은 자신의 위치의 음식을 자동 섭취하므로
	팩맨 위치(packman)의 maze값은 0으로 변경
	*/ 

	pacman = rand_pos(); //팩맨을 임의의 장소에 위치시키고
	maze[pacman.row][pacman.col]=0;  //팩맨위치의 maze값은 0으로 변경
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
	현재 진행 방향(direction)에 따라
	팩맨을 이동시켜 위치(pacman)를 갱신 
	현재 위치에 있는 음식은 자동으로 섭취하므로
	팩맨 위치(packman)의 maze값은 0으로 변경

	진행방향에 벽이 있으면 제자리에*/

	POS pos;
	int goto_x=0; //진행하려 하는 쪽의 X좌표
	int goto_y=0; //진행하려 하는 쪽의 Y좌표
	POS Go_Pacman;

	switch(direction) 
	{
	case LEFT: goto_x=-1; //벽 한 칸 앞에서 감지
		break;
	case RIGHT: goto_x=1; //벽 한 칸 뒤에서 감지
		break;
	case UP: goto_y=-1;  //벽 한 칸 앞에서 감지
		break;
	case DOWN: goto_y=1;  //벽 한 칸 뒤에서 감지
		break;
	}

	//진행하는 팩맨 좌표
	Go_Pacman.row = pacman.row + goto_y;
	Go_Pacman.col = pacman.col + goto_x;

	//만약 팩맨이 가려고 하는 좌표 앞이 벽이 아니라면
	if (!IsItWall(Go_Pacman.row, Go_Pacman.col)) 
	{
		pacman = Go_Pacman;	//그대로 움직인다
		if(maze[pacman.row][pacman.col] == PILL) //알약을 먹은 후에는
		{ 
			Converted = 1;  //각성 모드로 바뀌고
			Partytime = clock();  //속도가 빨라짐
		}
		maze[pacman.row][pacman.col] = 0; 
	}
}

void About_wakeup() //각성타임에서는
{
	int i,j;
	if(Converted == 1)  //스위치가 on되면서
	{
		Speed = hot6;  //빠르기가 각성제 빠르기로 변함
		SETTEXTCOLOR_PLUM; //각성 시 벽색
	} 
	else 
	{
		Speed = REAL_Speed; //이외의 경우에는 지정된 빠르기로.
	}
	if (clock()-Partytime > wake_up_time) //지정된 각성시간이 끝나면
	{
		Converted = 0; //모드 스위치를 끄고 원래대로.
	}
}

// check
int check()
{
	/*  TODO
	미로(maze) 안에 남아있는 
	먹이(FOOD)나 알약(PILL)의 개수를 return
	전부 먹었으면 0을 return
	*/	
	int i,j, grounds = 0;
	for(i=0;i<MAZE_H;i++) 
	{
		for(j=0;j<MAZE_W;j++) 
		{
			if(maze[i][j]==FOOD || maze[i][j]==PILL) //음식이나 알약이 있으면
			{
				grounds++;  //잔여물 갯수 순차증가
			}
		}
	}
	return grounds;  //잔여물 있다고 반환해줌
}


BOOL IsItFood(POS pos)  //음식인지 아닌지 파악
{
	if (maze[pos.row][pos.col]==FOOD) //맵의 좌표에 음식이 있으면
		return TRUE; //참 리턴
	else  //그 외의 경우
		return FALSE;  //거짓 리턴
}

// random position
POS rand_pos()
{
	/*  TODO
	maze 안의 위치를 랜덤으로 선택한다.
	주의!
	FOOD가 있는 위치에서만 선택
	*/
	POS pos;
	do
	{
		pos.row = rand() % MAZE_H; //행에서의 위치 랜덤설정
		pos.col = rand() % MAZE_W; //열에서의 위치 랜덤설정
	}
	while(!IsItFood(pos)); 
	return pos; //pos를 되돌려줌
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
	packman을 그린다.
	방향(direction)에 따라 V ^ > < 모양으로
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

void luminescence() //팩맨이 약을 먹으면 발광하게 해 줌
{
	if(pacmanface == 1 && About_wakeup == 0) //안정 상태일 때는
	{
		SETTEXTCOLOR_YELLOW; //노란색으로
		pacmanface = 2;
	} 
	else if (pacmanface == 2 && Converted == 1) //약을 먹었을 때는
	{
		SETTEXTCOLOR_PLUM; //자주색과
		pacmanface = 1;
	} 
	else 
	{
		SETTEXTCOLOR_YELLOW; //노란색이 번갈아가며 발광하듯이 나타난다.
		pacmanface = 2;
	}
}

// draw maze
void draw_maze()
{
	/*  TODO
	미로(maze)를 그린다.
	FOOD  ". "
	PILL  "+ "
	WALL  tile 이용
	*/

	int i, j;

	for(i=0; i<MAZE_H; i++)
	{
		for(j=0; j<MAZE_W; j++)
		{
			switch(maze[i][j]) 
			{
			case FOOD: 
				SETTEXTCOLOR_SKY_BLUE; //음식 색은 하늘색으로
				printf(". ");
				break;
			case PILL:
				SETTEXTCOLOR_RED; //알약 색은 빨간색으로
				printf("+ ");
				break;
			default:
				if(IsItWall(i,j)) 
				{
					SETTEXTCOLOR_HIGH_GREEN; //기본 벽 색 연두색
					printf(tile[maze[i][j]-WALL]);
				} else 
				{
					printf("  ");
				}
			}
		}
		if(i!=MAZE_H-1)  //콘솔창의 아래 여백 없게 조절
		{
			printf("\n");
		}
	}
}


BOOL IsItWall(int i, int j)   //벽인지 아닌지 판단 함수
{
	if(32 <= maze[i][j] && maze[i][j] <= 43) //위에서 지정해 둔 벽 문자열 값 범위 내에 있으면
	{
		return TRUE; //벽이다.
	} 
	else  //이 외의 경우
	{
		return FALSE;  //벽이 아니다.
	}
}


// show instruction
void show_instruction()
{
	struct tm* today; //날짜 출력용 구조체 선언
	SETTEXTCOLOR_ORIGINAL;
	gotoxy(54,1);		puts("Left(4, A), Right(6, D)");
	gotoxy(54,2);		puts("Down(2, X), Up(8, W)");
	gotoxy(54,3);		puts("Quit(ESC)");
	gotoxy(54,5);		puts("Press any key to start...");
    //시간 출력. 마지막으로 움직였을 때의 년, 월, 일, 시, 분, 초가 출력됨. 보기 편하게 press ESC to exit이 출력된 후 이 자리에 삽입.
	Whattime(&today);
	gotoxy(54,14);       printf("%04d-%02d-%02d %02d:%02d:%02d",
		today->tm_year + 1900,   // tm_year는 1900을 더해야 서기 연도가 됨
		today->tm_mon + 1, // tm_mon은 1월을 0부터 입력받아 1을 더해줘야 월을 표시할 수 있음
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