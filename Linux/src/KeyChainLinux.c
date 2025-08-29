// KeyChainLinux.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef __cplusplus
extern "C" {
#endif

// Simple XOR obfuscation to simulate CryptProtectData / CryptUnprotectData

// _cryptProtectData: input string -> output buffer + size
__attribute__((visibility("default")))
int _cryptProtectData(const char* dataIn, int* sizeOut, char** dataOut) {
    if (!dataIn || !sizeOut || !dataOut) return 0;

    size_t len = strlen(dataIn);
    char* buf = (char*)malloc(len);  // Caller (C#) must free
    if (!buf) return 0;

    for (size_t i = 0; i < len; i++)
        buf[i] = dataIn[i] ^ 0xAA;  // XOR obfuscation

    *dataOut = buf;
    *sizeOut = (int)len;
    return 1;
}

// _cryptUnprotectData: input buffer + size -> output string
__attribute__((visibility("default")))
int _cryptUnprotectData(const char* dataIn, int size, char** dataOut) {
    if (!dataIn || !dataOut) return 0;

    char* buf = (char*)malloc(size + 1);  // Caller (C#) must free
    if (!buf) return 0;

    for (int i = 0; i < size; i++)
        buf[i] = dataIn[i] ^ 0xAA;  // reverse XOR

    buf[size] = '\0';
    *dataOut = buf;
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

    size_t written = fwrite(protected_data, 1, size, f);
    fclose(f);
    free(protected_data);

    return written == (size_t)size ? 1 : 0;
}

// Helper to load a secret from a file
__attribute__((visibility("default")))
int get_secret(const char* path, char** secretOut) {
    if (!path || !secretOut) return 0;

    FILE* f = fopen(path, "rb");
    if (!f) return 0;

    fseek(f, 0, SEEK_END);
    long size = ftell(f);
    rewind(f);

    if (size <= 0) { fclose(f); return 0; }

    char* data = (char*)malloc(size);
    if (!data) { fclose(f); return 0; }

    size_t read = fread(data, 1, size, f);
    fclose(f);

    if (read != (size_t)size) { free(data); return 0; }

    int result = _cryptUnprotectData(data, (int)size, secretOut);
    free(data);
    return result;
}

#ifdef __cplusplus
}
#endif
