#include "GState.h"

using namespace GAME;

const std::string GState::getCurrentDateTime(){
    time_t     now = time(0);
    struct tm  tstruct;
    char       buf[80];
    localtime_s(&tstruct, &now);
    strftime(buf, sizeof(buf), "%Y/%m/%d %X %z", &tstruct);

    return buf;
}

	bool GState::setTable(holdem_state* state, int nchairs, double sblind, double bblind, int userchair){
		strcpy_s(g->table, state->m_title);
		g->nChairs = nchairs;
		g->sBlind_ = sblind;
		g->bBlind_ = bblind;
		g->hero_ = userchair;

		
		//_cprintf("*********GS->setupTable*************\n");
		//_cprintf("New Table Setup for %c \n", g->table);
		////_cprintf("nChairs: %i  SBlind: %.2f BBlind: %.2f Hero: %i \n", g->nChairs, g->sBlind_, g->bBlind_, g->hero_);

		setupPlayers(state, nchairs);
		setupActivePlayers(state);

		isTableSet = true;

		g->GUI->Start();
		g->GUI->setNPlayers(nchairs);
		g->GUI->drawFramework();
		
		return true;
	}

	//this should be done only once at start
	void GState::setupPlayers(holdem_state* state, int nchairs){
		//_cprintf("*********GS->setupPlayers*************\n");
		//_cprintf("Initial %i Players Setup\n", nchairs);
		for(int chair = 0; chair < nchairs; chair++){
			PLAYERS.push_back((GPL_ptr)(new GPlayer(chair)));
			PLAYERS.at(chair)->setNewPlayer(chair, &state->m_player[chair]);
			//_cprintf("%i: %c [%.2f] \n", chair, PLAYERS.at(chair)->pName_, PLAYERS.at(chair)->pBalance);
		}
	}

	void GState::setBoardCards(holdem_state* state){
		memcpy(g->bCards, state->m_cards, sizeof(state->m_cards));
	}

	//set active players at beginning of the game
	void GState::setupActivePlayers(holdem_state* state){
		//Setup all players at active to start, because at newgame, players are not active until dealt cards

		
		g->dealer_  = state->m_dealer_chair;
		//nonscaped table will give dealerchair = 255. In that case, set it to first chair
		if (g->dealer_ > g->nChairs){ g->dealer_ = 0; }

		ACTIVE_PLAYERS.clear();
		g->nActivePlayers_ = 0;

		int relativeChair = g->dealer_ + 1; //this is sblind
		if (relativeChair == g->nChairs) { relativeChair = 0; }

		//_cprintf("*********GS->setupActivePlayers*************\n");
		//_cprintf("Dealer: %i [%i] \n", dealerChair, nChairs);

		for(int chair = 0; chair < g->nChairs; chair++){
			PLAYERS.at(relativeChair)->setStatus(&state->m_player[relativeChair]);
			ACTIVE_PLAYERS[relativeChair] = PLAYERS.at(relativeChair); //add the player into active pool
			
			g->nActivePlayers_++;
			//_cprintf("%i: Player%i - %c[%.2f] \n", chair, relativeChair, PLAYERS.at(relativeChair)->pName_, PLAYERS.at(relativeChair)->balance);
			
			relativeChair++;
			if (relativeChair == g->nChairs) { relativeChair = 0; }
		}
	}

	//this is called after newgame announced, to delete inactive players
	bool GState::updateActivePlayers(holdem_state* state){		
		int activePlayers = ACTIVE_PLAYERS.size();
		std::vector<int> inactivePlayers; //lets collect list of inactive players, delete after the iterator
		//for (int c = 0; c < activePlayers; c++){
			for(std::map<int, GPL_ptr>::iterator i = ACTIVE_PLAYERS.begin(); i  != ACTIVE_PLAYERS.end(); i++){
				if( i == ACTIVE_PLAYERS.end()){
					//don't do anything if it reached last element, whihc is actually empty
				} else {
					ACTIVE_PLAYERS.at(i->first)->setStatus(&state->m_player[i->first]);

					if (ACTIVE_PLAYERS.at(i->first)->isActive_){
						//this player is active
						
						//i->second->processState(&state->m_player[chair]);
						//_cprintf("Player %i[%c]: ACTIVE \n", i->first, ACTIVE_PLAYERS.at(i->first)->pName_);
					} else {
						//this player is inactive, but will not delete him yet
						inactivePlayers.push_back(i->first);
						//_cprintf("Player %i[%c]: Folded \n", i->first, ACTIVE_PLAYERS.at(i->first)->pName_);
					}
				}
			}
		//}

			if (g->gameround_ == GAMEROUND::NEW_GAME || g->gameround_ == GAMEROUND::PRE_FLOP || g->gameround_ == GAMEROUND::SHOWDOWN ){
				//do not delete inactive players if newgame or preflop, since players will have not dealt cards
			} else {
				for( std::vector<int>::iterator it = inactivePlayers.begin(); it != inactivePlayers.end(); ++it){
					ACTIVE_PLAYERS.erase(*it);
				}
			}

		g->nActivePlayers_ = ACTIVE_PLAYERS.size();
		return true;
	}


	char* GState::getCurrentRound(){
		char* r = "";
		switch(g->gameround_){
			case GAMEROUND::NEW_GAME:
				r = "NEW GAME";
			break;
			case GAMEROUND::PRE_FLOP:
				r = "PRE FLOP";
			break;
			case GAMEROUND::FLOP:
				r = "FLOP";
			break;
			case GAMEROUND::TURN:
				r = "TURN";
			break;
			case GAMEROUND::RIVER:
				r = "RIVER";
			break;
			case GAMEROUND::SHOWDOWN:
				r = "SHOW DOWN";
			break;
		}
		return r;
	}
	
	bool GState::resetGame(){ return true;}

	int GState::getNActivePlayers(){
		return g->nActivePlayers_;
	}

	bool GState::checkNActivePlayers(holdem_state* state){
		int nActive = 0;
		for ( int chair = 0; chair < g->nChairs; chair++){
			if ( (0 < (int)(state->m_player[chair].m_cards[0])) || (0 < (int)(state->m_player[chair].m_cards[1]))){
				nActive++;
			}
		}
		g->nActivePlayers_ = nActive;
		return true;
	}

	bool GState::isNewGame(holdem_state* state){
		if (g->dealer_ == (int) state->m_dealer_chair){
			//samegame
			return false;
		} else {
			//new game
			g->dealer_ = (int) state->m_dealer_chair;
			setNewHandNumber();
			return true;
		}
	}

	bool GState::setNewHandNumber(){
		char* newHN = GetHandnumberFromDll();
		g->hNumber_ = newHN;
		//memcpy(handnumber_, newHN , sizeof(newHN));
		return true;
	}

	//isNewRound checked every time bets are processed
	bool GState::isNewRound(int betRound, holdem_state* state){
		switch(betRound){
			case 1://this is Newgame or flop
				if (g->gameround_ == GAMEROUND::NEW_GAME){
					checkNActivePlayers(state);
					if (g->nActivePlayers_ > 0){
						g->gameround_ = GAMEROUND::PRE_FLOP;
						eventPreflop(state);
						return true;
					} else return false;
				} else if (g->gameround_ == GAMEROUND::PRE_FLOP){
					processBets(state);
					return false;
				}
				break;
			case 2: //this isflop
				if (g->gameround_ == GAMEROUND::FLOP){
					processBets(state);
					return false;
				} else {
					g->gameround_ = GAMEROUND::FLOP;
					finalizePrevRound(state, GAMEROUND::PRE_FLOP);
					eventFlop(state);
					return true;
				}
				break;
			case 3: //this is turn
				if (g->gameround_ == GAMEROUND::TURN){
					processBets(state);
					return false;
				} else {
					g->gameround_ = GAMEROUND::TURN;					
					finalizePrevRound(state, GAMEROUND::FLOP);
					eventTurn(state);
					return true;
				}
				break;
			case 4: //this is river or show down
				if(isShowdown(state)){
					g->gameround_ = GAMEROUND::SHOWDOWN;					
					finalizePrevRound(state, GAMEROUND::RIVER);
					eventShowDown(state);
					return true;
				} else if(g->gameround_ == GAMEROUND::RIVER){
					processBets(state);
					return false;
				} else { 
					g->gameround_ = GAMEROUND::RIVER;					
					finalizePrevRound(state, GAMEROUND::TURN);
					eventRiver(state);
					return true;
				}
				break;
			default:
				return false;
		}
	}

	bool GState::isNewBet(holdem_state* state){
		 return true;
	}

	bool GState::isShowdown(holdem_state* state){
		int cardsUpPlayers = 0;
		for ( int chair = 0; chair < g->nChairs; chair++){
			if ( (0 < (int)(state->m_player[chair].m_cards[0]) && (255 > (int)(state->m_player[chair].m_cards[0])))
				||
				(0 < (int)(state->m_player[chair].m_cards[1])) && (255 > (int)(state->m_player[chair].m_cards[1])) ){
					if( g->hero_ != chair){
					cardsUpPlayers++;
					PLAYERS.at(chair)->setCards(&state->m_player[chair]);
					}
			}
		}
		if(g->hero_ >= 0){
			if(cardsUpPlayers > 1){
				return true;
			}
			return false;
		} else if (cardsUpPlayers > 0){ 
			return true;
		} else {
			return false;
		}
	}

	bool GState::nextRound(){
		switch(g->gameround_){
			case GAMEROUND::NEW_GAME:
				g->gameround_ = GAMEROUND::PRE_FLOP;
			break;
			case GAMEROUND::PRE_FLOP:
				g->gameround_ = GAMEROUND::FLOP;
			break;
			case GAMEROUND::FLOP:
				g->gameround_ = GAMEROUND::RIVER;
			break;
			case GAMEROUND::RIVER:
				g->gameround_ = GAMEROUND::TURN;
			break;
			case GAMEROUND::TURN:
				g->gameround_ = GAMEROUND::SHOWDOWN;
			break;
			case GAMEROUND::SHOWDOWN:
				g->gameround_ = GAMEROUND::NEW_GAME;
			break;
		}
		return true;
	}


	
	bool GState::eventNewBlinds(holdem_state* state){ return true;}
	bool GState::eventBet(holdem_state* state){ return true;}

	bool GState::eventNewGame(holdem_state* state){
		//reset GUI
		g->GUI->resetNewGame();
		resetPots();
		resetBets();

		setDealer(state);
		setNewHandNumber();
				

		//resetCurrentBets of all players
		for(int chair = 0; chair <g->nChairs; chair++){
			PLAYERS.at(chair)->resetNewGame(&state->m_player[chair]);

			//setInitBalance GUI
			g->GUI->writePlayerInitBalance(chair, state->m_player[chair].m_balance);
		}

		
		setupActivePlayers(state);		//	this should come after player reset	
		setBoardCards(state);

		
		g->GUI->writeDashboardValue("HAND#", g->hNumber_);
		g->GUI->writeDashboardValue("TOTPOT", g->GUI->dblToChar(g->potTotal_));
		g->GUI->writeDashboardValue("CURRENTBET", g->GUI->dblToChar(g->betCurrent_));

		g->GUI->setPlayerDealer(g->dealer_);
		g->GUI->setPlayerHero(g->hero_);

		
		return true;
	}

	bool GState::eventPreflop(holdem_state* state){		
		isInitProcessBets = true; //do not process bets until cards are dealt
		setBoardCards(state);
		updateActivePlayers(state);
		//_cprintf("GAMEROUND: PREFLOP TotalPot: %.2f  CurrentBet: %.2f Players: %i \n", g->potTotal_, g->betCurrent_, g->nActivePlayers_);

		g->GUI->writeDashboardValue("ROUND", "PREFLOP");
		g->GUI->setRound("PREFLOP", true);

		return true; 
	}
	bool GState::eventFlop(holdem_state* state){
		//wait for new bets to process bets in this round
		setBoardCards(state);
		updateActivePlayers(state);
		//_cprintf("GAMEROUND: FLOP TotalPot: %.2f  CurrentBet: %.2f Players: %i \n", g->potTotal_, g->betCurrent_, g->nActivePlayers_);
		g->GUI->writeDashboardValue("ROUND", "PFLOP");
		g->GUI->setRound("FLOP", true);
		return true;
	}
	bool GState::eventTurn(holdem_state* state){			
		isInitProcessBets = true; //process bets in this round
		setBoardCards(state);
		updateActivePlayers(state);
		//_cprintf("GAMEROUND: TURN  TotalPot: %.2f  CurrentBet: %.2f Players: %i \n", g->potTotal_, g->betCurrent_, g->nActivePlayers_);

		g->GUI->writeDashboardValue("ROUND", "TURN");
		g->GUI->setRound("TURN", true);
		return true;
	}
	bool GState::eventRiver(holdem_state* state){		
		isInitProcessBets = true; //process bets in this round
		setBoardCards(state);
		updateActivePlayers(state);
		//_cprintf("GAMEROUND: RIVER  TotalPot: %.2f  CurrentBet: %.2f Players: %i \n", g->potTotal_, g->betCurrent_, g->nActivePlayers_);

		g->GUI->writeDashboardValue("ROUND", "RIVER");
		g->GUI->setRound("RIVER", true);
		return true;
	}
	bool GState::eventShowDown(holdem_state* state){		
		isInitProcessBets = false; //do not process bets in this round
		setBoardCards(state);
		//_cprintf("GAMEROUND: SHOWDOWN  TotalPot: %.2f  CurrentBet: %.2f Players: %i \n", g->potTotal_, g->betCurrent_, g->nActivePlayers_);
		g->GUI->writeDashboardValue("ROUND", "SHOW");
		g->GUI->setRound("SHOW", true);
		return true;
	}
	
	bool GState::processBets(holdem_state* state){
		//get totalBetsOnTable	
		bool isNewBets = isNewBetsOnTable(state);
		//_cprintf("TotalBets: %.2f NewBets: %.2f \n", g->betsOnTable, g->betsUnclaimed);
		if (isNewBets == true){
			//_cprintf("TotalBets: %.2f NewBets: %.2f \n", g->betsOnTable, g->betsUnclaimed);
			//go deducting each table bet until its 0
			//loop through activePlayers, P should fold, bet or check
			for(std::map<int, GPL_ptr>::iterator i = ACTIVE_PLAYERS.begin(); i != ACTIVE_PLAYERS.end(); ++i){
			//if u reached inactive pAtAction, but still have bets firther, he must have checked and subsequent quy raised
				ACTIVE_PLAYERS.at(i->first)->processBet(&state->m_player[i->first], g);				
			}

			if(g->plyrsInactive.size() > 0 && g->plyrsInactive.size() < g->nChairs ){ //less than nchairs to avoid removing all plyrs at newgame
				for(std::vector<int>::iterator i = g->plyrsInactive.begin(); i != g->plyrsInactive.end(); i++){
					ACTIVE_PLAYERS.erase(*i);
				}
				g->plyrsInactive.clear(); 
			}
		}
		return true;
	}
	/*
	bool GState::processBets(holdem_state* state){
		//bets are added to table
		double totalBetOnTable = 0.00;
		for (int chair = 0; chair < nChairs; chair++){
			totalBetOnTable += state->m_player[chair].m_currentbet;
		}
		
		if (totalBetOnTable > potCurrentRound){
			//for(std::map<int, GPL_ptr>::iterator i = ACTIVE_PLAYERS.begin(); i != ACTIVE_PLAYERS.end(); ++i){
			for(int i = 0; i < nChairs; i++){
				// if (state->m_player[i->first].m_currentbet > i->second->betCurrentRound){
				if (state->m_player[i].m_currentbet > PLAYERS.at(i)->betCurrentRound){
					 //This player has new bet placed on table
					//_cprintf("Player%i: has new bet \n", i, PLAYERS.at(i)->betCurrentRound);
					 if (state->m_player[i].m_currentbet == currentBet){
						 double actualBet = state->m_player[i].m_currentbet -  PLAYERS.at(i)->betCurrentRound;
						 PotTotal += actualBet;
						 potCurrentRound += actualBet;
						  PLAYERS.at(i)->betCurrentRound = state->m_player[i].m_currentbet;
						  if(actualBet == state->m_player[i].m_currentbet){
							_cprintf("Player%i: Calls %.2f \n", i,  actualBet);
						  } else _cprintf("Player%i: Calls %.2f [%.2f] \n", i,  actualBet, potCurrentRound);
					 } else if(state->m_player[i].m_currentbet > currentBet ){
						 double actualBet = state->m_player[i].m_currentbet -  PLAYERS.at(i)->betCurrentRound;
						currentBet = state->m_player[i].m_currentbet;
						PotTotal += actualBet;
						 potCurrentRound += actualBet;
						 PLAYERS.at(i)->betCurrentRound = actualBet;						
						 if(actualBet == state->m_player[i].m_currentbet){
							_cprintf("Player%i: Raises %.2f \n", i,  actualBet);
						  } else _cprintf("Player%i: Raises %.2f [%.2f] \n", i,  actualBet, potCurrentRound);
					 }
				 } 
			}
			//_cprintf("TotalPot: %.2f  TablePot: %.2f  CurrentBet: %.2f \n", PotTotal, potCurrentRound, currentBet);
			return true;
		}	else if(totalBetOnTable == 0.00){
				//move potCurrentRound to Total Pot
			//PotTotal += potCurrentRound; //already added as we receive new bets
			potCurrentRound = 0.00;
			
			}

		//bets are moved to pot
		return false;
	}
	*/
	/*//Misscraping with single digits balance will have unexpected high bets
	bool GState::processBets(holdem_state* state){
		int relativeChair = dealerChair + 1; //this is sblind
		if (relativeChair == nChairs) { relativeChair = 0; }

		//_cprintf("*********GS->setupActivePlayers*************\n");
		//_cprintf("Dealer: %i [%i] \n", dealerChair, nChairs);

		for(int chair = 0; chair < nChairs; chair++){
			if(PLAYERS.at(relativeChair)->processBet(&state->m_player[relativeChair])){
				//if true, there are 2 possibilities, bet with currentbet or won with currentbet 0
				if(PLAYERS.at(relativeChair)->betCurrent > 0.00){
					//a bet
					if(currentBet == PLAYERS.at(relativeChair)->betCurrent){
						_cprintf("Player%i[%.2f -> %.2f]: Calls %.2f \n", relativeChair, PLAYERS.at(relativeChair)->balanceInitial, PLAYERS.at(relativeChair)->pBalance, PLAYERS.at(relativeChair)->betCurrent);
						PotTotal += PLAYERS.at(relativeChair)->betCurrent;
						potCurrentRound += PLAYERS.at(relativeChair)->betCurrent;
					} else {
						if(currentBet < PLAYERS.at(relativeChair)->betCurrent){
							_cprintf("Player%i[%.2f -> %.2f]: Raises %.2f \n", relativeChair,  PLAYERS.at(relativeChair)->balanceInitial, PLAYERS.at(relativeChair)->pBalance, PLAYERS.at(relativeChair)->betCurrent);
							currentBet = PLAYERS.at(relativeChair)->betCurrent;
							PotTotal += PLAYERS.at(relativeChair)->betCurrent;
							potCurrentRound += PLAYERS.at(relativeChair)->betCurrent;
						}
					}
				} else {
					//WON or balance invisible
					if(PLAYERS.at(relativeChair)->pBalance > PLAYERS.at(relativeChair)->balanceInitial){
					_cprintf("Player%i[%.2f -> %.2f]: WON %.2f \n", relativeChair, PLAYERS.at(relativeChair)->balanceInitial, PLAYERS.at(relativeChair)->pBalance,  (PLAYERS.at(relativeChair)->pBalance - PLAYERS.at(relativeChair)->balanceInitial) );
					}
				}
			}
			
			relativeChair++;
			if (relativeChair == nChairs) { relativeChair = 0; }
		}
		return true;
	}
	*/
	bool GState::sendState(){ return true;}
	bool GState::rcvState(){ return true;}
	bool GState::procRcvdState(){ return true;}

	bool GState::sendMsg(){ return true;}
	bool GState::rcvMsg(){ return true;}
	bool GState::procRcvdMsg(){ return true;}

	bool GState::processHoldemState(holdem_state* state){
		if(isNewGame(state)){
			g->gameround_ = GAMEROUND::NEW_GAME;
			eventNewGame(state);
		} else if(isNewRound((int)GetSymbolFromDll(0,"betround",isErr), state)){				
				resetNewRound(state);
		}
		
		updateActivePlayers(state);

		//liveCommentry();
		g->GUI->writeDashboardValue("TOTPOT",g->GUI->dblToChar(g->potTotal_));
		g->GUI->writeDashboardValue("CURRENTBET",g->GUI->dblToChar(g->betCurrent_));
		g->GUI->refreshScreen(); //this must be done at the end of processing.
		return true;
	}

	bool GState::printHoldemState(holdem_state* state){
		_cprintf("***************state************************\nTitle: %s\n", state->m_title);
		_cprintf("Pot-0: %.2f\n", state->m_pot[0]);
		_cprintf("Pot-1: %.2f\n", state->m_pot[1]);
		_cprintf("Board Cards: %u %u %u %u %u\n", state->m_cards[0], state->m_cards[1], state->m_cards[2], state->m_cards[3], state->m_cards[4]);
		_cprintf("Is Playing?: %u\n", state->m_is_playing);
		_cprintf("Is Posting?: %u\n", state->m_is_posting);
		_cprintf("Dealer Chair#: %u\n", state->m_dealer_chair);

		for ( int chair = 0; chair < g->nChairs; chair++){
			if ( (0 < (int)(state->m_player[chair].m_cards[0])) || (0 < (int)(state->m_player[chair].m_cards[1]))){
				_cprintf("Player%d#[%s Balance:(%.2f) Cards:[%u %u] ]: ", chair, state->m_player[chair].m_name, state->m_player[chair].m_balance, state->m_player[chair].m_cards[0], state->m_player[chair].m_cards[1], state->m_player[chair].m_currentbet);
				
				if (0.00 < state->m_player[chair].m_currentbet){
					_cprintf("Bet %.2f \n", state->m_player[chair].m_currentbet);
				} else _cprintf("\n");
			}
		}

		return true;
	}

	void GState::liveCommentry(){
		_cprintf("Table: %c \n", g->table);
		_cprintf("Hand#: %s Dealer#: %i[%i]  Players#: %i \n", g->hNumber_, g->dealer_, g->nChairs, g->nActivePlayers_ );
		_cprintf("TotalPot: %.2f TablePot: %.2f CurrentBet: %.2f \n", g->potTotal_, g->potCurrentRound_, g->betCurrent_ );
		_cprintf("Board: %u %u %u  %u  %u Round: %s \n",  g->bCards[0], g->bCards[1], g->bCards[2], g->bCards[3], g->bCards[4], getCurrentRound());
		if (g->hero_ >= 0){
		_cprintf("Hero[%i]: %c %c     Hand:  \n", g->hero_, PLAYERS[g->hero_]->pCards[0], PLAYERS[g->hero_]->pCards[1]);
		}
		_cprintf("\n");
		_cprintf("\n %s \n", handhistory_);
	}