// HackingGame.cpp : ����Ӧ�ó������ڵ㡣
//
#define GDIPVER 0x0110          //����߰汾��GDI+(1.1)  
#include "HackingGame.h"
#include <imm.h>
#include <Gdiplus.h>
#include <Gdipluseffects.h>
#include <mmsystem.h> 

#pragma  comment(lib,"ws2_32.lib")
#pragma comment (lib ,"imm32.lib")
#pragma comment (lib, "Gdiplus.lib")
#pragma comment (lib,"WinMM.Lib")
using namespace std;
using namespace Gdiplus;


#pragma region ȫ�ֱ���

#define MAX_LOADSTRING			100		

// ȫ�ֱ���: 
HINSTANCE hInst;								// ��ǰ����ʵ��
HWND hwd;										//��ǰ���ھ��
WCHAR szTitle[MAX_LOADSTRING];					// �������ı�
WCHAR szWindowClass[MAX_LOADSTRING];			// ����������
unsigned int timer = 0;							//��ʱ��
double fps;										//֡����
bool flag = 1;									//ȫ�ּ������
struct Recta recta[RECT_NUM_Y][RECT_NUM_X];		//��������
int shoot = 0;									//�ӵ�״̬
MyTimer mt;										//�߾��ȼ�ʱ��
int load = 0;									//������ʱ��
bool cure;										//������
int CD = 1;										//��������ȴʱ��


// TODO: �ڴ��������ȫ�ֱ���

//ͼ����Դ
Image *BossProtect;		//Boss������
Image *cyanbullet;		//��ͨ�ӵ�
Image *orangebullet;	//�����ӵ�
Image *rocket;			//�ɵ�	
Image* hero;			//����
Image *btmn;			//С��
Image* brickf2;			//ש��
HBITMAP brick1; 
HBITMAP brickf1; 

GdiplusStartupInput    m_Gdistart;
ULONG_PTR    m_GdiplusToken;

Stage* currentStage = NULL;		//��ǰ����״̬
Stage* customMode = NULL;		//�Զ���ģʽ�龰
Hero* theHero = NULL;			//����״̬
Boss* theBoss = NULL;			//��Boss״̬
vector<Button*> buttons;		//��ť
vector<Bullage*> bullets;		//�ӵ�
vector<Bullage*> barrages;		//��Ļ
vector<Batman*> batmans;		//С��
Batman* theRocket = NULL;		//�ɵ�

int mouseX = 0;
int mouseY = 0;
bool mouseDown = false;
bool keyUpDown = false;
bool keyDownDown = false;
bool keyLeftDown = false;
bool keyRightDown = false;


double const PI = acos(double(-1));
#pragma endregion


#pragma region Win32������



// �˴���ģ���а����ĺ�����ǰ������: 
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);




int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // ��ʼ��ȫ���ַ���
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_HACKINGGAME, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // ִ��Ӧ�ó����ʼ��: 
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_HACKINGGAME));

	//�����������뷨:
	HIMC g_hIMC = NULL;//g_hIMC ���ڻָ�ʱʹ��  
	g_hIMC = ImmAssociateContext(hwd, NULL);//�����������뷨

    MSG msg;

    // ����Ϣѭ��: 
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

	//�����������뷨
	ImmAssociateContext(hwd, g_hIMC);

    return (int) msg.wParam;
}



//
//  ����: MyRegisterClass()
//
//  Ŀ��: ע�ᴰ���ࡣ
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON1));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_HACKINGGAME);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_ICON1));

    return RegisterClassExW(&wcex);
}

//
//   ����: InitInstance(HINSTANCE, int)
//
//   Ŀ��: ����ʵ�����������������
//
//   ע��: 
//
//        �ڴ˺����У�������ȫ�ֱ����б���ʵ�������
//        ��������ʾ�����򴰿ڡ�
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // ��ʵ������洢��ȫ�ֱ�����

   HWND hWnd = CreateWindow(szWindowClass, szTitle,
	   WS_CLIPCHILDREN | WS_CLIPSIBLINGS | WS_POPUP, // ���ô�����ʽ�����ɸı��С���������,
	   CW_USEDEFAULT, 
	   CW_USEDEFAULT,
	   WINDOW_WIDTH,
	   WINDOW_HEIGHT,
	   nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);
   hwd = hWnd;
   return TRUE;
}

//
//  ����: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  Ŀ��:    ���������ڵ���Ϣ��
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
	case WM_CREATE:
		//���ھ���
		int scrWidth, scrHeight;
		RECT rect;
		//�����Ļ�ߴ�
		scrWidth = GetSystemMetrics(SM_CXSCREEN);
		scrHeight = GetSystemMetrics(SM_CYSCREEN);
		//��ȡ����ߴ�
		GetWindowRect(hWnd, &rect);
		rect.left = (scrWidth - rect.right) / 2;
		rect.top = (scrHeight - rect.bottom) / 2;
		//���ô���λ��
		SetWindowPos(hWnd, HWND_TOP, rect.left, rect.top, rect.right, rect.bottom, SWP_SHOWWINDOW);

		//��ʼ��GDI+
		GdiplusStartup(&m_GdiplusToken, &m_Gdistart, NULL);

		//MCI_OPEN_PARMS OpenParms; //���豸  
		//OpenParms.lpstrDeviceType = _T("sequencer"); // "mpegvideo";  
		//OpenParms.lpstrElementName = _T("./res/bgm/main.mp3");
		//OpenParms.wDeviceID = 1;
		//mciSendCommand(NULL, MCI_OPEN, MCI_OPEN_ELEMENT, (DWORD)&OpenParms);

		//MCI_PLAY_PARMS PlayParms; //�����ļ�  
		//PlayParms.dwFrom = 0;
		//mciSendCommand(OpenParms.wDeviceID, MCI_PLAY, MCI_NOTIFY, (DWORD)&PlayParms);

		// ��ʼ����Ϸ����
		InitStage(hWnd, -1);

		mciSendString(_T("open ./res/bgm/main.mp3 type MPEGVideo Alias main"), NULL, 0, NULL);
		mciSendString(_T("play main repeat"), NULL, 0, NULL);

		//��ʼ������ʱ��
		CreateThread(NULL, 0, Timer, NULL, 0, NULL);					//�������̣߳��̵߳���Timer����������ʱ/�����̣߳�

		//��Ӱ�ť
		{
			Button* startButton = CreateButton(1, L"./res/buttonplay.png", L"./res/buttonplayfocused.png", 442, 102, 326, 458);
			buttons.push_back(startButton);
			startButton = CreateButton(2, L"./res/buttonplay2.png", L"./res/buttonplay2focused.png", 125, 102, 793, 458);
			buttons.push_back(startButton);
			Button* back0 = CreateButton(3, L"./res/back.png", L"./res/back.png", 80, 80, 1180, 620);
			buttons.push_back(back0);
			Button* Skip = CreateButton(-1, L"./res/skip.png", L"./res/skip.png", 205, 204, 1053, 28);
			buttons.push_back(Skip);
			Button* pause = CreateButton(1001, L"./res/pause.png", L"./res/pause.png", 82, 88, 20, 10);
			buttons.push_back(pause);
			Button* restart = CreateButton(1002, L"./res/restart.png", L"./res/restart.png", 106, 106, 352, 482);
			buttons.push_back(restart);
			Button* home = CreateButton(1003, L"./res/home.png", L"./res/home.png", 106, 106, 587, 482);
			buttons.push_back(home);
			Button* next = CreateButton(1004, L"./res/next.png", L"./res/next.png", 106, 106, 822, 482);
			buttons.push_back(next);

			Button* restart1 = CreateButton(1005, L"./res/restart.png", L"./res/restart.png", 106, 106, 352, 344);
			buttons.push_back(restart1);
			Button* home1 = CreateButton(1006, L"./res/home.png", L"./res/home.png", 106, 106, 587, 344);
			buttons.push_back(home1);
			Button* next1 = CreateButton(1007, L"./res/next.png", L"./res/next.png", 106, 106, 822, 344);
			buttons.push_back(next1);

			Button* plus1 = CreateButton(2001, L"./res/plus.png", L"./res/plus.png", 36, 36, 918, 162);
			buttons.push_back(plus1);
			Button* minus1 = CreateButton(2002, L"./res/minus.png", L"./res/minus.png",36, 36, 758, 162);
			buttons.push_back(minus1);
			Button* plus2 = CreateButton(2003, L"./res/plus.png", L"./res/plus.png", 36, 36, 918, 242);
			buttons.push_back(plus2);
			Button* minus2 = CreateButton(2004, L"./res/minus.png", L"./res/minus.png", 36, 36, 758, 242);
			buttons.push_back(minus2);
			Button* plus3 = CreateButton(2005, L"./res/plus.png", L"./res/plus.png", 36, 36, 918, 322);
			buttons.push_back(plus3);
			Button* minus3 = CreateButton(2006, L"./res/minus.png", L"./res/minus.png", 36, 36, 758, 322);
			buttons.push_back(minus3);
			Button* plus4 = CreateButton(2007, L"./res/plus.png", L"./res/plus.png", 36, 36, 776, 396);
			buttons.push_back(plus4);
			Button* minus4 = CreateButton(2008, L"./res/minus.png", L"./res/minus.png", 36, 36, 616, 396);
			buttons.push_back(minus4);
			Button* plus5 = CreateButton(2009, L"./res/plus.png", L"./res/plus.png", 36, 36, 776, 461);
			buttons.push_back(plus5);
			Button* minus5 = CreateButton(2010, L"./res/minus.png", L"./res/minus.png", 36, 36, 616, 461);
			buttons.push_back(minus5);
			Button* plus6 = CreateButton(2011, L"./res/plus.png", L"./res/plus.png", 36, 36, 776, 522);
			buttons.push_back(plus6);
			Button* minus6 = CreateButton(2012, L"./res/minus.png", L"./res/minus.png", 36, 36, 616, 522);
			buttons.push_back(minus6);
			Button* play = CreateButton(2013, L"./res/custommodeplay.png", L"./res/custommodeplayfocused.png", 300, 70, 262, 582);
			buttons.push_back(play);
			Button* back = CreateButton(2014, L"./res/custommodeback.png", L"./res/custommodebackfocused.png", 300, 70, 729, 582);
			buttons.push_back(back);
			back = CreateButton(2015, L"./res/back.png", L"./res/back.png", 106, 106, 822, 482);
			buttons.push_back(back);
		}


		//��ʼ��ͼ����Դ
		BossProtect = new Image(L"./res/BossProtect.png");		//Boss������
		hero = new Image(L"./res/Hero1.png");					//����
		btmn = new Image(L"./res/batman.png");					//С��
		brickf2 = new Image(L"./res/brickf2.png");
		cyanbullet = new Image(L"./res/cyan_bullet.png");		//��ͨ�ӵ�
		orangebullet = new Image(L"./res/orange_bullet.png");	//�����ӵ�
		rocket = new Image(L"./res/rocket.png");				//�ɵ�
		brick1= LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance, MAKEINTRESOURCE(IDB_BITMAP1));
		brickf1= LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance, MAKEINTRESOURCE(IDB_BITMAP3));
		break;
	case WM_KEYDOWN:
		// ���̰����¼�
		KeyDown(hWnd, wParam, lParam);
		break;
	case WM_KEYUP:
		// �����ɿ��¼�
		KeyUp(hWnd, wParam, lParam);
		break;
	case WM_MOUSEMOVE:
		// ����ƶ��¼�
		MouseMove(hWnd, wParam, lParam);
		break;
	case WM_LBUTTONDOWN:
		// �����������¼�
		LButtonDown(hWnd, wParam, lParam);
		break;
	case WM_LBUTTONUP:
		// �������ɿ��¼�
		LButtonUp(hWnd, wParam, lParam);
		break;
	case WM_TIMER:
		// ��ʱ���¼�
		if (currentStage != NULL && currentStage->timerOn) TimerUpdate(hWnd, wParam, lParam);
		break;
    case WM_PAINT:
		mt.End();
		fps = 1000000.0 / mt.costTime;
		mt.Start();
		// ��ͼ
		Paint(hWnd);
        break;
    case WM_DESTROY:
		//ж��GDI+
		GdiplusShutdown(m_GdiplusToken);
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}


