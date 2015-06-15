#ifndef ICLIENT_H
#define ICLIENT_H


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

class  IClient{
public:
	virtual bool  loadConfig()=0;

	virtual void  SetupIOService(boost::asio::io_service& IOService)=0;
	virtual bool  Start(boost::asio::io_service& IOService)=0;
	virtual bool  Stop(boost::asio::io_service& IOService)=0;

	virtual bool  Connect(boost::asio::io_service& IOService) = 0;
	virtual bool  Disconnect(boost::asio::io_service& IOService)=0;

	virtual  MESSENGER::messenger_ptr  getMessenger()=0;
	virtual TCP::connection_ptr  GetConnectionHandle()=0;

	virtual void  SendMsg()=0;
	virtual void  SendMsgAsync()=0;
	virtual void  RcvMsg()=0;
	virtual void  RcvMsgAsync()=0;

	virtual void*  CreateTCPClientInstance(boost::asio::io_service& IOService)=0;

	virtual void  sockRead(const boost::system::error_code& e, TCP::connection_ptr conn)=0;
	virtual void  sockWrite(const boost::system::error_code& e, TCP::connection_ptr conn)=0;


};
	}//CLIENT

} // TCP

#endif //ICLIENT_H