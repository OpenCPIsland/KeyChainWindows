// KeyChainLinux.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Windows-like CryptProtectData / CryptUnprotectData simulation
int CryptProtectData(const void *pDataIn, void **pDataOut, size_t *pSizeOut) {
    if (!pDataIn || !pDataOut || !pSizeOut) return 0;
    size_t len = strlen((const char*)pDataIn);
    char *buf = malloc(len);
    for (size_t i = 0; i < len; i++) buf[i] = ((char*)pDataIn)[i] ^ 0xAA; // simple obfuscation
    *pDataOut = buf;
    *pSizeOut = len;
    return 1;
}

int CryptUnprotectData(const void *pDataIn, size_t size, void **pDataOut) {
    if (!pDataIn || !pDataOut) return 0;
    char *buf = malloc(size + 1);
    for (size_t i = 0; i < size; i++) buf[i] = ((char*)pDataIn)[i] ^ 0xAA;
    buf[size] = '\0';
    *pDataOut = buf;
    return 1;
}

// Save and load secret files (like Windows KeyChain)
__attribute__((visibility("default")))
int save_secret(const char *path, const char *secret) {
    void *protected_data;
    size_t size;
    if (!CryptProtectData(secret, &protected_data, &size)) return 0;

    FILE *f = fopen(path, "wb");
    if (!f) { free(protected_data); return 0; }
    fwrite(protected_data, 1, size, f);
    fclose(f);
    free(protected_data);
    return 1;
}

__attribute__((visibility("default")))
char *get_secret(const char *path) {
    FILE *f = fopen(path, "rb");
    if (!f) return NULL;

    fseek(f, 0, SEEK_END);
    long size = ftell(f);
    rewind(f);

    void *data = malloc(size);
    fread(data, 1, size, f);
    fclose(f);

    void *unprotected;
    if (!CryptUnprotectData(data, size, &unprotected)) {
        free(data);
        return NULL;
    }
    free(data);
    return (char*)unprotected;
}