#pragma endregion


#pragma region �¼�������

// ��ʼ����Ϸ���庯��
void InitGame(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	////����ͼ����Դ
	//bmp_Background = LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance, MAKEINTRESOURCE(IDB_BITMAP_BG));
	//bmp_StartButton = LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance, MAKEINTRESOURCE(IDB_BITMAP_START));

}

// ��ʼ����Ϸ��������
void InitStage(HWND hWnd, int stageID)
{
	// ��ʼ������ʵ��
	if (currentStage != NULL) delete currentStage;
	currentStage = new Stage();
	currentStage->stageID = stageID;
	currentStage->score = 0;
	currentStage->loading = false;
	currentStage->timerOn = false;
	currentStage->bg = NULL;

	if (stageID <= -2)
	{
		switch (stageID)
		{
		case -2:
		{
			for (int i = 0; i < buttons.size(); i++)
				if(buttons[i]->buttonID==-1)
					buttons[i]->visible = true;
				else
					buttons[i]->visible = false;
		}
			currentStage->bg = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP4));
			break;
		case -3:
			for (int i = 0; i < buttons.size(); i++)
				buttons[i]->visible = false;
			currentStage->bg = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP5));
			break;
		case -4:
			currentStage->bg = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP6));
			break;
		case -5:
			currentStage->bg = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP7));
			break;
		case -6:
			currentStage->bg = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP8));
			break;
		case -7:
			currentStage->bg = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP9));
			break;
		case -8:
			currentStage->bg = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP10));
			break;
		case -9:
			for (int i = 0; i < buttons.size(); i++)
				buttons[i]->visible = false;
			currentStage->bg = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP11));
			break;
		case -10:
			for (int i = 0; i < buttons.size(); i++)
				buttons[i]->visible = false;
			currentStage->bg = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP13));
			break;
		}
	}
	else if (stageID == -1)
	{
	}
	else if (stageID == 0)					//��ʼ����
	{
		//��ʾ��ʼ����İ�ť
		for (int i = 0; i<buttons.size(); i++)
		{
			Button* button = buttons[i];
			if (button->buttonID >= 1 && button->buttonID <= 3)
			{
				button->visible = true;
			}
			else
			{
				button->visible = false;
			}
		}
	}
	else if (stageID == 1999)
	{
		for (int i = 0; i < buttons.size(); i++)
			buttons[i]->visible = false;
		currentStage->bg = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP15));
	}
	else if (stageID == 2000)
	{
		if (customMode == NULL)
		{
			customMode = new Stage();
			customMode->batman = 5;
			customMode->callbatman = 300;
			customMode->numbarrage = 8;
			customMode->rec[0] = 12;
			customMode->rec[1] = 12;
			customMode->rec[2] = 12;
			customMode->timebarrage = 30;
		}
		for (int i = 0; i < buttons.size(); i++)
		{
			Button* button = buttons[i];
			if(button->buttonID>=2001&& button->buttonID<=2014)
				button->visible = true;
			else
				button->visible = false;
		}
		currentStage->bg = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP16));
	}
	else if (stageID >= 1001)
	{
		if (stageID == 2001)
		{
			currentStage->bg = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP2));
			currentStage->batman = customMode->batman;
			currentStage->callbatman = customMode->callbatman;
			currentStage->rec[0] = customMode->rec[0];
			currentStage->rec[1] = customMode->rec[1];
			currentStage->rec[2] = customMode->rec[2];
			currentStage->timebarrage = customMode->timebarrage;
			currentStage->numbarrage = 5;
			currentStage->barrage[0] = 1;
			currentStage->barrage[1] = 4;
			currentStage->barrage[2] = 5;
			currentStage->barrage[3] = 8;
			currentStage->barrage[4] = 9;
		}
		else
		{
			//��settings.ini�ж�ȡ������Ϣ
			int Resource, n1, n2, n3, numbatman, numbarrage;
			char stage[16], a[200];
			sprintf(stage, "#Stage=%d", stageID);
			freopen("settings.ini", "r", stdin);
			char c;
			while (1)
			{
				scanf("%[^'\n']", a);
				getchar();
				if (strcmp(a, "#end") == 0)
				{
					InitStage(hWnd, 1999);
					return;
				}
				if (strcmp(a, stage) == 0)
				{
					getchar();
					if ((c = getchar()) == '/')
					{
						while ((c = getchar()) != '\n')
							;
					}
					scanf("%d", &Resource);
					getchar();
					if ((c = getchar()) == '/')
					{
						while ((c = getchar()) != '\n')
							;
					}
					scanf("%d%d%d", &(currentStage->rec)[0], &(currentStage->rec)[1], &(currentStage->rec)[2]);
					getchar();
					if ((c = getchar()) == '/')
					{
						while ((c = getchar()) != '\n')
							;
					}
					scanf("%d", &(currentStage->batman));
					getchar();
					if ((c = getchar()) == '/')
					{
						while ((c = getchar()) != '\n')
							;
					}
					scanf("%d", &(currentStage->numbarrage));
					getchar();
					if ((c = getchar()) == '/')
					{
						while ((c = getchar()) != '\n')
							;
					}
					for (int i = 0; i <= currentStage->numbarrage - 1; i++)
						scanf("%d", &(currentStage->barrage)[i]);
					getchar();
					if ((c = getchar()) == '/')
					{
						while ((c = getchar()) != '\n')
							;
					}
					scanf("%d", &currentStage->timebarrage);
					getchar();
					if ((c = getchar()) == '/')
					{
						while ((c = getchar()) != '\n')
							;
					}
					scanf("%d", &currentStage->callbatman);
					break;
				}
			}
			currentStage->bg = LoadBitmap(hInst, MAKEINTRESOURCE(Resource));
		}
		
		currentStage->loading = true;
		Paint(hWnd);
		currentStage->win = 0;

		//�������������
		if (stageID >= 1007)
		{
			KillTimer(hWnd, 1);
			cure = false;
			SetTimer(hWnd, 1, 1000, NULL);
			CD = 1;
		}
			

		//��ʾ��Ϸ����İ�ť
		for (int i = 0; i < buttons.size(); i++)
		{
			Button* button = buttons[i];
			if (button->buttonID == 1001) //TODO��������Ϸ������Ҫ�İ�ť
			{
				button->visible = true;
			}
			else
			{
				button->visible = false;
			}
		}


		//��ʼ����������
		InitRecta(currentStage->rec[0], currentStage->rec[1], currentStage->rec[2]);

		// ��ʼ������
		if (theHero != NULL) delete theHero;
		int xH, yH;
		xH = 10;
		yH = 9;
		while (recta[yH][xH].health != 0)
		{
			if (xH > 0)
				xH--;
			else
			{
				xH = RECT_NUM_X / 2;
				yH++;
			}
		}
		theHero = CreateHero(1, recta[yH][xH].x + RECT_SIZE_X / 2, recta[yH][xH].y + RECT_SIZE_Y / 2, HERO_HEALTH);

		//��ʼ����Boss
		if (theBoss != NULL) delete theBoss;
		while (1)
		{
			int xB, yB;
			srand((unsigned)time(NULL));
			xB = rand() % (21 - 10 + 1) + 10;
			srand((unsigned)time(NULL));
			yB = rand() % (3 - 0 + 1) + 0;
			if (recta[yB][xB].health == 0)
			{
				theBoss = CreateBoss(recta[yB][xB].x + RECT_SIZE_X / 2, recta[yB][xB].y + RECT_SIZE_Y / 2, BOSS_HEALTH);
				break;
			}
		}

		//��ʼ��С��
		vector<struct Batman*>::iterator iter = batmans.begin();
		while (iter != batmans.end())
		{
			delete *iter;
			*iter = NULL;
			iter = batmans.erase(iter);
		}
		int i = 1;
		srand((unsigned)time(NULL));
		while (i <= currentStage->batman)
		{
			int xB, yB;
			xB = rand() % (RECT_NUM_X - 1 - 0 + 1) + 0;
			yB = rand() % (RECT_NUM_Y - 1 - 0 + 1) + 0;
			if (!(recta[yB][xB].health == 0 &&
				abs(recta[yB][xB].x + RECT_SIZE_X / 2 - theBoss->x) >= SAFE_DISTANCE&&abs(recta[yB][xB].y + RECT_SIZE_Y / 2 - theBoss->y) >= SAFE_DISTANCE&&
				abs(recta[yB][xB].x + RECT_SIZE_X / 2 - theHero->x) >= SAFE_DISTANCE&&abs(recta[yB][xB].y + RECT_SIZE_Y / 2 - theHero->y) >= SAFE_DISTANCE))
				continue;
			for (int j = 0; j < batmans.size(); j++)
			{
				Batman* iter = batmans[j];
				if ((int)(iter->x) == recta[yB][xB].x + RECT_SIZE_X / 2 && (int)(iter->y) == recta[yB][xB].y + RECT_SIZE_Y / 2)
					continue;
			}
			batmans.push_back(CreateBatman(recta[yB][xB].x + RECT_SIZE_X / 2, recta[yB][xB].y + RECT_SIZE_Y / 2));
			i++;
		}

		//��յ�Ļ
		vector<struct Bullage*>::iterator iter1 = barrages.begin();
		while (iter1 != barrages.end())
		{
			delete *iter1;
			*iter1 = NULL;
			iter1 = barrages.erase(iter1);
		}

		//����ӵ�
		vector<struct Bullage*>::iterator iter2 = bullets.begin();
		while (iter2 != bullets.end())
		{
			delete *iter2;
			*iter2 = NULL;
			iter2 = bullets.erase(iter2);
		}

		//����Ƿ�ɹ���ʼ��
		if (theBoss == NULL || theHero == NULL)
			InitStage(hWnd, stageID);

		currentStage->loading = false;
		currentStage->timerOn = true;
	}

	//ˢ����ʾ
	InvalidateRect(hWnd, NULL, FALSE);
}

// ���̰����¼�������
void KeyDown(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	switch (wParam)
	{
	case 'W':
		keyUpDown = true;
		break;
	case 'S':
		keyDownDown = true;
		break;
	case 'A':
		keyLeftDown = true;
		break;
	case 'D':
		keyRightDown = true;
		break;
	case VK_ESCAPE:
		currentStage->timerOn = false;
		for (int i = 0; i < buttons.size(); i++)
		{
			Button* buntton = buttons[i];
			if (buntton->buttonID == 1005 || buntton->buttonID == 1006 || buntton->buttonID == 1007)
				buntton->visible = true;
			else
				buntton->visible = false;
		}
		break;
	case 'E':
		if (cure == true)
		{
			theHero->health = HERO_HEALTH;
			cure = false;
			CD = 1;
			SetTimer(hWnd, 1, 1000, NULL);
		}
		break;
	default:
		break;
	}
}

// �����ɿ��¼�������
void KeyUp(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	// TODO
	switch (wParam)
	{
	case 'W':
		keyUpDown = false;
		break;
	case 'S':
		keyDownDown = false;
		break;
	case 'A':
		keyLeftDown = false;
		break;
	case 'D':
		keyRightDown = false;
		break;
	default:
		break;
	}
}

// ����ƶ��¼�������
void MouseMove(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	mouseX = LOWORD(lParam);
	mouseY = HIWORD(lParam);

	for (int i = 0; i < buttons.size(); i++)//ͬһλ�õİ�ť�����ڷ����ӵ�(��Ļ)
	{
		Button* button = buttons[i];
		if (button->visible)
		{
			if (button->x <= mouseX
				&& button->x + button->width >= mouseX
				&& button->y <= mouseY
				&& button->y + button->height >= mouseY)
				button->focused = true;
			else
			{
				button->click = 0;
				button->focused = false;
			}
		}
	}
}

