#ifndef MSGQ_H
#define MSGQ_H

#include <boost/bind.hpp>
#include <boost\shared_ptr.hpp>
#include "SockHandler.h"
#include <boost/serialization/vector.hpp>
#include <vector>
#include "MsgProcessor.h"

namespace TCP {
	namespace MESSENGER {

class MsgQ{

//friend class boost::serialization::access;

public:
	#pragma warning(disable:4251)
	bool I_; //Incoming msg Q
	bool O_; //Outgoing msg Q
	bool P_; //Processing msg Q
	bool Pd_; //Processed msg Q
	enum { QPd_LIMIT = 20 };
	MsgProcessor msgP;
	boost::asio::deadline_timer timer1a_;
	boost::asio::deadline_timer timer1b_;
	boost::asio::deadline_timer timer10_;


	enum QT{ I, O, P, Pd };
	std::vector<Msg> QIncoming;
	std::vector<Msg> QOutgoing;
	std::vector<Msg> QProcessing;
	std::vector<Msg> QProcessed;

	SockHandler_ptr sockHandler_;
	#pragma warning(disable:4251)
	/**/
	
	
	MsgQ(boost::asio::io_service& io_service): sockHandler_(new SockHandler(io_service)),
		I_(false),
		 O_(false),
		 P_(false),
		 Pd_(false),
		 timer1a_(io_service, boost::posix_time::seconds(1)), //triggers timer every second
		 timer1b_(io_service, boost::posix_time::seconds(1)), //triggers timer every second
		 timer10_(io_service, boost::posix_time::seconds(10)) //triggers timer 10 second
		{ 
			
			startTimers();
		}
	
	virtual ~MsgQ(){}

	//template<class Archive>
	//void serialize(Archive &ar, const unsigned int version){}

	//function handler that triggers socketIncoming and Outgoing every second
	void triggerSocketCheck(){
			//std::cout << "Calling trigger socket check" << std::endl;
		I_ = true; //next processQ will trigger socket for checkingIncoming
		O_ = true; //next processQ will trigger socket for checkingOutgoing
		
		timer1a_.expires_at(timer1a_.expires_at() + boost::posix_time::seconds(1)); //calls after 1 second
		timer1a_.async_wait(boost::bind(&MsgQ::triggerSocketCheck, this));	
	}

	void triggerProcessQ(const boost::system::error_code& e){
		int count = sockHandler_->countConnections();
		for (int c = 0; c < count; c++){
			for(std::map<unsigned short, connection_ptr>::iterator i = sockHandler_->connections_.begin(); i  != sockHandler_->connections_.end(); i++){
				processQ(e, i->second);
			}
		}
		timer1b_.expires_at(timer1b_.expires_at() + boost::posix_time::seconds(1)); //calls after 1 second
		timer1b_.async_wait(boost::bind(&MsgQ::triggerProcessQ, this, boost::asio::placeholders::error));	
	}

	void startTimers(){
		//Run processQ every 1 sec
		timer1a_.expires_at(timer1a_.expires_at() + boost::posix_time::seconds(1)); //calls after 1 second
		timer1a_.async_wait(boost::bind(&MsgQ::triggerSocketCheck, this));

		//Run processQ every 1 sec
		timer1b_.expires_at(timer1b_.expires_at() + boost::posix_time::seconds(1)); //calls after 1 second
		timer1b_.async_wait(boost::bind(&MsgQ::triggerProcessQ, this, boost::asio::placeholders::error));

		//Run cout summary every 10 sec
		//timer10_.expires_at(timer10_.expires_at() + boost::posix_time::seconds(10)); //calls after 1 second
		//timer10_.async_wait(boost::bind(&MsgQ::coutSummary, this));
	}
	
	void Add(QT q, Msg  msgRaw){	
		if (q == I){
			QIncoming.push_back(msgRaw);
			I_ = true;
		

		} else if (q == O) {
			QOutgoing.push_back(msgRaw);
			O_ = true;

		} else {
			//cannot add to other qs
			return;
		}
	}

	void Remove(QT q, int ind){
	}

	void processQ(const boost::system::error_code& e, boost::shared_ptr<TCPCom> conn_ptr){
		//std::cout << "Running Process Q for Port" << std::endl;
		if(I_){
			processQIncoming( e, conn_ptr);
		}

		if(O_){
			processQOutgoing(e, conn_ptr);
		}

		if(P_){
			processQProcessing(e, conn_ptr);
		}

		if(Pd_){
			processQProcessed(e, conn_ptr);
		}

	}


	void processQIncoming(const boost::system::error_code& e, boost::shared_ptr<TCPCom> conn_ptr){

		conn_ptr->asyncRcvMsg(QIncoming, boost::bind(&MsgQ::processQProcessing, this, boost::asio::placeholders::error, conn_ptr));
		//if any received, push them to processing workflow
		if (!(QIncoming.empty())){
			for(unsigned int i = 0; i < QIncoming.size(); ++i){
				msgP.processIncoming(QIncoming[i]);
				QProcessing.push_back(QIncoming[i]);
				QIncoming.erase(QIncoming.begin() + i);
			}
			I_ = false;
			P_ = true;		
		}	
	}

	void processQOutgoing(const boost::system::error_code& e, boost::shared_ptr<TCPCom> conn_ptr){
		if (!(QOutgoing.empty())){		
			conn_ptr->asyncSendMsg(QOutgoing, boost::bind(&MsgQ::processQProcessing, this, boost::asio::placeholders::error, conn_ptr));
			//now push the sent messages to processed folder
			for(unsigned int i = 0; i < QOutgoing.size(); ++i){
				msgP.processIncoming(QOutgoing[i]);
				QProcessed.push_back(QOutgoing[i]);
				QOutgoing.erase(QOutgoing.begin() + i);
			}
			O_ = false;
			Pd_ = true;
		}
	}

