﻿#define _CRT_SECURE_NO_WARNINGS
#include <windows.h> 
#include <fstream>
#define MAX_SIZE 32
#define MAX_STR_SIZE 2048
HANDLE hBitmap;

LRESULT CALLBACK SoftwareMainProcedure(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp);
WNDCLASS NewWindowClass(HBRUSH BGColor, HCURSOR Cursor, HINSTANCE hInst, HICON Icon, LPCWSTR Name, WNDPROC Procedure);
static void AlternateSize(HDC hdc, wchar_t** str, int width, int height);
void RefillMatrix();
const int WHEIGHT = 750, WWIDTH = 1000;
static int prevH = 0, prevW = 0, horizontals = 5, verticals = 3, height, width;
static bool scaleChanged = false;
static wchar_t* stringMatrix[MAX_SIZE][MAX_SIZE];
static wchar_t allTexts[MAX_SIZE * MAX_SIZE][MAX_STR_SIZE];
const wchar_t* EMPTY = L"";




int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR args, int ncmdshow)
{
	WNDCLASS SoftwareMainClass = NewWindowClass((HBRUSH)COLOR_WINDOW, LoadCursor(NULL, IDC_ARROW), hInst, LoadIcon(NULL, NULL),
		L"MainWindowClass", SoftwareMainProcedure);

	if (!RegisterClassW(&SoftwareMainClass)) { return -1; }
	MSG SoftwareMainMessage = { 0 };
																		//Cordinates and size
	CreateWindow(L"MainWindowClass", L"LAB 2", WS_OVERLAPPEDWINDOW | WS_VISIBLE, 50, 50, WWIDTH, WHEIGHT, NULL, NULL, NULL, NULL);

	for (int i = 0; i < MAX_SIZE * MAX_SIZE; i++)
		lstrcpy(allTexts[i],(wchar_t*)L"");

	FILE* input;
	if (fopen_s(&input, "input.txt", "r") == 0)
	{
		int i = 0;
		while (!feof(input))
		{
			char str[MAX_STR_SIZE];
			fgets(str, MAX_STR_SIZE, input);
			if (!feof(input))
				mbstowcs(allTexts[i], str, strlen(str) - 1);
			i++;
		}
	}

	for (int i = 0; i < MAX_SIZE; i++)
	{
		for (int j = 0; j < MAX_SIZE; j++)
			stringMatrix[i][j] = (wchar_t*)L"";
	}
	RefillMatrix();


	while (GetMessage(&SoftwareMainMessage, NULL, NULL, NULL))
	{
		TranslateMessage(&SoftwareMainMessage);
		DispatchMessage(&SoftwareMainMessage);
	}
	return 0;
}

WNDCLASS NewWindowClass(HBRUSH BGColor, HCURSOR Cursor, HINSTANCE hInst, HICON Icon, LPCWSTR Name, WNDPROC Procedure)
{
	WNDCLASS NWC = { 0 };
	NWC.hIcon = Icon;
	NWC.hCursor = Cursor;
	NWC.hInstance = hInst;
	NWC.lpszClassName = Name;
	NWC.hbrBackground = BGColor;
	NWC.lpfnWndProc = Procedure;

	return NWC;
}

LRESULT CALLBACK SoftwareMainProcedure(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp)
{
	HDC hdc, hmdc;
	PAINTSTRUCT ps;
	static bool mouseClick = false;
	RECT winRect;
	GetClientRect(hWnd, &winRect);


	switch (msg) {
	case WM_CREATE:
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		height = (winRect.bottom - winRect.top) / verticals;
		width = (winRect.right - winRect.left) / horizontals;
		for (int i = 0; i < verticals; i++)
			for (int j = 0; j < horizontals; j++)
			{
				RECT rect;
				rect.left = j * width;
				rect.top = i * height;
				rect.right = j * width + width;
				rect.bottom = i * height + height;
				Rectangle(hdc, rect.left, rect.top, rect.right, rect.bottom);
				AlternateSize(hdc, &stringMatrix[i][j], width, height);
				rect.top += height / 5;
				DrawText(hdc, stringMatrix[i][j], lstrlen(stringMatrix[i][j]), &rect, DT_CENTER);

			}
		EndPaint(hWnd, &ps);
		break;
	case WM_SIZE:
	{
		InvalidateRect(hWnd, 0, true);
		break;
	}
	case WM_KEYDOWN:
	{
		switch (wp)
		{
		case VK_LEFT:
		{
			if (horizontals > 1)
				horizontals--;
			break;
		}
		case VK_RIGHT:
		{
			horizontals++;
			break;
		}
		case VK_UP:
		{
			if (verticals > 1)
				verticals--;
			break;
		}
		case VK_DOWN:
		{
			verticals++;
			break;
		}
		}
		scaleChanged = true;
		RefillMatrix();
		InvalidateRect(hWnd, 0, true);
	}

	case WM_LBUTTONDOWN: {
		mouseClick = true;
		HINSTANCE hInst = (HINSTANCE)GetWindowLong(hWnd, NULL);
		CreateWindow(L"TextWindowClass", L"Input text", WS_OVERLAPPEDWINDOW | WS_VISIBLE, 50, 50, 500, 500, hWnd, NULL, hInst, NULL);
		break;
	}
	default: return DefWindowProc(hWnd, msg, wp, lp);
	}
}

static void AlternateSize(HDC hdc, wchar_t** str, int width, int height)
{
	SIZE size;
	wchar_t* result = new wchar_t(256);
	//wchar_t* result = (wchar_t*)L"";
	//lstrcpy(result, *str);
	GetTextExtentPoint32(hdc, *str, lstrlen(*str), &size);
	if (size.cx >= width)
	{
		/*for (int i = lstrlen(*str) - 1; i > 0; i--)
		{
			if (result[i] == (wchar_t)32)
			{
				result[i] = (wchar_t)L"\r\n";
				break;
			}
		}*/
	}
	//*str = result;
}

void RefillMatrix()
{
	for (int i = 0; i < verticals; i++)
		for (int j = 0; j < horizontals; j++)
		{
			stringMatrix[i][j] = allTexts[i * horizontals + j];
		}
}