// �����������¼�������
void LButtonDown(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	int process = 0;
	mouseX = LOWORD(lParam);
	mouseY = HIWORD(lParam);
	mouseDown = true;
	for (int i = 0; i<buttons.size(); i++)//ͬһλ�õİ�ť�����ڷ����ӵ�(��Ļ)
	{
		Button* button = buttons[i];
		if (button->visible)
		{
			if (button->x <= mouseX
				&& button->x + button->width >= mouseX
				&& button->y <= mouseY
				&& button->y + button->height >= mouseY)
			{
				process = 1;
				button->click = 1;
			}
		}
	}
	if (process == 0)			//û�а�ť���ʱ�Ķ���
	{
		if (currentStage->stageID <= -2 && currentStage->stageID >= -7)
		{
			InitStage(hwd, currentStage->stageID - 1);
		}
		else if (currentStage->stageID == -8)
		{
			InitStage(hwd, 0);
		}
		else if (currentStage->stageID == -9)
		{
			InitStage(hwd, 1007);
		}
		else if (currentStage->stageID == -10)
		{
			InitStage(hwd, 1011);
		}
		else if (currentStage->stageID == 1999)
		{
			InitStage(hwd, 0);
		}
		else if(currentStage->stageID>=1)
			shoot = 1;
	}
	else
		shoot = 0;
	
}

// �������ɿ��¼�������
void LButtonUp(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	mouseX = LOWORD(lParam);
	mouseY = HIWORD(lParam);
	mouseDown = false;
	shoot = 0;
	for (int i = 0; i < buttons.size(); i++)//ͬһλ�õİ�ť�����ڷ����ӵ�(��Ļ)
	{
		Button* button = buttons[i];
		if (button->click&&button->focused)
		{
			button->click = 0;
			Paint(hwd);
			switch (button->buttonID) 
			{
			case 1:
				InitStage(hWnd, 1001);
				break;
			case 2:
				InitStage(hWnd, 2000);
				break;
			case 3:
				PostQuitMessage(0);
				break;
			case -1:
				InitStage(hWnd, 0);
				break;
			case 1001:
				currentStage->timerOn = false;
				for (int i = 0; i < buttons.size(); i++)
				{
					Button* buntton = buttons[i];
					if (buntton->buttonID == 1005 || buntton->buttonID == 1006 || buntton->buttonID == 1007)
						buntton->visible = true;
					else
						buntton->visible = false;
				}
				break;
			case 1002:
				InitStage(hWnd,currentStage->stageID);
				break;
			case 1003:
				InitStage(hWnd,0);
				break;
			case 1004:
				if (currentStage->stageID == 1006)
					InitStage(hWnd, -9);
				else if(currentStage->stageID==1010)
					InitStage(hWnd, -10);
				else
					InitStage(hWnd, currentStage->stageID + 1);
				break;
			case 1005:
				InitStage(hWnd, currentStage->stageID);
				break;
			case 1006:
				InitStage(hWnd, 0);
				break;
			case 1007:
				currentStage->timerOn = true;
				for (int i = 0; i < buttons.size(); i++)
				{
					Button* buntton = buttons[i];
					if (buntton->buttonID == 1001)
						buntton->visible = true;
					else
						buntton->visible = false;
				}
				break;
			case 2001:
				if (customMode->rec[0] < 30)
					customMode->rec[0]++;
				break;
			case 2002:
				if (customMode->rec[0] > 0)
					customMode->rec[0]--;
				break;
			case 2003:
				if (customMode->rec[1] < 30)
					customMode->rec[1]++;
				break;
			case 2004:
				if (customMode->rec[1] > 0)
					customMode->rec[1]--;
				break;
			case 2005:
				if (customMode->rec[2] < 30)
					customMode->rec[2]++;
				break;
			case 2006:
				if (customMode->rec[2] > 0)
					customMode->rec[2]--;
				break;
			case 2007:
				if (customMode->batman < 15)
					customMode->batman++;
				break;
			case 2008:
				if (customMode->batman > 0)
					customMode->batman--;
				break;
			case 2009:
				if (customMode->callbatman < 1500)
					customMode->callbatman += 100;
				break;
			case 2010:
				if (customMode->callbatman > 0)
					customMode->callbatman -= 100;
				break;
			case 2011:
				if (customMode->timebarrage < 95)
					customMode->timebarrage += 5;
				break;
			case 2012:
				if (customMode->timebarrage > 0)
					customMode->timebarrage -= 5;
				break;
			case 2013:
				InitStage(hWnd, 2001);
				break;
			case 2014:
				InitStage(hWnd, 0);
				break;
			case 2015:
				InitStage(hWnd, 2000);
				break;
			}
		}
		button->click = 0;
	}
}

// ��ʱ���¼�������
void TimerUpdate(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	switch (wParam)
	{
	case 1:
	{
		if (CD <= 10)
		{
			cure = false;
			CD++;
		}
		else
		{
			cure = true;
			KillTimer(hWnd, 1);
		}
		break;
	}
	}
}


#pragma endregion


#pragma region �����ϷԪ�غ���



//TODO: �����Ϸ��Ҫ�ĸ��ຯ��

//���Boss����
Boss* CreateBoss(int x, int y, int health)
{
	Boss* a_Boss = new Boss;
	a_Boss->x = x;
	a_Boss->y = y;
	a_Boss->vx = a_Boss->vy = 0;
	a_Boss->health = health;
	return a_Boss;
}

//����ӵ�����
Bullage* CreateBullet(double dirX, double dirY, int x, int y, int cl)
{
	Bullage* bullet = new Bullage();
	bullet->Class = cl;
	bullet->v = BULLET_MINSPEED;	//�ӵ�(��Ļ)���ų��ٶ�
	bullet->dirX = dirX;			//�ӵ�(��Ļ)x�������
	bullet->dirY = dirY;			//�ӵ�(��Ļ)y�������
	bullet->x = x;
	bullet->y = y;
	return bullet;
}

//��ӵ�Ļ����
Bullage* CreateBarrage(int Class, double dirX, double dirY, int x, int y, float v)
{
	Bullage* barrage = new Bullage();
	barrage->Class = Class;			//��Ļ����
	barrage->v = v;					//��Ļ�ٶ�
	barrage->dirX = dirX;			//��Ļx�������
	barrage->dirY = dirY;			//��Ļy�������
	barrage->x = x;
	barrage->y = y;

	return barrage;
}

// ��Ӱ�ť����
Button* CreateButton(int buttonID, wchar_t* img, wchar_t* foucusedimg,int width, int height, int x, int y)
{
	Button* button = new Button();
	button->buttonID = buttonID;
	button->img = img;
	button->focusedimg = foucusedimg;
	button->width = width;
	button->height = height;
	button->x = x;
	button->y = y;

	button->visible = false;
	return button;
}

// ������Ǻ���
Hero* CreateHero(int cl, int x, int y, int health)
{
	Hero* hero = new Hero();
	hero->Class = cl;
	hero->x = x;
	hero->y = y;
	hero->vx = 0;
	hero->vy = 0;
	hero->health = health;
	return hero;
}

// ���С������
Batman* CreateBatman(int x, int y)
{
	Batman* batman = new Batman();
	batman->x = x;
	batman->y = y;
	return batman;
}

//��ʼ���������򣨴���ש�飩����
//n1:�ɱ������ӵ�(��Ļ)�ƻ���ש��(����ֵΪ��)
//n2:���ɱ������ӵ�(��Ļ)�ƻ���ש��(����ֵΪ-1)
//n3:����ש��(����ֵΪ-2)
void InitRecta(int n1, int n2, int n3)
{
	int n = 0;
	//�������������ʼ��
	for (int i = 0; i <= RECT_NUM_Y - 1; i++)
		for (int j = 0; j <= RECT_NUM_X - 1; j++)
		{
			recta[i][j].x = BEGIN_X + RECT_SIZE_X*j;
			recta[i][j].y = BEGIN_Y + RECT_SIZE_Y*i;
			recta[i][j].health = BRICK_HEALTH;
		}


	//���������ש������
	int n0 = RECT_NUM_X * RECT_NUM_Y - n1 - n2 - n3;		//n0Ϊ��ש��ľ���������
	n = 0;													//nΪ������
	for (int i = 1; i <= 100; i++)					//100Ϊ���ϵ��
	{
		srand((unsigned)GetTickCount());
		while (n < n0*i / 100)
		{
			int i, j;
			j = rand() % RECT_NUM_X;
			i = rand() % RECT_NUM_Y;
			if (recta[i][j].health != 0)
			{
				recta[i][j].health = 0;
				if (ifConnected())
					n++;
				else
					recta[i][j].health = BRICK_HEALTH;
			}
		}
	}

	//������ɲ��ɱ������ӵ�(��Ļ)�ƻ���ש��
	n = 0;
	srand((unsigned)GetTickCount());
	while (n < n2)
	{
		int i, j;
		j = rand() % RECT_NUM_X;
		i = rand() % RECT_NUM_Y;
		if (recta[i][j].health > 0)
		{
			recta[i][j].health = -1;
			n++;
		}
	}

	//�����������ש��
	n = 0;
	srand((unsigned)GetTickCount());
	while (n < n3)
	{
		int i, j;
		j = rand() % RECT_NUM_X;
		i = rand() % RECT_NUM_Y;
		if (recta[i][j].health > 0)
		{
			recta[i][j].health = -2;
			n++;
		}
	}

	if (!ifConnected())
		InitRecta(n1, n2, n3);
	//// ��ʼ�����·ͼ
	//for (int m = 1; m <= 200; m++)					//����240�������
	//{
	//	srand((unsigned)GetTickCount());
	//	while (1)
	//	{
	//		int i, j;
	//		i = (rand() % (RECT_NUM_X*RECT_SIZE_X));
	//		j = (rand() % (RECT_NUM_Y*RECT_SIZE_Y));
	//		int a, b;
	//		a = i / RECT_SIZE_X;
	//		b = j / RECT_SIZE_Y;
	//		if (recta[b][a].health == 0)
	//		{
	//			for (int l = 0; l < nodes.size(); l++)
	//				if (nodes[l]->x == i + BEGIN_X&&nodes[l]->y == j + BEGIN_Y)
	//					goto end;
	//			Node* a_node = (Node*)malloc(sizeof(Node));
	//			a_node->x = i + BEGIN_X;
	//			a_node->y = j + BEGIN_Y;
	//			Path *head, *p, *q;
	//			head = p = q = NULL;
	//			for (int l = 0; l < nodes.size(); l++)
	//			{
	//				Node *iter = nodes[l];
	//				if (!IsRouteAvailable(recta, a_node->x, a_node->y, iter->x, iter->y, BATMAN_SIZE, BATMAN_SPEED))
	//					continue;
	//				p = (Path*)malloc(sizeof(Path));
	//				p->dst = iter;
	//				Path *m = (Path*)malloc(sizeof(Path));
	//				m->dst = a_node;
	//				m->next = NULL;
	//				if (iter->path == NULL)
	//				{
	//					iter->path = m;
	//				}
	//				else
	//				{
	//					Path *n = iter->path;
	//					while (n->next != NULL)
	//						n = n->next;
	//					n->next = m;
	//				}
	//				if (head == NULL)
	//					head = p;
	//				else
	//					q->next = p;
	//				q = p;
	//			}
	//			if (q != NULL)
	//				q->next = NULL;
	//			a_node->path = head;
	//			nodes.push_back(a_node);
	//			break;
			//}
	//	end:;
	//	}
	//}

	// ɾȥ���õĵ�
	//vector<struct Node*>::iterator iter = nodes.begin();
	//while (iter != nodes.end())
	//{
	//	if ((*iter)->path == NULL)
	//	{
	//		free(*iter);
	//		*iter = NULL;
	//		iter = nodes.erase(iter);
	//		continue;
	//	}
	//	iter++;
	//}

}

