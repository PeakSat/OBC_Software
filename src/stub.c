// Dummy stub functions, so the linker doesnt complain
// These functions are not intented to be used, if you need them
// Implement them and adapt them to your needs
// With love i kai oxi Tats Foundation

#include "stub.h"

// _close: Close a file (stub, no filesystem, always fails)
int _close(int file) {
    return -1;  // Always fail
}

// _fstat: Get file status (stub, no filesystem, always fails)
int _fstat(int file, struct stat *st) {
    errno = ENOENT;  // Set errno to indicate no such file or directory
    return -1;       // Always fail
}

// _isatty: Check if the file descriptor is a terminal (stub, no tty, always fails)
int _isatty(int file) {
    return 0;  // Return 0 to indicate that this is not a terminal
}

// _lseek: Move the read/write file pointer (stub, no filesystem, always fails)
int _lseek(int file, int ptr, int dir) {
    return -1;  // Always fail
}

// _read: Read from a file (stub, no filesystem, always fails)
int _read(int file, char *ptr, int len) {
    return 0;  // Always return 0 (no data)
}

// _write: Write to a file (stub, no filesystem, always fails)
int _write(int file, char *ptr, int len) {
    return len;  // Pretend to succeed by returning length of bytes "written"
}

// _kill: Send a signal to a process (stub, no processes, always fails)
int _kill(int pid, int sig) {
    errno = EINVAL;  // Invalid argument
    return -1;       // Always fail
}

// _getpid: Get current process ID (stub, no processes, return a dummy PID)
int _getpid(void) {
    return 1;  // Return a fixed process ID
}



