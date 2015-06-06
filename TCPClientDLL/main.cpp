#include "TCPClient.h"

using namespace TCP::CLIENT;

int main(int argc, char* argv[])
{
	try{

	boost::asio::io_service io_service;	
	TCPClient c(io_service);		
	c.Start(io_service);
	boost::thread_group tg;
	//std::string* txtMsg = new std::string("Thank you from client ");
	//SendTxtMsg(txtMsg);//will send than you messages
		
		//IOService.run();
	//tg.create_thread(boost::bind(&TCP::MESSENGER::Messenger::asyncSendTxtMsg, *c.messenger_, boost::asio::placeholders::error, *c.conn_));

		//asyncRcvMsg(conn); //do not call this again for this connection we just got
	//tg.create_thread(boost::bind(&TCP::MESSENGER::Messenger::handleRead, *c.messenger_, boost::asio::placeholders::error, *c.conn_));
		
	tg.join_all();
	} catch(std::exception& e) {
		std::cerr << e.what() << std::endl;
	}
	

	system("pause");
}