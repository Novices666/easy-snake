#include<stdio.h> //c����
#include<graphics.h>//ͼ�ν���
#include<conio.h>//����
#include<stdlib.h>//�����
#include<mmsystem.h>//��������
#include<Windows.h>
#include<string.h>
#include <iostream>
#include<sstream>

using namespace std;

#pragma comment(lib,"winmm.lib")//��������
#define RC_FILE Resource.resx

#define Snake_num  500  //��󳤶�
#define Win_Height 480.0
#define Win_Width  640.0

void Start();
void SetGame();
void GameInit();
void GameDraw();
void GameOver();
void EatFood();
void SnakeMove();
void SnakeAlive();
void KeyScan();

//���尴ť
enum {
	UP,
	DOWN,
	LEFT,
	RIGHT
};

//����snake
struct Snake
{
	int size;//����
	int dir;//�˶�����
	int speed;//�ٶ�
	int r;
	int score; //����

	bool flag;//�Ƿ����

	DWORD color;
	POINT coor[Snake_num];

}snake;

//������Ϸ�趨
struct GameSet
{
	bool Twall; //�Ƿ���Դ�ǽ
	bool AutoColor;  //�Ƿ��Զ���ɫ��snake��
	bool FoodChange; //ʳ���С��ɫ�Ƿ�ı�

}set;

//����ʳ��
struct Food
{
	int x;
	int y;
	int r;		//ʳ��뾶
	bool flag;   //ʳ��״̬   �Ƿ����
	DWORD color; 
}food;

string Int_to_String(int n)

{
	ostringstream stream;
	stream << n;  //nΪint����
	return stream.str();
}

//Start
void Start() {
	//��ʼ�� ����
	initgraph(Win_Width, Win_Height);
	IMAGE bj_img;
	
	loadimage(&bj_img, _T("./res/bj.jpg"), Win_Width, Win_Height, true);
	int x = 10;
	while (!kbhit())
	{
		BeginBatchDraw();
		putimage(0, 0, &bj_img);
		settextcolor(RGB(rand() % 256, rand() % 256, rand() % 256));
		settextstyle(x, 0, "����");
		setbkmode(TRANSPARENT);
		outtextxy((Win_Width-10*x)/2, Win_Height*0.9, "���������������Ϸ��");
		
		if (x > 20) {
			Sleep(500);
			x--;
		}else { 
			Sleep(500);
			x++;
		}

		EndBatchDraw();
	}
}

//��Ϸ��ʼ��
void GameInit() {
	//BGM
	mciSendString("open ./res/bgm.mp3 alias BGM", 0, 0, 0);
	mciSendString("open ./res/eatfood.wav alias EAT", 0, 0, 0);
	mciSendString("play BGM repeat", 0, 0, 0);

	

	//�����������  GetTickCount() ��ȡ����������
	srand(GetTickCount());

	//��ʼ�� snake
	snake.size = 3;
	snake.speed = 10;
	snake.r = 5;
	snake.dir=RIGHT;
	snake.score = 0;
	snake.color = RGB(0, 0, 0);
	snake.flag = true;
	for (int i = 0;i < snake.size;i++) {
		snake.coor[i].x = (snake.size)*10-10*i;
		snake.coor[i].y = 10;
	}

	//��ʼ��food
	if (set.FoodChange == true) {
		food.color = RGB(rand() % 256, rand() % 256, rand() % 256);
		food.r = rand() % 10 + 5;
	}
	else {
		food.color = RGB(0, 0, 0);
		food.r = snake.r;
	}

	food.x = rand() % 640;
	food.y = rand() % 480;
	
	food.flag = true;

	string score = "��ǰ�÷֣�" + Int_to_String(snake.score);
	SetWindowText(GetHWnd(), score.c_str());
}

