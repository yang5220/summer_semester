#pragma once
#pragma once
#include<Windows.h>
#pragma comment(lib,"winmm.lib")
#define ROW         4
#define COL         5
#define Click_W      1
#define Click_B     0
#define Click_Error 2
#define Score       30

typedef struct _GAME_COORD
{
	int x;
	int y;
}GAME_COORD, *PGAME_COORD;

HBITMAP getNote1(HWND);
void PlayMusic();
void AddLevel();
void AddScore();
int GetScore();
int GetLevel();
//bool Miss();
//bool Start();

void SetBoundary(int x, int y);
PGAME_COORD GetBoundary();

int* GetK();
POINT GetPosit(int, int);

void CreateKuai();//��ʼ���ڰ׿�λ��
bool ReInit();//���һ�κ���ºڰ׿�λ
bool SpeedUp(HWND hwnd);//������ӿ���Ϸ�ٶ�
			 
void OnKeyDown(DWORD vk, HWND hwnd);//ÿ����һ�������жϺ�
//int BlcOrWht(int);//���ذ�����Ϣ

//��ʱ����ʱ�󣬸��ݰ�����Ϣ������һ������		  
void OnTimer(HWND hwnd);

void CreateGame(HWND hwnd,
	DWORD dwInitTimerElapse,
	DOUBLE level_speedup_ratio,
	int boundary_x, int boundary_y);
//extern bool flag1;

