#pragma once

#ifdef OC_PLATFORM_WINDOWS
	#ifdef OC_BUILD_DLL
		#define OCEAN_API __declspec(dllexport)
	#else
		#define OCEAN_API __declspec(dllimport)
	#endif
#else
	#error Ocean only supports Windows!
#endif