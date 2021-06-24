#include "stuff.h"

uintptr_t utilities::getLibraryHandle(const char* library)
{
    char filename[0xFF] = {0},
    buffer[2048] = {0};
    FILE *fp = 0;
    uintptr_t address = 0;

    sprintf( filename, "/proc/%d/maps", getpid() );

    fp = fopen( filename, "rt" );
    if(fp == 0)
    {
        goto done;
    }

    while(fgets(buffer, sizeof(buffer), fp))
    {
        if( strstr( buffer, library ) )
        {
            address = (uintptr_t)strtoul( buffer, 0, 16 );
            break;
        }
    }

    done:

    if(fp)
      fclose(fp);

    return address;
}

uintptr_t utilities::findMethod(const char *libname, const char *name)
{
    auto result = dlsym(dlopen(libname, RTLD_NOW), name);

    Dl_info info;
    dladdr((void*)result, &info);

    return ((uintptr_t)info.dli_saddr - utilities::getLibraryHandle(libname));
}

void utilities::unprot(uintptr_t ptr, size_t dwSize)
{
	if (dwSize)
	{
		unsigned char* to_page = (unsigned char*)((unsigned int)(ptr) & 0xFFFFF000);
		size_t page_size = 0;

		for (int i = 0, j = 0; i < dwSize; ++i)
		{
			page_size = j * 4096;
			if (&((unsigned char*)(ptr))[i] >= &to_page[page_size])
				++j;
		}

		mprotect(to_page, page_size, PROT_READ | PROT_WRITE | PROT_EXEC);
		return;
	}
}

void utilities::hook(uintptr_t addr, uintptr_t func, uintptr_t *orig)
{
    utilities::unprot(addr);
    *orig = *(uintptr_t*)addr;
    *(uintptr_t*)addr = func;
}