#ifndef MSG_H //VERSION:1.00
#define MSG_H

#ifdef MSG_EXPORTS
#define MSG_API __declspec(dllexport)
#else
#define MSG_API __declspec(dllimport)
#endif

#include <string>

namespace TCP{

MSG_API  enum COMMAND_KEY{
	SET,
	INFO,
	NEW,
	CLOSE,
	RESET,
	REQUEST,
	COMMENT
};


MSG_API enum COMMAND_VALUE{
	GAME,
	ROUND,
	TABLE,
	BOARD,
	POT,
	PLAYER,
	DEALER,
	HERO,
	POCKET,
	ACTION,
	HELLO,
	THANKYOU,
	MESSAGE,
	QUERY
};


MSG_API enum EVENTS{
	//NEW GAME


};


MSG_API struct Msg {
	double VERSION;
	int TO;
	int FROM;
	int COMMAND_KEY;
	int COMMAND_VALUE;
	int LAST_MSG;
	//************GAME******************//
	int TYPE; //TH_NL TH_L TH_PL
	int ROUND; //PREFLOP, FLOP, TURN, RIVER, SHOW
	int HAND; //
	double BLIND;// 2.00
	//************TABLE******************//
	std::string TITLE;
	int CHAIRS; //9 6
	int DEALER; //5
	int SB;  //6
	int BB;
	int POT_NO;
	double POT_VALUE;
	//************PLAYER******************//
	std::string NAME;
	int	POSITION;
	double	BALANCE;
	int	POCKET_CARD_1;
	int	POCKET_CARD_2;
	int	ACTION; // FOLD, CALL, CHECK, BET, RAISE
	double	BET;
	//	ACTION_REQUEST;
	int	IS_SEATED; //if sitting_out=0
	int	IS_ACTIVE; //if holding cards, =1
	int	IS_MY_TURN; //1
	//************MISC******************//
	std::string JSON_STRING;
	std::string SUMMARY;

	 template <typename Archive>
	void serialize(Archive& ar, const unsigned int version)
	{
		ar & VERSION;
		ar & TO;
		ar & FROM;
		ar & COMMAND_KEY;
		ar & COMMAND_VALUE;
		//************GAME******************//
		ar & TYPE;
		ar & ROUND;
		ar & HAND;
		ar & BLIND;
		//************TABLE******************//
		ar & TITLE;
		ar & CHAIRS;
		ar & DEALER;
		ar & SB;
		ar & BB;
		ar & POT_NO;
		ar & POT_VALUE;
		//************PLAYER******************//
		ar & NAME;
		ar & POSITION;
		ar & BALANCE;
		ar & POCKET_CARD_1;
		ar & POCKET_CARD_2;
		ar & ACTION;
		ar & BET;
		ar & IS_SEATED;
		ar & IS_ACTIVE;
		ar & IS_MY_TURN;
		//************MISC******************//
		ar & JSON_STRING;
		ar & SUMMARY;
	}
}Message;

}//namespace TCP

#endif //MSG_H