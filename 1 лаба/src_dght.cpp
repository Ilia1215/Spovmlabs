#include <windows.h>
#include <iostream>
#include <stdio.h>
#include <time.h>
 
using namespace std;

int main() 
{
SYSTEMTIME st;    
GetLocalTime(&st);
cout<<endl<<"Create child process sucess"<<endl;
cout<<"Time in the child process:"<<endl;
printf( "%d-%02d-%02d %02d:%02d:%02d.%03d", 
             st.wYear,
             st.wMonth, 
             st.wDay,                      
             st.wHour, 
             st.wMinute, 
             st.wSecond,
             st.wMilliseconds);
cout<<endl;
return 0;
}