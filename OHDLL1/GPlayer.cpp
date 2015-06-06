#include "GPlayer.h"
using namespace GAME;

bool GPlayer::setStatus(holdem_player* HPlr){
	if ((0 < (int)(HPlr->m_cards[0])) || (0 < (int)(HPlr->m_cards[1]))){
		//card value is 1 or more
		isActive_ = true;
		isDealt_ = true;
		isSeated_ = true;
		if ((255 > (int)(HPlr->m_cards[0])) || (255 > (int)(HPlr->m_cards[1]))){
			//card value is between 1-244
			setCards(HPlr);
			isShow_ = true;
		}  else {
			//card value is 255
			isShow_ = false;
		}
		return true;
	} else {
		//card value 0 or less than 0			
		isActive_ = false;
		isDealt_ = false;
		isShow_ = false;
		return false;
	}
	return false;
}

bool GPlayer::isActive(holdem_player* HPlr){
	setStatus(HPlr);
	return isActive_;
}

bool GPlayer::isShow(holdem_player* HPlr){
	setStatus(HPlr);
	return isShow_;
}

bool GPlayer::isSeated(holdem_player* HPlr){
	setStatus(HPlr);
	return isSeated_;
}

bool GPlayer::calcBetsToClaim(holdem_player* HPlr){
		if (pBalance != HPlr->m_balance){
			betToClaim = pBalance - HPlr->m_balance;
			//_cprintf("Player#%i: Bets to Claim:%.2f \n", position_, betToClaim);
			return true;
		} else return false;

	}

bool GPlayer::setCards(holdem_player* HPlr){
	memcpy(pCards, HPlr->m_cards, sizeof(HPlr->m_cards));
	return true;
}

void GPlayer::setNewPlayer(char position, holdem_player* HPlr){
	position_ = position;
	strcpy_s(pName_, HPlr->m_name);
	pBalance = HPlr->m_balance;
}
void GPlayer::resetNewGame(holdem_player* HPlr){
	if (HPlr->m_currentbet == 0.00){
		pBalance = HPlr->m_balance;
		balanceInitial = HPlr->m_balance;
		balanceCurrent = HPlr->m_balance;

		betCurrent=HPlr->m_currentbet;
		betCurrentRound = HPlr->m_currentbet;
		betCurrentHand = HPlr->m_currentbet;
	} else {
		pBalance = HPlr->m_balance + HPlr->m_currentbet; // 2000 = 1990 + 10 so we ignore the new bet
		balanceInitial = HPlr->m_balance + HPlr->m_currentbet; // 2000 = 1990 + 10 so we ignore the new bet
		balanceCurrent = HPlr->m_balance + HPlr->m_currentbet; // 2000 = 1990 + 10 so we ignore the new bet

		betCurrent=HPlr->m_currentbet;
		betCurrentRound = 0.00; //will staill set it to 0, so next scrape will catch the new bet
		betCurrentHand = 0.00;//will staill set it to 0, so next scrape will catch the new bet
	}
}

