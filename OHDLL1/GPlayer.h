#ifndef GPLAYER_H //VERSION:1.00
#define GPLAYER_H

#include "G.h"

namespace GAME{

class GPlayer{
public:
	int position_;
	bool isSeated_;
	bool isActive_;
	bool isDealt_;
	bool isShow_; //showing up cards
	char pName_[16]; //parsedname
	char name_[16]; //reaname
	bool isDealer_; 
	unsigned char pCards[2];

	double pBalance;
	double balanceInitial;
	double balanceLast;
	double balanceCurrent;

	double betCurrent;
	double betCurrentRound;
	double betCurrentHand;
	double betToClaim;

	std::map<int, pBet> PLAYER_ACTIONS;

	GPlayer(int chairPosition){
		position_ = chairPosition;
	}
	virtual ~GPlayer(){}

	bool setStatus(holdem_player* HPlr);

	bool isActive(holdem_player* HPlr);

	bool isShow(holdem_player* HPlr);

	bool isSeated(holdem_player* HPlr);

	bool calcBetsToClaim(holdem_player* HPlr);

	bool setCards(holdem_player* HPlr);

	void setNewPlayer(char position, holdem_player* HPlr);

	void resetNewGame(holdem_player* HPlr);

	void resetNewRound(holdem_player* HPlr);

	void resetPlayerActions(){
		PLAYER_ACTIONS.clear();
	}

	
	void addPlayerAction(GAMEROUND gRound, GAME_ACTION gAction, double amt){
		pBet pb(new plrAction(gRound, gAction, amt));
		int index = PLAYER_ACTIONS.size();
		PLAYER_ACTIONS[index] = pb;
	}

	double myTotalContrib(){
		double contrib = 0.00;
		for( std::map<int, pBet>::iterator it = PLAYER_ACTIONS.begin(); it != PLAYER_ACTIONS.end(); ++it){
			contrib += PLAYER_ACTIONS.at(it->first)->getAmount();
		}
		return contrib;
	}

	void finalizePrevRound(G_ptr g, int prevRound){
		double missingChips = (balanceInitial - balanceCurrent) - myTotalContrib();
		if (missingChips > 0){	g->GUI->writePlayerAction(position_, prevRound, "A", missingChips); }
	}

	bool processBet(holdem_player* HPlr, G_ptr g);
	bool processBet1(holdem_player* HPlr, G_ptr g);
	bool claimBet(holdem_player* HPlr, G_ptr g, double amtToClaim);

	//updates with new scrape info	
	bool updateNewState(holdem_player* HPlr);

	bool processState(holdem_player* HPlr);

	void cout();
};
typedef boost::shared_ptr<GPlayer> GPL_ptr;
}//namespace GAME

#endif // GPLAYER_H