#ifndef TCP_CLIENT_H
#define TCP_CLIENT_H

#ifdef __dll__
#define TCP_CLIENT_EXP __declspec(dllexport)
#else
#define TCP_CLIENT_EXP __declspec(dllimport)
#endif 	// __dll__

#include "IClient.h"

namespace TCP{
	namespace CLIENT{



extern "C" class TCPClient : public IClient {
public:
	connection_ptr conn_;
	MESSENGER::messenger_ptr messenger_;
	MESSENGER::MsgQ Q;
	CONFIG* config_;
	bool isConnected;
	string myAddress;
	unsigned short myPort;
	string serverAddress;
	unsigned short serverPort;
	boost::thread_group tPool;


	TCPClient(boost::asio::io_service& IOService):conn_(new TCPCom(IOService)), isConnected(false),
		messenger_(new MESSENGER::Messenger(IOService)),config_(new CONFIG("Settings/ClientConfig.ini")),Q(IOService){

		loadConfig();
		Connect(IOService);
		RcvMsgAsync(); //will start listening to socket as soon as connected to server
		SendMsgAsync();
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
	bool __cdecl loadConfig();

	void __cdecl SetupIOService(boost::asio::io_service& IOService);
	bool __cdecl Start(boost::asio::io_service& IOService);
	bool __cdecl Stop(boost::asio::io_service& IOService);

	bool __cdecl Connect(boost::asio::io_service& IOService);
	bool __cdecl Disconnect(boost::asio::io_service& IOService);

	MESSENGER::messenger_ptr __cdecl getMessenger();
	TCP::connection_ptr __cdecl GetConnectionHandle();

	void __cdecl SendMsg();
	void __cdecl SendMsgAsync();
	void __cdecl RcvMsg();
	void __cdecl RcvMsgAsync();

	void __cdecl sockRead(const boost::system::error_code& e, TCP::connection_ptr conn);
	void __cdecl sockWrite(const boost::system::error_code& e, TCP::connection_ptr conn);

	void* __cdecl CreateTCPClientInstance(boost::asio::io_service& IOService);

};
	}//CLIENT
}//TCP

#endif //TCP_CLIENT_H