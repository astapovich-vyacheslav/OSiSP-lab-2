#include <windows.h> 
HANDLE hBitmap;

LRESULT CALLBACK SoftwareMainProcedure(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp);
WNDCLASS NewWindowClass(HBRUSH BGColor, HCURSOR Cursor, HINSTANCE hInst, HICON Icon, LPCWSTR Name, WNDPROC Procedure);
static int x = 50, y = 50, dx = 20, dy = 20;
const int WHEIGHT = 750, WWIDTH = 1000;
static int bmHeight = 50, bmWidth = 50;


int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR args, int ncmdshow)
{
	WNDCLASS SoftwareMainClass = NewWindowClass((HBRUSH)COLOR_WINDOW, LoadCursor(NULL, IDC_ARROW), hInst, LoadIcon(NULL, NULL),
		L"MainWindowClass", SoftwareMainProcedure);

	if (!RegisterClassW(&SoftwareMainClass)) { return -1; }
	MSG SoftwareMainMessage = { 0 };
																		//Cordinates and size
	CreateWindow(L"MainWindowClass", L"LAB 1", WS_OVERLAPPEDWINDOW | WS_VISIBLE, 50, 50, WWIDTH, WHEIGHT, NULL, NULL, NULL, NULL);

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
	int horizontals = 5, verticals = 3; //TODO: read those from file
	static bool mouseClick = false;

	RECT winRect;
	GetClientRect(hWnd, &winRect);
	int height, width;


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
				Rectangle(hdc, j * width, i * height, j * width + width, i * height + height);
			}
		InvalidateRect(hWnd, 0, true);
		break;
	case WM_KEYDOWN:
	{
		switch (wp)
		{
		case VK_LEFT:
		{
			x -= dx;
			break;
		}
		case VK_RIGHT:
		{
			x += dx;
			break;
		}
		case VK_UP:
		{
			y -= dy;
			break;
		}
		case VK_DOWN:
		{
			y += dy;
			break;
		}
		}
		InvalidateRect(hWnd, 0, true);
	}
	case WM_MOUSEWHEEL:
	{
		int delta = (short)HIWORD(wp) / 5;
		if (LOWORD(wp) == MK_SHIFT) {
			x += delta;
		}
		else {
			y -= delta;
		}
		InvalidateRect(hWnd, 0, TRUE);
		break;
	}
	case WM_LBUTTONDOWN: {
		mouseClick = true;
		break;
	}
	case WM_LBUTTONUP: {
		mouseClick = false;
		break;
	}
	case WM_MOUSEMOVE: {
		if (mouseClick && GetPixel(GetDC(hWnd), LOWORD(lp), HIWORD(lp)) != COLOR_WINDOW) {
			x = LOWORD(lp) - 50;
			y = HIWORD(lp) - 50;
			InvalidateRect(hWnd, 0, TRUE);
		}
		break;
	}

	default: return DefWindowProc(hWnd, msg, wp, lp);
	}
}