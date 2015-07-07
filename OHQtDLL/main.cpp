#ifndef _WIN32_WINNT
# define _WIN32_WINNT 0x602
#endif

//#include "TCPClient.h"
//#include  "TCPConn.h"// Must come before boost/serialization headers.
//#include "Messenger.h" //will load dll, but crash after include MsgQ.h, even though no issues including all includes on it

//#include "Cfg.h" //we get DLL loadng error 126 while loading dll
#include "OHUser.h"

/////////////////////////////////////////////////////

/////////////////////////////////
//DLLMAIN
/////////////////////////////////
BOOL APIENTRY DllMain(HANDLE hModule, DWORD ul_reason_for_call, LPVOID lpReserved) {
	//GameState* pGState = new GameState(IOService);

    switch (ul_reason_for_call) {
                case DLL_PROCESS_ATTACH:  //DLL Attached to the host
                        AllocConsole();

					 // Client* client = new Client();
					 // client->start("127.0.0.1", 1234);
						//MessageBoxA(NULL,"Hello from DLL process atach","Caption",NULL);
                        break;
                case DLL_THREAD_ATTACH:
					//MessageBoxA(NULL,"Hello from DLL thread atach","Caption",NULL);
                        break;
                case DLL_THREAD_DETACH:
					//MessageBoxA(NULL,"Hello from DLL thread detach","Caption",NULL);
                        break;
                case DLL_PROCESS_DETACH: //DLL Detatched from the host
                        FreeConsole();
						//MessageBoxA(NULL,"Hello from DLL process detach","Caption",NULL);
                        break;
    }
    return TRUE;
}
/////////////////////////////////
