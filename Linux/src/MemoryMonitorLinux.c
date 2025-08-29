// MemoryMonitorLinux.c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdint.h>

// This mirrors Windows ReadProcessMemory API
ssize_t ReadProcessMemory(void *hProcess, void *lpBaseAddress, void *lpBuffer, size_t nSize, size_t *lpNumberOfBytesRead) {
    pid_t pid = (pid_t)(uintptr_t)hProcess; // Use process ID as "handle"
    char mem_path[64];
    snprintf(mem_path, sizeof(mem_path), "/proc/%d/mem", pid);

    int fd = open(mem_path, O_RDONLY);
    if (fd == -1) {
        perror("open");
        if (lpNumberOfBytesRead) *lpNumberOfBytesRead = 0;
        return 0;
    }

    ssize_t nread = pread(fd, lpBuffer, nSize, (uintptr_t)lpBaseAddress);
    close(fd);

    if (lpNumberOfBytesRead) *lpNumberOfBytesRead = (nread > 0) ? nread : 0;
    return (nread > 0);
}

// Export function for .so
__attribute__((visibility("default")))
ssize_t read_process_memory(void *hProcess, void *lpBaseAddress, void *lpBuffer, size_t nSize, size_t *lpNumberOfBytesRead) {
    return ReadProcessMemory(hProcess, lpBaseAddress, lpBuffer, nSize, lpNumberOfBytesRead);
}
