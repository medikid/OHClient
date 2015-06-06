#ifndef TCP_CLIENT_H
#define TCP_CLIENT_H

#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include "boost\lexical_cast.hpp"
#include <iostream>
#include <vector>
#include  "TCPCom.h"// Must come before boost/serialization headers.
//#include "Messenger.h"
//#include "MsgQ.h"
#include <boost/serialization/vector.hpp>
#include "Cfg.h"
#include <string>
#include <boost/thread.hpp>

#include <stdio.h>

namespace TCP{
	namespace CLIENT{



class TCPClient {
public:
	connection_ptr conn_;
	//MESSENGER::messenger_ptr messenger_;
	//CONFIG* config_;
	bool isConnected;
	std::string myAddress;
	unsigned short myPort;
	std::string serverAddress;
	unsigned short serverPort;
	boost::thread_group tPool;


	TCPClient(boost::asio::io_service& IOService):conn_(new TCPCom(IOService)), isConnected(false){ //, Q(IOService) {
		//messenger_(new MESSENGER::Messenger(IOService)) {//,config_(new CONFIG("ClientConfig.ini")){
		/*********************************/
		std::string address  = boost::lexical_cast<std::string>("localhost");
		unsigned short port = boost::lexical_cast<unsigned short>("1234");
		serverAddress = address;
		serverPort = port;
		/*********************************/

		Connect(IOService);
		//RcvMsgAsync(); //will start listening to socket as soon as connected to server
		//SendMsgAsync();
		/*conn_->async_read(messenger_->msgQ_.QOutgoing , boost::bind(&MESSENGER::Messenger::handleRead, messenger_, boost::asio::placeholders::error, conn_));
		
		std::string* txtMsg = new std::string("");
		Msg* msg = new Msg();
		int uPort = boost::lexical_cast<int>(conn_->socket_.remote_endpoint().port());
		int iPort = boost::lexical_cast<int>(myPort);
		int cm = 6;
		int cv = 12;
		memcpy(&msg->TO, &uPort, sizeof(int));
		memcpy(&msg->FROM, &iPort, sizeof(int));
		memcpy(&msg->COMMAND_KEY, &cm, sizeof(int));
		memcpy(&msg->COMMAND_VALUE, &cv, sizeof(int));
		memcpy(&msg->SUMMARY, txtMsg, sizeof(string));
		messenger_->msgQ_.Add(messenger_->msgQ_.O, *msg);
		conn_->async_write(messenger_->msgQ_.QOutgoing , boost::bind(&MESSENGER::Messenger::asyncSendTxtMsg, messenger_, boost::asio::placeholders::error, conn_));
		*/
		
	}
	virtual ~TCPClient(){}
	bool loadConfig();

	void SetupIOService(boost::asio::io_service& IOService);
	bool Start(boost::asio::io_service& IOService);
	bool Stop(boost::asio::io_service& IOService);

	bool Connect(boost::asio::io_service& IOService);
	bool Disconnect(boost::asio::io_service& IOService);

	//MESSENGER::messenger_ptr getMessenger();
	//TCP::connection_ptr GetConnectionHandle();

	void SendMsg();
	void SendMsgAsync();
	void RcvMsg();
	void RcvMsgAsync();

	void __cdecl sockRead(const boost::system::error_code& e, TCP::connection_ptr conn);
	void __cdecl sockWrite(const boost::system::error_code& e, TCP::connection_ptr conn);

	void* CreateTCPClientInstance(boost::asio::io_service& IOService);

};
	}//CLIENT
}//TCP

#endif //TCP_CLIENT_H