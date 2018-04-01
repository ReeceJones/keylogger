#include <iostream>
#include <fstream>
#include <string>
#include <Windows.h>

//array containing keystates
BYTE keyboardState[256];
//buffer containing the current/latest key pressed down/up
LPWSTR buf;

LRESULT CALLBACK KeyboardProc(int code, WPARAM wParam, LPARAM lParam)
{
	auto kp = (KBDLLHOOKSTRUCT*)lParam;
	ToUnicode(kp->vkCode, kp->scanCode, keyboardState, buf, 1, 0);
	if (kp->flags & LLKHF_UP || wParam == WM_KEYUP)
		std::cout << "key: \t" << std::dec << (LPCSTR)buf << "\tcode:\t [0x" << std::hex << kp->vkCode << "]" << std::endl;
	return CallNextHookEx(NULL, code, wParam, lParam);
}

int main()
{
	//allocate 1 byte for our buffer
	//1 byte because we don't care about unicode characters
	buf = (LPWSTR)malloc(sizeof(LPWSTR));
	//set our windows hook for our keyboard
	SetWindowsHookEx(WH_KEYBOARD_LL, KeyboardProc, 0, 0);
	//we have to do this for some reason, idk
	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0) > 0)
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return 0;
}