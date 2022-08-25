#include <windows.h>

#define FILE_MENU_NEW 1
#define FILE_MENU_OPEN 2
#define FILE_MENU_EXIT 3
#define CHANGE_TITLE 4

void AddMenu(HWND);

void AddControls(HWND);

LRESULT CALLBACK WindowProcedure(HWND, UINT, WPARAM, LPARAM);

HWND hEdit;
HWND hStatic;

int CALLBACK WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInst, _In_ LPSTR args, _In_ int ncmdshow)
{
	WNDCLASSA wc = { 0 };
	wc.hInstance = hInstance;
	wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
	wc.lpfnWndProc = WindowProcedure;
	wc.hCursor = LoadCursor(hInstance, IDC_ARROW);
	wc.lpszClassName = "myWindowClass";

	if (!RegisterClassA(&wc))
		return -1;

	CreateWindowA("myWindowClass", "My Window", WS_VISIBLE | WS_OVERLAPPEDWINDOW, 100, 100, 400, 400, NULL, NULL, NULL, NULL);

	MSG msg = { 0 };
	while (GetMessageA(&msg, NULL, NULL, NULL))
	{
		TranslateMessage(&msg);
		DispatchMessageA(&msg);
	}

	return 0;
}

LRESULT CALLBACK WindowProcedure(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp)
{
	switch (msg)
	{
	case WM_COMMAND:
		switch (wp)
		{
		case FILE_MENU_EXIT:
			DestroyWindow(hWnd);
			break;
		case FILE_MENU_OPEN:
			break;
		case FILE_MENU_NEW:
			break;
		case CHANGE_TITLE:
			char text[100];
			GetWindowTextA(hEdit, text, 100);
			SetWindowTextA(hStatic, text);
			break;
		}
		break;
	case WM_CREATE:
		AddMenu(hWnd);
		AddControls(hWnd);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProcA(hWnd, msg, wp, lp);
	}
}

void AddMenu(HWND hWnd)
{
	HMENU hMenu = CreateMenu();
	HMENU hFileMenu = CreateMenu();
	HMENU hSubMenu = CreateMenu();

	AppendMenuA(hMenu, MF_POPUP, (UINT_PTR)hFileMenu, "File");
	AppendMenuA(hMenu, MF_STRING, NULL, "Help");

	AppendMenuA(hFileMenu, MF_STRING, FILE_MENU_NEW, "New");
	AppendMenuA(hFileMenu, MF_POPUP, (UINT_PTR)hSubMenu, "Open SubMenu");
	AppendMenuA(hFileMenu, MF_SEPARATOR, NULL, NULL);
	AppendMenuA(hFileMenu, MF_STRING, FILE_MENU_EXIT, "Exit");

	AppendMenuA(hSubMenu, MF_STRING, CHANGE_TITLE, "Change Title");

	SetMenu(hWnd, hMenu);
}

void AddControls(HWND hWnd)
{
	hStatic = CreateWindowA("static", "Enter text here :", WS_VISIBLE | WS_CHILD | WS_BORDER | SS_CENTER, 100, 50, 100, 50, hWnd, NULL, NULL, NULL);
	hEdit = CreateWindowA("edit", "...", WS_VISIBLE | WS_CHILD | WS_BORDER | ES_MULTILINE | ES_AUTOVSCROLL | ES_AUTOHSCROLL, 100, 102, 100, 50, hWnd, NULL, NULL, NULL);

}