#define OHUSER_EXPORTS

#include "OHUser.h"
//#include "TCPClient.h"


bool ClientStarted = false;
//TCP::CLIENT::TCPClient* tClient;
//std::vector<GAME::GS_ptr> GS_V;

////////////////////////////////////
//consecutive states
holdem_state    m_holdem_state[256];
holdem_state*	prev_h_state = {0};
holdem_state*	curr_h_state = {0} ;

unsigned char   m_ndx = 0;
pfgws_t m_pget_winholdem_symbol = NULL; // lets initiate this to null to check later if this is obtained from OH

////////////////////////////////////

void initClient(){
	//boost::asio::io_service IOService;
	//TCP::CLIENT::TCPClient c(IOService);
	//tClient = new TCP::CLIENT::TCPClient(IOService);
	//c.Start(IOService);
	//GS_V.push_back( GAME::GS_ptr(new GAME::GState(IOService)));
	ClientStarted = true;
}


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

bool isChangedState(holdem_state* a_state, holdem_state* b_state){
	if ( &(a_state->m_dealer_chair) != &(b_state->m_dealer_chair)) {
		//New Game Event
		_cprintf("NEW GAME\n");
	} else if ( 254 == (int)(&a_state->m_cards[0])){
		// Preflop or showdown
		if (254 > (int)(&b_state->m_cards[0]) ){
			//FLOP event
			_cprintf("FLOP\n");
		}
	} else if (254 == (int)(&a_state->m_cards[3]))  {		
		//flop
		if (254 > (int)(&b_state->m_cards[3])) {
			//TURN EVENT
			_cprintf("TURN\n");
		}
	} else if (254 ==(int)(&a_state->m_cards[4])) {
		//flop
		if (254 > (int)(&b_state->m_cards[4])){
			//TURN RIVER
			_cprintf("RIVER\n");
		} //NISHA ADDED THIS
	}
	return true;
}

bool compare_holdem_states(holdem_state* a_state, holdem_state* b_state){
	
	//memcpy(c->h_state, a_state, sizeof(holdem_state));
	//memcpy(d->h_state, b_state, sizeof(holdem_state));
	
	//memcpy(&c.h_state, a_state, sizeof(holdem_state));
	//memcpy(&d.h_state, b_state, sizeof(holdem_state));

	if ( a_state == b_state ){
			 _cprintf("Same Struct \n");
		} else _cprintf("Change in Struct \n");
		
	
		return false;
	
	//return false;
}


double process_state( holdem_state* pstate ) {
        if (pstate!=NULL) { 
			 //m_holdem_state[ (++m_ndx)&0xff ] = *pstate; 
			//_cprintf("pState: %s", m_ndx&0xff);
			//bool isErr;
			 prev_h_state = curr_h_state;
			 //memset(curr_h_state,0,sizeof(holdem_state));
			 curr_h_state = pstate;
		//memcpy( prev_h_state, cur_h_state, sizeof(holdem_state));
		//memcpy( cur_h_state, pstate, sizeof(holdem_state));
		return compare_holdem_states(prev_h_state, pstate);
		//return isChangedState(prev_h_state, curr_h_state); 
		//	 print_state(pstate);
		}
        return 0;
}

void print_state( holdem_state* pstate ){
	_cprintf("############### HOLDEM STATE SUMMARY ########################## \n");
	_cprintf("Table:	%s \n", pstate->m_title );         ;       //table title
	_cprintf("Total Pot[0]:	%.2f \n", pstate->m_pot[0] );
	_cprintf("Community Cards:	" );
		print_card(pstate->m_cards[0]);
		print_card(pstate->m_cards[1]);
		print_card(pstate->m_cards[2]);
		print_card(pstate->m_cards[3]);
		print_card(pstate->m_cards[4]); _cprintf(" %d", (int)pstate->m_cards[4]);
		/*RANK(pstate->m_cards[0]), SUIT(pstate->m_cards[0]),
		RANK(pstate->m_cards[1]), SUIT(pstate->m_cards[1]),
		RANK(pstate->m_cards[2]), SUIT(pstate->m_cards[2]),
		RANK(pstate->m_cards[3]), SUIT(pstate->m_cards[3]),
		RANK(pstate->m_cards[4]), SUIT(pstate->m_cards[4])
		 );
		 */
	_cprintf("\n");

	_cprintf("Is Hero Playing:	%s \n", (pstate->m_is_playing == 1) ? "Yes" : "No") ;
	_cprintf("Is Autopost On:	%s \n", ( pstate->m_is_posting == 1) ? "On" : "Off") ;
	_cprintf("nDealerChair:	%d \n",(int) pstate->m_dealer_chair );
	_cprintf("/////////////////////// PLAYERS SUMMARY \\\\\\\\\\\\\\\\\\\\\\\\n");
	for(int i=0; i<10; i++){
		_cprintf("%d:",i); 
		print_player((holdem_player*) &pstate->m_player[i]);
	}
}

void print_player(holdem_player* pplayer){
	_cprintf("%s (%.2f) [", pplayer->m_name, pplayer->m_balance);
		print_card(pplayer->m_cards[0]);
		print_card(pplayer->m_cards[1]);
	_cprintf("]: Bet %.2f \n", pplayer->m_currentbet);
}

void print_card(unsigned char c_card){
	//int c_rank = (card % 13) + 2; //add two to Normalize
	//int c_suit = (card / 13);
	int card = (int)c_card;	
	if (card < 254){
		_cprintf("%c%c ", C_RANKS[C_RANK(card)], C_SUITS[C_SUIT(card)] );
	} else _cprintf(" ");
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
				return process_state( (holdem_state*) param );
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