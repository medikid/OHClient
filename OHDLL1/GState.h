#ifndef GSTATE_H //VERSION:1.00
#define GSTATE_H

#include "GPlayer.h"

using namespace TCP::CLIENT;

	
namespace GAME{
	
class GState{

private:
	TCPClient GClient;
	G_ptr g;	
	std::vector<GPL_ptr> PLAYERS;
	std::map<int, GPL_ptr> ACTIVE_PLAYERS;
	std::string handhistory_;

public:
	bool isClientStarted, isErr, isTableSet, isInitProcessBets;

	GState(boost::asio::io_service& io_service): GClient(io_service),g(new G()){
		g->nChairs = 0; g->hNumber_ = 0; g->dealer_ = 0;
		isTableSet = false;
		g->gameround_ = GAMEROUND::NEW_GAME;
		GClient.Start(io_service);
		isClientStarted = true;
	}
	virtual ~GState(){}

	
	const std::string getCurrentDateTime();

	bool setTable(holdem_state* state, int nchairs, double sblind, double bblind, int userchair);

	//this should be done only once at start
	void setupPlayers(holdem_state* state, int nchairs);

	void setBoardCards(holdem_state* state);

	//set active players at beginning of the game
	void setupActivePlayers(holdem_state* state);

	bool setNewHandNumber();

	void setDealer(holdem_state* state){
		g->dealer_ = (int) state->m_dealer_chair;
	}

	//this is called after newgame announced, to delete inactive players
	bool updateActivePlayers(holdem_state* state);


	
	//called at beginning of each game
	bool resetPots(){
		g->potTotal_ = 0.00; g->pot0 = 0.00; g->pot1 = 0.00; g->pot2 = 0.00; g->potCurrentRound_ = 0.00;
		return true;
	}

	//called at beginning of each round
	bool resetBets(){
		g->betCurrent_ = 0.00; g->betsOnTable = 0.00; g->betsUnclaimed = 0.00; g->potCurrentRound_ = 0.00;
		return true;
	}

	bool resetGame();

	bool resetNewRound(holdem_state* state){
		resetBets();

		for(int chair = 0; chair < g->nChairs; chair++){
			PLAYERS.at(chair)->resetNewRound(&state->m_player[chair]);
		}
		return true;
	}

	double calcTotalBetsOnTable(holdem_state* state){
		double currentBetsOnTable = 0.00;
		for (int chair = 0; chair < g->nChairs; chair++){
			currentBetsOnTable += state->m_player[chair].m_currentbet;
		}
		return currentBetsOnTable;
	}
	
	

	char* getCurrentRound();

	//will set BetsOnTable nad Unclaimed, return true if new bets placed. Unclaimed=NewBets
	bool isNewBetsOnTable(holdem_state* state){
		double currentBetsOnTable = calcTotalBetsOnTable(state);

		if (currentBetsOnTable == g->betsOnTable){
			//no new bets
			return false;
		} else {
			g->betsUnclaimed = currentBetsOnTable - g->betsOnTable;
			g->betsOnTable = currentBetsOnTable;
			return true;
		}
	}

	bool isNewGame(holdem_state* state);
	

	bool isNewRound(int betRound, holdem_state* state);
	

	bool isNewBet(holdem_state* state);

	bool isShowdown(holdem_state* state);

	bool claimBetsOnTable(holdem_state* state, int chair){
		//g->betsUnclaimed = 0.00;
		//g->betsUnclaimed = g->betsOnTable - g->potCurrentRound_;
		return true;
	}

	void finalizePrevRound(holdem_state* state, int prevRound){
		double contrib = 0.00;
		for(std::map<int, GPL_ptr>::iterator i = ACTIVE_PLAYERS.begin(); i  != ACTIVE_PLAYERS.end(); ++i){
			ACTIVE_PLAYERS.at(i->first)->balanceCurrent = state->m_player[i->first].m_balance; //set current balance first
			ACTIVE_PLAYERS.at(i->first)->finalizePrevRound(g, prevRound);
			//for( std::vector<pBet>::iterator it = (i->second)->PLAYER_ACTIONS.begin(); it != (i->second)->PLAYER_ACTIONS.end(); ++it){
			//	contrib += 1.00;//(*it)->amount;
			//}
			//g->GUI->writePlayerAction(prevRound, i->first, "A", contrib );	
		} 
	}
	

	int getNActivePlayers();

	bool checkNActivePlayers(holdem_state* state);

	

	bool nextRound();


	
	bool eventNewBlinds(holdem_state* state);
	bool eventBet(holdem_state* state);

	bool eventNewGame(holdem_state* state);

	bool eventPreflop(holdem_state* state);
	bool eventFlop(holdem_state* state);
	bool eventTurn(holdem_state* state);
	bool eventRiver(holdem_state* state);
	bool eventShowDown(holdem_state* state);
	
	bool processBets(holdem_state* state);

	bool sendState();
	bool rcvState();
	bool procRcvdState();

	bool sendMsg();
	bool rcvMsg();
	bool procRcvdMsg();

	bool processHoldemState(holdem_state* state);

	bool printHoldemState(holdem_state* state);

	void liveCommentry();

};
typedef boost::shared_ptr<GState> GS_ptr;

}//namespace GAME

#endif //GSTATE_H