#pragma once

#pragma region 头文件引用

// Windows 头文件: 
#include <windows.h>

// C 运行时头文件
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>

// 资源头文件
#include "resource.h"
#pragma comment(lib, "Msimg32.lib")


// TODO:  在此处引用程序需要的其他头文件
#include <vector>
#include <math.h>
#include <time.h>
#include <math.h>
#include <assert.h>
#include <string.h>

#pragma endregion


#pragma region 宏定义

#define WINDOW_WIDTH			1280	//游戏窗口宽度
#define WINDOW_HEIGHT			720		//游戏窗口高度

#define STAGE_LOADING			-1		//加载画面的ID
#define STAGE_STARTMENU			0		//开始画面的ID

#define HERO_SIZE				20		//主角半径
#define HERO_ACCELERATION		0.5		//主角加速度
#define HERO_MAXSPEED			5.0		//主角最大速度
#define HERO_HEALTH				5.0		//主角血量

#define BULLET_SIZE_X			7		//子弹的宽度
#define BULLET_SIZE_Y			27		//子弹的长度
#define BULLET_MINSPEED			10.0	//子弹初速度
#define BULLET_ACCELERATION		1.0		//子弹加速度
#define BULLET_MAXSPEED			15.0	//子弹最大速度

#define BARRAGE_SPEED			2			//弹幕速度
#define BARRAGE_SIZE			10			//弹幕半径

#define BOSS_SIZE				25			//BOSS半径
#define BOSS_HEALTH				10.0		//BOSS血量
#define BOSS_SPEED				0.5			//BOSS速度

#define BATMAN_SPEED			0.5			//小兵速度
#define BATMAN_INTERVAL			200			//产生小兵的间隔
#define BATMAN_SIZE				25			//小兵的活动半径

#define ROCKET_SIZE				20			//飞弹的活动半径
#define ROCKET_SPEED			3.0			//飞弹的速度

#define SAFE_DISTANCE				100		//安全距离

#define BUTTON_STARTGAME			1001	//开始游戏按钮ID

#define BEGIN_X						180		//第1个矩形区域的左上角X坐标
#define BEGIN_Y						48		//第1个矩形区域的左上角Y坐标
#define RECT_SIZE_X					50		//矩形区域宽度
#define RECT_SIZE_Y					52		//矩形区域高度
#define RECT_NUM_X					21		//矩形区域X行数
#define RECT_NUM_Y					12		//矩形区域Y列数
#define BRICK_HEALTH				3		//砖块血量

#pragma endregion


#pragma region 结构体声明

//-1为加载界面，0为开始界面
// 场景结构体
struct Stage
{
	HBITMAP bg;				//背景图片
	int stageID;			//场景编号
	//int timeCountDown;	//游戏时间倒计时
	bool timerOn;			//计时器是否运行（游戏是否被暂停）
	int score;				//本局分数
	bool loading;			//是否加载中
	int rec[3];				//砖块的分类
	int batman;				//小兵的个数
	int numbarrage;			//弹幕的数量
	int barrage[8];			//弹幕的发射方式
	int win;				//赢了为1，输了为-1，进行为0
	int timebarrage;		//发射弹幕的间隔时间
	int callbatman;			//召唤小兵的间隔时间 0为不召唤
};

// 按钮结构体
struct Button
{
	int buttonID;	//按钮编号
	bool visible;	//按钮是否可见
	bool focused;	//按钮是否被聚焦
	int click;		//按钮是否被点击
	wchar_t *img;	//图片路径
	wchar_t *focusedimg;	//聚焦图片路径
	int x;			//左上角坐标x
	int y;			//右上角坐标y
	int width;		//宽度
	int height;		//高度
};

// 主角结构体
struct Hero
{
	int Class;		//英雄种类
	//int ID;		//ID
	int x;			//中心坐标x
	int y;			//中心坐标y
	double vx;		//速度x
	double vy;		//速度y
	double dir;     //主角的朝向
	int health;		//生命值
	bool available;	//是否有护甲
};

//子弹(弹幕)结构体
struct Bullage
{
	int Class;			//类型，+为子弹，-1为可被破坏的弹幕,-2为不可被破坏的弹幕
	float x;			//中心坐标x
	float y;			//中心坐标y
	float v;			//速度
	double dirX;			//x方向
	double dirY;			//y方向
};

//Boss结构体
struct Boss
{
	bool available;	//是否可以攻击
	float x;		//坐标x
	float y;		//坐标y
	int health;		//生命值
	float vx;		//x速度
	float vy;		//y速度
};

//小兵结构体
struct Batman
{
	float x;		//坐标x
	float y;		//坐标y
	double dirX;		//x方向
	double dirY;		//y方向
};

