#pragma once

#ifdef OC_DLL

	#ifdef _MSC_VER

		#define OC_API __declspec(dllexport)

	#else

		#define OC_API __attribute((visibility("default")))

	#endif

#else

	#ifdef _MSC_VER

		#define OC_API __declspec(dllimport)

	#else

		#define OC_API

	#endif

#endif
