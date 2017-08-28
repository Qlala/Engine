#include "LibHeader.h"
#include "console.h"
VOID
CALLBACK
workcallback(
    PTP_CALLBACK_INSTANCE Instance,
    PVOID                 Parameter,
    PTP_WORK              Work
    )
{

    consoleEcho("thread%i",*((int*)Parameter));
    delete (int*)Parameter;
    Sleep(INFINITE);
}
int WINAPI WinMain(HINSTANCE hInstance,
                   HINSTANCE hPrevInstance,
                   LPSTR lpCmdLine,
                   int nCmdShow)
{
initConsole();
//accès au nombre de processeur
SYSTEM_INFO sysinfo;
GetSystemInfo(&sysinfo);
int proc = sysinfo.dwNumberOfProcessors;

consoleEcho("test");
consoleEcho("%i",proc);
consoleEcho("test1");
consoleEcho("test2");
consoleEcho("test3");
consoleEcho("test4");

return 0;
}


