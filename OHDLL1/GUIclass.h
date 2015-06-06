#ifndef GUICLASS_H //VERSION:1.00
#define GUICLASS_H

#include <boost/shared_ptr.hpp>
#include <iostream>
#include <stdlib.h>
#include <atlstr.h>
#include <conio.h>
#include <curses.h>
#include <map>

#include <locale.h>
#include <stdio.h>
#include <stdlib.h>
#include <wctype.h>
#include <wchar.h>

#include <string>
#include <cstring>
#include <sstream>

namespace GAME{

struct point{
public:
	int X, Y;
	int offset;
	point(){};
	point(int x, int y): X(x), Y(y){ }
	void setOffset(int ofs){ offset = ofs; }
};

class GUIclass{
	int nPlayers, maxheight, maxwidth, num, colorEven, colorOdd;
public:	
	std::map<char*, point> OFFSETS;

	GUIclass(int NPlayers){
		num = 0;
		nPlayers = NPlayers;
	}

	void Start(){
		initscr();		
		getmaxyx(stdscr, maxheight, maxwidth);
		initColors();
		initOffsets();
		
	}

	void Stop(){
		endwin();
	}

	void setNPlayers(int NPlayers){
		nPlayers = NPlayers;
	}

	void initColors(){
		start_color();
		init_pair(1, COLOR_WHITE, COLOR_CYAN);
		init_pair(2, COLOR_GREEN, COLOR_GREEN);
		init_pair(3, COLOR_BLACK, COLOR_WHITE);
		init_pair(4, COLOR_WHITE, COLOR_BLACK);
		init_pair(5, COLOR_WHITE, COLOR_GREEN);
		init_pair(6, COLOR_WHITE, COLOR_CYAN);

		colorEven = 3; colorOdd = 4;
	}

	void initOffsets(){
		OFFSETS["HAND#"] =  point(9,1); OFFSETS["HAND#"].setOffset(15);
		OFFSETS["ROUND"] =  point(33, 1);OFFSETS["ROUND"].setOffset(15);
		OFFSETS["BOARD"] = point(9,2); OFFSETS["BOARD"].setOffset(15);
		OFFSETS["TOTPOT"] = point(33,2); OFFSETS["TOTPOT"].setOffset(15);
		OFFSETS["CURRENTBET"] = point(62,2); OFFSETS["CURRENTBET"].setOffset(15);
		OFFSETS["POCKET"] = point(9,3); OFFSETS["POCKET"].setOffset(15);
		OFFSETS["CONTRIB"] = point(33,3); OFFSETS["CONTRIB"].setOffset(15);
		OFFSETS["HANDVALUE"] = point(62,3); OFFSETS["HANDVALUE"].setOffset(15);

		OFFSETS["PLAYERS"] = point(1, 5);
		
		OFFSETS["PLAYERPOSITION"] = point(1, 5);OFFSETS["PLAYERPOSITION"].setOffset(3);
		OFFSETS["PLAYERTAG"] = point(5,5); OFFSETS["PREFLOP"].setOffset(3);
		OFFSETS["PLAYERINITBALANCE"] = point(9,5); OFFSETS["PLAYERINITBALANCE"].setOffset(8);

		OFFSETS["PREFLOP"] = point(18,5); OFFSETS["PREFLOP"].setOffset(10);//18->28, prepadded
		OFFSETS["FLOP"] = point(31,5); OFFSETS["FLOP"].setOffset(10);//31->41
		OFFSETS["TURN"] = point(44,5); OFFSETS["TURN"].setOffset(10); //44->54
		OFFSETS["RIVER"] = point(57,5); OFFSETS["RIVER"].setOffset(10); //57->67
		OFFSETS["SHOW"] = point(69,5); OFFSETS["SHOW"].setOffset(9); //57->67
	}

	char* padText(char* value, char* pad, int length){
		char val[25];
		char* valc = new char[25];
		//char* pad = " ";
		//char * val = (char *) malloc(1 + OFFSETS[label].offset );
		strcpy_s(val, pad);
		strcat_s(val, value);
		for(int i = strlen(val); i < length; i++){
			strcat_s(val, pad);
		}
		puts(val);

		memcpy_s(valc, (strlen(valc)+1), val, (strlen(val)+1));
		puts(valc);

		return valc;
	}

	char* iToChar(int value){
		char* valueC = new char[25] ;
		_itoa_s(value, valueC, strlen(valueC), 10);

		return valueC;
	}

	char* dblToChar(double value){
		char* valc = new char[25] ;
		std::ostringstream ss;
		ss << value;
		memcpy_s(valc, (strlen(valc)+1), ss.str().c_str(), (strlen(ss.str().c_str())+1));

		return valc;
	}

