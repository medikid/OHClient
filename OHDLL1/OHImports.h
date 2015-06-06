#ifndef OHIMPORT_H //VERSION:1.00
#define OHIMPORTS_H

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
};

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

#endif //OHIMPORTS_H