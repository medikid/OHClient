#ifndef OHIMPORT_H //VERSION:1.00
#define OHIMPORTS_H

//#include "TCPClient.h"
//#include <boost/shared_ptr.hpp>
#include <atlstr.h>
#include <conio.h>
#include <iostream>
#include <time.h>
#include <windows.h> //windows should come at the end
//#include "GUIclass.h"

extern "C" __declspec(dllimport) double __stdcall GetSymbolFromDll(const int chair, const char* name, bool& iserr); //imported function, we need OH.lib
	extern "C" __declspec(dllimport) void   __stdcall SendChatMessageFomDll(const char *msg);
	extern "C" __declspec(dllimport) void*  __stdcall GetPhl1kFromDll();
	extern "C" __declspec(dllimport) void*  __stdcall GetPrw1326FromDll();
	extern "C" __declspec(dllimport) char*  __stdcall GetHandnumberFromDll();
	extern "C" __declspec(dllimport) void   __stdcall WriteLogFromDll(char* fmt, ...);

struct holdem_player {
	char            m_name[16]          ;       //player name if known
	double          m_balance           ;       //player balance
	double          m_currentbet        ;       //player current bet
	unsigned char   m_cards[2]          ;       //player cards

	unsigned char   m_name_known    : 1 ;       //0=no 1=yes
	unsigned char   m_balance_known : 1 ;       //0=no 1=yes
	unsigned char   m_fillerbits    : 6 ;       //filler bits
	unsigned char   m_fillerbyte        ;       //filler bytes
} ;

struct holdem_state {
				char            m_title[64]         ;       //table title
				double          m_pot[10]           ;       //total in each pot

				unsigned char   m_cards[5]          ;       //common cards

				unsigned char   m_is_playing    : 1 ;       //0=sitting-out, 1=sitting-in
				unsigned char   m_is_posting    : 1 ;       //0=autopost-off, 1=autopost-on
				unsigned char   m_fillerbits    : 6 ;       //filler bits

				unsigned char   m_fillerbyte        ;       //filler byte
				unsigned char   m_dealer_chair      ;       //0-9

				holdem_player   m_player[10]        ;       //player records
			};

union iter_holdem_state{
			holdem_state* h_state;
			int components[9];
};

/////////////////////////////////////
//card macros
#define RANK(c) ((c>>4)&0x0f)
#define SUIT(c) ((c>>0)&0x0f)
#define C_RANK(c) ((int)(c % 13))
#define C_SUIT(c) ((int)(c / 13))
#define ISCARDBACK(c) (c==0xff)
#define ISUNKNOWN(c) (c==0)
/////////////////////////////////////

//#define COMPARE_STATES(c, d) for(size_t i{0}; i < sizeof(c.components); ++i) { if (c.components[i] != d.components[i]) { return true; } };


static char C_RANKS[13] = {'2', '3', '4', '5', '6', '7', '8', '9', 'T', 'J', 'Q', 'K', 'A' } ;
static char C_SUITS[4] = {'h', 'd', 'c', 's' } ;

#endif //OHIMPORTS_H