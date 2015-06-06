#ifndef G_H //VERSION:1.00
#define G_H

#include "OHImports.h"
#include "TCPClient.h"
#include <boost/shared_ptr.hpp>
#include <atlstr.h>
#include <conio.h>
#include <iostream>
#include <time.h>
#include <windows.h> //windows should come at the end
#include "GUIclass.h"

namespace GAME{
	enum GAMEROUND{ NEW_GAME, PRE_FLOP, FLOP, TURN, RIVER, SHOWDOWN };
	enum GAME_ACTION{ FOLD, CHECK, CALL, RAISE, BET, SMALLBLIND, BIGBLIND, ALLIN, MUCK};
	struct plrAction{ 
		GAMEROUND gRound;
		GAME_ACTION action; 
		double amount;
		
		plrAction(GAMEROUND round, GAME_ACTION actn, double amt)
			  : gRound(round), action(actn), amount(amt)
			{ gRound = round; action = actn; amount =amt; };
			
		double getAmount(){ return amount; }
	};
	

class G{
public:
	GUI_ptr GUI;

	char table[64];
	double sBlind_;
	double bBlind_;
	char* hNumber_;

	
	int nChairs;
	int dealer_;
	int sbPosn_;
	int bbPosn_;
	int hero_;

	int nActivePlayers_;

	unsigned char bCards[5];

	GAMEROUND gameround_;//0-NewGame,1-PreFlop,2-PostFlop,3-PostRiver,4-PostTurn,5-ShowDown
	double betCurrent_;

	double potTotal_;
	double pot0, pot1, pot2;
	double potCurrentRound_;

	double betsOnTable;
	double betsClaimed;
	double betsUnclaimed;
	std::vector<int> plyrsPendingAction;
	std::vector<int> plyrsInactive;
	int LastRaised;

	G():GUI(new GUIclass(0)){}

};
typedef boost::shared_ptr<G> G_ptr;
typedef boost::shared_ptr<plrAction> pBet;
}

#endif //G_H