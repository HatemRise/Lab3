#include "stdafx.h"
#include "stdio.h"
#include <locale.h>
#include <windows.h>
#include <iostream>

using namespace std;

short res1 = 10, res2 = 20; // перменные для индивидуального задания
int global = 100; // глобальная переменная, значение которой увеличивают потоки
DWORD tid1, tid2, res; // параметры созданного потока
HANDLE ht1, ht2; // указатель на потоки
HANDLE hEvent1, hEvent2;

// функция первого потока
DWORD WINAPI ThreadProc1(LPVOID lpParameter)
{
	//SetThreadPriority(ht1, THREAD_PRIORITY_TIME_CRITICAL);
	for (int i = 0; i < 5; i++) { ///если до 3х - то норм результат
		WaitForSingleObject(hEvent1, 500);
		ResetEvent(hEvent1);
		WaitForSingleObject(hEvent2, 500);
		ResetEvent(hEvent2);
		res1 = 11;
		res2 = 111;
		printf("res1 = %d, res2 = %d\tпоток 1\n", res1, res2);
		SetEvent(hEvent1);
		SetEvent(hEvent2);
		//завершение критического участка
	}
	return 0;
}
// функция второго потока
DWORD WINAPI ThreadProc2(LPVOID lpParameter)
{
	for (int i = 0; i < 5; i++) { ///если до 3х - то норм результат
								  //проверка возможности входа
		WaitForSingleObject(hEvent2, 500);
		ResetEvent(hEvent2);
		WaitForSingleObject(hEvent1, 500);
		ResetEvent(hEvent1);
		res1 = 22;
		res2 = 222;
		printf("res1 = %d, res2 = %d\tпоток 2\n", res1, res2);
		SetEvent(hEvent2);
		SetEvent(hEvent1);
	}
	return 0;
}

int _tmain(int argc, _TCHAR* argv[])
{
	setlocale(LC_ALL, "Rus");
	HANDLE msh[2];
	int m; // основной поток приложения

	hEvent1 = CreateEvent(NULL, TRUE, TRUE, NULL); //создает объект-событие;  тип сброса TRUE - ручной, начальное состояние FALSE - не сигнальное
	hEvent2 = CreateEvent(NULL, TRUE, TRUE, NULL);

	ht1 = CreateThread(NULL, 0, &ThreadProc1, NULL, 0, &tid1);
	ht2 = CreateThread(NULL, 0, &ThreadProc2, NULL, 0, &tid2);
	msh[0] = ht1;
	msh[1] = ht2;
	// ожидание завершения работы потоков
	if (WaitForMultipleObjects(2, msh, TRUE, 1000) == WAIT_TIMEOUT)
		printf("Over time\n");

	printf("Итог:\tres1 = %d, res2 = %d\n", res1, res2);
	CloseHandle(hEvent1);//удаление
	CloseHandle(hEvent2);//удаление
	return 0;
}
