// client.cc
#include "Client.h"
#include <QHostAddress>

Client::Client(QObject* parent): QObject(parent)
{
	conn_ = new Connection(this);
  conn_->SetSocket(1);
  connect(conn_->getSocket(), SIGNAL(connected()), this, SLOT(startTransfer()));
}

Client::~Client()
{
  conn_->getSocket()->close();
}

void Client::start(QString address, quint16 port)
{	
	servAddress_ = address;
	servPort_ = port;
	ConnectToServer();
}

void Client::ConnectToServer(){	
  conn_->getSocket()->connectToHost(servAddress_, servPort_);
  connected_ = true;
  qDebug() << "Connected to server \n"<< endl;
}

void Client::startTransfer()
{
	

 //conn->getSocket()->write("Hello, world", 13);
  //qDebug() << "data sent \n"<< endl;
	Msg msg = getDummyMsg();
  conn_->SendMsg(msg);
}

Msg Client::getDummyMsg(){
		Msg s;
			s.VERSION = 1.00 ; //is a constant cannot modify
			s.TO = 1234 ;
			s.FROM = 1234 ;
			s.COMMAND_KEY = 1;
			s.COMMAND_VALUE = 1 ;
			s.LAST_MSG = 0;
			// ************GAME****************** //
			s.TYPE = 1 ;
			s.ROUND =2 ;
			s.HAND = 1234568 ;
			s.BLIND = 0.25 ;
			// ************TABLE****************** //
			s.TITLE = "Harries Poker Stars Table" ;
			s.CHAIRS = 6 ;
			s.DEALER = 2 ;
			s.SB = 3 ;
			s.BB = 5 ;
			s.POT_NO = 0 ;
			s.POT_VALUE = 5.00 ;
			// ************PLAYER****************** //
			s.NAME = "Hero is" ;
			s.POSITION = 1 ;
			s.BALANCE = 3.00 ;
			s.POCKET_CARD_1 = 12 ;
			s.POCKET_CARD_2 = 25 ;
			s.ACTION = 2 ;
			s.BET = 1.00 ;
			s.IS_SEATED = 1 ;
			s.IS_ACTIVE = 1 ;
			s.IS_MY_TURN = 0 ;
			// ************MISC****************** //
			s.JSON_STRING = "JSON {String::Value}" ;
			s.SUMMARY = "This is a message from summary" ;

			return s;
	}

