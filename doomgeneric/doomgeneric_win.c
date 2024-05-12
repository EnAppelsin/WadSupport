#include "doomkeys.h"

#include "doomgeneric.h"

#include <stdio.h>

#include <Windows.h>

#include <png.h>

static BITMAPINFO s_Bmi = { sizeof(BITMAPINFOHEADER), DOOMGENERIC_RESX, -DOOMGENERIC_RESY, 1, 32 };
static HWND s_Hwnd = 0;
static HDC s_Hdc = 0;
static HANDLE s_Hframe = 0;
static HDC s_HdcFrame = 0;
static HBITMAP s_HworkingBitmap = 0;
static HDC s_HdcWorking = 0;

// HWND Hijacking Stuff
char hwnd_oldTitle[256];
LONG_PTR hwnd_originalWndProc;

#define KEYQUEUE_SIZE 16

static unsigned short s_KeyQueue[KEYQUEUE_SIZE];
static unsigned int s_KeyQueueWriteIndex = 0;
static unsigned int s_KeyQueueReadIndex = 0;

static unsigned char convertToDoomKey(unsigned char key)
{
	switch (key)
	{
	case VK_RETURN:
		key = KEY_ENTER;
		break;
	case VK_ESCAPE:
		key = KEY_ESCAPE;
		break;
	case VK_LEFT:
		key = KEY_LEFTARROW;
		break;
	case VK_RIGHT:
		key = KEY_RIGHTARROW;
		break;
	case VK_UP:
		key = KEY_UPARROW;
		break;
	case VK_DOWN:
		key = KEY_DOWNARROW;
		break;
	case VK_CONTROL:
		key = KEY_FIRE;
		break;
	case VK_SPACE:
		key = KEY_USE;
		break;
	case VK_SHIFT:
		key = KEY_RSHIFT;
		break;
	case VK_MENU:
		key = KEY_LALT;
		break;
	case VK_F1:
		key = KEY_F1;
		break;
	case VK_F2:
		key = KEY_F2;
		break;
	case VK_F3:
		key = KEY_F3;
		break;
	case VK_F4:
		key = KEY_F4;
		break;
	case VK_F5:
		key = KEY_F5;
		break;
	case VK_F6:
		key = KEY_F6;
		break;
	case VK_F7:
		key = KEY_F7;
		break;
	case VK_F8:
		key = KEY_F8;
		break;
	case VK_F9:
		key = KEY_F9;
		break;
	case VK_F10:
		key = KEY_F10;
		break;
	case VK_F11:
		key = KEY_F11;
		break;
	case VK_F12:
		key = KEY_F12;
		break;
	case VK_OEM_PLUS:
		key = KEY_EQUALS;
		break;
	case VK_OEM_MINUS:
		key = KEY_MINUS;
		break;
	default:
		key = tolower(key);
		break;
	}

	return key;
}

static void addKeyToQueue(int pressed, unsigned char keyCode)
{
	unsigned char key = convertToDoomKey(keyCode);

	unsigned short keyData = (pressed << 8) | key;

	s_KeyQueue[s_KeyQueueWriteIndex] = keyData;
	s_KeyQueueWriteIndex++;
	s_KeyQueueWriteIndex %= KEYQUEUE_SIZE;
}

static LRESULT CALLBACK wndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_CLOSE:
		DestroyWindow(hwnd);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		ExitProcess(0);
		break;
	case WM_SYSKEYDOWN:
	case WM_KEYDOWN:
		addKeyToQueue(1, wParam);
		break;
	case WM_SYSKEYUP:
	case WM_KEYUP:
		addKeyToQueue(0, wParam);
		break;
	default:
		return DefWindowProcA(hwnd, msg, wParam, lParam);
	}
	return 0;
}

void DG_LoadFrame(void* buffer, int size)
{
	png_image pngimg;
	pngimg.version = PNG_IMAGE_VERSION;
	pngimg.opaque = NULL;

	png_image_begin_read_from_memory(&pngimg, buffer, size);
	pngimg.format = PNG_FORMAT_RGBA;
	unsigned char* image_data = (unsigned char*)malloc(PNG_IMAGE_SIZE(pngimg));
	png_image_finish_read(&pngimg, NULL, image_data, 0, NULL);

	for (unsigned int y = 0; y < pngimg.height; y++) {
		unsigned char* line = image_data + y * PNG_IMAGE_ROW_STRIDE(pngimg);
		for (unsigned int x = 0; x < pngimg.width; x++) {
			BYTE blue = (BYTE)((DWORD)line[0] * line[3] / 255);
			line[1] = (BYTE)((DWORD)line[1] * line[3] / 255);
			line[0] = (BYTE)((DWORD)line[2] * line[3] / 255);
			line[2] = blue;
			line += 4;
		}
	}

	s_Hframe = CreateCompatibleBitmap(s_Hdc, pngimg.width, pngimg.height);
	s_HdcFrame = CreateCompatibleDC(s_Hdc);
	BITMAPINFO png_bmi;
	png_bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	png_bmi.bmiHeader.biWidth = pngimg.width;
	png_bmi.bmiHeader.biHeight = -pngimg.height;
	png_bmi.bmiHeader.biPlanes = 1;
	png_bmi.bmiHeader.biBitCount = 32;
	png_bmi.bmiHeader.biCompression = BI_RGB;
	png_bmi.bmiHeader.biSizeImage = -0;
	png_bmi.bmiHeader.biClrUsed = 0;
	SetDIBits(s_HdcFrame, s_Hframe, 0, pngimg.height, image_data, &png_bmi, 0);

	free(image_data);
	png_image_free(&pngimg);

	SelectObject(s_HdcFrame, s_Hframe);
	s_HdcWorking = CreateCompatibleDC(s_Hdc);
	s_HworkingBitmap = CreateCompatibleBitmap(s_Hdc, 640, 480);
	SelectObject(s_HdcWorking, s_HworkingBitmap);
}

