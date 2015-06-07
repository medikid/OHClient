#include "TCPServer.h"

using namespace TCP::SERVER;


bool TCPServer::loadConfig(){return true;};

void TCPServer::setupIOService(boost::asio::io_service& IOService){
}

bool TCPServer::start(boost::asio::io_service& IOService){
	IOService.run();
	return true;
}

bool TCPServer::stop(boost::asio::io_service& IOService){return true;}

/// Handle completion of a accept operation.
void TCPServer::HandleAccept(const boost::system::error_code& e, TCP::connection_ptr conn){
	std::cout << "Calling server.cpp->handle_accept()" << std::endl;
	if (!e)
	{
		std::cout << "A remote client connected at port " << conn->socket_.remote_endpoint().port() << std::endl;
		myAddress = conn->GetPort();
		messenger_->AddNewConnection(myAddress, conn); // ex map[ 1024, connection_ptr]
		
		// Successfully accepted a new connection. Send the list of stocks to the
		// client. The connection::async_write() function will automatically
		// serialize the data structure for us.


		asyncRcvMsg(conn);
		//asyncSendMsg(conn);
		SendMsg(conn);

		//messenger_.Send();
		//conn->async_read(messenger_->msgQ_.QIncoming, boost::bind(&MESSENGER::Messenger::handleRead, messenger_, boost::asio::placeholders::error, conn));
		//messenger_.msgQ_.RcvMsgFrom(conn); //this is initiated only once, one thread per socket

		// Start an accept operation for a new connection.
		TCP::connection_ptr new_conn(new TCPCom(acceptor_.get_io_service()));
		acceptor_.async_accept(new_conn->socket_,  boost::bind(&SERVER::TCPServer::HandleAccept, this, boost::asio::placeholders::error, new_conn));

	}
	else
	{
		// An error occurred. Log it and return. Since we are not starting a new
		// accept operation the io_service will run out of work to do and the
		// server will exit.
		std::cerr << e.message() << std::endl;
	}

}

TCP::MESSENGER::messenger_ptr TCPServer::getMessenger(){return messenger_;}

void TCPServer::SendMsg( TCP::connection_ptr conn){
	conn->asyncSendMsg(messenger_->msgQ_.QOutgoing , boost::bind(&MESSENGER::Messenger::SendDummyMsg, messenger_, boost::asio::placeholders::error, conn));
}

void TCPServer::asyncSendMsg( TCP::connection_ptr conn){
}

void TCPServer::RcvMsg( TCP::connection_ptr conn){
}

void TCPServer::asyncRcvMsg( TCP::connection_ptr conn){
	conn->asyncRcvMsg(messenger_->msgQ_.QIncoming , boost::bind(&MESSENGER::Messenger::handleRead, messenger_, boost::asio::placeholders::error, conn));
}