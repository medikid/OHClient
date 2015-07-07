#ifndef MSG_H
#define MSG_H

#include <qstring.h>
#include <qdatastream.h>

enum re8k_ice_nominal_frequency { nominalFrequency, Frequency50Hz };
struct re8k_ics_settings_inout
{
    unsigned char header;

    struct re8k_ics_settings_inout_nominalvoltages
    {
        unsigned int A;
        unsigned int B;
        unsigned int C;
    } nominalVoltages;

	re8k_ice_nominal_frequency FREQUENCY;

	
};

enum COMMAND_KEY{
	SET,
	INFO,
	NEW,
	CLOSE,
	RESET,
	REQUEST,
	COMMENT
};


enum COMMAND_VALUE{
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

enum EVENTS{
	//NEW GAME


};


struct Msg {
	double VERSION;
	unsigned int TO;
	unsigned int FROM;
	unsigned int COMMAND_KEY;
	unsigned int COMMAND_VALUE;
	unsigned int LAST_MSG;
	unsigned int fillerbites_1;
	//************GAME******************//
	unsigned int TYPE; //TH_NL TH_L TH_PL
	unsigned int ROUND; //PREFLOP, FLOP, TURN, RIVER, SHOW
	unsigned int HAND; //
	double BLIND;// 2.00
	unsigned int fillerbites_2;
	//************TABLE******************//
	QString TITLE;
	unsigned int CHAIRS; //9 6
	unsigned int DEALER; //5
	unsigned int SB;  //6
	unsigned int BB;
	unsigned int POT_NO;
	double POT_VALUE;
	unsigned int fillerbites_3;
	//************PLAYER******************//
	std::string NAME;
	unsigned int	POSITION;
	double	BALANCE;
	unsigned int	POCKET_CARD_1;
	unsigned int	POCKET_CARD_2;
	unsigned int	ACTION; // FOLD, CALL, CHECK, BET, RAISE
	double	BET;	
	unsigned int fillerbites_4;
	//	ACTION_REQUEST;
	unsigned int	IS_SEATED; //if sitting_out=0
	unsigned int	IS_ACTIVE; //if holding cards, =1
	unsigned int	IS_MY_TURN; //1
	unsigned int fillerbites_5;
	//************MISC******************//
	std::string JSON_STRING;
	std::string SUMMARY;
	unsigned int fillerbites_6;

};
/*
QDataStream &operator<<(QDataStream &out, const Msg &p)
	{
		out << (QString) p.TITLE;
		out << (double)p.VERSION;
		out << (double)p.BET;
		out << (double)p.BALANCE;
		out << (qint32)p.CHAIRS;
		return out;
	}

QDataStream &operator>>(QDataStream &in, re8k_ics_settings_inout &p)
	{ int freq;
		in >>p.header;
		in >> p.nominalVoltages.A;
		in >> p.nominalVoltages.B;
		in >> p.nominalVoltages.C;
		in >> freq;
		return in;
	}
	*/

#endif //MSG_H