void DG_Init()
{
	memset(s_KeyQueue, 0, KEYQUEUE_SIZE * sizeof(unsigned short));
}

void DG_Cleanup()
{
	DeleteDC(s_HdcFrame);
	DeleteDC(s_HdcWorking);
	DeleteObject(s_Hframe);
	DeleteObject(s_HworkingBitmap);
	ReleaseDC(s_Hwnd, s_Hdc);

	s_HdcFrame = 0;
	s_Hframe = 0;
	s_HworkingBitmap = 0;
	s_HdcWorking = 0;
	s_Hdc = 0;
	s_Hwnd = 0;
}

void DG_SetHwnd(void* hwnd)
{
	s_Hwnd = (HWND)hwnd;
	s_Hdc = GetDC(s_Hwnd);
	GetWindowTextA(s_Hwnd, hwnd_oldTitle, 256);
	hwnd_originalWndProc = SetWindowLongPtrA(s_Hwnd, GWLP_WNDPROC, (LONG_PTR)wndProc);
}

void DG_RestoreHwnd()
{
	SetWindowTextA(s_Hwnd, hwnd_oldTitle);
	SetWindowLongPtrA(s_Hwnd, GWLP_WNDPROC, hwnd_originalWndProc);
}

void DG_DrawFrame()
{
	MSG msg;
	memset(&msg, 0, sizeof(msg));

	while (PeekMessageA(&msg, 0, 0, 0, PM_REMOVE) > 0)
	{
		TranslateMessage(&msg);
		DispatchMessageA(&msg);
	}

	RECT    rcCli;
	GetClientRect(WindowFromDC(s_Hdc), &rcCli);

	SetStretchBltMode(s_HdcWorking, HALFTONE);
	static const int DOOM_X0 = 46;
	static const int DOOM_Y0 = 41;
	static const int DOOM_X1 = 46+552;
	static const int DOOM_Y1 = 41+398;
	StretchDIBits(s_HdcWorking, DOOM_X0, DOOM_Y0, DOOM_X1 - DOOM_X0, DOOM_Y1 - DOOM_Y0,
		0, 0, DOOMGENERIC_RESX, DOOMGENERIC_RESY, DG_ScreenBuffer, &s_Bmi, 0, SRCCOPY);
	BLENDFUNCTION bf;
	bf.BlendOp = AC_SRC_OVER;
	bf.BlendFlags = 0;
	bf.SourceConstantAlpha = 255;
	bf.AlphaFormat = AC_SRC_ALPHA;
	AlphaBlend(s_HdcWorking, 0, 0, 640, 480, s_HdcFrame, 0, 0, 640, 480, bf);

	StretchBlt(s_Hdc, 0, 0, rcCli.right - rcCli.left, rcCli.bottom - rcCli.top, s_HdcWorking, 0, 0, 640, 480, SRCCOPY);

	//StretchDIBits(s_Hdc, 0, 0, rcCli.right - rcCli.left , rcCli.bottom - rcCli.top, 0, 0, DOOMGENERIC_RESX, DOOMGENERIC_RESY, DG_ScreenBuffer, &s_Bmi, 0, SRCCOPY);

	SwapBuffers(s_Hdc);
}

void DG_SleepMs(uint32_t ms)
{
	Sleep(ms);
}

uint32_t DG_GetTicksMs()
{
	return GetTickCount();
}

int DG_GetKey(int* pressed, unsigned char* doomKey)
{
	if (s_KeyQueueReadIndex == s_KeyQueueWriteIndex)
	{
		//key queue is empty

		return 0;
	}
	else
	{
		unsigned short keyData = s_KeyQueue[s_KeyQueueReadIndex];
		s_KeyQueueReadIndex++;
		s_KeyQueueReadIndex %= KEYQUEUE_SIZE;

		*pressed = keyData >> 8;
		*doomKey = keyData & 0xFF;

		return 1;
	}
}

void DG_SetWindowTitle(const char * title)
{
	if (s_Hwnd)
	{
		SetWindowTextA(s_Hwnd, title);
	}
}
