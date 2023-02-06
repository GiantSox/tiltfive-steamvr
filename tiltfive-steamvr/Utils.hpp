#pragma once
#include <string>

#include "DirectModeComponent.h"

namespace utils
{
	void log(LPCSTR message)
	{
		OutputDebugStringA(message);
	}

	void log(const std::string& message)
	{
		log(message.c_str());
	}
}