void GPlayer::resetNewRound(holdem_player* HPlr){	
	if (betToClaim != 0.00){ //_cprintf("Player#%i: Missing claiming bet %.2f \n", position_, betToClaim); 
	}
	betCurrent = 0.00;
	betToClaim = 0.00;

	if (HPlr->m_currentbet == 0.00){
		betCurrentHand += betCurrentRound;
		betCurrentRound = 0.00;
		
	} else {
		betCurrentHand += betCurrentRound;
		betCurrentRound = 0.00; //so we ignore the new bet so we capture next time
		betCurrent = 0.00;
		betToClaim = 0.00;
	}
}
bool GPlayer::processBet1(holdem_player* HPlr, G_ptr g){
	
	if(isActive(HPlr) == false){
		addPlayerAction(g->gameround_, GAME_ACTION::FOLD, 0.00);
		g->plyrsInactive.push_back(position_);
		g->GUI->writePlayerAction(g->gameround_, position_, "F", 0.00);
		return true;
	}  else {
		if( HPlr->m_currentbet > 0){ //a bet placed on table
			betCurrent = HPlr->m_currentbet;
			balanceLast = balanceCurrent;
			balanceCurrent = HPlr->m_balance;
			double actualBet = betCurrent -  betCurrentRound; //190 = 200 - 10

			if ( HPlr->m_currentbet > g->betCurrent_ ){
				//RAISES plrBet > currentbet
				if ( g->gameround_ == GAMEROUND::PRE_FLOP ){
					if (HPlr->m_currentbet == g->sBlind_){
						//Small Blind
						claimBet(HPlr, g, actualBet);
						addPlayerAction(g->gameround_, GAME_ACTION::SMALLBLIND, HPlr->m_currentbet);
						g->GUI->writePlayerAction(g->gameround_, position_, "sB", HPlr->m_currentbet);
						g->GUI->setPlayerSB(position_);
						return true;
					} else if( HPlr->m_currentbet == g->bBlind_){
						//Big Blind
						claimBet(HPlr, g, actualBet);
						addPlayerAction(g->gameround_, GAME_ACTION::BIGBLIND, HPlr->m_currentbet);
						g->GUI->writePlayerAction(g->gameround_, position_, "sB", HPlr->m_currentbet);
						g->GUI->setPlayerBB(position_);
						return true;
					}
				}
				
				if( g->betCurrent_ == 0) {
					//bets, first bet in the round
					claimBet(HPlr, g, actualBet);
					addPlayerAction(g->gameround_, GAME_ACTION::BET, actualBet);
					g->GUI->writePlayerAction(g->gameround_, position_, "B", HPlr->m_currentbet);
				} else {
					//raise
					claimBet(HPlr, g, actualBet);
					addPlayerAction(g->gameround_, GAME_ACTION::RAISE, actualBet);
					g->GUI->writePlayerAction(g->gameround_, position_, "R", HPlr->m_currentbet);
				}


			} else if ( HPlr->m_currentbet == g->betCurrent_ ){
				//CALLS betontable = currentBet
				claimBet(HPlr, g, actualBet);
				addPlayerAction(g->gameround_, GAME_ACTION::CALL, actualBet);
				g->GUI->writePlayerAction(g->gameround_, position_, "C", HPlr->m_currentbet);
			} else {
				//plrBet is less than currentBet
			}
			//BETS
			//ALLIN



		}
	}

	return false;
}

