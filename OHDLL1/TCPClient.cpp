#include "TCPClient.h"

using namespace TCP::CLIENT;

bool TCPClient::loadConfig(){
	//config_->defaultSettings(); // to initialize and populate default settings first
	//config_->Write(); create sample config.ini if not present
	//config_->Load();

	std::string address  = boost::lexical_cast<std::string>("localhost");
	unsigned short port = boost::lexical_cast<unsigned short>("1234");
	serverAddress = address;
	serverPort = port;
	std::cout << "Server address config settings= "<< serverAddress <<":"<< serverPort <<std::endl;
	//config_->Print();
	return true;
}

void TCPClient::SetupIOService(boost::asio::io_service& IOService){
}
bool TCPClient::Start(boost::asio::io_service& IOService){
	IOService.run();
	return true;
}

bool TCPClient::Stop(boost::asio::io_service& IOService){
	IOService.stop();
	return true;
}

bool TCPClient::Connect(boost::asio::io_service& IOService){
	if (isConnected == false){
		const std::string s_address = boost::lexical_cast<std::string>(serverAddress);
		const std::string s_port = boost::lexical_cast<std::string>(serverPort);		
		std::cout << "IsConnected=False, trying to connect to <string> "<< s_address <<":"<< s_port<<std::endl;
		conn_->ResolveAddress(IOService, s_address , s_port);
		std::cout << "IsConnected=False, trying to connect to localVars "<< serverAddress <<":"<< serverPort <<std::endl;
		conn_->connect(conn_->endpoint_iterator_);
		std::cout << "Completed conn_->connect()" << std::endl;
		myPort = conn_->GetPort();
	//	messenger_->AddNewConnection(myPort, conn_);
	//	messenger_->coutSummary();
		isConnected = true;
		std::cout << "Connected to server at port "<< myPort << std::endl;
	}

	return isConnected;
}

bool TCPClient::Disconnect(boost::asio::io_service& IOService){
	Stop(IOService);
	isConnected = false;
	return true;
}
/*
TCP::MESSENGER::messenger_ptr TCPClient::getMessenger(){
	return messenger_;
}


TCP::connection_ptr TCPClient::GetConnectionHandle(){
	return conn_;
}
*/

void TCPClient::SendMsg(){
//	messenger_->msgQ_.Add(messenger_->msgQ_.O, *msgToSend);
	
}

void TCPClient::SendMsgAsync(){
	std::cout << "Call from Client::asyncSendMsg" <<std::endl;
	//conn_->asyncSendMsg(Q.QOutgoing , boost::bind(&TCPClient::sockWrite, *this, boost::asio::placeholders::error, conn_));
}

void TCPClient::RcvMsg(){}
  

void TCPClient::RcvMsgAsync(){
	
	std::cout << "Call from Client::asyncRcvMsg" <<std::endl;
	//conn_->asyncRcvMsg(Q.QIncoming , boost::bind(&TCPClient::sockRead, *this, boost::asio::placeholders::error, conn_));
}

void* TCPClient::CreateTCPClientInstance(boost::asio::io_service& IOService){

	return static_cast< void* > (new TCPClient(IOService));
}


/// Handle completion of a read operation
  void TCPClient::sockRead(const boost::system::error_code& e, TCP::connection_ptr conn)
  {
    if (!e)
    {
      // Print out the data that was received.
		//if (!Q.QIncoming.empty()) { Q.coutQMsgs(Q.QIncoming); }
		
		//conn->asyncRcvMsg(Q.QIncoming, boost::bind(&TCPClient::sockRead, this, boost::asio::placeholders::error, conn));
    }
    else
    {
      // An error occurred.
      std::cerr << e.message() << std::endl;
    }

    // Since we are not starting a new operation the io_service will run out of
    // work to do and the client will exit.
  }

  /// Handle completion of a write operation.
	void TCPClient::sockWrite(const boost::system::error_code& e, TCP::connection_ptr conn)
	  {
		// Nothing to do. The socket will be closed automatically when the last
		// reference to the connection object goes away.
		

		//conn->async_write(msgQ_.QOutgoing , boost::bind(&MESSENGER::Messenger::SendTextMsg, this, boost::asio::placeholders::error, conn, msgText));
	
	  }
  
	