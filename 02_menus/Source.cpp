#include <windows.h>

#define FILE_MENU_NEW 1
#define FILE_MENU_OPEN 2
#define FILE_MENU_EXIT 3

LRESULT CALLBACK WindowProcedure(HWND, UINT, WPARAM, LPARAM);

HMENU hMenu;
void AddMenu(HWND);

int CALLBACK WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR args, int ncmdshow)
{
	WNDCLASSA wc = { 0 };
	wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
	wc.hCursor = LoadCursor(hInst, IDC_ARROW);
	wc.hInstance = hInst;
	wc.lpszClassName = "myWindowClass";
	wc.lpfnWndProc = WindowProcedure;
	if (!RegisterClassA(&wc))
		return -1;

	CreateWindowA("myWindowClass", "my Window", WS_VISIBLE | WS_OVERLAPPEDWINDOW, 100, 100, 300, 300, NULL, NULL, NULL, NULL);

	MSG msg = { 0 };
	while (GetMessageA(&msg, NULL, NULL, NULL))
	{
		TranslateMessage(&msg);
		DispatchMessageA(&msg);
	}

	return 0;
}

LRESULT CALLBACK WindowProcedure(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp) {
	switch (msg)
	{
	case WM_COMMAND:
		switch (wp)
		{
		case FILE_MENU_EXIT:
			DestroyWindow(hWnd);
			break;
		case FILE_MENU_NEW:
			MessageBeep(MB_OK);
			break;
		case FILE_MENU_OPEN:
			MessageBeep(MB_ICONINFORMATION);
			break;
		default:
			break;
		}
		break;
	case WM_CREATE:				// call when the window is created
		AddMenu(hWnd);
		break;
	case WM_DESTROY:			// call when the window is closed (press the cross button)
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProcA(hWnd, msg, wp, lp);
	}
}

void AddMenu(HWND hWnd)
{
	hMenu = CreateMenu();
	HMENU hFileMenu = CreateMenu();
	HMENU hSubMenu = CreateMenu();

	AppendMenu(hSubMenu, MF_STRING, NULL, L"SubMenu Item");

	AppendMenu(hFileMenu, MF_STRING, FILE_MENU_NEW, L"New");
	AppendMenu(hFileMenu, MF_POPUP, (UINT_PTR)hSubMenu, L"Open SubMenu");
	AppendMenu(hFileMenu, MF_SEPARATOR, NULL, NULL);
	AppendMenu(hFileMenu, MF_STRING, FILE_MENU_EXIT, L"Exit");


	AppendMenu(hMenu, MF_POPUP, (UINT_PTR)hFileMenu, L"File");
	AppendMenu(hMenu, MF_STRING, NULL, L"Help");

	SetMenu(hWnd, hMenu);
}