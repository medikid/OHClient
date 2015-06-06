#ifndef SOCKHANDLER_H
#define SOCKHANDLER_H

#include <boost/asio.hpp>
#include <map>
#include <vector>
#include "TCPCom.h"


namespace TCP{
	namespace MESSENGER{
class SockHandler {
private:

public:
	#pragma warning(disable:4251)
	boost::asio::deadline_timer deadline_;
	boost::asio::deadline_timer heartbeat_timer_;
	std::map<unsigned short, connection_ptr> connections_;
	//DataHandler dataHandler_;
	#pragma warning(disable:4251)

	SockHandler(boost::asio::io_service& io_service): /* dataHandler_(io_service), */ deadline_(io_service), heartbeat_timer_(io_service){}
	virtual ~SockHandler(){}

	connection_ptr SelectConnection(unsigned short port){ return connections_[port]; }
	boost::asio::ip::tcp::socket& SelectSock(unsigned short port){ return connections_[port]->socket_; }

	void ConnectionAll();

	int countConnections(){ return connections_.size(); }

};
typedef boost::shared_ptr<SockHandler> SockHandler_ptr;
	}//namespace Messenger
}//namespace TCP

#endif //SOCKHANDLER_H