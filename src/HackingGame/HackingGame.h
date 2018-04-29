#pragma once

#pragma region ͷ�ļ�����

// Windows ͷ�ļ�: 
#include <windows.h>

// C ����ʱͷ�ļ�
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>

// ��Դͷ�ļ�
#include "resource.h"
#pragma comment(lib, "Msimg32.lib")


// TODO:  �ڴ˴����ó�����Ҫ������ͷ�ļ�
#include <vector>
#include <math.h>
#include <time.h>
#include <math.h>
#include <assert.h>
#include <string.h>

#pragma endregion


#pragma region �궨��

#define WINDOW_WIDTH			1280	//��Ϸ���ڿ��
#define WINDOW_HEIGHT			720		//��Ϸ���ڸ߶�

#define STAGE_LOADING			-1		//���ػ����ID
#define STAGE_STARTMENU			0		//��ʼ�����ID

#define HERO_SIZE				20		//���ǰ뾶
#define HERO_ACCELERATION		0.5		//���Ǽ��ٶ�
#define HERO_MAXSPEED			5.0		//��������ٶ�
#define HERO_HEALTH				5.0		//����Ѫ��

#define BULLET_SIZE_X			7		//�ӵ��Ŀ��
#define BULLET_SIZE_Y			27		//�ӵ��ĳ���
#define BULLET_MINSPEED			10.0	//�ӵ����ٶ�
#define BULLET_ACCELERATION		1.0		//�ӵ����ٶ�
#define BULLET_MAXSPEED			15.0	//�ӵ�����ٶ�

#define BARRAGE_SPEED			2			//��Ļ�ٶ�
#define BARRAGE_SIZE			10			//��Ļ�뾶

#define BOSS_SIZE				25			//BOSS�뾶
#define BOSS_HEALTH				10.0		//BOSSѪ��
#define BOSS_SPEED				0.5			//BOSS�ٶ�

#define BATMAN_SPEED			0.5			//С���ٶ�
#define BATMAN_INTERVAL			200			//����С���ļ��
#define BATMAN_SIZE				25			//С���Ļ�뾶

#define ROCKET_SIZE				20			//�ɵ��Ļ�뾶
#define ROCKET_SPEED			3.0			//�ɵ����ٶ�

#define SAFE_DISTANCE				100		//��ȫ����

#define BUTTON_STARTGAME			1001	//��ʼ��Ϸ��ťID

#define BEGIN_X						180		//��1��������������Ͻ�X����
#define BEGIN_Y						48		//��1��������������Ͻ�Y����
#define RECT_SIZE_X					50		//����������
#define RECT_SIZE_Y					52		//��������߶�
#define RECT_NUM_X					21		//��������X����
#define RECT_NUM_Y					12		//��������Y����
#define BRICK_HEALTH				3		//ש��Ѫ��

#pragma endregion


#pragma region �ṹ������

//-1Ϊ���ؽ��棬0Ϊ��ʼ����
// �����ṹ��
struct Stage
{
	HBITMAP bg;				//����ͼƬ
	int stageID;			//�������
	//int timeCountDown;	//��Ϸʱ�䵹��ʱ
	bool timerOn;			//��ʱ���Ƿ����У���Ϸ�Ƿ���ͣ��
	int score;				//���ַ���
	bool loading;			//�Ƿ������
	int rec[3];				//ש��ķ���
	int batman;				//С���ĸ���
	int numbarrage;			//��Ļ������
	int barrage[8];			//��Ļ�ķ��䷽ʽ
	int win;				//Ӯ��Ϊ1������Ϊ-1������Ϊ0
	int timebarrage;		//���䵯Ļ�ļ��ʱ��
	int callbatman;			//�ٻ�С���ļ��ʱ�� 0Ϊ���ٻ�
};

