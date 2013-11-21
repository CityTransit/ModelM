#pragma once

#ifndef NOTIFYICON_VERSION_4
#define NOTIFYICON_VERSION_4 4
#endif

#ifndef NIF_MSG
#define NIF_MSG (WM_APP + 0x100)
#endif

class CTrayIcon	{
public:
	CTrayIcon();
	~CTrayIcon();

	NOTIFYICONDATA nid;
	OSVERSIONINFO vi;

public:
	void SetTrayIcon (HWND, LPWSTR);
	void ShowBalloon (DWORD, LPWSTR, LPWSTR);
	void StartBlinkTrayIcon (DWORD);
	void StopBlinkTrayIcon ();
	void DelTrayIcon ();
};

UINT BlinkIconThread(LPVOID lP);

typedef struct BlinkIconData	{
	HANDLE hBlink;
	DWORD iDelay;
}blinkData;

extern CTrayIcon cTray;

/*
LRESULT ::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	CMenu PopupMenu;
	CPoint point;

	if (message == NIF_MSG)
	{
		switch (LOWORD(lParam))
		{
			case WM_LBUTTONUP:
			{
			}break;

			case WM_RBUTTONUP:
			{
				PopupMenu.LoadMenu(IDR_TRAY);
				CMenu *pPopupMenu = PopupMenu.GetSubMenu(0);
				::GetCursorPos(&point);
				SetForegroundWindow();
				pPopupMenu->TrackPopupMenu(TPM_LEFTALIGN | TPM_LEFTBUTTON, point.x, point.y, this, NULL);
				PostMessage(WM_NULL);
			}break;
		}
	}

	return CDialog::WindowProc(message, wParam, lParam);
}
*/