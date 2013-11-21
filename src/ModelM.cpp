/*********************************************************
 * Author:		Nico Richard (NRichard)
 * Purpose 1:	Example keylogger
 * Purpose 2:	Simulate the sounds of a Model M Keyboard
 *
 * Note:		Annoying
 *********************************************************/
#include <stdio.h>
#include <windows.h>
#include <shellapi.h>
#include <irrKlang.h>
#include <sstream>
#pragma comment(lib, "irrKlang.lib")
using namespace irrklang;
ISoundEngine* sndEngine;

#define WM_MYMESSAGE (WM_USER + 1)

/*
  if(pKeyBoard->vkCode == VK_RETURN)			printf("\n"		);
  else if (pKeyBoard->vkCode == VK_SPACE)		printf(" "		);
  else if (pKeyBoard->vkCode == VK_TAB)		printf("    "	);
  else {
  BYTE keyboard_state[256];
  WORD wCharacter = 0;
  GetKeyboardState(keyboard_state);
  ToAscii((UINT)pKeyBoard->vkCode, pKeyBoard->scanCode,keyboard_state, &wCharacter, 0);
  printf("%c", wCharacter);
*/

LRESULT CALLBACK LowLevelKeyboardProc( int nCode, WPARAM wParam, LPARAM lParam ){
  KBDLLHOOKSTRUCT *pKeyBoard = (KBDLLHOOKSTRUCT *)lParam;
  std::stringstream ss;
  switch( wParam ) {
  case WM_KEYDOWN: { 
    if(rand() % 2)  ss << "..\\sound\\" << 1 + rand() % 10 << "_.mp3";
    else			ss << "..\\sound\\" << 1 + rand() % 10 << ".mp3" ;
    sndEngine->play2D(ss.str().c_str());
  }
  default:
    return CallNextHookEx( NULL, nCode, wParam, lParam );
  }
  return 0;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam){
  switch (msg)
    {
    case WM_MYMESSAGE:
      switch(lParam)
        {
        case WM_LBUTTONUP:
          if(sndEngine->getSoundVolume() != 0)	sndEngine->setSoundVolume(0);
          else	sndEngine->setSoundVolume(1.0f);
          break;
        default:
          return DefWindowProc(hWnd, msg, wParam, lParam);
        };
      break;
    default:
      return DefWindowProc(hWnd, msg, wParam, lParam);
    };
  return 0;
}

int main(int argc, char *argv[]) {
  HWND hWnd = GetConsoleWindow();		//Hide the program
  ShowWindow( hWnd, SW_HIDE );		//So we can have secrets
	
  NOTIFYICONDATA nid;					//Make a tray icon
  nid.cbSize = sizeof(NOTIFYICONDATA);
  nid.hWnd = hWnd;
  nid.uID = 100;
  nid.uVersion = NOTIFYICON_VERSION;
  nid.uCallbackMessage = WM_MYMESSAGE;
  nid.hIcon = LoadIcon(NULL, IDI_ASTERISK);
  wcscpy_s(nid.szTip, L"Model M Sim");
  nid.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;
  Shell_NotifyIcon(NIM_ADD, &nid);

  sndEngine = createIrrKlangDevice(); // Initialize sound engine for Model M Sim
  if (!sndEngine)	return 0;			// Error initializing. Panic and quit
										
  // Retrieve the applications instance and set a global Windows Hook to capture keystrokes.
  HINSTANCE appInstance = GetModuleHandle(NULL);
  SetWindowsHookEx( WH_KEYBOARD_LL, LowLevelKeyboardProc, appInstance, 0 );
  MSG msg;							
  while(GetMessage(&msg, NULL, 0, 0) > 0){
    TranslateMessage(&msg);		
    DispatchMessage(&msg);		
  }									
  sndEngine->drop();					// Delete engine
	
  Shell_NotifyIcon(NIM_DELETE, &nid);	// Kill tray icon

  return 0;
}