//����Ƿ���ͨ����
bool ifConnected()
{
	for (int i = 0; i <= RECT_NUM_Y - 1; i++)
		for (int j = 0; j <= RECT_NUM_X - 1; j++)
		{
			if (recta[i][j].health != 0)
				continue;
			if (i >= 1 && j >= 1)
			{
				if (!(recta[i - 1][j].health == 0 || recta[i + 1][j].health == 0 || recta[i][j - 1].health == 0 || recta[i][j + 1].health == 0))
					return false;
			}
			else if (j >= 1 && !(recta[i + 1][j].health == 0 || recta[i][j - 1].health == 0 || recta[i][j + 1].health == 0))
				return false;
			else if (i >= 1 && !(recta[i + 1][j].health == 0 || recta[i - 1][j].health == 0 || recta[i][j + 1].health == 0))
				return false;
			else if (!(recta[i + 1][j].health == 0 || recta[i][j + 1].health == 0))
				return false;
		}
	return true;
}
#pragma endregion


#pragma region ˢ��״̬����

// ˢ������״̬����
void UpdateHero()
{
	if (theHero != NULL)
	{
		if (theHero->health <= 0)
		{
			currentStage->win = -1;
			currentStage->timerOn = false;
			if (currentStage->stageID <= 1998)
			{
				for (int i = 0; i < buttons.size(); i++)
					if (buttons[i]->buttonID >= 1002 && buttons[i]->buttonID <= 1003)
						buttons[i]->visible = true;
					else
						buttons[i]->visible = false;
			}
			else if (currentStage->stageID == 2001)
			{
				for (int i = 0; i < buttons.size(); i++)
					if (buttons[i]->buttonID == 1002 || buttons[i]->buttonID == 1003 || buttons[i]->buttonID == 2015)
						buttons[i]->visible = true;
					else
						buttons[i]->visible = false;
			}
			return;
		}

		//y�����ٶ�
		if (keyUpDown && !keyDownDown && theHero->vy > -1 * HERO_MAXSPEED)
			theHero->vy -= HERO_ACCELERATION;
		else if (!keyUpDown && keyDownDown && theHero->vy < HERO_MAXSPEED)
			theHero->vy += HERO_ACCELERATION;
		else if (theHero->vy > 0)theHero->vy -= HERO_ACCELERATION;
		else if (theHero->vy < 0)theHero->vy += HERO_ACCELERATION;

		//x�����ٶ�
		if (keyLeftDown && !keyRightDown && theHero->vx > -1 * HERO_MAXSPEED)
			theHero->vx -= HERO_ACCELERATION;
		else if (!keyLeftDown && keyRightDown && theHero->vx < HERO_MAXSPEED)
			theHero->vx += HERO_ACCELERATION;
		else if (theHero->vx > 0)
		{
			theHero->vx -= HERO_ACCELERATION;
		}
		else if (theHero->vx < 0)
		{
			theHero->vx += HERO_ACCELERATION;
		}

		//ȷ����ֹ�ٶ�
		if (fabs(theHero->vx) < HERO_ACCELERATION)
			theHero->vx = 0;
		else if (fabs(theHero->vy) < HERO_ACCELERATION)
			theHero->vy = 0;

		//���������BOSS�Ƿ���ײ
		if (CheckEllipse(theHero->x, theHero->y, theBoss->x, theBoss->y, HERO_SIZE, BOSS_SIZE))
		{
			//�����ٶȷ���Boss�ٶȲ���
			theHero->vx = -theHero->vx;
			theHero->vy = -theHero->vy;
		}

		//Ϊ��������ж��߿�,���ӷ���Ч�������ȿ��ǣ�
		if ((theHero->vx > 0) && (theHero->x >= BEGIN_X + RECT_SIZE_X*RECT_NUM_X - HERO_SIZE))
			theHero->vx = -1 * theHero->vx;
		if ((theHero->vx < 0) && (theHero->x <= BEGIN_X + HERO_SIZE))
			theHero->vx = -1 * theHero->vx;
		if ((theHero->vy > 0) && (theHero->y >= BEGIN_Y + RECT_SIZE_Y*RECT_NUM_Y - HERO_SIZE))
			theHero->vy = -1 * theHero->vy;
		if ((theHero->vy < 0) && (theHero->y <= BEGIN_Y + HERO_SIZE))
			theHero->vy = -1 * theHero->vy;

		theHero->x += (int)(theHero->vx);
		theHero->y += (int)(theHero->vy);

		//������ש����ײ���
		CheckHero_Recta(theHero);

		int dirX = mouseX - theHero->x;
		int dirY = mouseY - theHero->y;
		theHero->dir = GetHeroDir(dirX, dirY);
	}
}

// ˢ��BOSS״̬����
void UpdateBoss()
{
	//double length = sqrt(pow(theHero->x - theBoss->x, 2) + pow(theHero->y - theBoss->y, 2));
	//double v = sqrt(pow(theBoss->vx, 2.0) + pow(theBoss->vy, 2.0));
	//if (v < BOSS_SPEED)
	//	v += BOSS_ACCELERATION;
	//static bool flag[RECT_NUM_Y - 1][RECT_NUM_X - 1] = { false };
	if (theBoss->health <= 0)
	{
		currentStage->win = 1;
		currentStage->timerOn = false;
		if (currentStage->stageID <= 1998)
		{
			for (int i = 0; i < buttons.size(); i++)
				if (buttons[i]->buttonID >= 1002 && buttons[i]->buttonID <= 1004)
					buttons[i]->visible = true;
				else
					buttons[i]->visible = false;
		}
		else if (currentStage->stageID == 2001)
		{
			for (int i = 0; i < buttons.size(); i++)
				if (buttons[i]->buttonID == 1002 || buttons[i]->buttonID == 1003 || buttons[i]->buttonID == 2015)
					buttons[i]->visible = true;
				else
					buttons[i]->visible = false;
		}
		return;
	}

	if (batmans.size() == 0)
	{
		theBoss->available = true;
		currentStage->callbatman = 0;
	}
	else
		theBoss->available = false;

	if (((int)theBoss->x - RECT_SIZE_X / 2 - BEGIN_X) % RECT_SIZE_X == 0 &&
		((int)theBoss->y - RECT_SIZE_Y / 2 - BEGIN_Y) % RECT_SIZE_Y == 0)
	{
		int xB, yB;
		xB = (theBoss->x - RECT_SIZE_X / 2 - BEGIN_X) / RECT_SIZE_X;
		yB = (theBoss->y - RECT_SIZE_Y / 2 - BEGIN_Y) / RECT_SIZE_Y;
		if (abs(theHero->x - theBoss->x) > abs(theHero->y - theBoss->y))
		{
			if (recta[yB][xB + (int)((theHero->x - theBoss->x) / abs(theHero->x - theBoss->x))].health == 0)
			{
				theBoss->vx = (theHero->x - theBoss->x) / abs(theHero->x - theBoss->x)*BOSS_SPEED;
				theBoss->vy = 0;
			}
			else if (recta[yB + (int)((theHero->y - theBoss->y) / abs(theHero->y - theBoss->y))][xB].health == 0)
			{
				theBoss->vy = (theHero->y - theBoss->y) / abs(theHero->y - theBoss->y)*BOSS_SPEED;
				theBoss->vx = 0;
			}
			else
			{
				if (recta[yB - (int)((theHero->y - theBoss->y) / abs(theHero->y - theBoss->y))][xB].health == 0)
				{
					theBoss->vy = -(theHero->y - theBoss->y) / abs(theHero->y - theBoss->y)*BOSS_SPEED;
					theBoss->vx = 0;
				}
				else
				{
					theBoss->vx = -(theHero->x - theBoss->x) / abs(theHero->x - theBoss->x)*BOSS_SPEED;
					theBoss->vy = 0;
				}
			}
		}
		else
		{
			if (recta[yB + (int)((theHero->y - theBoss->y) / abs(theHero->y - theBoss->y))][xB].health == 0)
			{
				theBoss->vy = (theHero->y - theBoss->y) / abs(theHero->y - theBoss->y)*BOSS_SPEED;
				theBoss->vx = 0;
			}
			else if (recta[yB][xB + (int)((theHero->x - theBoss->x) / abs(theHero->x - theBoss->x))].health == 0)
			{
				theBoss->vx = (theHero->x - theBoss->x) / abs(theHero->x - theBoss->x)*BOSS_SPEED;
				theBoss->vy = 0;
			}
			else if (recta[yB][xB - (int)((theHero->x - theBoss->x) / abs(theHero->x - theBoss->x))].health == 0)
			{
				theBoss->vx = -(theHero->x - theBoss->x) / abs(theHero->x - theBoss->x)*BOSS_SPEED;
				theBoss->vy = 0;
			}
			else
			{
				theBoss->vy = -(theHero->y - theBoss->y) / abs(theHero->y - theBoss->y)*BOSS_SPEED;
				theBoss->vx = 0;
			}
		}
	}

	//ΪBOSS����ж��߿�,���ӷ���Ч��
	if ((theBoss->vx > 0) && (theBoss->x >= WINDOW_WIDTH - BOSS_SIZE))
		theBoss->vx = -1 * theBoss->vx;
	if ((theBoss->vx < 0) && (theBoss->x <= BOSS_SIZE))
		theBoss->vx = -1 * theBoss->vx;
	if ((theBoss->vy > 0) && (theBoss->y >= WINDOW_HEIGHT - BOSS_SIZE))
		theBoss->vy = -1 * theBoss->vy;
	if ((theBoss->vy < 0) && (theBoss->y <= BOSS_SIZE))
		theBoss->vy = -1 * theBoss->vy;

	//theBoss->vx = v*(theHero->x - theBoss->x) / length;
	//theBoss->vy = v*(theHero->y - theBoss->y) / length;

	theBoss->x += theBoss->vx;
	theBoss->y += theBoss->vy;
}