// ��ť�ṹ��
struct Button
{
	int buttonID;	//��ť���
	bool visible;	//��ť�Ƿ�ɼ�
	bool focused;	//��ť�Ƿ񱻾۽�
	int click;		//��ť�Ƿ񱻵��
	wchar_t *img;	//ͼƬ·��
	wchar_t *focusedimg;	//�۽�ͼƬ·��
	int x;			//���Ͻ�����x
	int y;			//���Ͻ�����y
	int width;		//���
	int height;		//�߶�
};

// ���ǽṹ��
struct Hero
{
	int Class;		//Ӣ������
	//int ID;		//ID
	int x;			//��������x
	int y;			//��������y
	double vx;		//�ٶ�x
	double vy;		//�ٶ�y
	double dir;     //���ǵĳ���
	int health;		//����ֵ
	bool available;	//�Ƿ��л���
};

//�ӵ�(��Ļ)�ṹ��
struct Bullage
{
	int Class;			//���ͣ�+Ϊ�ӵ���-1Ϊ�ɱ��ƻ��ĵ�Ļ,-2Ϊ���ɱ��ƻ��ĵ�Ļ
	float x;			//��������x
	float y;			//��������y
	float v;			//�ٶ�
	double dirX;			//x����
	double dirY;			//y����
};

//Boss�ṹ��
struct Boss
{
	bool available;	//�Ƿ���Թ���
	float x;		//����x
	float y;		//����y
	int health;		//����ֵ
	float vx;		//x�ٶ�
	float vy;		//y�ٶ�
};

//С���ṹ��
struct Batman
{
	float x;		//����x
	float y;		//����y
	double dirX;		//x����
	double dirY;		//y����
};

//��������ṹ��
struct Recta
{
	int x;
	int y;
	int health;		//��������Ѫ��������Ϊ�ɱ������ӵ�(��Ļ)�ƻ���ש�飬0Ϊ������ש�飬-1Ϊ���ɱ������ӵ�(��Ļ)�ƻ���ש�飬-2Ϊ����ש�飩
};


struct Node
{
	int x;
	int y;
	struct Path *path;
};

struct Path
{
	Node *dst;
	Path *next;
};

#pragma endregion


#pragma region �¼�����������


// ��ʼ����Ϸ���庯��
void InitGame(HWND hWnd, WPARAM wParam, LPARAM lParam);

// ���̰����¼�������
void KeyDown(HWND hWnd, WPARAM wParam, LPARAM lParam);

// �����ɿ��¼�������
void KeyUp(HWND hWnd, WPARAM wParam, LPARAM lParam);

// ����ƶ��¼�������
void MouseMove(HWND hWnd, WPARAM wParam, LPARAM lParam);

// �����������¼�������
void LButtonDown(HWND hWnd, WPARAM wParam, LPARAM lParam);

// �������ɿ��¼�������
void LButtonUp(HWND hWnd, WPARAM wParam, LPARAM lParam);

// ��ʱ���¼�������
void TimerUpdate(HWND hWnd, WPARAM wParam, LPARAM lParam);

#pragma endregion


#pragma region ������Ϸ״̬����������
//���Boss����
Boss* CreateBoss(int x, int y, int health);

//����ӵ�(��Ļ)����
Bullage* CreateBullet(double dirX, double dirY, int x, int y, int cl);

//��ӵ�Ļ����
Bullage* CreateBarrage(int Class, double dirX, double dirY, int x, int y, float v);

// ��Ӱ�ť����
Button* CreateButton(int buttonID, wchar_t* img, wchar_t* foucusedimg, int width, int height, int x, int y);

// ������Ǻ���
Hero* CreateHero(int cl, int x, int y, int health);

// ���С������
Batman* CreateBatman(int x, int y);

//��ʼ���������򣨴���ש�飩����
void InitRecta(int n1, int n2, int n3);

//����Ƿ���ͨ����
bool ifConnected();

// ��ʼ����������
void InitStage(HWND hWnd, int stageID);

//ˢ���ӵ�״̬����
void UpdateBullet();

