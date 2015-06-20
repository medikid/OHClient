#ifndef _OHUSER_h_
#define _OHUSER_h_

#ifdef OHUSER_EXPORTS
#define OHUSER_API __declspec(dllexport)
#else
#define OHUSER_API __declspec(dllimport)
#endif

#include "OHImports.h"

typedef double (*process_message_t)(const char* message, const void* param );
OHUSER_API double process_message( const char* message, const void* param );
typedef double (*pfgws_t)( int c, const char* psym, bool& iserr );

extern  pfgws_t m_pget_winholdem_symbol; //gws is outdated now

//functions to import from OpenHoldem.lib

void initClient();
double gws( const char* name );
double process_state(holdem_state* pstate);
void print_state(holdem_state* pstate);
double process_query(const char* pquery);

#endif //_OHUSER_h_