//ˢ���ӵ�״̬����
void UpdateBullet()
{
	int x1, x2, x3, x4, y1, y2, y3, y4;
	vector<struct Bullage*>::iterator iter = bullets.begin();
	while (iter != bullets.end())
	{

		//��ȡ�ӵ����ھ��ε��ĸ���������
		x1 = (*iter)->x + (*iter)->dirX*BULLET_SIZE_Y / 2.0 - (*iter)->dirY*BULLET_SIZE_X / 2.0;
		y1 = (*iter)->y + (*iter)->dirY*BULLET_SIZE_Y / 2.0 + (*iter)->dirX*BULLET_SIZE_X / 2.0;
		x2 = (*iter)->x + (*iter)->dirX*BULLET_SIZE_Y / 2.0 + (*iter)->dirY*BULLET_SIZE_X / 2.0;
		y2 = (*iter)->y + (*iter)->dirY*BULLET_SIZE_Y / 2.0 - (*iter)->dirX*BULLET_SIZE_X / 2.0;
		x3 = (*iter)->x - (*iter)->dirX*BULLET_SIZE_Y / 2.0 + (*iter)->dirY*BULLET_SIZE_X / 2.0;
		y3 = (*iter)->y - (*iter)->dirY*BULLET_SIZE_Y / 2.0 - (*iter)->dirX*BULLET_SIZE_X / 2.0;
		x4 = (*iter)->x - (*iter)->dirX*BULLET_SIZE_Y / 2.0 - (*iter)->dirY*BULLET_SIZE_X / 2.0;
		y4 = (*iter)->y - (*iter)->dirY*BULLET_SIZE_Y / 2.0 + (*iter)->dirX*BULLET_SIZE_X / 2.0;

		//Ϊ�ӵ���ӱ߽磬������Զ���ʧ
		if ((x1 <= BEGIN_X) || (x1 >= BEGIN_X + RECT_NUM_X*RECT_SIZE_X) || (x4 <= BEGIN_X) || (x4 >= BEGIN_X + RECT_NUM_X*RECT_SIZE_X) ||
			(y1 >= BEGIN_Y + RECT_NUM_Y*RECT_SIZE_Y) || (y1 <= BEGIN_Y) || (y4 >= BEGIN_Y + RECT_NUM_Y*RECT_SIZE_Y) || (y4 <= BEGIN_Y))
		{
			delete *iter;
			*iter = NULL;
			iter = bullets.erase(iter);
			goto end;
		}

		//�ӵ���ɵ���ײ���
		if (theRocket != NULL)
			if (CheckRec_Ellipse(x1, y1, x2, y2, x3, y3, x4, y4, theRocket->x, theRocket->y, ROCKET_SIZE))
			{
				delete theRocket;
				theRocket = NULL;
				delete *iter;
				*iter = NULL;
				iter = bullets.erase(iter);
				goto end;
			}
		

		//�ӵ���BOSS��ײ���
		if (CheckRec_Ellipse(x1, y1, x2, y2, x3, y3, x4, y4, theBoss->x, theBoss->y, BOSS_SIZE))
		{
			if (theBoss->available)
			{
				currentStage->score += 20;
				theBoss->health -= (*iter)->Class;
			}

			//�˴���Ӷ���������Ч��

			delete *iter;
			*iter = NULL;
			iter = bullets.erase(iter);
			goto end;
		}

		//�ӵ���С������ײ���
		{
			vector<struct Batman*>::iterator iter2 = batmans.begin();
			while (iter2 != batmans.end())
			{
				if (CheckRec_Ellipse(x1, y1, x2, y2, x3, y3, x4, y4, (*iter2)->x, (*iter2)->y, BATMAN_SIZE))
				{
					currentStage->score += 10;
					delete *iter2;
					*iter2 = NULL;
					iter2 = batmans.erase(iter2);

					//�˴���Ӷ���������Ч��
					delete *iter;
					*iter = NULL;
					iter = bullets.erase(iter);
					goto end;
				}
				iter2++;
			}
		}

		//�ӵ��뵯Ļ��ײ���
		{
			vector<struct Bullage*>::iterator iter1 = barrages.begin();
			while (iter1 != barrages.end())
			{
				if (CheckRec_Ellipse(x1, y1, x2, y2, x3, y3, x4, y4, (*iter1)->x, (*iter1)->y, BARRAGE_SIZE))
				{

					//�˴���Ӷ���������Ч��
					if ((*iter1)->Class == -1)
					{
						currentStage->score += 2;
						delete *iter1;
						*iter1 = NULL;
						iter1 = barrages.erase(iter1);
					}
					if ((*iter)->Class == 1)
					{
						delete *iter;
						*iter = NULL;
						iter = bullets.erase(iter);
						goto end;
					}
					else
					{
						(*iter)->Class = 1;
						break;
					}
				}
				iter1++;
			}
		}

		//�ӵ���ש�����ײ���
		int i, j;
		if (CheckBullage_Recta(*iter, &i, &j))
		{
			currentStage->score += 2;

			if (recta[i][j].health > 0)
				recta[i][j].health--;

			//�˴���Ӷ���������Ч��

			delete *iter;
			*iter = NULL;
			iter = bullets.erase(iter);
			goto end;
		}

		//���ӵ�����һ�����ٶ�
		if ((*iter)->v < BULLET_MAXSPEED)
			(*iter)->v += BULLET_ACCELERATION;

		(*iter)->x += (int)((*iter)->v*(*iter)->dirX);
		(*iter)->y += (int)((*iter)->v*(*iter)->dirY);

		iter++;

	end:
		;
	}
}

// ˢ�µ�Ļ״̬����
void UpdateBarrage()
{
	int x1, x2, x3, x4, y1, y2, y3, y4;
	vector<struct Bullage*>::iterator iter = barrages.begin();
	while (iter != barrages.end())
	{
		//Ϊ��Ļ����ж��߿򣬳�����Զ�ɾ��
		if (((*iter)->x <= BEGIN_X + BARRAGE_SIZE) || ((*iter)->x >= BEGIN_X + RECT_NUM_X*RECT_SIZE_X - BARRAGE_SIZE) ||
			((*iter)->y >= BEGIN_Y + RECT_NUM_Y*RECT_SIZE_Y - BARRAGE_SIZE) || ((*iter)->y <= BEGIN_Y + BARRAGE_SIZE))
		{
			delete *iter;
			*iter = NULL;
			iter = barrages.erase(iter);
			continue;
		}

		//��Ļ��Hero��ײ���
		if (CheckEllipse((*iter)->x, (*iter)->y, theHero->x, theHero->y, BARRAGE_SIZE, HERO_SIZE))
		{
			theHero->health--;

			//�˴���Ӷ���������Ч��

			delete *iter;
			*iter = NULL;
			iter = barrages.erase(iter);
			continue;
		}
			
		//��Ļ��ש�����ײ���
		int a, b;
		a = ((*iter)->x - BEGIN_X) / RECT_SIZE_X;
		b = ((*iter)->y - BEGIN_Y) / RECT_SIZE_Y;
		for (int k = a - 1; k <= a + 1; k++)
			for (int l = b - 1; l <= b + 1; l++)
				if (recta[l][k].health != 0&& 
					!((*iter)->x + BARRAGE_SIZE<recta[l][k].x || (*iter)->x - BARRAGE_SIZE>recta[l][k].x + RECT_SIZE_X
					|| (*iter)->y + BARRAGE_SIZE<recta[l][k].y || (*iter)->y - BARRAGE_SIZE>recta[l][k].y + RECT_SIZE_Y))
				{
					delete *iter;
					*iter = NULL;
					iter = barrages.erase(iter);
					goto end;
				}

		(*iter)->x += (*iter)->v*(*iter)->dirX;
		(*iter)->y += (*iter)->v*(*iter)->dirY;
		iter++;
	end:
		;
	}
}

// ˢ��С��״̬����
void UpdateBatman()
{
	for (int i = 0; i < batmans.size(); i++)
	{
		Batman* iter = batmans[i];
		double length;
		length = sqrt(pow(theHero->x - iter->x, 2) + pow(theHero->y - iter->y, 2));
		iter->dirX = (theHero->x - iter->x) / length;
		iter->dirY = (theHero->y - iter->y) / length;
		int a, b;
		a = (iter->x - BEGIN_X) / RECT_SIZE_X;
		b = (iter->y - BEGIN_Y) / RECT_SIZE_Y;
		for (int k = a - 1; k <= a + 1; k++)
			for (int l = b - 1; l <= b + 1; l++)
			{
				if (recta[l][k].health == 0)
					continue;
				if (IsCircleIntersectRectangle(iter->x + BATMAN_SPEED*iter->dirX, iter->y + BATMAN_SPEED*iter->dirY, BATMAN_SIZE, recta[l][k].x + RECT_SIZE_X / 2, recta[l][k].y + RECT_SIZE_Y / 2, recta[l][k].x + RECT_SIZE_X / 2, recta[l][k].y, recta[l][k].x + RECT_SIZE_X, recta[l][k].y + RECT_SIZE_Y / 2))
					goto end1;
			}
		iter->x += iter->dirX*BATMAN_SPEED;
		iter->y += iter->dirY*BATMAN_SPEED;
		return;

		end1:
		for (int k = a - 1; k <= a + 1; k++)
			for (int l = b - 1; l <= b + 1; l++)
			{
				if (recta[l][k].health == 0)
					continue;
				if (IsCircleIntersectRectangle(iter->x, iter->y, BATMAN_SIZE , recta[l][k].x + RECT_SIZE_X / 2, recta[l][k].y + RECT_SIZE_Y / 2, recta[l][k].x + RECT_SIZE_X / 2, recta[l][k].y, recta[l][k].x + RECT_SIZE_X, recta[l][k].y + RECT_SIZE_Y / 2))
				{
					//��
					if (abs(iter->x - recta[l][k].x) < BATMAN_SIZE &&iter->y > recta[l][k].y&&iter->y < recta[l][k].y + RECT_SIZE_Y)
					{
						iter->dirX = 0;
						iter->dirY = iter->dirY / abs(iter->dirY);
						iter->x = recta[l][k].x - BATMAN_SIZE;
						goto end;
					}
					//��
					if (abs(iter->x - recta[l][k].x - RECT_SIZE_X) < BATMAN_SIZE&&iter->y > recta[l][k].y&&iter->y < recta[l][k].y + RECT_SIZE_Y)
					{
						iter->dirX = 0;
						iter->dirY = iter->dirY / abs(iter->dirY);
						iter->x = recta[l][k].x + RECT_SIZE_X + BATMAN_SIZE;
						goto end;
					}
					//��
					if (abs(iter->y - recta[l][k].y) < BATMAN_SIZE&&iter->x > recta[l][k].x&&iter->x < recta[l][k].x + RECT_SIZE_X)
					{
						iter->dirY = 0;
						iter->dirX = iter->dirX / abs(iter->dirX);
						iter->y = recta[l][k].y - BATMAN_SIZE;
						goto end;
					}
					//��
					if (abs(iter->y - recta[l][k].y - RECT_SIZE_Y) < BATMAN_SIZE&&iter->x > recta[l][k].x&&iter->x < recta[l][k].x + RECT_SIZE_X)
					{
						iter->dirY = 0;
						iter->dirX = iter->dirX / abs(iter->dirX);
						iter->y = recta[l][k].y + RECT_SIZE_Y + BATMAN_SIZE;
						goto end;
					}
					double length;
					//����
					length = sqrt(pow(iter->x - recta[l][k].x, 2) + pow(iter->y - recta[l][k].y, 2));
					if ((int)length < BATMAN_SIZE)
					{
						double dirX = (iter->x - recta[l][k].x) / length;
						double dirY = (iter->y - recta[l][k].y) / length;
						iter->x = recta[l][k].x + BATMAN_SIZE*dirX;
						iter->y = recta[l][k].y + BATMAN_SIZE*dirY;
						iter->dirX = dirY;
						iter->dirY = -dirX;
						goto end;
					}
					//����
					length = sqrt(pow(recta[l][k].x + RECT_SIZE_X - iter->x, 2) + pow(recta[l][k].y - iter->y, 2));
					if ((int)length < BATMAN_SIZE)
					{
						double dirX = (iter->x - recta[l][k].x - RECT_SIZE_X) / length;
						double dirY = (iter->y - recta[l][k].y) / length;
						iter->x = recta[l][k].x + RECT_SIZE_X + BATMAN_SIZE*dirX;
						iter->y = recta[l][k].y + BATMAN_SIZE*dirY;
						iter->dirX = dirY;
						iter->dirY = -dirX;
						goto end;
					}
					//����
					length = sqrt(pow(recta[l][k].x - iter->x, 2) + pow(recta[l][k].y + RECT_SIZE_Y - iter->y, 2));
					if ((int)length < BATMAN_SIZE)
					{
						double dirX = (iter->x - recta[l][k].x) / length;
						double dirY = (iter->y - recta[l][k].y - RECT_SIZE_Y) / length;
						iter->x = recta[l][k].x + BATMAN_SIZE*dirX;
						iter->y = recta[l][k].y + RECT_SIZE_Y + BATMAN_SIZE*dirY;
						iter->dirX = dirY;
						iter->dirY = -dirX;
						goto end;
					}
					//����
					length = sqrt(pow(recta[l][k].x + RECT_SIZE_X - iter->x, 2) + pow(recta[l][k].y + RECT_SIZE_Y - iter->y, 2));
					if ((int)length < BATMAN_SIZE)
					{
						double dirX = (iter->x - recta[l][k].x - RECT_SIZE_X) / length;
						double dirY = (iter->y - recta[l][k].y - RECT_SIZE_Y) / length;
						iter->x = recta[l][k].x + RECT_SIZE_X + BATMAN_SIZE*dirX;
						iter->y = recta[l][k].y + RECT_SIZE_Y + BATMAN_SIZE*dirY;
						iter->dirX = dirY;
						iter->dirY = -dirX;
						goto end;
					}
				}
			}

	end:
		iter->x += iter->dirX*BATMAN_SPEED;
		iter->y += iter->dirY*BATMAN_SPEED;
	}
}

