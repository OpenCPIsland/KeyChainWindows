#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

// Protect: input C-string -> outputs protected buffer + length
// int _cryptProtectData(const char* dataIn, int* sizeOut, char** dataOut)
__attribute__((visibility("default")))
int _cryptProtectData(const char* dataIn, int* sizeOut, char** dataOut) {
    if (!dataIn || !sizeOut || !dataOut) return 0;

    size_t inLen = strlen(dataIn) + 1; // include terminating NUL in input length
    // For "protected" data we'll produce binary data of same length (xor obfuscation)
    char* buf = (char*)malloc(inLen);
    if (!buf) return 0;

    for (size_t i = 0; i < inLen; ++i) {
        // XOR every byte including the terminating 0 so protected blob contains that byte
        buf[i] = ((const unsigned char*)dataIn)[i] ^ 0xAA;
    }

    *dataOut = buf;
    *sizeOut = (int)inLen;
    return 1;
}

// Unprotect: input binary buffer + length -> outputs C-string (NUL terminated)
// int _cryptUnprotectData(const unsigned char* dataIn, int dataInLength, char** dataOut)
__attribute__((visibility("default")))
int _cryptUnprotectData(const unsigned char* dataIn, int dataInLength, char** dataOut) {
    if (!dataIn || dataInLength <= 0 || !dataOut) return 0;

    // We will allocate size+1 to ensure a terminating NUL for Marshal.PtrToStringAnsi
    char* out = (char*)malloc((size_t)dataInLength + 1);
    if (!out) return 0;

    for (int i = 0; i < dataInLength; ++i) {
        out[i] = (char)(dataIn[i] ^ 0xAA);
    }
    // Ensure NUL termination
    out[dataInLength] = '\0';

    *dataOut = out;
    return 1;
}

// Helper to save a secret to a file
__attribute__((visibility("default")))
int save_secret(const char* path, const char* secret) {
    if (!path || !secret) return 0;

    char* protected_data = NULL;
    int size = 0;
    if (!_cryptProtectData(secret, &size, &protected_data)) return 0;

    FILE* f = fopen(path, "wb");
    if (!f) { free(protected_data); return 0; }

    size_t written = fwrite(protected_data, 1, (size_t)size, f);
    fclose(f);
    free(protected_data);

    return written == (size_t)size ? 1 : 0;
}

// Load file (protected) -> unprotected string (caller must free returned pointer)
__attribute__((visibility("default")))
int get_secret(const char* path, char** secretOut) {
    if (!path || !secretOut) return 0;

    FILE* f = fopen(path, "rb");
    if (!f) return 0;

    if (fseek(f, 0, SEEK_END) != 0) { fclose(f); return 0; }
    long size = ftell(f);
    rewind(f);

    if (size <= 0) { fclose(f); return 0; }

    unsigned char* data = (unsigned char*)malloc((size_t)size);
    if (!data) { fclose(f); return 0; }

    size_t r = fread(data, 1, (size_t)size, f);
    fclose(f);

    if (r != (size_t)size) { free(data); return 0; }

    int ok = _cryptUnprotectData(data, (int)size, secretOut);
    free(data);
    return ok;
}

#ifdef __cplusplus
}
#endif
