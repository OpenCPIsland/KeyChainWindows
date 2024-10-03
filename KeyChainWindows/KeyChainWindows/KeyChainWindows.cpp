// KeyChainWindows.cpp : Defines the exported functions for the DLL.
//


#include "pch.h"
#include "framework.h"
#include "KeyChainWindows.h"

#include <windows.h>
#include <wincrypt.h>
#include <combaseapi.h>
#include <errno.h>
#pragma comment(lib, "Crypt32.lib")


// This is an example of an exported function.
KEYCHAINWINDOWS_API void _cryptProtectData(char* dataIn, size_t* dataOutSize, LPSTR* dataOut)
{
    DATA_BLOB pDataIn;
    DATA_BLOB pDataOut;
    pDataIn.cbData = strlen(dataIn) + 1;
    pDataIn.pbData = (BYTE*)dataIn;
    bool result = CryptProtectData(&pDataIn, NULL, NULL, NULL, NULL, 0, &pDataOut);


    if (result != 0) {
        LPSTR _Dst = (LPSTR)CoTaskMemAlloc(pDataOut.cbData);
        *dataOut = _Dst;

        if (pDataOut.cbData != 0) {
            if (_Dst != 0) {
                if (pDataOut.pbData != 0) {
                    memcpy(_Dst, pDataOut.pbData, pDataOut.cbData);
                    LocalFree(pDataOut.pbData);
                    *dataOutSize = pDataOut.cbData;
                    return;
                }
                memset(_Dst, 0, pDataOut.cbData);
            }
            errno = EINVAL;
            _invalid_parameter_noinfo();
        }
        LocalFree(pDataOut.pbData);
        *dataOutSize = pDataOut.cbData;
        return;
    }
    *dataOutSize = 0;
}


KEYCHAINWINDOWS_API LPSTR _cryptUnprotectData(BYTE* dataIn, DWORD dataInLength) {
    DATA_BLOB pDataIn;
    pDataIn.pbData = dataIn;
    pDataIn.cbData = dataInLength;
    DATA_BLOB pDataOut;
    BOOL protectResult;


    protectResult = CryptUnprotectData(&pDataIn, NULL, NULL, NULL, NULL, 0, &pDataOut); // weird programming but ok cp team

    if (protectResult != 0) {
        char* buff = (char*)CoTaskMemAlloc(pDataOut.cbData);
        strcpy_s(buff, pDataOut.cbData, (char*)pDataOut.pbData);
        return buff;
    }
}