//ˢ�·ɵ�״̬����
void UpdateRocket()
{
	if (theRocket != NULL)
	{
		//Ϊ��Ļ����ж��߿򣬳�����Զ�ɾ��
		if ((theRocket->x <= BEGIN_X + ROCKET_SIZE) || (theRocket->x >= BEGIN_X + RECT_NUM_X*RECT_SIZE_X - ROCKET_SIZE) ||
			(theRocket->y >= BEGIN_Y + RECT_NUM_Y*RECT_SIZE_Y - ROCKET_SIZE) || (theRocket->y <= BEGIN_Y + ROCKET_SIZE))
		{
			delete theRocket;
			theRocket = NULL;
			return;
		}

		double length;
		length = sqrt(pow(theHero->x - theRocket->x, 2) + pow(theHero->y - theRocket->y, 2));
		if (length <= ROCKET_SIZE + HERO_SIZE)
		{
			theHero->health -= 2;
			delete theRocket;
			theRocket = NULL;
		}
		else
		{
			theRocket->dirX = (theHero->x - theRocket->x) / length;
			theRocket->dirY = (theHero->y - theRocket->y) / length;
			theRocket->x += theRocket->dirX*ROCKET_SPEED;
			theRocket->y += theRocket->dirY*ROCKET_SPEED;
		}
	}

}

// �������ǵĵ�ǰ����
double GetHeroDir(int dirX, int dirY) 
{
	double length = sqrt(dirX*dirX + dirY*dirY);
	double cos = dirY / length;
	if (dirX >= 0)
		return 180 - acos(cos) * 180 / PI;
	else
		return acos(cos) * 180 / PI + 180;
}

//�����ӵ���ǰ����
double GetBulletDir(double dirX, double dirY)
{
	if (dirX >= 0)
		return 180 - acos(dirY) * 180 / PI;
	else
		return acos(dirY) * 180 / PI + 180;
}

//�ӵ�(��Ļ)��ש����ײ���
bool CheckBullage_Recta(Bullage *iter, int* i, int* j)
{
	double x1, x2, x3, x4, y1, y2, y3, y4;
	x1 = iter->x + iter->dirX*BULLET_SIZE_Y / 2.0 - iter->dirY*BULLET_SIZE_X / 2.0;
	y1 = iter->y + iter->dirY*BULLET_SIZE_Y / 2.0 + iter->dirX*BULLET_SIZE_X / 2.0;
	x2 = iter->x + iter->dirX*BULLET_SIZE_Y / 2.0 + iter->dirY*BULLET_SIZE_X / 2.0;
	y2 = iter->y + iter->dirY*BULLET_SIZE_Y / 2.0 - iter->dirX*BULLET_SIZE_X / 2.0;
	x3 = iter->x - iter->dirX*BULLET_SIZE_Y / 2.0 + iter->dirY*BULLET_SIZE_X / 2.0;
	y3 = iter->y - iter->dirY*BULLET_SIZE_Y / 2.0 - iter->dirX*BULLET_SIZE_X / 2.0;
	x4 = iter->x - iter->dirX*BULLET_SIZE_Y / 2.0 - iter->dirY*BULLET_SIZE_X / 2.0;
	y4 = iter->y - iter->dirY*BULLET_SIZE_Y / 2.0 + iter->dirX*BULLET_SIZE_X / 2.0;
	int a, b;
	a = (iter->x - BEGIN_X) / RECT_SIZE_X;
	b = (iter->y - BEGIN_Y) / RECT_SIZE_Y;
	for (int k = a - 1; k <= a + 1; k++)
		for (int l = b - 1; l <= b + 1; l++)
		{
			if (recta[l][k].health == 0)
				continue;
			for (double m = 0; m <= 1.0; m = m + 0.001)			//0.01Ϊ��ײ��⾫��
				if (((x1 + m*iter->v*iter->dirX >= recta[l][k].x) && (x1 + m*iter->v*iter->dirX <= recta[l][k].x + RECT_SIZE_X) && (y1 + m*iter->v*iter->dirY >= recta[l][k].y) && (y1 + m*iter->v*iter->dirY <= recta[l][k].y + RECT_SIZE_Y)) ||
					(x2 + m*iter->v*iter->dirX >= recta[l][k].x) && (x2 + m*iter->v*iter->dirX <= recta[l][k].x + RECT_SIZE_X) && (y2 + m*iter->v*iter->dirY >= recta[l][k].y) && (y2 + m*iter->v*iter->dirY <= recta[l][k].y + RECT_SIZE_Y) ||
					(x3 + m*iter->v*iter->dirX >= recta[l][k].x) && (x3 + m*iter->v*iter->dirX <= recta[l][k].x + RECT_SIZE_X) && (y3 + m*iter->v*iter->dirY >= recta[l][k].y) && (y3 + m*iter->v*iter->dirY <= recta[l][k].y + RECT_SIZE_Y) ||
					(x4 + m*iter->v*iter->dirX >= recta[l][k].x) && (x4 + m*iter->v*iter->dirX <= recta[l][k].x + RECT_SIZE_X) && (y4 + m*iter->v*iter->dirY >= recta[l][k].y) && (y1 + m*iter->v*iter->dirY <= recta[l][k].y + RECT_SIZE_Y))
				{
					*i = l;
					*j = k;
					return true;
				}
		}
	return false;

}

//������ש����ײ���
void CheckHero_Recta(Hero* a_Hero)
{
	int a, b;
	a = (a_Hero->x - BEGIN_X) / RECT_SIZE_X;
	b = (a_Hero->y - BEGIN_Y) / RECT_SIZE_Y;
	for (int k = a - 1; k <= a + 1; k++)
		for (int l = b - 1; l <= b + 1; l++)
		{
			if (recta[l][k].health == 0)
				continue;

			if (a_Hero->x + HERO_SIZE<recta[l][k].x || a_Hero->x - HERO_SIZE>recta[l][k].x + RECT_SIZE_X
				|| a_Hero->y + HERO_SIZE<recta[l][k].y || a_Hero->y - HERO_SIZE>recta[l][k].y + RECT_SIZE_Y)
				continue;

			//�ж�����ש��
			if (recta[l][k].health == -2)
			{
				if (sqrt(pow(recta[l][k].x + RECT_SIZE_X / 2 - a_Hero->x, 2) + pow(recta[l][k].y + RECT_SIZE_Y / 2 - a_Hero->y, 2)) <= 25 + HERO_SIZE)
				{
					double length = sqrt(pow(recta[l][k].x + RECT_SIZE_X / 2 - a_Hero->x, 2) + pow(recta[l][k].y + RECT_SIZE_Y / 2 - a_Hero->y, 2));
					double dirX = (a_Hero->x - recta[l][k].x - RECT_SIZE_X / 2) / length;
					double dirY = (a_Hero->y - recta[l][k].y - RECT_SIZE_Y / 2) / length;
					double v = sqrt(pow(a_Hero->vx, 2) + pow(a_Hero->vy, 2));
					a_Hero->vx = v*dirX;
					a_Hero->vy = v*dirY;
					a_Hero->x = recta[l][k].x + RECT_SIZE_X / 2 + dirX* (25 + HERO_SIZE + 5);
					a_Hero->y = recta[l][k].y + RECT_SIZE_Y / 2 + dirY* (25 + HERO_SIZE + 5);
					a_Hero->health--;
					break;
				}
				else
					continue;
			}

			//��
			if (abs(a_Hero->x - recta[l][k].x) <= HERO_SIZE &&a_Hero->y >= recta[l][k].y&&a_Hero->y <= recta[l][k].y + RECT_SIZE_Y)
			{
				a_Hero->vx = -0.5*a_Hero->vx;
				a_Hero->x = recta[l][k].x - HERO_SIZE - 1;
			}
			//��
			if (abs(a_Hero->x - recta[l][k].x - RECT_SIZE_X) <= HERO_SIZE&&a_Hero->y >= recta[l][k].y&&a_Hero->y <= recta[l][k].y + RECT_SIZE_Y)
			{
				a_Hero->vx = -0.5*a_Hero->vx;
				a_Hero->x = recta[l][k].x + RECT_SIZE_X + HERO_SIZE + 1;
			}
			//��
			if (abs(a_Hero->y - recta[l][k].y) <= HERO_SIZE&&a_Hero->x >= recta[l][k].x&&a_Hero->x <= recta[l][k].x + RECT_SIZE_X)
			{
				a_Hero->vy = -0.5*a_Hero->vy;
				a_Hero->y = recta[l][k].y - HERO_SIZE - 1;
			}
			//��
			if (abs(a_Hero->y - recta[l][k].y - RECT_SIZE_Y) <= HERO_SIZE&&a_Hero->x >= recta[l][k].x&&a_Hero->x <= recta[l][k].x + RECT_SIZE_X)
			{
				a_Hero->vy = -0.5*a_Hero->vy;
				a_Hero->y = recta[l][k].y + RECT_SIZE_Y + HERO_SIZE + 1;
			}
		}
}

#pragma endregion


