// The following ifdef block is the standard way of creating macros which make exporting
// from a DLL simpler. All files within this DLL are compiled with the KEYCHAINWINDOWS_EXPORTS
// symbol defined on the command line. This symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see
// KEYCHAINWINDOWS_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
#ifdef KEYCHAINWINDOWS_EXPORTS
#define KEYCHAINWINDOWS_API __declspec(dllexport)
#else
#define KEYCHAINWINDOWS_API __declspec(dllimport)
#endif

// This class is exported from the dll
extern "C" KEYCHAINWINDOWS_API void _cryptProtectData(char* dataIn, size_t * dataOutSize, LPSTR * dataOut);
extern "C" KEYCHAINWINDOWS_API LPSTR _cryptUnprotectData(BYTE * dataIn, DWORD dataInLength);