//ˢ�µ�Ļ״̬����
void UpdateBarrage();

// ˢ��С��״̬����
void UpdateBatman();

//ˢ��Boss״̬����
void UpdateBoss();

// ˢ������״̬����
void UpdateHero();

// ˢ�·ɵ�״̬����
void UpdateRocket();

// �������ǵĵ�ǰ����
double GetHeroDir(int dirX, int dirY);

//�����ӵ�(��Ļ)��ǰ����
double GetBulletDir(double dirX, double dirY);

//TODO: �����Ϸ��Ҫ�ĸ��ຯ��

#pragma endregion


#pragma region ��ײ��⺯��

// ��������֮��ľ���  
float DistanceBetweenTwoPoints(float x1, float y1, float x2, float y2);

//Բ��Բ��ײ���
bool CheckEllipse(int x1, int y1, int x2, int y2, double r1, double r2);

//Բ��ƽ���ı�����ײ���
bool CheckRec_Ellipse(int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4, double x, double y, double r);

// Բ�������ײ���  
// Բ��(x, y), �뾶r, ��������(x0, y0), �����ϱ�����(x1, y1), �����ұ�����(x2, y2)  
bool IsCircleIntersectRectangle(float x, float y, float r, float x0, float y0, float x1, float y1, float x2, float y2);

//�ӵ�(��Ļ)��ש����ײ���
bool CheckBullage_Recta(Bullage *iter, int* i, int* j);

double Distance(int x1, int y1, int x2, int y2);

//������ש����ײ���
void CheckHero_Recta(Hero* a_Hero);

// Ԥ��С�������������ײ���
bool CheckBatman_Recta(Recta recta[RECT_NUM_Y][RECT_NUM_X], int x, int y, int size);

#pragma endregion


#pragma region ��ͼ��������

// ��ͼ����
void Paint(HWND hWnd);

#pragma endregion


#pragma region ��ʱ������

//����ʱ/�����߳�
DWORD WINAPI Timer(LPVOID lpParameter);

#pragma endregion


#pragma region ���䵯Ļ����
//ѭ�����n*10���뷢�䵯Ļ(�ܲ�����Ϊm,��Ļ���������������cl[])����
void loop(int x, int y, int n, int m, int cl[]);

//���䵯Ļ����
void shootbarrage1(int cl);

void shootbarrage2(int x0, int y0, int x, int y, int cl);

void shootbarrage3(int cl);

void shootbarrage4(int cl);

void shootbarrage5(int cl);

#pragma endregion


#pragma region ·���滮����
bool routePlanning(Recta recta[RECT_NUM_Y][RECT_NUM_X], int x0, int y0, int x1, int y1, float* dirX, float *dirY);

bool IsRouteAvailable(Recta recta[RECT_NUM_Y][RECT_NUM_X], int x0, int y0, int x1, int y1, int size, float speed);

#pragma endregion

#pragma region �߾��ȼ�ʱ��

class MyTimer
{
private:
	int _freq;
	LARGE_INTEGER _begin;
	LARGE_INTEGER _end;

public:
	long costTime;            // ���ѵ�ʱ��(��ȷ��΢��)  

public:
	MyTimer()
	{
		LARGE_INTEGER tmp;
		QueryPerformanceFrequency(&tmp);//QueryPerformanceFrequency()���ã�����Ӳ��֧�ֵĸ߾��ȼ�������Ƶ�ʡ�  

		_freq = tmp.QuadPart;
		costTime = 0;
	}

	void Start()            // ��ʼ��ʱ  
	{
		QueryPerformanceCounter(&_begin);//��ó�ʼֵ  
	}

	void End()                // ������ʱ  
	{
		QueryPerformanceCounter(&_end);//�����ֵֹ  
		costTime = (long)((_end.QuadPart - _begin.QuadPart) * 1000000 / _freq);
	}

	void Reset()            // ��ʱ��0  
	{
		costTime = 0;
	}
};

#pragma endregion