#pragma region ��ͼ����
// ��ͼ����
void Paint(HWND hWnd)
{
	PAINTSTRUCT ps;
	HDC hdc_window = BeginPaint(hWnd, &ps);

	HDC hdc_memBuffer = CreateCompatibleDC(hdc_window);
	HDC hdc_loadBmp = CreateCompatibleDC(hdc_window);

	//��ʼ������
	HBITMAP	blankBmp = CreateCompatibleBitmap(hdc_window, WINDOW_WIDTH, WINDOW_HEIGHT);
	SelectObject(hdc_memBuffer, blankBmp);

	//��ʼ��graph����ͻ��ʻ�ˢ
	::Graphics graph(hdc_memBuffer);
	graph.SetSmoothingMode(SmoothingModeHighQuality);

	if (currentStage->loading)							// �����ж���
	{
		//����ģ������
		Blur m_blur;
		BlurParams bp;
		bp.expandEdge = false;
		bp.radius = 20.0;
		m_blur.SetParameters(&bp);
		Bitmap bmp2(currentStage->bg, NULL);
		RECT rc = { 0, 0, bmp2.GetWidth(), bmp2.GetHeight() };
		bmp2.ApplyEffect(&m_blur, &rc);
		graph.DrawImage(&bmp2, 0, 0);

		//����Loaing����
		PointF P = PointF(440, 300);
		SolidBrush brushWhite(Color(255, 255, 255, 255));
		FontFamily fontFfps(L"Segoe Print");
		Font fontfps(&fontFfps, 80, FontStyleRegular, UnitPixel);
		graph.DrawString(L"Loading...", -1, &fontfps, P, &brushWhite);

	}
	else
	{
		// ����������������ݵ�����
		if (currentStage == NULL || currentStage->stageID == -1) 
		{
			//Blur m_blur;
			//BlurParams bp;
			//bp.expandEdge = false;
			//bp.radius = 6.0*(3 - load);
			//m_blur.SetParameters(&bp);
			////͸��HBITMAPתBitmap  
			//Bitmap bmp2(bg1, NULL);
			//RECT rc = { 0, 0, bmp2.GetWidth(), bmp2.GetHeight() };
			//bmp2.ApplyEffect(&m_blur, &rc);
			//graph.DrawImage(&bmp2, 0, 0);
			Image* ld;
			switch (load)
			{
			case 0:
				ld = new Image(L"./res/loading0.png");
				break;
			case 1:
				ld = new Image(L"./res/loading1.png");
				break;
			case 2:
				ld = new Image(L"./res/loading2.png");
				break;
			case 3:
				ld = new Image(L"./res/loading3.png");
				break;
			}
			graph.DrawImage(ld, 0, 0, ld->GetWidth(), ld->GetHeight());
			delete ld;
		}
		else if (currentStage->stageID <= -2 && currentStage->stageID >= -10)
		{
			SelectObject(hdc_loadBmp, currentStage->bg);
			BitBlt(hdc_memBuffer, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, hdc_loadBmp, 0, 0, SRCCOPY);
		}
		else if (currentStage->stageID == 0)
		{
			Image* startbg = new Image(L"./res/startbg.png");						//����ָ��·����ͼ���ļ�
			graph.DrawImage(startbg, 0, 0, startbg->GetWidth(), startbg->GetHeight()); //��ָ��ͼ��ؼ��������л���ͼ��
			delete startbg;
		}
		else if (currentStage->stageID == 1999)
		{
			//������Ϸ����������
			SelectObject(hdc_loadBmp, currentStage->bg);
			BitBlt(hdc_memBuffer, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, hdc_loadBmp, 0, 0, SRCCOPY);
		}
		else if (currentStage->stageID == 2000)
		{
			//������Ϸ����������
			SelectObject(hdc_loadBmp, currentStage->bg);
			BitBlt(hdc_memBuffer, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, hdc_loadBmp, 0, 0, SRCCOPY);

			//������Ӧ���ݵ�����
			SolidBrush brushWhite(Color(255, 0, 0, 0));
			WCHAR  wstr[8];
			char st[8] = { 0 };
			PointF P;
			FontFamily fontFstage(L"΢���ź�");
			Font fontstage(&fontFstage, 45, FontStyleRegular, UnitPixel);
			sprintf(st, "%d", customMode->rec[0]);
			MultiByteToWideChar(CP_ACP, 0, st, -1, wstr, sizeof(wstr));
			P = PointF(825, 150);
			graph.DrawString(wstr, -1, &fontstage, P, &brushWhite);
			sprintf(st, "%d", customMode->rec[1]);
			MultiByteToWideChar(CP_ACP, 0, st, -1, wstr, sizeof(wstr));
			P = PointF(825, 230);
			graph.DrawString(wstr, -1, &fontstage, P, &brushWhite);
			sprintf(st, "%d", customMode->rec[2]);
			MultiByteToWideChar(CP_ACP, 0, st, -1, wstr, sizeof(wstr));
			P = PointF(825, 310);
			graph.DrawString(wstr, -1, &fontstage, P, &brushWhite);
			sprintf(st, "%d", customMode->batman);
			MultiByteToWideChar(CP_ACP, 0, st, -1, wstr, sizeof(wstr));
			P = PointF(675, 383);
			graph.DrawString(wstr, -1, &fontstage, P, &brushWhite);
			sprintf(st, "%d", customMode->callbatman / 100);
			MultiByteToWideChar(CP_ACP, 0, st, -1, wstr, sizeof(wstr));
			P = PointF(675, 447);
			graph.DrawString(wstr, -1, &fontstage, P, &brushWhite);
			sprintf(st, "%d", customMode->timebarrage * 10);
			MultiByteToWideChar(CP_ACP, 0, st, -1, wstr, sizeof(wstr));
			P = PointF(675, 510);
			graph.DrawString(wstr, -1, &fontstage, P, &brushWhite);

		}
		else if (currentStage->stageID >= 1001) //TODO����Ӷ����Ϸ����
		{
			if (currentStage->timerOn == true)
			{
				//������Ϸ����������
				SelectObject(hdc_loadBmp, currentStage->bg);
				BitBlt(hdc_memBuffer, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, hdc_loadBmp, 0, 0, SRCCOPY);

				SolidBrush brushRed(Color(255, 255, 0, 0));
				SolidBrush brushGreen(Color(255, 0, 255, 0));
				SolidBrush brushOrange(Color(255, 253, 104, 13));
				SolidBrush brushBlue(Color(255, 153, 217, 234));

				//�����ӵ�������
				for (int i = 0; i < bullets.size(); i++)
				{
					Bullage* iter = bullets[i];
					if (iter == NULL)
						continue;
					graph.RotateTransform(GetBulletDir((*iter).dirX, (*iter).dirY), MatrixOrderAppend);
					graph.TranslateTransform((*iter).x, (*iter).y, MatrixOrderAppend);
					if(iter->Class==1)
						graph.DrawImage(cyanbullet, -BULLET_SIZE_X / 2, -BULLET_SIZE_Y / 2, BULLET_SIZE_X, BULLET_SIZE_Y);
					else
						graph.DrawImage(orangebullet, -BULLET_SIZE_X / 2, -BULLET_SIZE_Y / 2, BULLET_SIZE_X, BULLET_SIZE_Y);
					graph.ResetTransform();
				}

				//���Ƶ�Ļ������
				for (int i = 0; i < barrages.size(); i++)
				{
					Bullage* iter = barrages[i];
					if (iter == NULL)
						continue;
					switch (iter->Class)
					{
					case -2:
						graph.FillEllipse(&brushOrange, (int)iter->x - BARRAGE_SIZE, (int)iter->y - BARRAGE_SIZE, 2 * BARRAGE_SIZE, 2 * BARRAGE_SIZE);
						break;
					case -1:
						graph.FillEllipse(&brushBlue, (int)iter->x - BARRAGE_SIZE, (int)iter->y - BARRAGE_SIZE, 2 * BARRAGE_SIZE, 2 * BARRAGE_SIZE);
						break;
					}
				}

				// ����С��������
				for (int i = 0; i < batmans.size(); i++)
				{
					Batman* iter = batmans[i];
					if (iter == NULL)
						continue;
					int dirX = theHero->x - iter->x;
					int dirY = theHero->y - iter->y;
					graph.RotateTransform(GetHeroDir(dirX, dirY), MatrixOrderAppend);
					graph.TranslateTransform((*iter).x, (*iter).y, MatrixOrderAppend);
					graph.DrawImage(btmn, -10, -25, 20, 35);
					graph.ResetTransform();
				}


				//���ƾ������򵽻���
				for (int i = 0; i <= RECT_NUM_Y - 1; i++)
					for (int j = 0; j <= RECT_NUM_X - 1; j++)
					{
						if (recta[i][j].health == 0)
							continue;
						if (recta[i][j].health > 0)
						{
							SelectObject(hdc_loadBmp, brick1);
							BitBlt(hdc_memBuffer, recta[i][j].x, recta[i][j].y, RECT_SIZE_X, RECT_SIZE_Y, hdc_loadBmp, 0, 0, SRCCOPY);
							continue;
						}
						if (recta[i][j].health == -1)
						{
							SelectObject(hdc_loadBmp, brickf1);
							BitBlt(hdc_memBuffer, recta[i][j].x, recta[i][j].y, RECT_SIZE_X, RECT_SIZE_Y, hdc_loadBmp, 0, 0, SRCCOPY);
							continue;
						}
						if (recta[i][j].health == -2)
						{
							graph.DrawImage(brickf2, recta[i][j].x, recta[i][j].y, 50, 50);
							continue;
						}
					}

				// �������ǵ�����
				graph.RotateTransform(theHero->dir, MatrixOrderAppend);
				graph.TranslateTransform(theHero->x, theHero->y, MatrixOrderAppend);
				graph.DrawImage(hero, (int)(-1 * HERO_SIZE), (int)(-1 * HERO_SIZE));
				graph.ResetTransform();

				//���Ʒɵ�������
				if (theRocket != NULL)
				{
					graph.RotateTransform(GetBulletDir(theRocket->dirX, theRocket->dirY), MatrixOrderAppend);
					graph.TranslateTransform(theRocket->x, theRocket->y, MatrixOrderAppend);
					graph.DrawImage(rocket, -ROCKET_SIZE, -ROCKET_SIZE, 40, 90);
					graph.ResetTransform();
				}

				//����BOSS������
				graph.FillEllipse(&brushGreen, (int)theBoss->x - BOSS_SIZE, (int)theBoss->y - BOSS_SIZE, 2 * BOSS_SIZE, 2 * BOSS_SIZE);

				//����Boss�����ֵ�����
				if (theBoss->available == false)
					graph.DrawImage(BossProtect, (int)theBoss->x - 35, (int)theBoss->y - 35, 70, 70);

				//����BOSSѪ����������
				graph.FillRectangle(&brushRed, (int)theBoss->x - BOSS_SIZE, (int)theBoss->y - BOSS_SIZE - 10, theBoss->health / BOSS_HEALTH * BOSS_SIZE * 2, 5);

				//��������Ѫ����������
				graph.FillRectangle(&brushRed, (int)theHero->x - HERO_SIZE, (int)theHero->y - HERO_SIZE - 10, theHero->health / HERO_HEALTH * HERO_SIZE * 2, 5);

				SolidBrush brushWhite(Color(255, 255, 255, 255));
				WCHAR  wstr[100];
				PointF P;

				if (currentStage->stageID <= 1998)
				{
					//���ƹ���������
					char st[5] = { 0 };
					sprintf(st, "%d", currentStage->stageID - 1000);
					MultiByteToWideChar(CP_ACP, 0, st, -1, wstr, sizeof(wstr));
					P = PointF(30, 182);
					FontFamily fontFstage(L"����");
					Font fontstage(&fontFstage, 45, FontStyleRegular, UnitPixel);
					graph.DrawString(wstr, -1, &fontstage, P, &brushOrange);
				}

				//���Ʒ���������
				char score[10] = { 0 };
				sprintf(score, "%d", currentStage->score);
				MultiByteToWideChar(CP_ACP, 0, score, -1, wstr, sizeof(wstr));
				P = PointF(22, 475);
				FontFamily fontFscore(L"Comic Sans MS");
				Font fontscore(&fontFscore, 36, FontStyleRegular, UnitPixel);
				graph.DrawString(wstr, -1, &fontscore, P, &brushWhite);

				//����Ѫ��������
				char a[10] = { 0 };
				sprintf(a, "%d/%.0f", theHero->health, HERO_HEALTH);
				MultiByteToWideChar(CP_ACP, 0, a, -1, wstr, sizeof(wstr));
				P = PointF(50, 367);
				FontFamily fontFblood(L"Lucida Handwriting");
				Font fontblood(&fontFblood, 36, FontStyleRegular, UnitPixel);
				graph.DrawString(wstr, -1, &fontblood, P, &brushWhite);

				//����֡���ʵ�����
				char b[20] = { 0 };
				sprintf(b, "%.2lf FPS", fps);
				MultiByteToWideChar(CP_ACP, 0, b, -1, wstr, sizeof(wstr));
				P = PointF(4, 700);
				FontFamily fontFfps(L"Times New Roman");
				Font fontfps(&fontFfps, 18, FontStyleItalic, UnitPixel);
				graph.DrawString(wstr, -1, &fontfps, P, &brushWhite);

				//����������ͼ�굽����
				if (currentStage->stageID >= 1007)
				{
					if (cure)
					{
						Image *image = new Image(L"./res/bloodalready.png");
						graph.DrawImage(image, 19, 557, 87, 87);
						delete image;
					}
					else
					{
						Image *image = new Image(L"./res/blood.png");
						graph.DrawImage(image, 19, 557, 87, 87);
						delete image;
					}
				}
			}
			else if (currentStage->win == 0)
			{
				//����ģ������
				Blur m_blur;
				BlurParams bp;
				bp.expandEdge = false;
				bp.radius = 20.0;
				m_blur.SetParameters(&bp);
				Bitmap bmp2(currentStage->bg, NULL);
				RECT rc = { 0, 0, bmp2.GetWidth(), bmp2.GetHeight() };
				bmp2.ApplyEffect(&m_blur, &rc);
				graph.DrawImage(&bmp2, 0, 0);

				//���ƶԻ���
				Image *image = new Image(L"./res/pausebg.png");
				graph.DrawImage(image, 240, 254, 800, 211);
				delete image;

				//���Ʒ���
				SolidBrush brushBlack(Color(255, 0, 0, 0));
				WCHAR  wstr[10];
				PointF P;
				char a[10] = { 0 };
				sprintf(a, "%d", currentStage->score);
				MultiByteToWideChar(CP_ACP, 0, a, -1, wstr, sizeof(wstr));
				P = PointF(667, 260);
				FontFamily fontFblood(L"Comic Sans MS");
				Font fontblood(&fontFblood, 48, FontStyleRegular, UnitPixel);
				graph.DrawString(wstr, -1, &fontblood, P, &brushBlack);

			}

			//Ӯ��
			else if (currentStage->win == 1)
			{
				//����ģ������
				Blur m_blur;
				BlurParams bp;
				bp.expandEdge = false;
				bp.radius = 20.0;
				m_blur.SetParameters(&bp);
				Bitmap bmp2(currentStage->bg, NULL);
				RECT rc = { 0, 0, bmp2.GetWidth(), bmp2.GetHeight() };
				bmp2.ApplyEffect(&m_blur, &rc);
				graph.DrawImage(&bmp2, 0, 0);

				//���ƶԻ���
				Image* win = new Image(L"./res/win.png");
				graph.DrawImage(win, 240, 121, 800, 479);
				delete win;

				//���Ʒ���
				SolidBrush brushBlack(Color(255, 0, 0, 0));
				WCHAR  wstr[10];
				PointF P;
				char a[10] = { 0 };
				sprintf(a, "%d", currentStage->score);
				MultiByteToWideChar(CP_ACP, 0, a, -1, wstr, sizeof(wstr));
				P = PointF(667, 395);
				FontFamily fontFblood(L"Comic Sans MS");
				Font fontblood(&fontFblood, 48, FontStyleRegular, UnitPixel);
				graph.DrawString(wstr, -1, &fontblood, P, &brushBlack);
			}

			//����
			else if (currentStage->win == -1)
			{
				//����ģ������
				Blur m_blur;
				BlurParams bp;
				bp.expandEdge = false;
				bp.radius = 20.0;
				m_blur.SetParameters(&bp);
				Bitmap bmp2(currentStage->bg, NULL);
				RECT rc = { 0, 0, bmp2.GetWidth(), bmp2.GetHeight() };
				bmp2.ApplyEffect(&m_blur, &rc);
				graph.DrawImage(&bmp2, 0, 0);

				//���ƶԻ���
				Image* lose = new Image(L"./res/lose.png");
				graph.DrawImage(lose, 240, 121, 800, 479);
				delete lose;

				//���Ʒ���
				SolidBrush brushBlack(Color(255, 0, 0, 0));
				WCHAR  wstr[10];
				PointF P;
				char a[10] = { 0 };
				sprintf(a, "%d", currentStage->score);
				MultiByteToWideChar(CP_ACP, 0, a, -1, wstr, sizeof(wstr));
				P = PointF(667, 395);
				FontFamily fontFblood(L"Comic Sans MS");
				Font fontblood(&fontFblood, 48, FontStyleRegular, UnitPixel);
				graph.DrawString(wstr, -1, &fontblood, P, &brushBlack);
			}
		}

		// ���ư�ť������
		for (int i = 0; i<buttons.size(); i++)
		{
			Button* button = buttons[i];
			if (button->visible)
			{
				if (button->focused)
				{
					Image* img = new Image(button->focusedimg);
					if (button->click > 0)
					{
						graph.DrawImage(img, (int)(button->x + button->width / 2.0 - button->width / (2 * (button->click))), (int)(button->y + button->height / 2.0 - button->height / (2 * (button->click))), (int)(button->width / (button->click)), (int)(button->height / (button->click)));
						if (button->click <= 5)
							button->click++;
					}
					else
					{
						graph.DrawImage(img, button->x, button->y, button->width, button->height);
					}
					delete img;
				}
				else
				{
					Image* img = new Image(button->img);
					graph.DrawImage(img, button->x, button->y, button->width, button->height);
					delete img;
				}
			}
		}
	}
	
	// ������е���Ϣ���Ƶ���Ļ��
	BitBlt(hdc_window, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, hdc_memBuffer, 0, 0, SRCCOPY);

	// ������Դ��ռ���ڴ棨�ǳ���Ҫ��
	DeleteObject(blankBmp);
	DeleteDC(hdc_memBuffer);
	DeleteDC(hdc_loadBmp);

	// ��������
	EndPaint(hWnd, &ps);
}
#pragma endregion