	void writeDashboardValue(char* label, char* value){
		 char* valc = padText(value, " ", OFFSETS[label].offset);
			
		attron(COLOR_PAIR(3));
		moveCursor(OFFSETS[label]);
		printw("%s", valc);

		attroff(COLOR_PAIR(3));
	}

	void setRound(char* round, bool setOn ){
		int ColorPairNo = (setOn) ? 5 : 4;

		attron(COLOR_PAIR(ColorPairNo));
		char prepadded[25];
		strcpy_s(prepadded, "  ");
		strcat_s(prepadded, round);
		char* paddedRoundLbl = padText(prepadded, " ",  (OFFSETS[round].offset + 1));

		moveCursor(OFFSETS[round]);
		printw("%s", paddedRoundLbl);

		attroff(COLOR_PAIR(ColorPairNo));
		
	}

	void resetNewGame(){
		writeDashboardValue("HAND#", "");
		writeDashboardValue("ROUND", "");
		writeDashboardValue("BOARD", "");
		writeDashboardValue("TOTPOT", "");
		writeDashboardValue("CURRENTBET", "");
		writeDashboardValue("POCKET", "");
		writeDashboardValue("CONTRIB", "");
		writeDashboardValue("HANDVALUE", "");

		setRound("PREFLOP", false);
		setRound("FLOP", false);
		setRound("TURN", false);
		setRound("RIVER", false);
		setRound("SHOW", false);

		drawPlayers(nPlayers);
	}

	void setDash(){
		//writeDashboardValue("HAND#", g->hNumber_);
		writeDashboardValue("BOARD", "");
		//writeDashboardValue("TOTPOT", dblToChar(g->potTotal_));
		//writeDashboardValue("CURRENTBET", dblToChar(g->betCurrent_));
		writeDashboardValue("POCKET", "");
		writeDashboardValue("CONTRIB", "");
		writeDashboardValue("HANDVALUE", "");
	}

	void moveCursor(point p){
		move(p.Y, p.X);
	}

	void drawFramework(){
		drawLine(2, point(0,0), point(maxwidth-1, 0));
		drawLine(2, point(maxwidth-1, 0), point(maxwidth-1, maxheight-1));
		drawLine(2, point(0, maxheight-1), point(maxwidth-1, maxheight-1));		
		drawLine(2, point(0,0), point(0, maxheight-1));

		drawLine(2, point(0,4), point(maxwidth-1, 4));

		drawLabel(1, point(1,1), "Hand#");
		drawLabel(1, point(25,1), "Round");
		drawLabel(1, point(1,2), "Board");
		drawLabel(1, point(25,2), "TotPot");
		drawLabel(1, point(50,2), "CurrentBet");
		drawLabel(1, point(1,3), "Pocket");
		drawLabel(1, point(25,3), "Contrib");
		drawLabel(1, point(50,3), "HandValue");

		drawLabel(1, point(18,5), " PREFLOP  ");//18->28, prepadded
		drawLabel(1, point(31,5), "  FLOP    ");//31->41
		drawLabel(1, point(44,5), "  TURN    "); //44->54
		drawLabel(1, point(57,5), "  RIVER   "); //57->67
		drawLabel(1, point(69,5), "  SHOW   "); //57->67
		drawPlayers(nPlayers);

		point pLabels = point(OFFSETS["PLAYERS"].X, (OFFSETS["PLAYERS"].Y - 1));

		drawLine(2, point(4,(OFFSETS["PLAYERS"].Y - 1)), point(4,(OFFSETS["PLAYERS"].Y + nPlayers + 1)));
		drawLine(2, point(8,(OFFSETS["PLAYERS"].Y - 1)), point(8,(OFFSETS["PLAYERS"].Y + nPlayers + 1)));
		drawLine(2, point(17,(OFFSETS["PLAYERS"].Y - 1)), point(17,(OFFSETS["PLAYERS"].Y + nPlayers + 1)));

		drawLine(2, point(29,(OFFSETS["PLAYERS"].Y - 1)), point(29,(OFFSETS["PLAYERS"].Y + nPlayers + 1))); 
		drawLine(2, point(30,(OFFSETS["PLAYERS"].Y - 1)), point(30,(OFFSETS["PLAYERS"].Y + nPlayers + 1)));

		drawLine(2, point(42,(OFFSETS["PLAYERS"].Y - 1)), point(42,(OFFSETS["PLAYERS"].Y + nPlayers + 1))); 
		drawLine(2, point(43,(OFFSETS["PLAYERS"].Y - 1)), point(43,(OFFSETS["PLAYERS"].Y + nPlayers + 1)));

		drawLine(2, point(55,(OFFSETS["PLAYERS"].Y - 1)), point(55,(OFFSETS["PLAYERS"].Y + nPlayers + 1))); 
		drawLine(2, point(56,(OFFSETS["PLAYERS"].Y - 1)), point(56,(OFFSETS["PLAYERS"].Y + nPlayers + 1)));

		drawLine(2, point(68,(OFFSETS["PLAYERS"].Y - 1)), point(68,(OFFSETS["PLAYERS"].Y + nPlayers + 1)));
				
	}

