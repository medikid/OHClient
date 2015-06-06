#ifndef ISERVER_H
#define ISERVER_H

#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include "boost\lexical_cast.hpp"
#include <iostream>
#include <vector>
//#include  "TCPCom.h"// Must come before boost/serialization headers.
#include "Messenger.h"
#include <boost/serialization/vector.hpp>
#include "Cfg.h"
#include <string>
#include "boost\thread.hpp"


namespace TCP {
	namespace SERVER {

class IServer{
public:
	virtual bool loadConfig()=0;

	virtual void setupIOService(boost::asio::io_service& IOService)=0;
	virtual bool start(boost::asio::io_service& IOService)=0;
	virtual bool stop(boost::asio::io_service& IOService)=0;

	virtual void HandleAccept(const boost::system::error_code& e, connection_ptr conn) = 0;

	virtual  MESSENGER::messenger_ptr getMessenger()=0;

	virtual void SendMsg( connection_ptr conn)=0;
	virtual void asyncSendMsg( connection_ptr conn)=0;
	virtual void RcvMsg( connection_ptr conn)=0;
	virtual void asyncRcvMsg( connection_ptr conn)=0;



};
	}//Server

} // TCP

#endif //ISERVER_H