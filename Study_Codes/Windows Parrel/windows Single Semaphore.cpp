#include<iostream>
#include<thread>
#include<Windows.h>

#define MAX_SEM_CNT 1
#define THREAD_CNT 5

HANDLE ghSemaphore;

int gvalue = 0;

void threadProc() {
	
	for (int i = 0; i < 2500; ++i) {
		WaitForSingleObject(ghSemaphore, INFINITE);

		gvalue += 1;

		ReleaseSemaphore(ghSemaphore, 1, NULL);
	}


}





int main() {

	DWORD threadId[THREAD_CNT];
	HANDLE aThread[THREAD_CNT];
	ghSemaphore = CreateSemaphore(
		NULL,
		MAX_SEM_CNT,
		MAX_SEM_CNT,
		NULL
	);

	if (ghSemaphore == NULL) return 1;

	for (int i = 0; i < THREAD_CNT; i++) {
		aThread[i] = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)threadProc, NULL, 0, &threadId[i]);
	}

	WaitForMultipleObjects(THREAD_CNT, aThread, TRUE, INFINITE);

	for (int i = 0; i < THREAD_CNT; ++i) {
		CloseHandle(aThread[i]);
	}

	CloseHandle(ghSemaphore);
	
	std::cout << gvalue << std::endl;

}