	void drawLine(int ColorPairNo, point startPoint, point endPoint){
		attron(COLOR_PAIR(ColorPairNo));
		if (startPoint.Y == endPoint.Y){
			//horizontal line
			for(int i = startPoint.X; i < endPoint.X; i++){
				moveCursor(point( i, startPoint.Y));
				addch(char(219));
			}
		} else {
			//¨vertical line
			for(int i = startPoint.Y; i < endPoint.Y; i++){
				moveCursor(point(startPoint.X, i));
				addch(char(219));
			}
		}
		attroff(COLOR_PAIR(ColorPairNo));
	}

	void drawLabel(int ColorPairNo, point p, char* labelText){
		attron(COLOR_PAIR(ColorPairNo));
		moveCursor(p);
		printw(" %s: ", labelText);
		attroff(COLOR_PAIR(ColorPairNo));
	}

	void drawPlayers(int nPlayers){
		int colorPlayer = 0;

		for(int i = 0; i < nPlayers; i++){
			accessPlayerField(i, "PLAYERS");
			printw(" %d %c   %c        %c           %c%c           %c%c           %c%c           %c           ",
				i, char(219),  char(219), char(219), char(219), char(219), char(219), char(219), char(219), char(219), char(219));
		} // 1,5,9,18,31,44,57,69

		drawLine(2, point(1, ( OFFSETS["PLAYERS"].Y + nPlayers + 1)), point((maxwidth-1),( OFFSETS["PLAYERS"].Y + nPlayers + 1)));
		
	}

	void accessPlayerField(int playerPosition, char* sField){
		int colorPlayer;
		if ((playerPosition % 2) == 1) { colorPlayer = colorOdd; } else { colorPlayer = colorEven;}
		attron(COLOR_PAIR(colorPlayer));
		moveCursor(point( OFFSETS[sField].X, ( OFFSETS["PLAYERS"].Y + playerPosition + 1)));
	}


	void incNum(){
		num++;
	}
	void writePlayerTag(int nPosition, char* tag){
		
	}
	void setPlayerDealer(int playerPosition){
		accessPlayerField(playerPosition, "PLAYERTAG");
		printw(" D ");
	}
	void setPlayerSB(int playerPosition){
		accessPlayerField(playerPosition, "PLAYERTAG");
		printw(" sB");
	}
	void setPlayerBB(int playerPosition){
		accessPlayerField(playerPosition, "PLAYERTAG");
		printw(" bB");
	}
	void setPlayerHero(int playerPosition){
		accessPlayerField(playerPosition, "PLAYERPOSITION");
		printw("*");
	}
	void writePlayerInitBalance(int playerPosition, double balance){
		accessPlayerField(playerPosition, "PLAYERINITBALANCE");
		printw(" %s", dblToChar(balance));
	}

	char* getStrRound(int nRound){
		char* sRound = "";
		switch(nRound){
			case 0:
				sRound = "PREFLOP"; //write newgame actions in preflop column
			break;
			case 1:
				sRound = "PREFLOP";
			break;
			case 2:
				sRound = "FLOP";
			break;
			case 3:
				sRound = "TURN";
			break;
			case 4:
				sRound = "RIVER";
			break;
			case 5:
				sRound = "SHOW";
			break;
		};
		return sRound;
	}

	void writePlayerAction(int nRound, int playerPosition, char* sAction, double amt){
		char* sRound = getStrRound(nRound);
		accessPlayerField(playerPosition, sRound);
		printw(" %s  %s", sAction, dblToChar(amt));
	}

	void setPlayerFolded(int position, int nRound){
		int maxRound = 5; //show round

		for (int round = nRound; round < maxRound; round++){			
			char* sRound = getStrRound(round + 1);
			accessPlayerField(position, sRound);		
			char prepadded[25];
			strcpy_s(prepadded, "#");
			strcat_s(prepadded, "#");
			char* paddedRoundLbl = padText(prepadded, "#",  (OFFSETS[sRound].offset + 1));

			accessPlayerField(position, sRound);
			printw("%s", paddedRoundLbl);
		}
	}
	void displayScreen(){

	
	}

	void refreshScreen(){
		refresh();
	}

	void clrScreen(){
		system("CLS");
	}
};
typedef boost::shared_ptr<GUIclass> GUI_ptr;
} //namespace GAME

#endif //GUICLASS_H