#include "stdafx.h"
#include "KeyHandler.h"
#include <windowsx.h>

void KeyHandler::InvokeKeyHandler(WPARAM &wParam)
{
	switch (wParam)
	{
		case VK_RETURN:
		{
			SendMessageW(ButtonObjects.hOKBtn, BM_CLICK, (WPARAM)(INT)0, 0);
		}
		break;
	}
}