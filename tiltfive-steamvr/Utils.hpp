#pragma once
#include <string>

#include "DirectModeComponent.h"


namespace utils
{
	inline void log(LPCSTR message)
	{
		OutputDebugStringA(message);
	}

	inline void log(const std::string& message)
	{
		log(message.c_str());
	}
}
