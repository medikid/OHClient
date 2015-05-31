#define OHUSER_EXPORTS

#include "OHUser.h"

bool ClientStarted = false;
//TCP::CLIENT::TCPClient* tClient;
//std::vector<GAME::GS_ptr> GS_V;
pfgws_t m_pget_winholdem_symbol = NULL; // lets initiate this to null to check later if this is obtained from OH

void initClient(){
	//boost::asio::io_service IOService;
	//TCP::CLIENT::TCPClient c(IOService);
	//tClient = new TCP::CLIENT::TCPClient(IOService);
	//c.Start(IOService);
	//GS_V.push_back( GAME::GS_ptr(new GAME::GState(IOService)));
	ClientStarted = true;
}

/////////////////////////////////////
//card macros
#define RANK(c) ((c>>4)&0x0f)
#define SUIT(c) ((c>>0)&0x0f)
#define ISCARDBACK(c) (c==0xff)
#define ISUNKNOWN(c) (c==0)
/////////////////////////////////////

////////////////////////////////////
//consecutive states
holdem_state    m_holdem_state[256];
unsigned char   m_ndx;
////////////////////////////////////




double GetSymbolFromDll( const char* name ) {
   bool iserr;
   int mychair = (int) GetSymbolFromDll(0,"userchair",iserr);
   double symb= GetSymbolFromDll(mychair,name,iserr);

   if(iserr) 
   {
      _cprintf("Error with requesting symbol %s",name);
      return -1;
   }

   return symb;
}

void tryGWS(){
	bool isErr;
	char* HandNumber = GetHandnumberFromDll();
	int nChairs =  (int)GetSymbolFromDll(0, "nchairs", isErr);
	int betRound = (int)GetSymbolFromDll(0, "betround", isErr);
	int nchairs=  (int)GetSymbolFromDll(0, "nchairs", isErr);
	double sblind=  GetSymbolFromDll(0, "sblind", isErr);
	double bblind=  GetSymbolFromDll(0, "bblind", isErr);
	int userchair=  (int)GetSymbolFromDll(0, "userchair", isErr);

	_cprintf("HandNumber: %s \n", HandNumber);
	_cprintf("nChairs: %i \n", nChairs);
	_cprintf("betround: %i \n", betRound);
	_cprintf("nchairs: %i \n", nchairs);
	_cprintf("sblind: %.2f \n", sblind);
	_cprintf("bblind: %.2f \n", bblind);
	_cprintf("userchair: %i \n", userchair);

}

double process_query( const char* pquery ) {
        if (pquery==NULL)
                return 0;

        if (strncmp(pquery,"dll$test",8)==0)
                return GetSymbolFromDll("random");

        if (strncmp(pquery,"dll$spend",9)==0)
                return GetSymbolFromDll("f$spend");

        if (strncmp(pquery,"dll$recurse",11)==0)
                return GetSymbolFromDll("dll$mynumber");

        if (strncmp(pquery,"dll$mynumber",12)==0)
                return 12345.67;

        if (strncmp(pquery,"dll$complex",11)==0)
                return GetSymbolFromDll("f$spend/25 * 1.1");

        return 0;
}


double process_state( holdem_state* pstate ) {
        if (pstate!=NULL) { 
			 m_holdem_state[ (++m_ndx)&0xff ] = *pstate; 
			_cprintf("pState: %s", m_ndx&0xff);
			bool isErr;
			
		}
        return 0;
}



/////////////////////////////////////////////////////
//WINHOLDEM RUNTIME ENTRY POINT
/////////////////////////////////////////////////////
OHUSER_API double process_message (const char* pmessage, const void* param) {
        if (pmessage==NULL) { return 0; }
        if (param==NULL) { return 0; }

        if (strcmp(pmessage,"state")==0) { 
                //holdem_state *state = (holdem_state*) param;
				//if (ClientStarted != true){ initClient(); }
			 return process_state( (holdem_state*)param ); 
        }

        if (strcmp(pmessage,"query")==0) { 
//              MessageBox(NULL, (LPCSTR) param, "query", MB_OK);
                return process_query( (const char*)param ); 
        }

		//pfgws is outdated now, use exportable functions like GetSymbolFromDll etc
        if (strcmp(pmessage,"pfgws")==0) {      
//              MessageBox(NULL, "pfgws", "MESSAGE", MB_OK);
                m_pget_winholdem_symbol = (pfgws_t)param;
                return 0;
        }

        if (strcmp(pmessage,"event")==0 && strcmp((const char *) param, "load")==0) { 
			if (ClientStarted != true){ initClient(); }
//              MessageBox(NULL, "event-load", "MESSAGE", MB_OK);
        }

        if (strcmp(pmessage,"event")==0 && strcmp((const char *) param, "unload")==0) { 
//              MessageBox(NULL, "event-unload", "MESSAGE", MB_OK);
        }

        return 0;
}