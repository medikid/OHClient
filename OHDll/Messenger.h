#ifndef MESSENGER_H
#define MESSENGER_H

#include <boost\asio.hpp>
#include <boost\lexical_cast.hpp>
#include "MsgQ.h"
#include <string>
#include <iostream>

/*
TCPServer
TCPConnector
Messenger		
MsgQ	SocketHandler
Msg.h	DaTAhANDLER

*/


namespace TCP{
	namespace MESSENGER{
class Messenger{

public:
	#pragma warning(disable:4251)
	MsgQ msgQ_;
	int myAddress;
	#pragma warning(disable:4251)

	Messenger(boost::asio::io_service& io_service): msgQ_(io_service){ }
	virtual ~Messenger(){}

	//*************For Multiple COnnections *************************//
	
	void AddNewConnection(unsigned short port, connection_ptr new_conn_ptr){
		msgQ_.sockHandler_->connections_[port] = new_conn_ptr;
	}
	void RemoveConnection(unsigned short port){
		msgQ_.sockHandler_->connections_.erase(port);		
	}

	int CountConnections(){ return msgQ_.sockHandler_->connections_.size(); }
	void coutSummary(){
		int count = CountConnections();
		std::cout << "********Connections Summary( "<< count << " )***************"<< std::endl;
		
		for (int c = 0; c < count; c++){
			for(std::map<unsigned short, connection_ptr>::iterator i = msgQ_.sockHandler_->connections_.begin(); i  != msgQ_.sockHandler_->connections_.end(); i++){
				std::cout << c <<"). Port:  "<< i->first << std::endl;
			}
		}
		std::cout << "*************************************************************"<< std::endl;
	}
	std::vector<unsigned short> ListConnectedPorts();
	
	void SendDummyMsg(const boost::system::error_code& e, connection_ptr conn){
		printf("Writing SendDummyMsg() ");
		/*
		Msg_ptr s(new Msg());
			//s->VERSION = 1.00 ; is a constant cannot modify
			s->TO = 1234 ;
			s->FROM = 1234 ;
			s->COMMAND_KEY = TCP::COMMAND_KEY::COMMENT ;
			s->COMMAND_VALUE =TCP::COMMAND_VALUE::MESSAGE ;
			s->LAST_MSG = 0;
			// ************GAME****************** //
			s->TYPE = 1 ;
			s->ROUND =2 ;
			s->HAND = 1234568 ;
			s->BLIND = 0.25 ;
			// ************TABLE****************** //
			s->TITLE = "Harries Poker Stars Table" ;
			s->CHAIRS = 6 ;
			s->DEALER = 2 ;
			s->SB = 3 ;
			s->BB = 5 ;
			s->POT_NO = 0 ;
			s->POT_VALUE = 5.00 ;
			// ************PLAYER****************** //
			s->NAME = "Hero is" ;
			s->POSITION = 1 ;
			s->BALANCE = 3.00 ;
			s->POCKET_CARD_1 = 12 ;
			s->POCKET_CARD_2 = 25 ;
			s->ACTION = 2 ;
			s->BET = 1.00 ;
			s->IS_SEATED = 1 ;
			s->IS_ACTIVE = 1 ;
			s->IS_MY_TURN = 0 ;
			// ************MISC****************** //
			s->JSON_STRING = "JSON {String::Value}" ;
			s->SUMMARY = "This is a message from summary" ;

				msgQ_.Add(msgQ_.O, *s);
				s.reset();

			s->TO = 1234 ;
			s->FROM = 1234 ;
			s->COMMAND_KEY = TCP::COMMAND_KEY::COMMENT ;
			s->COMMAND_VALUE =TCP::COMMAND_VALUE::MESSAGE ;
			s->LAST_MSG = 1;
			// ************GAME****************** //
			s->TYPE = 1 ;
			s->ROUND =2 ;
			s->HAND = 1234568 ;
			s->BLIND = 0.25 ;
			// ************TABLE****************** //
			s->TITLE = "Harries Poker Stars Table" ;
			s->CHAIRS = 6 ;
			s->DEALER = 2 ;
			s->SB = 3 ;
			s->BB = 5 ;
			s->POT_NO = 0 ;
			s->POT_VALUE = 5.00 ;
			// ************PLAYER****************** //
			s->NAME = "Hero is" ;
			s->POSITION = 1 ;
			s->BALANCE = 3.00 ;
			s->POCKET_CARD_1 = 12 ;
			s->POCKET_CARD_2 = 25 ;
			s->ACTION = 2 ;
			s->BET = 1.00 ;
			s->IS_SEATED = 1 ;
			s->IS_ACTIVE = 1 ;
			s->IS_MY_TURN = 0 ;
			// ************MISC****************** //
			s->JSON_STRING = "JSON {String::Value}" ;
			s->SUMMARY = "Second message from summary" ; */
			Msg s = {0};
			s.SUMMARY = "Test summary";

			//msgQ_.Add(msgQ_.O, s);
			//msgQ_.triggerSocketCheck();

		//conn->asyncSendMsg(msgQ_.QOutgoing, boost::bind(&Messenger::handleWrite, this, boost::asio::placeholders::error, conn)); 
	}

	/// Handle completion of a write operation.
	void handleWrite(const boost::system::error_code& e, connection_ptr conn)
	  {
		// Nothing to do. The socket will be closed automatically when the last
		// reference to the connection object goes away.
		

		//conn->async_write(msgQ_.QOutgoing , boost::bind(&MESSENGER::Messenger::SendTextMsg, this, boost::asio::placeholders::error, conn, msgText));
	
	  }

	/// Handle completion of a read operation
  void handleRead(const boost::system::error_code& e, connection_ptr conn)
  {
    if (!e)
    {
      // Print out the data that was received.
		if (!msgQ_.QIncoming.empty()) { msgQ_.coutQMsgs(msgQ_.QIncoming); }
		
		conn->asyncRcvMsg(msgQ_.QIncoming, boost::bind(&Messenger::handleRead, this, boost::asio::placeholders::error, conn));
    }
    else
    {
      // An error occurred.
      std::cerr << e.message() << std::endl;
    }

    // Since we are not starting a new operation the io_service will run out of
    // work to do and the client will exit.
  }
  
	
};

typedef boost::shared_ptr<Messenger> messenger_ptr;

	}//namespace Messenger
}//namespace TCP

#endif //MESSENGER_H