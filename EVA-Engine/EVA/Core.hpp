#pragma once

#ifdef EVA_PLATFORM_WINDOWS
	#ifdef EVA_BUILD_DLL
		#define EVA_API __declspec(dllexport)
	#else
		#define EVA_API __declspec(dllimport)
	#endif // EVA_BUILD_DLL
#else
	#error EVA only supports windows
#endif // EVA_PLATFORM_WINDOWS