/*Начальный поток является управляющим. Он принимает поток ввода с
клавиатуры и контролирует дочерние потоки. По нажатию клавиши ‘+’ до-
бавляется новый поток, ‘-’ – удаляется последний добавленный, ‘q’ – про-
грамма завершается. Каждый дочерний поток посимвольно выводит на экран
в вечном цикле свою уникальную строку. При этом операция вывода строки
должна быть атомарной, т.е. процесс вывода должен быть синхронизирован та-
ким образом, чтобы строки на экране не перемешивались. Выполняется в двух
вариантах: под Linux и Windows. В качества метода синхронизации использо-
вать сигналы/события.*/

#include <pthread.h>
#include <stdio.h>
#include <iostream>
#include <ncurses.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <sys/types.h>
#include <errno.h>
#include <string>
#include <sstream>
#include <cstring>

#define MAX_COUNT 100

using namespace std;

pthread_t mas_id[MAX_COUNT];
pthread_mutex_t mutex;

int mas_thread[MAX_COUNT];
int COUNT_THREAD;

bool mas_mutex[MAX_COUNT];
bool delete_flag;
bool on_off;

char buf[5];
int id_thread;

void * thread_function(void * arg)
{
	id_thread = (int)gettid();
	int count = *(int*)arg;
	
	char buffer[20];
	sprintf(buffer, "%d", (int)gettid());
	
	while(true)
	{	
		pthread_mutex_lock(&mutex);
		
		if(mas_mutex[count])
		{
			if(delete_flag)
			{
				delete_flag = false;
				if(COUNT_THREAD > 0)
					mas_mutex[0] = true;
				pthread_mutex_unlock(&mutex);
				pthread_exit(0);
			}

			attron(COLOR_PAIR(1)); printw("%d", count+1); attroff(COLOR_PAIR(1)); usleep(500000); printw(" "); refresh(); usleep(500000); printw("-"); refresh(); usleep(500000); printw(" "); refresh(); usleep(500000);

			for(int i = 0; buffer[i]!='\0';i++)
			{
				printw("%c", buffer[i]);
				refresh();
				usleep(500000);
			}

			printw("\n\r");
			refresh();

			mas_mutex[count] = false;

			if(!on_off)
			{
				if(count == 0 && COUNT_THREAD == 1)
					mas_mutex[count] = true;
				else if(count + 1 < COUNT_THREAD)
					mas_mutex[count + 1] = true;
				else if(count + 1 == COUNT_THREAD)
					mas_mutex[0] = true;
			}
		}

		pthread_mutex_unlock(&mutex);
		sleep(1);
	}
	return 0;
}

int main()
{
	char choose = 0;

	initscr();
	noecho();
	clear();
	refresh();

	start_color();
   	clear();
   	refresh();

    init_pair(1, COLOR_GREEN, COLOR_BLACK);
    init_pair(2, COLOR_RED, COLOR_BLACK);
    init_pair(3, COLOR_YELLOW, COLOR_BLACK);
    init_pair(4, COLOR_WHITE, COLOR_BLACK);

	printw("Add thread - "); attron(COLOR_PAIR(1)); printw("<+>\n\r"); refresh(); attron(COLOR_PAIR(4));
	printw("Delete last thread - "); attron(COLOR_PAIR(2)); printw("<->\n\r"); refresh(); attron(COLOR_PAIR(4));
	printw("Exit from the program - "); attron(COLOR_PAIR(3)); printw("<q>\n\n\r"); refresh(); attron(COLOR_PAIR(4));

	COUNT_THREAD = 0;
	int cnt = 0;

	pthread_mutex_init(&mutex, NULL);
	
	while(true)
	{
		switch(choose = getchar())
		{
			case '+':
				memset(buf,0,5);
				sprintf(buf, "%d", cnt + 1);

				on_off = true;
				pthread_mutex_lock(&mutex);
				
				for(int i = 0; i <= cnt; i++)
					mas_mutex[i] = false;

				delete_flag = false;
				mas_thread[cnt] = pthread_create(&mas_id[cnt], NULL, thread_function, &cnt);
				if(mas_thread[cnt] != 0)
				{
					printw("\n\r Error! Thread is not created!\n\n\r");
					return 0;
				}

				sleep(1);
				printw("\r\n\n NEW ");
				attron(COLOR_PAIR(1));
				addstr(buf);
				attroff(COLOR_PAIR(1));
				printw(" THREAD CREATED - "); attron(COLOR_PAIR(1));  printw("%d", id_thread); attroff(COLOR_PAIR(1)); printw("\n\n\r");
				refresh();
  
				
				mas_mutex[cnt] = true;

				cnt++;
				COUNT_THREAD++;

				on_off = false;
			
				pthread_mutex_unlock(&mutex);
				break;
		
			case '-':

				on_off = true;

				if(COUNT_THREAD == 0)
				{
					printw("\r\n You have no threads!\n\r");
					refresh();
				}
				
				else
				{
					pthread_mutex_lock(&mutex);

					mas_mutex[cnt - 1] = true;

					sleep(1);
					printw("\r\n\n DELETE ");
					attron(COLOR_PAIR(1));
					addstr(buf);
					attroff(COLOR_PAIR(1));
					printw(" THREAD - "); attron(COLOR_PAIR(1));  printw("%d", id_thread); attroff(COLOR_PAIR(1)); printw("\n\n\r");
					refresh();

					delete_flag = true;

					cnt--;
					COUNT_THREAD--;
					
					memset(buf,0,5);
					sprintf(buf, "%d", cnt);

					on_off = false;

					pthread_mutex_unlock(&mutex);

				}
				break;

			case 'q':
				on_off = true;

				pthread_mutex_lock(&mutex);

				for(int i = 0; i < cnt; i++)
				{
					close(mas_thread[i]);
				}

				sleep(1);
				printw("\r\n Exit from the program...\n\r");
				refresh();
				sleep(1.5);
				clear();
				endwin();
				return 0;
		}
		refresh();
	}

	endwin();
	return 0;
}

