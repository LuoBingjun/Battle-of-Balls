#include "HackingGame.h"
#pragma region 碰撞检测函数

// 计算两点之间的距离
double Distance(int x1, int y1, int x2, int y2)
{
	return sqrt(pow(x1 - x2, 2) + pow(y1 - y2, 2));
}

// 计算两点之间的距离  
float DistanceBetweenTwoPoints(float x1, float y1, float x2, float y2)
{
	return sqrt((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1));
}

// 计算点(x, y)到经过两点(x1, y1)和(x2, y2)的直线的距离  
float DistanceFromPointToLine(float x, float y, float x1, float y1, float x2, float y2)
{
	float a = y2 - y1;
	float b = x1 - x2;
	float c = x2 * y1 - x1 * y2;

	/*assert(fabs(a) > 0.00001f || fabs(b) > 0.00001f);*/

	return fabs(a * x + b * y + c) / sqrt(a * a + b * b);
}

// 圆与矩形碰撞检测  
// 圆心(x, y), 半径r, 矩形中心(x0, y0), 矩形上边中心(x1, y1), 矩形右边中心(x2, y2)  
bool IsCircleIntersectRectangle(float x, float y, float r, float x0, float y0, float x1, float y1, float x2, float y2)
{
	float w1 = DistanceBetweenTwoPoints(x0, y0, x2, y2);
	float h1 = DistanceBetweenTwoPoints(x0, y0, x1, y1);
	float w2 = DistanceFromPointToLine(x, y, x0, y0, x1, y1);
	float h2 = DistanceFromPointToLine(x, y, x0, y0, x2, y2);

	if (w2 > w1 + r)
		return false;
	if (h2 > h1 + r)
		return false;

	if (w2 <= w1)
		return true;
	if (h2 <= h1)
		return true;

	return (w2 - w1) * (w2 - w1) + (h2 - h1) * (h2 - h1) <= r * r;
}

//圆与圆碰撞检测
bool CheckEllipse(int x1, int y1, int x2, int y2, double r1, double r2)
{
	if (Distance(x1, y1, x2, y2) <= r1 + r2)
		return true;
	else
		return false;
}

//圆与矩形(or平行四边形)碰撞检测
bool CheckRec_Ellipse(int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4, double x, double y, double r)
{
	if (fabs(x1 - x) > r&&fabs(x2 - x) > r&&
		fabs(x3 - x) > r&&fabs(x4 - x) > r&&
		fabs(y1 - y) > r&&fabs(y2 - y) > r&&
		fabs(y3 - y) > r&&fabs(y4 - y) > r)
		return false;
	if (fabs((x1 - x)*(y2 - y) - (x2 - x)*(y1 - y)) / Distance(x1, y1, x2, y2) > r&&
		fabs((x2 - x)*(y3 - y) - (x3 - x)*(y2 - y)) / Distance(x2, y2, x3, y3)> r&&
		fabs((x3 - x)*(y4 - y) - (x4 - x)*(y3 - y)) / Distance(x3, y3, x4, y4)> r&&
		fabs((x4 - x)*(y1 - y) - (x1 - x)*(y4 - y)) / Distance(x4, y4, x1, y1)> r)
		return false;
	else if (Distance(x1, y1, x, y) > r&&Distance(x2, y2, x, y) > r&& Distance(x3, y3, x, y) > r&&Distance(x4, y4, x, y) > r)
		return false;
	return true;
}

// 预判小兵与矩形区域碰撞检测
bool CheckBatman_Recta(Recta recta[RECT_NUM_Y][RECT_NUM_X], int x, int y, int size)
{
	if ((x >= BEGIN_X + RECT_SIZE_X*RECT_NUM_X - size) || (x <= BEGIN_X + size) || (y >= BEGIN_Y + RECT_SIZE_Y*RECT_NUM_Y - size) || (y <= BEGIN_Y + size))
		return false;
	int a, b;
	a = (x - BEGIN_X) / RECT_SIZE_X;
	b = (y - BEGIN_Y) / RECT_SIZE_Y;
	for (int k = a - 1; k <= a + 1; k++)
		for (int l = b - 1; l <= b + 1; l++)
		{
			if (recta[l][k].health == 0)
				continue;
			if (IsCircleIntersectRectangle(x, y, size, recta[l][k].x + RECT_SIZE_X/2, recta[l][k].y + RECT_SIZE_Y/2, recta[l][k].x + RECT_SIZE_X/2, recta[l][k].y, recta[l][k].x + RECT_SIZE_X, recta[l][k].y + RECT_SIZE_Y / 2))
				return false;
		}
	return true;
}

#pragma endregion

#pragma region 路径规划函数

//主路径规划函数
bool routePlanning(Recta recta[RECT_NUM_Y][RECT_NUM_X], int x0, int y0, int x1, int y1, float* dirX, float *dirY)
{
	float length;
	length = sqrt(pow(x1 - x0, 2) + pow(y1 - y0, 2));
	if (fabs(length) < 0.01f)
		return false;
	if (IsRouteAvailable(recta, x0, y0, x1, y1, BATMAN_SIZE,BATMAN_SPEED))
	{
		*dirX = (x1 - x0) / length;
		*dirY = (y1 - y0) / length;
		return true;
	}

	return false;
}

//
bool IsRouteAvailable(Recta recta[RECT_NUM_Y][RECT_NUM_X], int x0, int y0, int x1, int y1, int size, float speed)
{
	double length;
	length = sqrt(pow(x1 - x0, 2) + pow(y1 - y0, 2));
	double dirX = (x1 - x0) / length;
	double dirY = (y1 - y0) / length;
	float x, y;
	x = x0;
	y = y0;
	int n = length / speed;
	for (int i = 0; i <= n; i++)
	{
		if (!CheckBatman_Recta(recta, x, y, size))
			return false;
		x += dirX;
		y += dirY;
	}
	return true;
}
#pragma endregion
