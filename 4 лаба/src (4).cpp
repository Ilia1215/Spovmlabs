/* Ќачальный поток €вл€етс€ управл€ющим. 
ќн принимает поток ввода с клавиатуры и контролирует дочерние процессы. 
ѕо нажатию клавиши С+Т добавл€етс€ новый поток, 
С-Т Ц удал€етс€ последний добавленный, 
СqТ Ц программа завершаетс€. 
 аждый дочерний поток посимвольно выводит на экран в вечном цикле свою уникальную строку. 
ѕри этом операци€ вывода строки должна быть атомарной, т.е. процесс вывода должен быть синхронизирован таким образом, 
чтобы строки на экране не перемешивались. 
¬ыполн€етс€ в двух вариантах: под Linux и Windows. 
¬ качества метода синхронизации использовать критические секции.
*/

#define UNICODE
#define _CRT_SECURE_NO_WARNINGS

#include <windows.h>
#include <stdio.h>
#include <vector>
#include <conio.h>
#include <iostream>

CRITICAL_SECTION critical_section;

#define MAX_COUNT 100

using namespace std;

HANDLE mas_thread[MAX_COUNT];
DWORD mas_id[MAX_COUNT];

bool mas_delete[MAX_COUNT];
bool mas_sections[MAX_COUNT];
bool on_off;

int MAX_THREAD;

DWORD WINAPI new_thread(int count)
{
	wchar_t buf[20];
	_itow(mas_id[count], buf, 10);
	
	while(1)
	{
		EnterCriticalSection(&critical_section);
		if(!mas_sections[count])
		{			
		if(!mas_delete[count])
		{
			if(MAX_THREAD > 0)
				mas_sections[0] = false;
			LeaveCriticalSection(&critical_section);
			CloseHandle(mas_thread[count]);
			ExitThread(0);
		}

		cout << count+1; Sleep(300); cout << " "; Sleep(300); cout << "-"; Sleep(300); cout << " "; Sleep(300);
		for(int i = 0; buf[i]!='\0'; i++)
		{
			putchar(buf[i]);
			Sleep(300);
		}
		
		cout << endl;
		mas_sections[count] = true;

		if(!on_off)
		{
		if(count == 0 && MAX_THREAD == 1)
			mas_sections[count] = false;
		else if(count + 1 < MAX_THREAD)
			mas_sections[count + 1] = false;
		else if(count + 1 == MAX_THREAD)
			mas_sections[0] = false;
		}	
		}
		LeaveCriticalSection(&critical_section);
		Sleep(1000);
	}
	return 0;	
}

int main()
{

MAX_THREAD = 0;
int count = 0;

bool plus = false;
bool minus = false;
bool q = false;
	
cout<<"Add thread <+>\n";
cout<<"Delete thread <->\n";
cout<<"Exit <q>\n";

InitializeCriticalSection(&critical_section);

while(true)
{

	if(plus == false) 
	{ 
		if(GetAsyncKeyState(0x6B))
		{
			on_off = true;
			EnterCriticalSection(&critical_section);

			for(int i = 0; i <= count; i++)
			{
				mas_sections[i] = true;
			}
			mas_delete[count] = true;

			mas_thread[count] = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)new_thread, LPVOID(count), 0, &mas_id[count]);
			mas_delete[count] = true;
			
			if(!mas_thread[count])
			{
				cout << endl << "Error! Thread is was not created!" << endl;
				return 0;
			}
			
			cout << endl << "NEW <" << count + 1 << ">" << " thread ID = " << mas_id[count] << endl << endl;

			mas_sections[count] = false;
			
			count++;
			MAX_THREAD++;
			
			plus = true;
			on_off = false;
			
		LeaveCriticalSection(&critical_section);
		}
	}
	else if(plus == true) { Sleep(200); plus = false; }
	

	if(minus == false)
	{
		if(GetAsyncKeyState(0x6D))
		{
			on_off = true;
			if(MAX_THREAD == 0)
			{
				cout << "You have no threads ! ! !" << endl;
				minus = true;
			}
			
			else
			{
			
			for(int i = 0; i < count; i++)
				if(!mas_sections[i])
					while(!mas_sections[i]){}
			
			while(!TryEnterCriticalSection(&critical_section));

			mas_sections[count - 1] = false;
			
			cout << endl << "DELETE <" << count << ">" << " thread ID = " << mas_id[count-1] << endl << endl;
			
			mas_delete[count-1] = false;
	
			count--;
			MAX_THREAD--;

			minus = true;
			on_off = false;

			LeaveCriticalSection(&critical_section);
			}
		}
	}
	else if(minus == true) { Sleep(200); minus = false; }

	if(q == false)
	{
		if(GetAsyncKeyState(0x51))
		{
			on_off = true;
			for(int i = 0; i < count; i++)
				if(!mas_sections[i])
					while(!mas_sections[i]){}

			DeleteCriticalSection(&critical_section);
			
			for(int i = 0; i < count; i++)
			{
				CloseHandle(mas_thread[i]);
			}

			cout << endl <<"Exit from the program..." << endl;
			Sleep(1500);		
			return 0;
		}
	}
}

return 0;

}