bool GPlayer::processBet(holdem_player* HPlr, G_ptr g){
	balanceLast = balanceCurrent;
	balanceCurrent = HPlr->m_balance;

	if(isActive(HPlr) == false){
		addPlayerAction(g->gameround_, GAME_ACTION::FOLD, 0.00);
		g->plyrsInactive.push_back(position_);
		g->GUI->writePlayerAction(g->gameround_, position_, "F", 0.00);
		g->GUI->setPlayerFolded(position_, g->gameround_);
		return true;
	}  else {
			betCurrent = HPlr->m_currentbet;
			balanceLast = balanceCurrent;
			balanceCurrent = HPlr->m_balance;
			double actualBet = betCurrent -  betCurrentRound; //190 = 200 - 10
			double missingChips = balanceInitial - HPlr->m_balance;
			
		if (HPlr->m_currentbet > betCurrentRound){ //200 > 10	

			if (HPlr->m_currentbet == g->betCurrent_){ // 200 = 200
				//calls
				if(actualBet == HPlr->m_currentbet){
					//new bet for this round
					claimBet(HPlr, g, actualBet);
					addPlayerAction(g->gameround_, GAME_ACTION::CALL, actualBet);
					g->GUI->writePlayerAction(g->gameround_, position_, "C", HPlr->m_currentbet);
				} else {
					//topup bet for this round
					claimBet(HPlr, g, actualBet);
					addPlayerAction(g->gameround_, GAME_ACTION::CALL, actualBet);
					g->GUI->writePlayerAction(g->gameround_, position_, "C", HPlr->m_currentbet);
				}
			} else if (HPlr->m_currentbet > g->betCurrent_){ // 200 > 10
				//raises

				if(g->gameround_ == GAMEROUND::PRE_FLOP){
					if (HPlr->m_currentbet == g->sBlind_ ){						
						claimBet(HPlr, g, actualBet);
						addPlayerAction(g->gameround_, GAME_ACTION::SMALLBLIND, actualBet);
						g->GUI->writePlayerAction(g->gameround_, position_, "sB", actualBet);
						g->GUI->setPlayerSB(position_);
					} else if (HPlr->m_currentbet == g->bBlind_){						
						claimBet(HPlr, g, actualBet);
						addPlayerAction(g->gameround_, GAME_ACTION::BIGBLIND, actualBet);
						g->GUI->writePlayerAction(g->gameround_, position_, "bB", actualBet);
						g->GUI->setPlayerBB(position_);
					} else if(actualBet == HPlr->m_currentbet){						
						claimBet(HPlr, g, actualBet);
						addPlayerAction(g->gameround_, GAME_ACTION::RAISE, actualBet);
						g->GUI->writePlayerAction(g->gameround_, position_, "R", actualBet);
					} else {						
						claimBet(HPlr, g, actualBet);
						addPlayerAction(g->gameround_, GAME_ACTION::CALL, betCurrentRound);
						g->GUI->writePlayerAction(g->gameround_, position_, "R", actualBet);
					}
				} else if(g->betCurrent_ == 0.00) {
					//bets, first person to bet in the round
					claimBet(HPlr, g, actualBet);
					addPlayerAction(g->gameround_, GAME_ACTION::BET, actualBet);
					g->GUI->writePlayerAction(g->gameround_, position_, "B", actualBet);
				} else if(actualBet == HPlr->m_currentbet){
					//raise, a new bet this round
					claimBet(HPlr, g, actualBet);
					addPlayerAction(g->gameround_, GAME_ACTION::RAISE, actualBet);
					g->GUI->writePlayerAction(g->gameround_, position_, "R", actualBet);
				} else {
					//topup bet for this round
					claimBet(HPlr, g, actualBet);
					addPlayerAction(g->gameround_, GAME_ACTION::RAISE, betCurrentRound);
					g->GUI->writePlayerAction(g->gameround_, position_, "R", actualBet);
				}
			}

			g->betCurrent_ = HPlr->m_currentbet ;
		}
	}

	return true;
}

//claim uncliamed bets
bool GPlayer::claimBet(holdem_player* HPlr, G_ptr g, double amtToClaim){
	if (amtToClaim > 0){
		g->potTotal_ += amtToClaim; //add playerBet to total pot
		g->potCurrentRound_ += amtToClaim; //add playerbet to totalBets this current round
		g->betsUnclaimed -= amtToClaim; //deduct amt from Unclaimed bets
		//_cprintf("Player%i: Claims %.2f [%.2f] \n", position_,  betCurrentRound, g->betsUnclaimed);

		//do player
		betCurrentRound += amtToClaim;
		betCurrentHand += amtToClaim;

		return true;
	} else {
		return false;
	}
}

//updates with new scrape info	
bool GPlayer::updateNewState(holdem_player* HPlr){
	strcpy_s(pName_ , HPlr->m_name);
	setCards(HPlr);
	pBalance = HPlr->m_balance;
	betCurrentRound = HPlr->m_currentbet;
	//betCurrentHand += betCurrentRound;

	return true;
}

bool GPlayer::processState(holdem_player* HPlr){
	//set status
	setStatus(HPlr);
	//check if active, if not active, set inactive, cards
	if(isActive_){
		//ACTIVE
		if(isShow_){
			setCards(HPlr);
		}

		if (HPlr->m_currentbet > 0.00){
			if (HPlr->m_currentbet == betCurrentRound){
				//same state, nothing do
				return true;
			} else {
				//new bet placed
				betCurrentRound += HPlr->m_currentbet ;
			}
		} else {
			//currentbet is 0.00, no need to do anyghing. NewRound wil take care
			return true;
		}
	} else {
		//INACTIVE
		setCards(HPlr);
	}
	return false;
}

void GPlayer::cout(){
	//_cprintf("Player#%d[%s BalanceInitial:(%.2f)-->BalanceCurrent:%.2f] Cards:[%u %u] ] Bet: %.2f[%.2f]", position_, pName_, balanceInitial, pBalance, pCards[0], pCards[1], betCurrent, betCurrentRound);
}