//元胞自动机
//作者：李国良
//Ver:1.0
//2017年7月4日晚编写完成
#include <Windows.h>
#include <cstdlib>
#include <ctime>
#include <cstdio>


HINSTANCE hInst;
bool arrWorld[100][100];
const int Uint = 10;
int arrTime[3] = { 1000, 200, 50 };
int iCell, iAge, xPos, yPos, prexPos, preyPos, speed;
char *ButtonText[4] = { "开始", "停止", "暂停", "恢复" };
bool bStart, bPause;
char str[100];





LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
VOID CALLBACK TimerProc(HWND, UINT, UINT_PTR, DWORD);
int Live();

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
	hInst = hInstance;
	MSG msg;
	static TCHAR szWindowClass[] = TEXT("GameOfLife");
	static TCHAR szTitle[] = TEXT("生命游戏");
	WNDCLASSEX wcex;
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = (HICON)LoadImage(NULL, "Icon.ico", IMAGE_ICON, 0, 0, LR_DEFAULTSIZE | LR_LOADFROMFILE);
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = CreateSolidBrush(RGB(240, 240, 240));
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(hInstance, NULL);
	if (!RegisterClassEx(&wcex)) {
		MessageBox(NULL, TEXT("调用RegisterClassEx失败!"), TEXT("提示"), MB_ICONWARNING);
		return 1;
	}
	HWND hWnd = CreateWindow(szWindowClass, szTitle, WS_CAPTION | WS_SYSMENU, 0, 0, 1280, 1050, NULL, NULL, hInstance, NULL);
	if (!hWnd) {
		MessageBox(NULL, TEXT("调用CreateWindow失败!"), TEXT("提示"), MB_ICONWARNING);
		return 1;
	}
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);
	while (GetMessage(&msg, NULL, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
	PAINTSTRUCT ps;
	HDC hdc;
	RECT rect;
	static HFONT hFont;
	static HPEN hPen;
	static HBRUSH hBrush;
	static HWND hBtnStart;
	static HWND hBtnRandom;
	static HWND hBtnPause;
	static HWND hStaticIteration;
	static HWND hRadioLow;
	static HWND hRadioMedium;
	static HWND hRadioHigh;
	static HWND hStaticLife;
	static HWND hStaticAge;




	switch (message) {
	case WM_COMMAND:
		hdc = GetDC(hWnd);
		switch (LOWORD(wParam)) {
		case 2:
			if (bStart) {
				bStart = false;
				bPause = false;
				KillTimer(hWnd, 13);
				SetWindowText(hBtnPause, ButtonText[2]);
				SetWindowText(hBtnStart, ButtonText[0]);
				EnableWindow(hBtnRandom, TRUE);
				EnableWindow(hBtnPause, FALSE);
				iCell = 0;
				iAge = 0;
				for (int i = 0; i < 100; ++i) {
					for (int j = 0; j < 100; ++j) {
						arrWorld[i][j] = false;
					}
				}
				SelectObject(hdc, hFont);
				SetBkColor(hdc, GetSysColor(COLOR_BTNFACE));
				wsprintf(str, TEXT("迭代次数: 0                "));
				rect = { 1020, 150, 1320, 200 };
				DrawText(hdc, str, -1, &rect, DT_SINGLELINE);
				wsprintf(str, TEXT("元胞数量: 0                "));
				rect = { 1020, 190, 1320, 240 };
				DrawText(hdc, str, -1, &rect, DT_SINGLELINE);
				SelectObject(hdc, hPen);
				SelectObject(hdc, GetStockObject(WHITE_BRUSH));
				Rectangle(hdc, 0, 0, 1001, 1001);
				for (int i = 0; i <= 1000; i = i + Uint) {
					MoveToEx(hdc, i, 0, NULL);
					LineTo(hdc, i, 1000);
					MoveToEx(hdc, 0, i, NULL);
					LineTo(hdc, 1000, i);
				}
			}
			else {
				bStart = true;
				SetTimer(hWnd, 13, arrTime[speed], TimerProc);
				SetWindowText(hBtnStart, ButtonText[1]);
				EnableWindow(hBtnRandom, FALSE);
				EnableWindow(hBtnPause, TRUE);
				SelectObject(hdc, GetStockObject(WHITE_PEN));
				for (int i = 0; i <= 1000; i = i + Uint) {
					MoveToEx(hdc, i, 0, NULL);
					LineTo(hdc, i, 1000);
					MoveToEx(hdc, 0, i, NULL);
					LineTo(hdc, 1000, i);
				}
			}
			break;
		case 3:
			for (int i = 0; i < 100; ++i) {
				for (int j = 0; j < 100; ++j) {
					arrWorld[i][j] = false;
				}
			}
			for (int i = 0; i < 100; ++i) {
				for (int j = 0; j < 100; ++j) {
					if (rand() % 100 < 20) {
						arrWorld[i][j] = true;
					}
				}
			}
			SelectObject(hdc, hPen);
			for (int i = 0; i < 100; ++i) {
				for (int j = 0; j < 100; ++j) {
					if (arrWorld[i][j]) {
						SelectObject(hdc, GetStockObject(BLACK_BRUSH));
						Rectangle(hdc, j * Uint, i * Uint, j * Uint + 11, i * Uint + 11);
					}
					else {
						SelectObject(hdc, GetStockObject(WHITE_BRUSH));
						Rectangle(hdc, j * Uint, i * Uint, j * Uint + 11, i * Uint + 11);
					}
				}
			}
			break;
		case 4:
			if (bPause) {
				bPause = false;
				SetWindowText(hBtnPause, ButtonText[2]);
			}
			else {
				bPause = true;
				SetWindowText(hBtnPause, ButtonText[3]);
			}
			break;
		case 6:
		case 7:
		case 8:
			speed = LOWORD(wParam) - 6;
			CheckRadioButton(hWnd, 6, 8, LOWORD(wParam));
			if (bStart) {
				KillTimer(hWnd, 13);
				SetTimer(hWnd, 13, arrTime[speed], TimerProc);
			}
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		ReleaseDC(hWnd, hdc);
		break;
	case WM_CREATE:

		srand(unsigned(time(NULL)));
		bStart = false;
		bPause = false;
		hFont = CreateFont(-15, -7, 0, 0, 600, false, false, false, DEFAULT_CHARSET, OUT_CHARACTER_PRECIS, CLIP_CHARACTER_PRECIS, DEFAULT_QUALITY, FF_DONTCARE, TEXT("微软雅黑"));
		hPen = CreatePen(PS_SOLID, 1, RGB(95, 141, 89));
		hBtnStart = CreateWindow(TEXT("button"), ButtonText[0], WS_CHILD | WS_VISIBLE, 1020, 20, 60, 30, hWnd, (HMENU)2, hInst, NULL);
		SendMessage(hBtnStart, WM_SETFONT, (WPARAM)hFont, NULL);
		hBtnRandom = CreateWindow(TEXT("button"), TEXT("随机"), WS_CHILD | WS_VISIBLE, 1100, 20, 60, 30, hWnd, (HMENU)3, hInst, NULL);
		SendMessage(hBtnRandom, WM_SETFONT, (WPARAM)hFont, NULL);
		hBtnPause = CreateWindow(TEXT("button"), ButtonText[2], WS_CHILD | WS_VISIBLE, 1180, 20, 60, 30, hWnd, (HMENU)4, hInst, NULL);
		SendMessage(hBtnPause, WM_SETFONT, (WPARAM)hFont, NULL);
		hStaticIteration = CreateWindow(TEXT("static"), TEXT("迭代速度"), WS_CHILD | WS_VISIBLE | SS_CENTER, 1020, 70, 220, 20, hWnd, (HMENU)5, hInst, NULL);
		SendMessage(hStaticIteration, WM_SETFONT, (WPARAM)hFont, NULL);
		hRadioLow = CreateWindow(TEXT("button"), TEXT("低速"), WS_CHILD | WS_VISIBLE | BS_LEFT | BS_AUTORADIOBUTTON | WS_GROUP, 1020, 100, 60, 30, hWnd, (HMENU)6, hInst, NULL);
		SendMessage(hRadioLow, WM_SETFONT, (WPARAM)hFont, NULL);
		hRadioMedium = CreateWindow(TEXT("button"), TEXT("中速"), WS_CHILD | WS_VISIBLE | BS_LEFT | BS_AUTORADIOBUTTON, 1080, 100, 60, 30, hWnd, (HMENU)7, hInst, NULL);
		SendMessage(hRadioMedium, WM_SETFONT, (WPARAM)hFont, NULL);
		hRadioHigh = CreateWindow(TEXT("button"), TEXT("高速"), WS_CHILD | WS_VISIBLE | BS_LEFT | BS_AUTORADIOBUTTON, 1140, 100, 60, 30, hWnd, (HMENU)8, hInst, NULL);
		SendMessage(hRadioHigh, WM_SETFONT, (WPARAM)hFont, NULL);

		EnableWindow(hBtnPause, false);
		CheckRadioButton(hWnd, 6, 8, 7);
		speed = 1;
		break;
	case WM_MOUSEMOVE:
		if (bStart) {
			break;
		}
		if (wParam & MK_LBUTTON || wParam & MK_RBUTTON) {
			xPos = HIWORD(lParam) / Uint;
			yPos = LOWORD(lParam) / Uint;
			if (xPos == prexPos && yPos == preyPos || xPos > 99 || yPos > 99) {
				break;
			}
			else {
				prexPos = xPos;
				preyPos = yPos;
			}
		}
		else {
			break;
		}
	case WM_LBUTTONDOWN:
	case WM_RBUTTONDOWN:
		if (bStart) {
			break;
		}
		xPos = HIWORD(lParam) / Uint;
		yPos = LOWORD(lParam) / Uint;
		if (xPos > 99 || yPos > 99) {
			break;
		}
		prexPos = xPos;
		preyPos = yPos;
		hdc = GetDC(hWnd);
		SelectObject(hdc, hPen);
		if (message == WM_LBUTTONDOWN || wParam&MK_LBUTTON)
		{
			arrWorld[xPos][yPos] = true;
			SelectObject(hdc, GetStockObject(BLACK_BRUSH));
			Rectangle(hdc, yPos * Uint, xPos * Uint, yPos * Uint + 11, xPos * Uint + 11);
		}
		else
		{
			arrWorld[xPos][yPos] = false;
			SelectObject(hdc, GetStockObject(WHITE_BRUSH));
			Rectangle(hdc, yPos * Uint, xPos * Uint, yPos * Uint + 11, xPos * Uint + 11);
		}
		ReleaseDC(hWnd, hdc);
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		SelectObject(hdc, hFont);
		SetBkColor(hdc, GetSysColor(COLOR_BTNFACE));
		wsprintf(str, TEXT("迭代次数: 0                "));
		rect = { 1020, 150, 1320, 200 };
		DrawText(hdc, str , -1, &rect,DT_SINGLELINE);
		wsprintf(str, TEXT("元胞数量: 0                "));
		rect = { 1020, 190, 1320, 240 };
		DrawText(hdc, str, -1, &rect, DT_SINGLELINE);
		wsprintf(str, TEXT("软件名称: 生命游戏"));
		rect = { 1020, 860, 1170, 890 };
		DrawText(hdc, str, -1, &rect, DT_SINGLELINE);
		wsprintf(str, TEXT("软件版本: 1.0"));
		rect = { 1020, 890, 1170, 920 };
		DrawText(hdc, str, -1, &rect, DT_SINGLELINE);
		wsprintf(str, TEXT("软件作者: 李国良"));
		rect = { 1020, 920, 1170, 950 };
		DrawText(hdc, str, -1, &rect, DT_SINGLELINE);
		wsprintf(str, TEXT("2017年7月4日"));
		rect = { 1150, 970, 1300, 1000 };
		DrawText(hdc, str, -1, &rect, DT_SINGLELINE);
		SelectObject(hdc, hPen);
		SelectObject(GetDC(hWnd), GetStockObject(WHITE_BRUSH));
		if (!bStart) {
			Rectangle(hdc, 0, 0, 1001, 1001);
			for (int i = 0; i <= 1000; i = i + Uint) {
				MoveToEx(hdc, i, 0, NULL);
				LineTo(hdc, i, 1000);
				MoveToEx(hdc, 0, i, NULL);
				LineTo(hdc, 1000, i);
			}
			for (int i = 0; i < 100; ++i) {
				for (int j = 0; j < 100; ++j) {
					if (arrWorld[i][j]) {
						SelectObject(hdc, GetStockObject(BLACK_BRUSH));
						Rectangle(hdc, j * Uint, i * Uint, j * Uint + 11, i * Uint + 11);
					}
				}
			}
		}
		else {
			SelectObject(hdc, GetStockObject(WHITE_PEN));
			for (int i = 0; i < 100; ++i) {
				for (int j = 0; j < 100; ++j) {
					if (arrWorld[i][j]) {
						SelectObject(hdc, GetStockObject(BLACK_BRUSH));
						Rectangle(hdc, j * Uint, i * Uint, j * Uint + 11, i * Uint + 11);
					}
					else {
						SelectObject(hdc, GetStockObject(WHITE_BRUSH));
						Rectangle(hdc, j * Uint, i * Uint, j * Uint + 11, i * Uint + 11);
					}
				}
			}
		}
		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
		DeleteObject(hPen);
		DeleteObject(hFont);
		PostQuitMessage(0);
		break;
	}
	return DefWindowProc(hWnd, message, wParam, lParam);
}

VOID CALLBACK TimerProc(HWND hWnd, UINT message, UINT_PTR id, DWORD dwTime) {
	if (bPause) {
		return;
	}
	iCell = Live();
	iAge++;
	HDC hdc;
	RECT rect;
	hdc = GetDC(hWnd);
	static HFONT hFont;
	hFont = CreateFont(-15, -7, 0, 0, 600, false, false, false, DEFAULT_CHARSET, OUT_CHARACTER_PRECIS, CLIP_CHARACTER_PRECIS, DEFAULT_QUALITY, FF_DONTCARE, TEXT("微软雅黑"));
	SelectObject(hdc, hFont);
	SetBkColor(hdc, GetSysColor(COLOR_BTNFACE));
	SelectObject(hdc, GetStockObject(WHITE_PEN));
	for (int i = 0; i < 100; ++i) {
		for (int j = 0; j < 100; ++j) {
			if (arrWorld[i][j]) {
				SelectObject(hdc, GetStockObject(BLACK_BRUSH));
				Rectangle(hdc, j * Uint, i * Uint, j * Uint + 11, i * Uint + 11);
			}
			else {
				SelectObject(hdc, GetStockObject(WHITE_BRUSH));
				Rectangle(hdc, j * Uint, i * Uint, j * Uint + 11, i * Uint + 11);
			}
		}
	}
	rect = { 1094, 150, 1194, 180 };
	wsprintf(str, TEXT("%d"), iAge);
	DrawText(hdc, str, -1, &rect, DT_SINGLELINE);
	rect = { 1094, 190, 1194, 220 };
	wsprintf(str, TEXT("%d"), iCell);
	DrawText(hdc, str, -1, &rect, DT_SINGLELINE);
	ReleaseDC(hWnd, hdc);
}

int Live()
{
	int numLife = 0;
	bool arrTemp[100][100];
	for (int i = 0; i < 100; ++i) {
		for (int j = 0; j < 100; ++j) {
			if (i - 1 >= 0 && j - 1 >= 0 && arrWorld[i - 1][j - 1])
				++numLife;
			if (i - 1 >= 0 && arrWorld[i - 1][j])
				++numLife;
			if (i - 1 >= 0 && j + 1 < 100 && arrWorld[i - 1][j + 1])
				++numLife;
			if (j - 1 >= 0 && arrWorld[i][j - 1])
				++numLife;
			if (j + 1 < 100 && arrWorld[i][j + 1])
				++numLife;
			if (i + 1 < 100 && j - 1 >= 0 && arrWorld[i + 1][j - 1])
				++numLife;
			if (i + 1 < 100 && arrWorld[i + 1][j])
				++numLife;
			if (i + 1 < 100 && j + 1 < 100 && arrWorld[i + 1][j + 1])
				++numLife;
			if (numLife == 3) {
				arrTemp[i][j] = true;
			}
			else if (numLife == 2) {
				arrTemp[i][j] = arrWorld[i][j];
			}
			else {
				arrTemp[i][j] = false;
			}
			numLife = 0;
		}
	}
	for (int i = 0; i < 100; ++i) {
		for (int j = 0; j < 100; ++j) {
			arrWorld[i][j] = arrTemp[i][j];
			if (arrWorld[i][j]) {
				++numLife;
			}
		}
	}
	return numLife;
}