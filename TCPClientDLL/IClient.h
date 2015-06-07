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
	virtual bool TCP_CLIENT_EXP loadConfig()=0;

	virtual void TCP_CLIENT_EXP SetupIOService(boost::asio::io_service& IOService)=0;
	virtual bool TCP_CLIENT_EXP Start(boost::asio::io_service& IOService)=0;
	virtual bool TCP_CLIENT_EXP Stop(boost::asio::io_service& IOService)=0;

	virtual bool TCP_CLIENT_EXP Connect(boost::asio::io_service& IOService) = 0;
	virtual bool TCP_CLIENT_EXP Disconnect(boost::asio::io_service& IOService)=0;

	virtual  MESSENGER::messenger_ptr TCP_CLIENT_EXP getMessenger()=0;
	virtual TCP::connection_ptr TCP_CLIENT_EXP GetConnectionHandle()=0;

	virtual void TCP_CLIENT_EXP SendMsg()=0;
	virtual void TCP_CLIENT_EXP SendMsgAsync()=0;
	virtual void TCP_CLIENT_EXP RcvMsg()=0;
	virtual void TCP_CLIENT_EXP RcvMsgAsync()=0;

	virtual void* TCP_CLIENT_EXP CreateTCPClientInstance(boost::asio::io_service& IOService)=0;

	virtual void TCP_CLIENT_EXP sockRead(const boost::system::error_code& e, TCP::connection_ptr conn)=0;
	virtual void TCP_CLIENT_EXP sockWrite(const boost::system::error_code& e, TCP::connection_ptr conn)=0;


};
	}//CLIENT

} // TCP

#endif //ICLIENT_H