	void processQProcessing(const boost::system::error_code& e, boost::shared_ptr<TCPCom> conn_ptr){
		if (!(QProcessing.empty())){
			for(unsigned int i = 0; i < QProcessing.size(); ++i){
					//not doing anything at this time, but pushing processing=>processed
				QProcessed.push_back(QProcessing[i]);
				QProcessing.erase(QProcessing.begin() + i);
			
			}
			P_ = false;
			Pd_ = true;
		}
	}

	void processQProcessed(const boost::system::error_code& e, boost::shared_ptr<TCPCom> conn_ptr){
		if ( QProcessed.size() > QPd_LIMIT){
			for(unsigned int i = 0; i < QProcessed.size(); ++i){

				QProcessed.erase(QProcessed.begin() + i);
			}		
			Pd_ = false;
		}
	}

	void coutSummary(){
		std::cout << "/****/Message Box Que Summary/****/" << std::endl;
		std::cout << "Incoming:	" << QIncoming.size() << std::endl;
		std::cout << "Outging:	" << QOutgoing.size() << std::endl;
		std::cout << "Processing:	" << QProcessing.size() << std::endl;
		std::cout << "Processed:	" << QProcessed.size() << std::endl;
		std::cout << "/**********************************/" << std::endl;
		
		timer10_.expires_at(timer10_.expires_at() + boost::posix_time::seconds(10)); //calls after 1 second
		timer10_.async_wait(boost::bind(&MsgQ::coutSummary, this));	
	}

	void coutQMsgs(std::vector<Msg> Msgs_){
		std::size_t c = Msgs_.size();
		std::cout << "MsgQ has " << c << "msgs in total." << std::endl;
		if(!QIncoming.empty()){
		std::cout << "@@@@@@@@@@@@@@@@@@@@@@@@@@@ INCOMING Q @@@@@@@@@@@@@@@@@@"<<std::endl;
		} else std::cout << "@@@@@@@@@@@@@@@@@@@@@@@@@@@ OUTGOING Q @@@@@@@@@@@@@@@@@@@@@@@@@@@"<<std::endl;
		for (std::size_t i = 0; i < c; ++i){
						std::cout << "MsgQ has " << i << "msgs." << std::endl;

						std::cout << "//************MsgHeader****************//" << "\n";
						std::cout << "  Version#: " << Msgs_[i].VERSION << "\n";
						std::cout << "  To: " << Msgs_[i].TO << "\n";
						std::cout << "  From: " << Msgs_[i].FROM << "\n";
						std::cout << "  CommandKey: " << Msgs_[i].COMMAND_KEY << "\n";
						std::cout << "  CommandValue: " << Msgs_[i].COMMAND_VALUE << "\n";
						std::cout << "  LastRecvd: " << Msgs_[i].LAST_MSG << "\n";
						std::cout << "//************GAME******************//" << "\n";
						std::cout << "  GameType: " << Msgs_[i].TYPE << "\n";
						std::cout << "  GameRound: " << Msgs_[i].ROUND << "\n";
						std::cout << "  GameHand#: " << Msgs_[i].HAND << "\n";
						std::cout << "  GameBlind: " << Msgs_[i].BLIND << "\n";
						std::cout << "//************TABLE******************//" << "\n";
						std::cout << "  TableName: " << Msgs_[i].TITLE<< "\n";
						std::cout << "  TableChairs#: " << Msgs_[i].CHAIRS<< "\n";
						std::cout << "  TableDealer: " << Msgs_[i].DEALER << "\n";
						std::cout << "  TableSmallBlind#: " << Msgs_[i].SB << "\n";
						std::cout << "  TableBigBlind#: " << Msgs_[i].BB << "\n";
						std::cout << "  TablePot#: " << Msgs_[i].POT_NO << "\n";
						std::cout << "  TablePotValue#: " << Msgs_[i].POT_VALUE << "\n";
						std::cout << "//************PLAYER******************//" << "\n";
						std::cout << "  PlayerName: " << Msgs_[i].NAME << "\n";
						std::cout << "  PlayerPosition: " << Msgs_[i].POSITION << "\n";
						std::cout << "  PlayerBalance: " << Msgs_[i].BALANCE << "\n";
						std::cout << "  PlayerPocket#1: " << Msgs_[i].POCKET_CARD_1 << "\n";
						std::cout << "  PlayerPocket#2: " << Msgs_[i].POCKET_CARD_2 << "\n";
						std::cout << "  PlayerAction: " << Msgs_[i].ACTION << "\n";
						std::cout << "  PlayerBet: " << Msgs_[i].BET << "\n";
						//std::cout << "//	ACTION_REQUEST;
						std::cout << "  PlayerIsSeated: " << Msgs_[i].IS_SEATED << "\n";
						std::cout << "  PlayerIsActive: " << Msgs_[i].IS_ACTIVE << "\n";
						std::cout << "  PlayerIsMyTurn: " << Msgs_[i].IS_MY_TURN << "\n";
						std::cout << "//************MISC******************//" << "\n";
						std::cout << "  JSON_String#: " << Msgs_[i].JSON_STRING << "\n";
						std::cout << "  MsgSummary: " << Msgs_[i].SUMMARY << "\n";

						std::cout << "\n";
			}
		std::cout << "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@"<<std::endl;
	}


}; // class
}//namespace MSG
} // namespace TCP

#endif //MSGQ_HP