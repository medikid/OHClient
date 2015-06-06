#ifndef ICLIENT_H
#define ICLIENT_H

#ifdef __dll__
#define TCP_CLIENT_EXP __declspec(dllexport)
#else
#define TCP_CLIENT_EXP __declspec(dllimport)
#endif 	// __dll__

#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include "boost\lexical_cast.hpp"
#include <iostream>
#include <vector>
//#include  "TCPConn.h"// Must come before boost/serialization headers.
#include "Messenger.h"
#include <boost/serialization/vector.hpp>
#include "Cfg.h"
#include <string>
#include <boost/thread.hpp>

#include <stdio.h>


namespace TCP {
	namespace CLIENT {

extern "C" class  IClient{
public:
	virtual bool __cdecl loadConfig()=0;

	virtual void __cdecl SetupIOService(boost::asio::io_service& IOService)=0;
	virtual bool __cdecl Start(boost::asio::io_service& IOService)=0;
	virtual bool __cdecl Stop(boost::asio::io_service& IOService)=0;

	virtual bool __cdecl Connect(boost::asio::io_service& IOService) = 0;
	virtual bool __cdecl Disconnect(boost::asio::io_service& IOService)=0;

	virtual  MESSENGER::messenger_ptr __cdecl getMessenger()=0;
	virtual TCP::connection_ptr __cdecl GetConnectionHandle()=0;

	virtual void __cdecl SendMsg()=0;
	virtual void __cdecl SendMsgAsync()=0;
	virtual void __cdecl RcvMsg()=0;
	virtual void __cdecl RcvMsgAsync()=0;

	virtual void* __cdecl CreateTCPClientInstance(boost::asio::io_service& IOService)=0;

	virtual void __cdecl sockRead(const boost::system::error_code& e, TCP::connection_ptr conn)=0;
	virtual void __cdecl sockWrite(const boost::system::error_code& e, TCP::connection_ptr conn)=0;


};
	}//CLIENT

} // TCP

#endif //ICLIENT_H