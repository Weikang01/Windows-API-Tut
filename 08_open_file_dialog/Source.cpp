#include <windows.h>
#include <stdlib.h>
#include <stdio.h>

#define FILE_MENU_NEW 1
#define FILE_MENU_OPEN 2
#define FILE_MENU_EXIT 3
#define GENERATE_BUTTON 4
#define OPEN_FILE 5


void AddMenu(HWND);

void AddControls(HWND);

LRESULT CALLBACK WindowProcedure(HWND, UINT, WPARAM, LPARAM);

void registerDialogClass(HINSTANCE);

void displayDialog(HWND);

void LoadImages();

HWND hWindow;
HWND hStatic;
HWND hButton;

HWND hName, hAge, hOutput, hLogo, hFile;
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

	registerDialogClass(hInstance);

	hWindow = CreateWindowA("myWindowClass", "My Window", WS_VISIBLE | WS_OVERLAPPEDWINDOW, 100, 100, 500, 700, NULL, NULL, NULL, NULL);


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

void displayFile(LPSTR path)
{
	FILE* file;
	file = fopen(path, "r");
	fseek(file, 0, SEEK_END);
	int _size = ftell(file);
	rewind(file);
	char* data = new char[_size + 1];
	fread(data, _size, 1, file);
	data[_size] = '\0';

	SetWindowTextA(hFile, data);

	fclose(file);
}

void openFile(HWND hWnd)
{
	OPENFILENAMEA ofn;
	ZeroMemory(&ofn, sizeof(OPENFILENAMEA));

	char file_name[100];
	ofn.lStructSize = sizeof(OPENFILENAMEA);
	ofn.hwndOwner = hWnd;
	ofn.lpstrFile = file_name;
	ofn.lpstrFile[0] = '\0';
	ofn.nMaxFile = 100;
	ofn.lpstrFilter = "All Files(*.*)\0*.*\0Source Files(*.cpp)\0*.cpp\0Text Files(*.txt)\0*.txt\0";
	ofn.nFilterIndex = 1; // All Files

	GetOpenFileNameA(&ofn);

	//MessageBoxA(NULL, ofn.lpstrFile, "", MB_OK);
	displayFile(ofn.lpstrFile);
}

int msgBoxVal = 0;
LRESULT CALLBACK WindowProcedure(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp)
{
	switch (msg)
	{
	case WM_COMMAND:
		switch (wp)
		{
		case FILE_MENU_EXIT:
			/*
			* styles of MessageBox
			MB_ABORTRETRYIGNORE
			MB_CANCELTRYCONTINUE
			MB_HELP
			MB_OK
			MB_OKCANCEL
			MB_RETRYCANCEL
			MB_YESNO
			MB_YESNOCANCEL
			*/
			/*
			* icons of MessageBox
			MB_ICONEXCLAMATION
			MB_ICONINFORMATION
			MB_ICONQUESTION
			MB_ICONSTOP
			MB_ICONERROR
			MB_ICONWARNING
			MB_ICONASTERISK
			*/
			msgBoxVal = MessageBoxA(hWnd, "Are you sure?", "Wait!", MB_YESNO | MB_ICONEXCLAMATION);
			/*
			* msgBoxVal values
			IDABORT		= 3
			IDCANCEL	= 2
			IDCONTINUE	= 11
			IDIGNORE	= 5
			IDNO		= 7
			IDOK		= 1
			IDRETRY		= 4
			IDTRYAGAIN	= 10
			IDYES		= 6
			*/
			if (msgBoxVal == IDYES)
				DestroyWindow(hWnd);
			break;
		case FILE_MENU_OPEN:
			break;
		case FILE_MENU_NEW:
			displayDialog(hWnd);
			break;
		case GENERATE_BUTTON:
			char name[30], age[10], out[50];
			GetWindowTextA(hName, name, 30);
			GetWindowTextA(hAge, age, 10);

			if (!strcmp("", name) && !strcmp("", age))
			{
				msgBoxVal = MessageBoxA(hWnd, "You did not enter anything!", NULL, MB_ABORTRETRYIGNORE);
				switch (msgBoxVal)
				{
				case IDABORT:
					DestroyWindow(hWnd);
				case IDRETRY:
					return 0;
				case IDIGNORE:
					break;
				}
			}

			strcpy_s(out, name);
			strcat_s(out, " is ");
			strcat_s(out, age);
			strcat_s(out, " years old.");

			SetWindowTextA(hOutput, out);
			break;
		case OPEN_FILE:
			openFile(hWnd);
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

	CreateWindowA("Button", "Open File", WS_VISIBLE | WS_CHILD | WS_TABSTOP, 150, 380, 98, 38, hWnd, (HMENU)OPEN_FILE, NULL, NULL);

	hFile = CreateWindowA("Edit", NULL, WS_VISIBLE | WS_CHILD | WS_BORDER | ES_MULTILINE | ES_AUTOVSCROLL, 100, 430, 300, 200, hWnd, NULL, NULL, NULL);
}

void LoadImages()
{
	hLogoImage = (HBITMAP)LoadImageA(NULL, "Images\\snail.bmp", IMAGE_BITMAP, 100, 100, LR_LOADFROMFILE);
	hGenerateImage = (HBITMAP)LoadImageA(NULL, "Images\\generate.bmp", IMAGE_BITMAP, 98, 38, LR_LOADFROMFILE);
}

LRESULT CALLBACK DialogProcedure(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp)
{
	switch (msg)
	{
	case WM_COMMAND:
		switch (wp)
		{
		case 1:
			EnableWindow(hWindow, TRUE);
			DestroyWindow(hWnd);
			break;
		}
	case WM_CLOSE:
		DestroyWindow(hWnd);
		break;
	default:
		return DefWindowProcA(hWnd, msg, wp, lp);
	}
	return 0;
}

void registerDialogClass(HINSTANCE hInstance)
{
	WNDCLASSA dialog = { 0 };

	dialog.hbrBackground = (HBRUSH)COLOR_WINDOW;
	dialog.hInstance = hInstance;
	dialog.lpfnWndProc = DialogProcedure;
	dialog.hCursor = LoadCursor(NULL, IDC_CROSS);
	dialog.lpszClassName = "myDialogClass";

	RegisterClassA(&dialog);
}

void displayDialog(HWND hWnd)
{
	HWND hDlg = CreateWindowA("myDialogClass", "Dialog", WS_VISIBLE | WS_OVERLAPPEDWINDOW, 400, 400, 200, 200, hWnd, NULL, NULL, NULL);

	CreateWindowA("Button", "Close", WS_VISIBLE | WS_CHILD, 20, 20, 100, 40, hDlg, (HMENU)1, NULL, NULL);
	EnableWindow(hWnd, FALSE);
}