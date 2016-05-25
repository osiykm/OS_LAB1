#include <iostream>
#include <Windows.h>
#include <conio.h>
#include <vector>
#include <string>

using namespace std;

DWORD WINAPI Writer(LPVOID i);
DWORD WINAPI Reader(int i);
string Novel;
HANDLE  Read, Write,INC;
int nReader;
HANDLE writer;
void main() {
	nReader = 0;
	Read = CreateEvent(NULL, TRUE, FALSE, NULL);
	Write = CreateEvent(NULL, TRUE, TRUE, NULL);
	INC = CreateEvent(NULL, FALSE, TRUE, NULL);
	setlocale(LC_ALL, "Russian");
	DWORD dwThreadId;
	vector<HANDLE> readers;
	writer = CreateThread(NULL, 0, Writer, (LPVOID)1, 0, &dwThreadId);
	for (int i = 0; i < 100; ++i) {
		readers.push_back(CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)Reader, (LPVOID)i, 0, &dwThreadId));
		Sleep(300);
	}
	_getch();

}


DWORD WINAPI Writer(LPVOID i) {
	while (true) {
		ResetEvent(Read);
		cout << "Писатель ожидает\n";
		WaitForSingleObject(Write, INFINITE);
		cout << "Писатель начал писать\n";
		int l = rand() % 500 + 100;
		Novel = to_string(l);
		Sleep(l);
		cout << "Писатель закончил писать\n";
		SetEvent(Read);
		Sleep(rand() % 500 + 100);
	}
	return 0;
}

DWORD WINAPI Reader(int i) {
	while (true) {

		WaitForSingleObject(Read, INFINITE);
		ResetEvent(Write);
		WaitForSingleObject(INC, INFINITE);
		++nReader;
		SetEvent(INC);
		string s = "Читатель " + to_string(i) + " начал читать (" + to_string(nReader) + ")\n";
		cout << s;
		WaitForSingleObject(INC, INFINITE);
		--nReader;
		SetEvent(INC);
		s = "Читатель " + to_string(i) + " закончил читать\n";
		cout << s;
		Sleep(rand() % 500 + 100);
		if (WaitForSingleObject(Read, 0) == WAIT_TIMEOUT && nReader == 0)
				SetEvent(Write);
	}
	return 0;
}