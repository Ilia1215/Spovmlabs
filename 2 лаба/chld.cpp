#include<Windows.h>
#include<stdio.h>
#include<iostream>

using namespace std;

int main()
{
	wchar_t buf[20];
	_itow(GetCurrentProcessId(), buf, 10);

	HANDLE hEvent = CreateEvent(NULL, FALSE, FALSE, buf);
	while (true) 
	{
		for(int i = 0; buf[i]!= '\0'; i++)
		{
			if(WaitForSingleObject(hEvent, 0) == WAIT_TIMEOUT)
				i = 0;
			
			if(WaitForSingleObject(hEvent, INFINITE) == WAIT_OBJECT_0)
			{
				putchar(buf[i]);
				Sleep(500);
			}
		}
		cout<<endl;
		ResetEvent(hEvent);
		}
	return 0;
}
