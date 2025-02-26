#pragma once

#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>

int _close(int file);

int _fstat(int file, struct stat *st);

int _isatty(int file);

int _lseek(int file, int ptr, int dir);

int _read(int file, char *ptr, int len);

int _write(int file, char *ptr, int len);

int _kill(int pid, int sig);

int _getpid(void);
