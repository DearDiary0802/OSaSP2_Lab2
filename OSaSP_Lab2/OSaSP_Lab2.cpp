#ifndef UNICODE
#define UNICODE
#endif // UNICODE

#include <Windows.h>
#include <vector>
#include <string>
#include <fstream>

using namespace std;

const int columnNumber = 5;
const int rowNumber = 3;

const int MAX_STRINGS = 14;

const int offset = 5;

const wchar_t text[MAX_STRINGS][255] = {
	L"Ты не считай друзей на перечет! Не тот твой друг, кем любопытство движет, А тот, кто с радостью с тобой разделит взлет. И кто в беде... твой тихий плач... услышит.",
	L"Я думаю, что лучше одиноким быть, Чем жар души кому-нибудь дарить. Бесценный дар отдав кому попало, Родного встретив, не сумеешь полюбить.",
	L"Мы все спешим, не успеваем Не долюбив, стараемся забыть. И все на завтра оставляем, а завтра может и не быть...",
	L"Зорко одно лишь сердце. Самого главного глазами не увидишь.",
	L"Ошибки - это знаки препинания жизни, без которых, как и в тексте, не будет смысла.",
	L"Жизнь коротка. Нет времени оставлять важные слова несказанными.",
	L"Ничто не может затмить свет, сияющий изнутри.",
	L"Есть преступления хуже, чем сжигать книги. Например, не читать их.",
	L"Трудно найти слова, когда действительно есть, что сказать.",
	L"Люди, полагающие, что они никому не нужны, на самом деле часто самые нужные.",
	L"Не позволяйте шуму чужих мнений перебить голос вашего сердца.",
	L"Мы так боимся быть навязчивыми, что кажемся равнодушными.",
	L"У нас есть все, чтобы быть счастливыми, но мы несчастны.",
	L"Дни жизни даже горькие цени, ведь навсегда уходят и они."
};

HPEN hPen = CreatePen(PS_SOLID, 3, RGB(0, 0, 128));
HFONT hFont = CreateFont(20, 10, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, DEFAULT_CHARSET, 
	                     OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, FF_ROMAN, (LPCWSTR)"Times New Roman");

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
void DrawTable(HDC hDC, int windowWidth, int windowHeight);
void InitDC(HWND hWnd, int windowWidth, int windowHeight);

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR pCmdLine, int nCmdShow)
{
	WNDCLASSEX wcex;
	HWND hWnd;
	MSG msg;

	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_VREDRAW | CS_HREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wcex.hCursor = LoadCursorW(NULL, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = L"SecondLab";
	wcex.hIconSm = wcex.hIcon;

	if (!RegisterClassEx(&wcex))
	{
		return 0;
	}

	hWnd = CreateWindowEx(0, L"SecondLab", L"Second lab", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT,
		CW_USEDEFAULT, CW_USEDEFAULT, NULL, NULL, hInstance, NULL);

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	static int width, height;

	switch (uMsg)
	{
	case WM_SIZE:
		width = LOWORD(lParam);
		height = HIWORD(lParam);
		break;
	case WM_PAINT:
		InitDC(hWnd, width, height);
		break;
	case WM_DESTROY:
		DeleteObject(hPen);
		DeleteObject(hFont);
		PostQuitMessage(0);
		break;
	}
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

void DrawVerticalBorders(HDC hDC, int windowWidth, int windowHeight)
{
	int columnWidth = windowWidth / columnNumber; 

	for (int i = 0; i < columnNumber; i++)
	{
		MoveToEx(hDC, i * columnWidth, 0, NULL);
		LineTo(hDC, i * columnWidth, windowHeight);
	}
	MoveToEx(hDC, windowWidth - 1, 0, NULL);
	LineTo(hDC, windowWidth - 1, windowHeight);
}

void DrawHorizontalBorders(HDC hDC, int windowWidth, int rowHeight)
{
	MoveToEx(hDC, 0, rowHeight, NULL);
	LineTo(hDC, windowWidth, rowHeight);
}

void DrawTable(HDC hDC, int windowWidth, int windowHeight)
{
	int columnWidth = windowWidth / columnNumber;

	DrawHorizontalBorders(hDC, windowWidth, 1);

	int i = 0;
	int textHeight = 0;
	while (i < MAX_STRINGS)
	{
		RECT rect = { 0, 0, 0, 0 };
		rect.top += textHeight;

		int maxTextHeight = 0;
		for (int j = 0; (j < columnNumber) && (i < MAX_STRINGS); j++)
		{
			const wchar_t* str = text[i];
			i++;
			
			rect.left = j * columnWidth + offset;
			rect.right = (j + 1) * columnWidth - offset;

			int currentTextHeight = DrawText(hDC, str, wcslen(str), &rect, DT_VCENTER | DT_EDITCONTROL | DT_WORDBREAK | DT_NOCLIP);
			if (maxTextHeight < currentTextHeight)
				maxTextHeight = currentTextHeight;
		}
		textHeight += maxTextHeight;
		DrawHorizontalBorders(hDC, windowWidth, textHeight);
	}
	DrawVerticalBorders(hDC, windowWidth, textHeight);
}

void InitDC(HWND hWnd, int windowWidth, int windowHeight)
{
	RECT rect;
	HDC memDC;
	HBITMAP hBmp, hOldBmp;
	PAINTSTRUCT ps;

	HDC currDC = BeginPaint(hWnd, &ps);

	GetClientRect(hWnd, &rect);
	memDC = CreateCompatibleDC(currDC);
	hBmp = CreateCompatibleBitmap(currDC, rect.right - rect.left, rect.bottom - rect.top);
	hOldBmp = (HBITMAP)SelectObject(memDC, hBmp);

	HBRUSH hBkgrndBrush = CreateSolidBrush(RGB(239, 228, 176));
	FillRect(memDC, &rect, hBkgrndBrush);
	DeleteObject(hBkgrndBrush);

	SetBkMode(memDC, TRANSPARENT);

	SelectObject(memDC, hPen);
	SetTextColor(memDC, RGB(0, 0, 128));

	SelectObject(memDC, hFont);

	DrawTable(memDC, rect.right - rect.left, rect.bottom - rect.top);
	BitBlt(currDC, rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top, memDC, 0, 0, SRCCOPY);

	SelectObject(memDC, hOldBmp);

	DeleteDC(memDC);

	EndPaint(hWnd, &ps);
}