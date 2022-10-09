#include <windows.h> 
HANDLE hBitmap;

LRESULT CALLBACK SoftwareMainProcedure(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp);
WNDCLASS NewWindowClass(HBRUSH BGColor, HCURSOR Cursor, HINSTANCE hInst, HICON Icon, LPCWSTR Name, WNDPROC Procedure);
static int x = 50, y = 50, dx = 20, dy = 20;
const int WHEIGHT = 750, WWIDTH = 1000;
static int prevH = 0, prevW = 0, horizontals = 5, verticals = 3, height, width;
static bool scaleChanged = false;
static wchar_t*** stringMatrix;
const wchar_t* EMPTY = L" ";


int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR args, int ncmdshow)
{
	WNDCLASS SoftwareMainClass = NewWindowClass((HBRUSH)COLOR_WINDOW, LoadCursor(NULL, IDC_ARROW), hInst, LoadIcon(NULL, NULL),
		L"MainWindowClass", SoftwareMainProcedure);

	if (!RegisterClassW(&SoftwareMainClass)) { return -1; }
	MSG SoftwareMainMessage = { 0 };
																		//Cordinates and size
	CreateWindow(L"MainWindowClass", L"LAB 2", WS_OVERLAPPEDWINDOW | WS_VISIBLE, 50, 50, WWIDTH, WHEIGHT, NULL, NULL, NULL, NULL);


	stringMatrix = new wchar_t** [verticals];
	for (int i = 0; i < verticals; i++)
	{
		stringMatrix[i] = new wchar_t*[horizontals];
		for (int j = 0; j < horizontals; j++)
			stringMatrix[i][j] = (wchar_t*)EMPTY;
	}

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
				Rectangle(hdc, j * width, i * height, j * width + width, i * height + height);
			}
		if (height != prevH || width != prevW || scaleChanged)
		{
			InvalidateRect(hWnd, 0, true);
			scaleChanged = false;
		}
		prevH = height;
		prevW = width;
		break;
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