//����
void GameDraw() {
	//����  ��ֹ����   ��������
	BeginBatchDraw();

	//���ڱ���
	//setbkcolor(RGB(58, 155, 7));
	setbkcolor(RGB(255, 182, 193));
	cleardevice();

	//����snake
	setfillcolor(snake.color);
	for (int i = 0;i<snake.size;i++){
		solidcircle(snake.coor[i].x, snake.coor[i].y, snake.r);
	}

	//����food
	setfillcolor(food.color);
	if (food.flag)solidcircle(food.x, food.y, food.r);

	//��������  ��������   ��ʾ��������
	EndBatchDraw();
}

//�ƶ�
void SnakeMove() {
	for (int i = snake.size-1;i>0;i--) {
		snake.coor[i] = snake.coor[i - 1];
	}

	switch (snake.dir) {
		case UP:
			if (snake.coor[0].y == 0&&set.Twall==true)snake.coor[0].y = 480;
			snake.coor[0].y-=snake.speed;
			break;
		case DOWN:
			if (snake.coor[0].y == 480 && set.Twall == true)snake.coor[0].y = 0;
			snake.coor[0].y+=snake.speed;
			break;
		case LEFT:
			if (snake.coor[0].x == 0 && set.Twall == true)snake.coor[0].x = 640;
			snake.coor[0].x-=snake.speed;
			break;
		case RIGHT:
			if (snake.coor[0].x == 640 && set.Twall == true)snake.coor[0].x = 0;
			snake.coor[0].x+=snake.speed;
			break;
	}

}

//��ȡ����
void KeyScan() {
	if (_kbhit()) {
		switch (_getch()) {//72  80  75  77  С���� �������� ��ֵ
			case 'w':
			case 'W':
			case 72:
				if(snake.dir != DOWN)snake.dir = UP;
				break;
			case 's':
			case 'S':
			case 80:
				if (snake.dir != UP)snake.dir = DOWN;
				break;
			case 'a':
			case 'A':
			case 75:
				if (snake.dir != RIGHT)snake.dir = LEFT;
				break;
			case 'd':
			case 'D':
			case 77:
				if (snake.dir != LEFT)snake.dir = RIGHT;
				break;
			case ' '://�ո���ͣ��Ϸ
				mciSendString("pause BGM", NULL, 0, NULL);
				while (1) {
					if (_getch() == ' ') {
						mciSendString("resume BGM", NULL, 0, NULL);
						break;
					}
				}
				break;
		}
				
	}
	SnakeMove();
}

//ʳ��
void EatFood() {
	if (!food.flag) {
		srand(GetTickCount());

		//��ʾ�÷�
		snake.score += 10;
		string score = "��ǰ�÷֣�" + Int_to_String(snake.score);
		SetWindowText(GetHWnd(), score.c_str());

		//��������food
		if (set.AutoColor == true) {
			snake.color = food.color;
		}else{
			snake.color = RGB(0, 0, 0);
		}

		if (set.FoodChange == true) {
			food.color = RGB(rand() % 256, rand() % 256, rand() % 256);
			food.r = rand() % 10 + 5;
		}
		else {
			food.color = RGB(0, 0, 0);
			food.r = snake.r;
		}
		food.x = rand() % 640;
		food.y = rand() % 480;
		food.flag = true;
	}
	else {
		if (snake.coor[0].x >= food.x - food.r && snake.coor[0].x <= food.x + food.r&&
			snake.coor[0].y >= food.y - food.r && snake.coor[0].y <= food.y + food.r)
		{
			food.flag = false;
			if (food.flag == false)PlaySound(TEXT("./res/eatfood.wav"), NULL, SND_FILENAME | SND_ASYNC);
			snake.size++;
		}
	}
	
}

//�ж����Ƿ����
void SnakeAlive() {
	for (int i = 1;i <= snake.size - 1;i++) {
		if (snake.coor[0].x >= snake.coor[i].x - snake.r && snake.coor[0].x <= snake.coor[i].x + snake.r&&
			snake.coor[0].y >= snake.coor[i].y - snake.r && snake.coor[0].y <= snake.coor[i].y + snake.r)
		{
			GameOver();
			snake.flag = false;
		}
		else snake.flag = true;
		if (set.Twall == false) {
			if (snake.coor[0].y == 0 || snake.coor[0].x == 0 || snake.coor[0].y == 480 || snake.coor[0].x == 640) {
				GameOver();
				snake.flag = false;
			}
			else snake.flag = true;
		}
	}
}