//矩形区域结构体
struct Recta
{
	int x;
	int y;
	int health;		//矩形区域血量（正数为可被主角子弹(弹幕)破坏的砖块，0为不存在砖块，-1为不可被主角子弹(弹幕)破坏的砖块，-2为陷阱砖块）
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


#pragma region 事件处理函数声明


// 初始化游戏窗体函数
void InitGame(HWND hWnd, WPARAM wParam, LPARAM lParam);

// 键盘按下事件处理函数
void KeyDown(HWND hWnd, WPARAM wParam, LPARAM lParam);

// 键盘松开事件处理函数
void KeyUp(HWND hWnd, WPARAM wParam, LPARAM lParam);

// 鼠标移动事件处理函数
void MouseMove(HWND hWnd, WPARAM wParam, LPARAM lParam);

// 鼠标左键按下事件处理函数
void LButtonDown(HWND hWnd, WPARAM wParam, LPARAM lParam);

// 鼠标左键松开事件处理函数
void LButtonUp(HWND hWnd, WPARAM wParam, LPARAM lParam);

// 定时器事件处理函数
void TimerUpdate(HWND hWnd, WPARAM wParam, LPARAM lParam);

#pragma endregion


#pragma region 其它游戏状态处理函数声明
//添加Boss函数
Boss* CreateBoss(int x, int y, int health);

//添加子弹(弹幕)函数
Bullage* CreateBullet(double dirX, double dirY, int x, int y, int cl);

//添加弹幕函数
Bullage* CreateBarrage(int Class, double dirX, double dirY, int x, int y, float v);

// 添加按钮函数
Button* CreateButton(int buttonID, wchar_t* img, wchar_t* foucusedimg, int width, int height, int x, int y);

// 添加主角函数
Hero* CreateHero(int cl, int x, int y, int health);

// 添加小兵函数
Batman* CreateBatman(int x, int y);

//初始化矩形区域（创建砖块）函数
void InitRecta(int n1, int n2, int n3);

//检测是否连通函数
bool ifConnected();

// 初始化场景函数
void InitStage(HWND hWnd, int stageID);

//刷新子弹状态函数
void UpdateBullet();

//刷新弹幕状态函数
void UpdateBarrage();

// 刷新小兵状态函数
void UpdateBatman();

//刷新Boss状态函数
void UpdateBoss();

// 刷新主角状态函数
void UpdateHero();

// 刷新飞弹状态函数
void UpdateRocket();

// 计算主角的当前朝向
double GetHeroDir(int dirX, int dirY);

//计算子弹(弹幕)当前朝向
double GetBulletDir(double dirX, double dirY);

//TODO: 添加游戏需要的更多函数

#pragma endregion


#pragma region 碰撞检测函数

// 计算两点之间的距离  
float DistanceBetweenTwoPoints(float x1, float y1, float x2, float y2);

//圆与圆碰撞检测
bool CheckEllipse(int x1, int y1, int x2, int y2, double r1, double r2);

//圆与平行四边形碰撞检测
bool CheckRec_Ellipse(int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4, double x, double y, double r);

// 圆与矩形碰撞检测  
// 圆心(x, y), 半径r, 矩形中心(x0, y0), 矩形上边中心(x1, y1), 矩形右边中心(x2, y2)  
bool IsCircleIntersectRectangle(float x, float y, float r, float x0, float y0, float x1, float y1, float x2, float y2);

//子弹(弹幕)与砖块碰撞检测
bool CheckBullage_Recta(Bullage *iter, int* i, int* j);

double Distance(int x1, int y1, int x2, int y2);

//主角与砖块碰撞检测
void CheckHero_Recta(Hero* a_Hero);

// 预判小兵与矩形区域碰撞检测
bool CheckBatman_Recta(Recta recta[RECT_NUM_Y][RECT_NUM_X], int x, int y, int size);

#pragma endregion


#pragma region 绘图函数声明

// 绘图函数
void Paint(HWND hWnd);

#pragma endregion


#pragma region 计时器函数

//主计时/计算线程
DWORD WINAPI Timer(LPVOID lpParameter);

#pragma endregion


#pragma region 发射弹幕函数
//循环间隔n*10毫秒发射弹幕(总操作数为m,弹幕类型依次组成数组cl[])函数
void loop(int x, int y, int n, int m, int cl[]);

//发射弹幕函数
void shootbarrage1(int cl);

void shootbarrage2(int x0, int y0, int x, int y, int cl);

void shootbarrage3(int cl);

void shootbarrage4(int cl);

void shootbarrage5(int cl);

#pragma endregion


#pragma region 路径规划函数
bool routePlanning(Recta recta[RECT_NUM_Y][RECT_NUM_X], int x0, int y0, int x1, int y1, float* dirX, float *dirY);

bool IsRouteAvailable(Recta recta[RECT_NUM_Y][RECT_NUM_X], int x0, int y0, int x1, int y1, int size, float speed);

#pragma endregion

#pragma region 高精度计时器

class MyTimer
{
private:
	int _freq;
	LARGE_INTEGER _begin;
	LARGE_INTEGER _end;

public:
	long costTime;            // 花费的时间(精确到微秒)  

public:
	MyTimer()
	{
		LARGE_INTEGER tmp;
		QueryPerformanceFrequency(&tmp);//QueryPerformanceFrequency()作用：返回硬件支持的高精度计数器的频率。  

		_freq = tmp.QuadPart;
		costTime = 0;
	}

	void Start()            // 开始计时  
	{
		QueryPerformanceCounter(&_begin);//获得初始值  
	}

	void End()                // 结束计时  
	{
		QueryPerformanceCounter(&_end);//获得终止值  
		costTime = (long)((_end.QuadPart - _begin.QuadPart) * 1000000 / _freq);
	}

	void Reset()            // 计时清0  
	{
		costTime = 0;
	}
};

#pragma endregion