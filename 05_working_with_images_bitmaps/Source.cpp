#include <windows.h>
#include <stdlib.h>

#define FILE_MENU_NEW 1
#define FILE_MENU_OPEN 2
#define FILE_MENU_EXIT 3
#define GENERATE_BUTTON 4


void AddMenu(HWND);

void AddControls(HWND);

LRESULT CALLBACK WindowProcedure(HWND, UINT, WPARAM, LPARAM);

void LoadImages();

HWND hWindow;
HWND hStatic;
HWND hButton;

HWND hName, hAge, hOutput, hLogo;
HBITMAP hLogoImage, hGenerateImage;


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

	hWindow = CreateWindowA("myWindowClass", "My Window", WS_VISIBLE | WS_OVERLAPPEDWINDOW, 100, 100, 500, 500, NULL, NULL, NULL, NULL);

	MSG msg = { 0 };
	while (GetMessageA(&msg, NULL, NULL, NULL))
	{
		if (!IsDialogMessageA(hWindow, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessageA(&msg);
		}
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
		case GENERATE_BUTTON:
			char name[30], age[10], out[50];
			GetWindowTextA(hName, name, 30);
			GetWindowTextA(hAge, age, 10);

			strcpy_s(out, name);
			strcat_s(out, " is ");
			strcat_s(out, age);
			strcat_s(out, " years old.");

			SetWindowTextA(hOutput, out);
			break;
		}
		break;
	case WM_CREATE:
		LoadImages();
		AddMenu(hWnd);
		AddControls(hWnd);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProcA(hWnd, msg, wp, lp);
	}
	return 0;
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

	SetMenu(hWnd, hMenu);
}

void AddControls(HWND hWnd)
{
	CreateWindowA("Static", "Name :", WS_VISIBLE | WS_CHILD, 100, 50, 98, 38, hWnd, NULL, NULL, NULL);
	hName = CreateWindowA("Edit", NULL, WS_VISIBLE | WS_CHILD | WS_BORDER | WS_TABSTOP, 200, 50, 98, 38, hWnd, NULL, NULL, NULL);

	CreateWindowA("Static", "Age :", WS_VISIBLE | WS_CHILD, 100, 90, 98, 38, hWnd, NULL, NULL, NULL);
	hAge = CreateWindowA("Edit", NULL, WS_VISIBLE | WS_CHILD | WS_BORDER | WS_TABSTOP, 200, 90, 98, 38, hWnd, NULL, NULL, NULL);

	HWND hBtn = CreateWindowA("Button", "Generate", WS_VISIBLE | WS_CHILD | WS_TABSTOP | BS_BITMAP, 150, 130, 98, 38, hWnd, (HMENU)GENERATE_BUTTON, NULL, NULL);
	SendMessageA(hBtn, BM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hGenerateImage);

	hOutput = CreateWindowA("Edit", NULL, WS_VISIBLE | WS_CHILD | WS_BORDER, 100, 170, 300, 200, hWnd, NULL, NULL, NULL);

	hLogo = CreateWindowA("Static", NULL, WS_VISIBLE | WS_CHILD | WS_BORDER | SS_BITMAP, 300, 50, 100, 100, hWnd, NULL, NULL, NULL);
	SendMessageA(hLogo, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hLogoImage);
}

void LoadImages()
{
	hLogoImage = (HBITMAP)LoadImageA(NULL, "Images\\snail.bmp", IMAGE_BITMAP, 100, 100, LR_LOADFROMFILE);
	hGenerateImage = (HBITMAP)LoadImageA(NULL, "Images\\generate.bmp", IMAGE_BITMAP, 98, 38, LR_LOADFROMFILE);
}