//GameOver
void GameOver()
{
		char massage[] = "�������ո����������Ϸ,����ESC�������á�";
		IMAGE gmor_img, gmor_img_1, gmor_img_2;//   �޺�׺  ԭͼ    _1  �ڵ�   _2  �׵�
		loadimage(&gmor_img_1, _T("./res/gameover_1.png"));
		loadimage(&gmor_img_2, _T("./res/gameover_2.png"));
		loadimage(&gmor_img, _T("./res/gameover.png"), 610, 223);

		
		bool isok = false;
		while (!isok)
		{
			double x = 20;// �����С
			putimage((Win_Width - 610) / 2, (Win_Height - 223) / 2-20, &gmor_img_2, SRCAND);
			putimage((Win_Width - 610) / 2, (Win_Height - 223) / 2-20, &gmor_img_1, SRCPAINT);
			settextcolor(RGB(rand() % 256, rand() % 256, rand() % 256));
			settextstyle(x, 0, "����");
			setbkmode(TRANSPARENT);
			outtextxy((Win_Width - 21.0* x) / 2.0, Win_Height*0.8+10, massage);

			x = 40;
			settextstyle(x, 0, "����");
			string score = "���յ÷�:" + Int_to_String(snake.score);
			outtextxy((Win_Width - 6.0* x) / 2.0, Win_Height*0.8 - 40.0,score.c_str());
			mciSendString("close BGM", NULL, 0, NULL);
			Sleep(500);
			if (!isok) {
				while (_kbhit()) {
					switch (_getch()) {
						case VK_ESCAPE:
							SetGame();
							GameInit();
							isok = true;
							break;
						case' ':
							GameInit();
							isok = true;
							break;
					}
					
				}
			}		
		}

}

//��Ϸ����
void SetGame() {
	IMAGE bj_img;
	loadimage(&bj_img, _T("./res/bj.jpg"), Win_Width, Win_Height, true);
	settextcolor(RGB(0,0,0));
	setbkmode(TRANSPARENT);
	bool isok = false;

	while (!isok) {
		BeginBatchDraw();
		putimage(0, 0, &bj_img);
		int x = 30;
		settextcolor(RGB(0, 69, 138));
		settextstyle(x, 0, "����");
		outtextxy((Win_Width - 7.0* x) / 2.0, Win_Height*0.2+x, "������Ϸ������");
		x = 20;
		settextstyle(x, 0, "����");
		outtextxy((Win_Width - 7.0* x) / 2.0, Win_Height*0.4-x,"�Ƿ�����ǽ��");
		outtextxy((Win_Width - 7.0* x) / 2.0, Win_Height*0.5 - x, "�Ƿ������ɫ��");
		outtextxy((Win_Width - 7.0* x) / 2.0, Win_Height*0.6 -x, "�Ƿ�仯ʳ�");
		
		outtextxy((Win_Width - 21.0* x) / 2.0, Win_Height*0.7 -x, "1������ 0����ر� ����������[�ո�]������");
		EndBatchDraw();
		if (!isok) {
			while (_kbhit()) {
				scanf("%d%d%d", &set.Twall,  &set.AutoColor,&set.FoodChange);
				if (set.AutoColor == 0)snake.color = RGB(0, 0, 0);
				isok = true;
				break;
			}
		}

	}
	
}

//������
int main()
{
	Start();
	SetGame();
	GameInit();
	if (snake.flag == true) {
		while (1) {
			GameDraw();
			Sleep(100);
			KeyScan();
			SnakeAlive();
			EatFood();
			
			//if (food.flag == 0)PlaySound(TEXT("./res/eatfood.wav"), NULL, SND_FILENAME | SND_ASYNC);
		}
	}
	
	return 0;
}