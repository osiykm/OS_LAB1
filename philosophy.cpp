#include <iostream>
#include <Windows.h>
#include <conio.h>
#include <vector>
#include <string>

using namespace std;

#define N_PHILOSOPHS 5

int minN(int i);
int maxN(int i);

DWORD WINAPI philosophTask(int i);

vector<HANDLE> fork; //вилкы

int main() {
	setlocale(LC_ALL, "Russian");
	DWORD dwThreadId;
	//инициализация семафор
	for (int i = 0; i < N_PHILOSOPHS; ++i) {
		fork.push_back(CreateSemaphore(NULL, 1, 1, NULL));
	}
	vector<HANDLE> philosophs;
	for (int i = 0; i < N_PHILOSOPHS; ++i) {
		philosophs.push_back(CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)philosophTask, (LPVOID)i, 0, &dwThreadId));
		Sleep(rand() % 300 + 100);
	}
	_getch();
}

DWORD WINAPI philosophTask(int i) {
	while (true) {
		Sleep(rand() % 100 + 300);
		string s = "Философ " + to_string(i) + " ожидает\n";
		cout << s;
		WaitForSingleObject(fork[minN(i)], INFINITE);
		WaitForSingleObject(fork[maxN(i)], INFINITE);
		s = "Философ " + to_string(i) + " начал есть\n";
		cout << s;
		Sleep(rand() % 500 + 300);
		ReleaseSemaphore(fork[maxN(i)], 1, NULL);
		ReleaseSemaphore(fork[minN(i)], 1, NULL);
	}
	return 0;
}


int minN(int i) {
	if (i == 0)
		return 0;
	return i - 1;
}

int maxN(int i) {
	if (i == 0)
		return N_PHILOSOPHS - 1;
	return i;
}

