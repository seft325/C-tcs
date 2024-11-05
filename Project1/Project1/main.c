#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <Windows.h>
#include <conio.h>
#define W 70
#define H 25
//定义蛇的结构体
typedef struct _body
{
	int x;
	int y;
}BODY;
typedef struct snake
{
	BODY list[W * H];  //最大数目
	int size;          //原始大小
	BODY food;      //食物
	COORD coord;      //定位光标坐标
	int dx;
	int dy;          //移动方向
	BODY tail;       //记录设为
	int score;
}SNAKE;
//设置图形边界
void test_ui()
{
	for (int i = 0; i < H; i++)
	{
		for (int j=0; j < W; j++) {
			printf("=");
		}
		printf("\n");
	}
}
void init_food(BODY * food)
{
	//随机数种子
	srand(time(NULL));
	food->x = rand() % W+1;
	food->y = rand() % H+1;
}
void init_snake(SNAKE * snake)
{
	snake->list[0].x = W / 2;
	snake->list[0].y = H / 2;
	snake->list[1].x = W / 2 - 1;
	snake->list[1].y = W / 2 - 1;
	snake->size = 2;
	//设置移动方向
	snake->dx = 1;  //默认方向向右
	snake->dy = 0;
	snake->score = 0;

	//设置食物
	init_food(&(snake->food));

}
void show_ui(SNAKE* snake)
{
	//显示蛇
	for (int i = 0; i < snake->size; i++)
	{
		snake->coord.X = snake->list[i].x;
		snake->coord.Y = snake->list[i].y;
		SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), snake->coord);
		if (i == 0) {
			printf("@");
		}
		else
		{
			printf("*");
		}
	}
	//显示食物
	snake->coord.X = snake->food.x;
	snake->coord.Y = snake->food.y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), snake->coord);
	printf("#");

	//清楚蛇的结尾
	snake->coord.X = snake->tail.x;
	snake->coord.Y = snake->tail.y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), snake->coord);
	printf(" ");
}
void show_wall()
{
	for (int i = 0; i <= H; i++)
	{
		for (int j = 0; j <= W; j++) {
			if (i==0||j==0||i==H||j==W)
			{
				printf("&");
			}
			else
			{
				printf(" ");
			}
		}
		printf("\n");
	}
}
//隐藏光标
void handle_cur()
{
	CONSOLE_CURSOR_INFO cci;
	cci.dwSize = sizeof(cci);
	cci.bVisible = FALSE;
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cci);
}
//蛇的移动
void snake_move(SNAKE * snake)
{
	snake->tail = snake->list[snake->size - 1];
	for (int i = snake->size-1; i >0; i--)
	{
		snake->list[i] = snake->list[i - 1];   //2=1   1=0
	}
	snake->list[0].x += snake->dx;
	snake->list[0].y += snake->dy;
}
void control_snake(SNAKE* snake)
{
	char key = 0;
	while (_kbhit()) {
		key = _getch();
	}
	//抬起按下动作 && 没有按下动作
	switch (key)
	{
	case'w':
		snake->dy = -1;
		snake->dx = 0;
		break;
	case's':
		snake->dy = 1;
		snake->dx = 0;
		break;
	case'a':
		snake->dy = 0;
		snake->dx = -1;
		break;
	case'd':
		snake->dy = 0;
		snake->dx = 1;
		break;
	}
}
void snake_eat_food(SNAKE* snake)
{
	if (snake->list[0].x == snake->food.x &&snake->list[0].y==snake->food.y)
	{
		snake->size++;//吃到食物且不需要食物消失
		init_food(&(snake->food));
		snake->score += 10;
	}
}
void game_overs(SNAKE* snake)
{
	snake->coord.X = 80;
	snake->coord.Y = 20;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), snake->coord);
	printf("游戏结束，总分数为：%d\n", snake->score);
}
void snake_eat_body(SNAKE* snake)
{
	for (int i = 1; i < snake->size; i++)
	{
		if (snake->list[0].x == snake->list[i].x && snake->list[0].y == snake->list[i].y) {
			//吃到身体游戏结束
			game_overs(snake);
			exit(0);
		}
	}
}
void start_gane(SNAKE* snake)
{
	while(snake->list[0].x>0&&snake->list[0].x <W&&snake->list[0].y>0&&snake->list[0].y<H)
	{
		//显示
		show_ui(snake);
		//先控制方向
		control_snake(snake);
		//吃食物
		snake_eat_food(snake);
		snake_eat_body(snake);
		//移动
		snake_move(snake);
		//延迟1s
		Sleep(250);	
	}
	game_overs(snake);
}
int main()
{
	handle_cur();
	SNAKE* snake = (SNAKE *)malloc(sizeof(SNAKE));
	init_snake(snake);
	show_wall();
	start_gane(snake);
	free(snake);
	while (1);
	return 0;
}

