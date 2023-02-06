#pragma once
#include <string>

#include "Windows.h"

struct utils
{
	static void OpenDebugConsole()
	{
#ifdef _DEBUG
		AllocConsole();
#endif
	}

	static void log(LPCSTR message)
	{
		OutputDebugStringA(message);

#ifdef _DEBUG
		if (HANDLE consoleOutput = GetStdHandle(STD_OUTPUT_HANDLE); consoleOutput != INVALID_HANDLE_VALUE)
		{
			const auto byteLen = strlen(message);
			DWORD nbBytesWritten;
			(void)WriteFile(consoleOutput, message, byteLen, &nbBytesWritten, nullptr);
		}
#endif
	}

	static void log(const std::string& message)
	{
		log(message.c_str());
	}
};
