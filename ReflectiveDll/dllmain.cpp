#include <windows.h>
#include <stdio.h>
#include <string>
#include "ReflectiveLoader.h"

extern HINSTANCE hAppInstance;

int LetsGo() {
	// Create named pipe: this will get patched in CNA
	HANDLE hPipe = CreateNamedPipeA(
		"\\\\.\\pipe\\youcantpatchthis",
		PIPE_ACCESS_DUPLEX,
		(PIPE_TYPE_MESSAGE | PIPE_READMODE_MESSAGE | PIPE_WAIT),
		1,
		0x1000,
		0x1000,
		0,
		NULL);

	if (hPipe == INVALID_HANDLE_VALUE) {
		return 1;
	}

	// Wait for beacon to connect
	while (!ConnectNamedPipe(hPipe, 0) && GetLastError() != ERROR_PIPE_CONNECTED);

	// send output
	std::string message = "[+] Hello from rDLL (via named pipe)!";
	DWORD dwBytesWritten = 0;
	WriteFile(hPipe, message.c_str(), message.length(), &dwBytesWritten, 0);
	FlushFileBuffers(hPipe);

	// cleanup
	DisconnectNamedPipe(hPipe);
	CloseHandle(hPipe);
	return 0;
}

BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD dwReason, LPVOID lpReserved)
{
	BOOL bReturnValue = TRUE;
	switch (dwReason)
	{
	case DLL_QUERY_HMODULE:
		if (lpReserved != NULL)
			*(HMODULE*)lpReserved = hAppInstance;
		break;
	case DLL_PROCESS_ATTACH:
		hAppInstance = hinstDLL;
		LetsGo();
		fflush(stdout);
	case DLL_PROCESS_DETACH:
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
		break;
	}
	return bReturnValue;
}