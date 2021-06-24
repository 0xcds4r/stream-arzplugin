#pragma once

#include <jni.h>
#include <dlfcn.h>
#include <link.h>
#include <sys/mman.h>
#include <cstdlib>
#include <string>
#include <vector>
#include <list>
#include <unistd.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <libgen.h>
#include <pthread.h>
#include <algorithm>
#include <chrono>
#include <string.h>
#include <stdio.h>
#include <map>
#include <set>
#include <sstream>
#include <cstdio>
#include <cstring>

namespace utilities {
    uintptr_t getLibraryHandle(const char* library);
    uintptr_t findMethod(const char *libname, const char *name);
    void unprot(uintptr_t ptr, size_t dwSize = 100);
    void hook(uintptr_t addr, uintptr_t func, uintptr_t *orig);
};