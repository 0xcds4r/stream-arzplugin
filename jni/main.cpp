#include "stuff.h"

bool bPluginInitialized = false;

uintptr_t g_SAMPAdr = 0;
uintptr_t pUI = 0;
uintptr_t pNetGame = 0;
uintptr_t pGame = 0;

void printLog(const char* format, ...)
{
    char info[512] = {0};
    va_list arg;
	va_start(arg, format);
	vsnprintf(info, sizeof(info), format, arg);
	va_end(arg);

    char path[0xFF] = {0};
	FILE* fileLog = nullptr;

	sprintf(path, "%sdebug.log", "/storage/emulated/0/");
	fileLog = fopen(path, "a");

	if(fileLog == nullptr) {
        return;
    }

	fprintf(fileLog, "%s\n", info);
	fflush(fileLog);
    fclose(fileLog);
}

void (*CALLER_InitGUI)(void);
void HOOK_InitGUI(void)
{
    if(!pNetGame)
    {
        // memcpy((void*)(g_SAMPAdr + 0xEF638), "Plugin Inited!", strlen("Plugin Inited!"));
        // memcpy((void*)(g_SAMPAdr + 0x49FCB6), "play.uifserver.net", strlen("play.uifserver.net"));
        // *(uintptr_t*)(g_SAMPAdr + 0x49FCD4) = 7776;
    }

    CALLER_InitGUI();
}

void (*CALLER_NetGame)(uintptr_t thiz, const char *address, int port, const char *nick, const char *pass);
void HOOK_NetGame(uintptr_t thiz, const char *address, int port, const char *nick, const char *pass)
{
    printLog("ip changed!");
    CALLER_NetGame(thiz, "play.uifserver.net", 7776, "Zalupka_Dedamorozasu", nullptr);
}

void doPluginExecute()
{
    static bool bOnce = false;

    if(!bOnce)
    {
        utilities::unprot(g_SAMPAdr + 0x49FCB6);
        utilities::unprot(g_SAMPAdr + 0x49FCD4);

        printLog("plugin execute..");

        utilities::hook(g_SAMPAdr + 0x368DBC, (uintptr_t)HOOK_InitGUI, (uintptr_t*)&CALLER_InitGUI);
        utilities::hook(g_SAMPAdr + 0x36964C, (uintptr_t)HOOK_NetGame, (uintptr_t*)&CALLER_NetGame);

        // 36964C 51 F4 0E 00             _ZN8CNetGameC2EPKciS1_S1__pt
        
        printLog("plugin executed..");
        bOnce = true;
    }
}

void doPluginInit()
{
    if(bPluginInitialized) {
        return;
    }
    
    printLog("do plugin init..");

    pGame = *(uintptr_t*)(g_SAMPAdr + 0x49FC68);
    pUI = *(uintptr_t*)(g_SAMPAdr + 0x49FC64);
    pNetGame = *(uintptr_t*)(g_SAMPAdr + 0x49FC6C);

    if(pGame) 
    {
        printLog("plugin inited..");
        bPluginInitialized = true;
    }
}

void* pluginThread(void *p)
{
    while(true) 
    {
        if(bPluginInitialized) 
        {
            doPluginExecute();
        }
        else
        {
            doPluginInit();   
        }
    }

    pthread_exit(0);
}

__attribute__ ((constructor))
void pluginLoad()
{
    printLog("initializing plugin..");

    g_SAMPAdr = utilities::getLibraryHandle("libsamp.so");

    pthread_t thread;
    pthread_create(&thread, 0, pluginThread, 0);
}