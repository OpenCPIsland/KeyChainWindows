// KeyChainWindows.cpp : Defines the exported functions for the DLL.
#include "pch.h"
#include "KeyChainWindows.h"

#include <windows.h>
#include <wincrypt.h>
#include <combaseapi.h>
#include <errno.h>
#include <string.h>
#pragma comment(lib, "Crypt32.lib")

#ifdef __cplusplus
extern "C" {
#endif

// Exported function: protect data
KEYCHAINWINDOWS_API int _cryptProtectData(const char* dataIn, size_t* sizeOut, char** dataOut)
{
    if (!dataIn || !sizeOut || !dataOut) return 0;

    DATA_BLOB pDataIn = {0};
    DATA_BLOB pDataOut = {0};

    pDataIn.cbData = (DWORD)(strlen(dataIn) + 1);
    pDataIn.pbData = (BYTE*)dataIn;

    if (!CryptProtectData(&pDataIn, NULL, NULL, NULL, NULL, 0, &pDataOut)) {
        *sizeOut = 0;
        *dataOut = nullptr;
        return 0;
    }

    char* buf = (char*)CoTaskMemAlloc(pDataOut.cbData);
    if (!buf) {
        LocalFree(pDataOut.pbData);
        *sizeOut = 0;
        *dataOut = nullptr;
        return 0;
    }

    memcpy(buf, pDataOut.pbData, pDataOut.cbData);
    LocalFree(pDataOut.pbData);

    *dataOut = buf;
    *sizeOut = pDataOut.cbData;
    return 1;
}

// Exported function: unprotect data
KEYCHAINWINDOWS_API int _cryptUnprotectData(const char* dataIn, size_t size, char** dataOut)
{
    if (!dataIn || !dataOut || size == 0) return 0;

    DATA_BLOB pDataIn = {0};
    DATA_BLOB pDataOut = {0};

    pDataIn.cbData = (DWORD)size;
    pDataIn.pbData = (BYTE*)dataIn;

    if (!CryptUnprotectData(&pDataIn, NULL, NULL, NULL, NULL, 0, &pDataOut)) {
        *dataOut = nullptr;
        return 0;
    }

    char* buf = (char*)CoTaskMemAlloc(pDataOut.cbData);
    if (!buf) {
        LocalFree(pDataOut.pbData);
        *dataOut = nullptr;
        return 0;
    }

    memcpy(buf, pDataOut.pbData, pDataOut.cbData);
    LocalFree(pDataOut.pbData);

    *dataOut = buf;
    return 1;
}

#ifdef __cplusplus
}
#endif
