#include "stdafx.h"
#include "KeyHandler.h"

void KeyHandler::InvokeKeyHandler(WPARAM &wParam)
{
	if (wParam == VK_RETURN) {
		::SendMessageW(ButtonObjects.hOKBtn, BM_CLICK, (WPARAM)(INT)0, 0);
	}
}