#pragma region ��ʱ������
DWORD WINAPI Timer(LPVOID lpParameter)
{
	while (1)
	{
		timer++;

		// �ж��Ƿ��ڼ���״̬
		if (currentStage->stageID == -1)
		{
			if (timer % 40 == 0)
				load++;
			if (load == 4)
			{
				//��ʼ����ʼ����
				InitStage(hwd, -2);
				load = 0;
			}
			goto end;
		}

		if (currentStage->stageID == 0)
		{
			goto end;
		}

		// �ж��Ƿ������Ϸ
		if (currentStage->timerOn == true && currentStage->loading == false)
		{
			//�����ɵ�
			if(currentStage->stageID>=1011)
				if (timer % 1500 == 0)
				{
					theRocket = CreateBatman(theBoss->x, theBoss->y);
				}
			//�����ӵ�
			if (timer % 20 == 0 && shoot > 0)
			{
				double length, dirX, dirY;
				length = sqrt((mouseX - theHero->x)*(mouseX - theHero->x) + (mouseY - theHero->y)*(mouseY - theHero->y));
				dirX = (mouseX - theHero->x) / length;
				dirY = (mouseY - theHero->y) / length;
				if (shoot % 5 == 0)
				{
					Bullage* a_bullet = CreateBullet(dirX, dirY, theHero->x, theHero->y, 3);
					bullets.push_back(a_bullet);
				}
				else
				{
					Bullage* a_bullet = CreateBullet(dirX, dirY, theHero->x, theHero->y, 1);
					bullets.push_back(a_bullet);
				}
				shoot++;
			}

			//������Ļ
			//clΪ���浯Ļ���͵�����
			loop(theBoss->x, theBoss->y, currentStage->timebarrage, currentStage->numbarrage, currentStage->barrage);
			int clbatman[1] = { 3 };
			for (int i = 0; i < batmans.size(); i++)
			{
				Batman* iter = batmans[i];
				int dirX = theHero->x - iter->x;
				int dirY = theHero->y - iter->y;
				double length = sqrt(pow(dirX, 2) + pow(dirY, 2));
				loop(iter->x + BATMAN_SIZE*dirX / length, iter->y + BATMAN_SIZE*dirY / length, 60, 1, clbatman);
			}

			//����С��
			if (currentStage->callbatman > 0)
			{
				if (timer%currentStage->callbatman == 0)
				{
					while (1)
					{
						int xB, yB;
						xB = rand() % (RECT_NUM_X - 1 - 0 + 1) + 0;
						yB = rand() % (RECT_NUM_Y - 1 - 0 + 1) + 0;
						if (!(recta[yB][xB].health == 0 &&
							abs(recta[yB][xB].x + RECT_SIZE_X / 2 - theBoss->x) >= SAFE_DISTANCE&&abs(recta[yB][xB].y + RECT_SIZE_Y / 2 - theBoss->y) >= SAFE_DISTANCE&&
							abs(recta[yB][xB].x + RECT_SIZE_X / 2 - theHero->x) >= SAFE_DISTANCE&&abs(recta[yB][xB].y + RECT_SIZE_Y / 2 - theHero->y) >= SAFE_DISTANCE))
							continue;
						for (int j = 0; j < batmans.size(); j++)
						{
							Batman* iter = batmans[j];
							if ((int)(iter->x) == recta[yB][xB].x + RECT_SIZE_X / 2 && (int)(iter->y) == recta[yB][xB].y + RECT_SIZE_Y / 2)
								continue;
						}
						batmans.push_back(CreateBatman(recta[yB][xB].x + RECT_SIZE_X / 2, recta[yB][xB].y + RECT_SIZE_Y / 2));
						break;
					}
				}
			}

			//������ϷԪ��״̬
			UpdateBatman();
			UpdateBullet();
			UpdateBarrage();
			UpdateRocket();
			UpdateBoss();
			UpdateHero();
		}
	end:
		if (timer % 2 == 0)
		{
			// ˢ����ʾ
			InvalidateRect(hwd, NULL, FALSE);
		}
		Sleep(10);
	}
	return 0;
}

#pragma endregion


#pragma region ���䵯Ļ����

//ѭ�����n*5���뷢�䵯Ļ(�ܲ�����Ϊm,��Ļ���������������cl[])����
void loop(int x, int y, int n, int m, int cl[])
{
	if (n == 0)
		return;
	for (int i = m; i >=1; i--)
		if (timer % (n*i) == 0)
		{
			switch (cl[i - 1])
			{
			case 1:
				shootbarrage1(-1);
				break;
			case 2:
				shootbarrage1(-2);
				break;
			case 3:
				shootbarrage2(x, y, theHero->x - x, theHero->y - y, -1);
				break;
			case 4:
				shootbarrage2(x, y, theHero->x - x, theHero->y - y, -2);
				break;
			case 5:
				shootbarrage3(-1);
				break;
			case 6:
				shootbarrage3(-2);
				break;
			case 7:
				shootbarrage4(-1);
				break;
			case 8:
				shootbarrage4(-2);
				break;
			case 9:
				shootbarrage5(-1);
				break;
			case 10:
				shootbarrage5(-2);
				break;
			}
			break;
		}
}

//���䵯Ļ����

//ID1-2:һ�η���һ��ȫ��8���ɱ��ƻ�/���ɱ��ƻ��ĵ�Ļ
void shootbarrage1(int cl)
{
	for (int j = 1; j <= 8; j++)
	{
		double dirX = sin(j / 4.0 *3.1415926);
		double dirY = cos(j / 4.0 *3.1415926);
		Bullage* a_barrage = CreateBarrage(cl, dirX, dirY,
			theBoss->x + BARRAGE_SIZE*dirX, theBoss->y + BARRAGE_SIZE*dirY, BARRAGE_SPEED);
		barrages.push_back(a_barrage);
	}
}

//ID3-4:һ�η���һ�Ŵ�(x0,y0)�����ҷ�������Ϊ(x,y)�Ŀɱ��ƻ�/���ɱ��ƻ��ĵ�Ļ
void shootbarrage2(int x0, int y0, int x, int y, int cl)
{
	double dirX, dirY;
	dirX = x / sqrt(x*x + y*y);
	dirY = y / sqrt(x*x + y*y);
	Bullage* a_barrage = CreateBarrage(cl, dirX, dirY, x0, y0, BARRAGE_SPEED);
	barrages.push_back(a_barrage);
}

//ID5-6:һ�η���һ��ȫ��16���ɱ��ƻ�/���ɱ��ƻ��ĵ�Ļ
void shootbarrage3(int cl)
{
	for (int j = 1; j <= 16; j++)
	{
		double dirX = sin(j / 8.0 *3.1415926);
		double dirY = cos(j / 8.0 *3.1415926);
		Bullage* a_barrage = CreateBarrage(cl, dirX, dirY,
			theBoss->x + BARRAGE_SIZE*dirX, theBoss->y + BARRAGE_SIZE*dirY, BARRAGE_SPEED);
		barrages.push_back(a_barrage);
	}
}

//ID7-8:һ�η���һ��8���οɱ��ƻ�/���ɱ��ƻ��ĵ�Ļ
void shootbarrage4(int cl)
{
	for (int j = 1; j <= 32; j++)
	{
		double dirX = sin(j / 8.0 *3.1415926);
		double dirY = cos(j / 16.0 *3.1415926);
		Bullage* a_barrage = CreateBarrage(cl, dirX, dirY,
			theBoss->x + BARRAGE_SIZE*dirX, theBoss->y + BARRAGE_SIZE*dirY, BARRAGE_SPEED);
		if (dirX*dirX + dirY*dirY <= 0.5)
		{
			delete a_barrage;
			continue;
		}
		barrages.push_back(a_barrage);
	}
}

//ID9-10:һ�η���һ�������߿ɱ��ƻ�/���ɱ��ƻ��ĵ�Ļ
void shootbarrage5(int cl)
{
	for (int j = 1; j <= 32; j++)
	{
		double dirX = sin(j / 16.0 *3.1415926);
		double dirY = cos(j / 8.0 *3.1415926);
		Bullage* a_barrage = CreateBarrage(cl, dirX, dirY,
			theBoss->x + BARRAGE_SIZE*dirX, theBoss->y + BARRAGE_SIZE*dirY, BARRAGE_SPEED);
		if (dirX*dirX + dirY*dirY <= 0.5)
		{
			delete a_barrage;
			continue;
		}
		barrages.push_back(a_barrage);
	}
}

#pragma endregion