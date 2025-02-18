#include <stdio.h>
#include <windows.h>
#define PROGNAME (LPWSTR)".\\crackme.exe"
STARTUPINFO si;
PROCESS_INFORMATION pi;
WORD nopnop = 0x9090; /* Коды двух команд NOP */
DWORD old_flags; /* Старые флаги доступа */
DWORD p = 0x12345678; /* Адрес, куда вписывать байты */
int main() {
	CreateProcess(PROGNAME, (LPWSTR)"", NULL, NULL, TRUE, 0, NULL, NULL,
		(STARTUPINFO*)&si, (PROCESS_INFORMATION*)&pi);
	HANDLE h = OpenProcess(PROCESS_VM_OPERATION | PROCESS_VM_READ | PROCESS_VM_WRITE,
		TRUE, pi.dwProcessId);
	VirtualProtectEx(h, (void*)p, 2, PAGE_EXECUTE_READWRITE, &old_flags);
	WriteProcessMemory(h, (void*)p, (void*)&nopnop, 2, NULL);
	CloseHandle(h);
	system("pause >nul");
}