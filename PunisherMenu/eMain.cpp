#include "eMain.h"
#include "eMenu.h"
#include "punisher.h"
#include "core.h"

WNDPROC wOriginalWndProc = nullptr;
static bool	bWindowInFocus = true;

LRESULT CALLBACK MainHooks::HookedWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_KEYDOWN:
		if (wParam == TheMenu.KeyEnableMenu)
			TheMenu.OnKeyToggle();

		if (TheMenu.m_active)
		{
			if (wParam == TheMenu.KeyMenuItemDOWN)
				TheMenu.OnKeyDown();

			if (wParam == TheMenu.KeyMenuItemUP)
				TheMenu.OnKeyUp();

			if (wParam == TheMenu.KeyMenuItemLEFT)
				TheMenu.OnKeyLeft();

			if (wParam == TheMenu.KeyMenuItemRIGHT)
				TheMenu.OnKeyRight();

			if (wParam == TheMenu.KeyMenuExecute)
				TheMenu.OnKeyExecute();
		}

		break;
	case WM_KILLFOCUS:
		bWindowInFocus = false;
		break;
	case WM_SETFOCUS:
		bWindowInFocus = true;
		break;
	default:
		break;
	}
	return STDCallAndReturn<LRESULT, 0x524D40, HWND, UINT, WPARAM, LPARAM>(hWnd, uMsg, wParam, lParam);//CallWindowProc(wOriginalWndProc, hWnd, uMsg, wParam, lParam);
}

void MainHooks::HookWndProc()
{
	if (!wOriginalWndProc)
	{
		wOriginalWndProc = (WNDPROC)SetWindowLongPtr(FindWindow("The Punisher", 0), GWLP_WNDPROC, (LONG_PTR)HookedWndProc);
	}
}


bool IsWindowFocused()
{
	return bWindowInFocus;
}
