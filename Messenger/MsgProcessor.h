#include <iostream>
#include "Msg.h"

namespace TCP{
namespace MESSENGER{

class MsgProcessor{
	double version_;
	TCP::Msg* msg_;
public:
	
	bool processIncoming(Msg msg){
		msg_ = &msg;
		switch(msg_->COMMAND_KEY){
		case TCP::COMMAND_KEY::COMMENT :
			switch(msg_->COMMAND_VALUE){
			case TCP::COMMAND_VALUE::MESSAGE:
				std::cout<<"Rcvd("<< msg_->FROM <<"):" << msg_->SUMMARY <<std::endl;
				break;
			}
			break;
		}
	return true;
}

bool processOutgoing(Msg msg){
	msg_ = &msg;
	switch(msg_->COMMAND_KEY){
	case TCP::COMMAND_KEY::COMMENT :
		switch(msg_->COMMAND_VALUE){
		case TCP::COMMAND_VALUE::MESSAGE:
			std::cout<<"Sent("<< msg_->FROM <<"):" << msg_->SUMMARY <<std::endl;
			break;
		}
		break;
	}
	return true;
}
};
}//Messenger
}//TCP