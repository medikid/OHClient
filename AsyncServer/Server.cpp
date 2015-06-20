#include "Server.h"

using namespace TCP;

  /// Handle completion of a accept operation.
  void Server::handle_accept(const boost::system::error_code& e, connection_ptr conn){
    if (!e)
    {
      // Successfully accepted a new connection. Send the list of stocks to the
      // client. The connection::async_write() function will automatically
      // serialize the data structure for us.
		std::cout << "A client connected successfully \n" ;
		//async_send(e, conn);
		async_recieve(e, conn);
   
	}
    // Start an accept operation for a new connection.
    connection_ptr new_conn(new Connection(acceptor_.get_io_service()));
    acceptor_.async_accept(new_conn->socket(), boost::bind(&Server::handle_accept, this, boost::asio::placeholders::error, new_conn));
  }

  void Server::async_send(const boost::system::error_code& e, connection_ptr conn){
	  for (std::size_t i = 0; i < msg_.size(); ++i){
			std::cout << "#################### SENDING MESSAGE-" << i << " ############## \n" ;
			std::cout << "MsgQ has " << i << "msgs." << std::endl;

						std::cout << "//************MsgHeader****************//" << "\n";
						std::cout << "  Version#: " << msg_[i].VERSION << "\n";
						std::cout << "  To: " << msg_[i].TO << "\n";
						std::cout << "  From: " << msg_[i].FROM << "\n";
						std::cout << "  CommandKey: " << msg_[i].COMMAND_KEY << "\n";
						std::cout << "  CommandValue: " << msg_[i].COMMAND_VALUE << "\n";
						std::cout << "  LastRecvd: " << msg_[i].LAST_MSG << "\n";
						std::cout << "//************GAME******************//" << "\n";
						std::cout << "  GameType: " << msg_[i].TYPE << "\n";
						std::cout << "  GameRound: " << msg_[i].ROUND << "\n";
						std::cout << "  GameHand#: " << msg_[i].HAND << "\n";
						std::cout << "  GameBlind: " << msg_[i].BLIND << "\n";
						std::cout << "//************TABLE******************//" << "\n";
						std::cout << "  TableName: " << msg_[i].TITLE<< "\n";
						std::cout << "  TableChairs#: " << msg_[i].CHAIRS<< "\n";
						std::cout << "  TableDealer: " << msg_[i].DEALER << "\n";
						std::cout << "  TableSmallBlind#: " << msg_[i].SB << "\n";
						std::cout << "  TableBigBlind#: " << msg_[i].BB << "\n";
						std::cout << "  TablePot#: " << msg_[i].POT_NO << "\n";
						std::cout << "  TablePotValue#: " << msg_[i].POT_VALUE << "\n";
						std::cout << "//************PLAYER******************//" << "\n";
						std::cout << "  PlayerName: " << msg_[i].NAME << "\n";
						std::cout << "  PlayerPosition: " << msg_[i].POSITION << "\n";
						std::cout << "  PlayerBalance: " << msg_[i].BALANCE << "\n";
						std::cout << "  PlayerPocket#1: " << msg_[i].POCKET_CARD_1 << "\n";
						std::cout << "  PlayerPocket#2: " << msg_[i].POCKET_CARD_2 << "\n";
						std::cout << "  PlayerAction: " << msg_[i].ACTION << "\n";
						std::cout << "  PlayerBet: " << msg_[i].BET << "\n";
						//std::cout << "//	ACTION_REQUEST;
						std::cout << "  PlayerIsSeated: " << msg_[i].IS_SEATED << "\n";
						std::cout << "  PlayerIsActive: " << msg_[i].IS_ACTIVE << "\n";
						std::cout << "  PlayerIsMyTurn: " << msg_[i].IS_MY_TURN << "\n";
						std::cout << "//************MISC******************//" << "\n";
						std::cout << "  JSON_String#: " << msg_[i].JSON_STRING << "\n";
						std::cout << "  MsgSummary: " << msg_[i].SUMMARY << "\n";

						std::cout << "\n";

		}
		conn->async_write(msg_, boost::bind(&Server::handle_write, this, boost::asio::placeholders::error, conn));
  }

  /// Handle completion of a write operation.
  void Server::handle_write(const boost::system::error_code& e, connection_ptr conn)
  {
    // Nothing to do. The socket will be closed automatically when the last
    // reference to the connection object goes away.
  }

   void Server::async_recieve(const boost::system::error_code& e, connection_ptr conn){
	  conn->async_read(msg_, boost::bind(&Server::handle_read, this, boost::asio::placeholders::error, conn));
  }

  /// Handle completion of a read operation.
  void Server::handle_read(const boost::system::error_code& e,  connection_ptr conn)  {
    if (!e)
    {
      // Print out the data that was received.
      for (std::size_t i = 0; i < msg_.size(); ++i){
		  std::cout << "#################### Received MESSAGE-" << i << " ############## \n" ;
        std::cout << "MsgQ has " << i << "msgs." << std::endl;

						std::cout << "//************MsgHeader****************//" << "\n";
						std::cout << "  Version#: " << msg_[i].VERSION << "\n";
						std::cout << "  To: " << msg_[i].TO << "\n";
						std::cout << "  From: " << msg_[i].FROM << "\n";
						std::cout << "  CommandKey: " << msg_[i].COMMAND_KEY << "\n";
						std::cout << "  CommandValue: " << msg_[i].COMMAND_VALUE << "\n";
						std::cout << "  LastRecvd: " << msg_[i].LAST_MSG << "\n";
						std::cout << "//************GAME******************//" << "\n";
						std::cout << "  GameType: " << msg_[i].TYPE << "\n";
						std::cout << "  GameRound: " << msg_[i].ROUND << "\n";
						std::cout << "  GameHand#: " << msg_[i].HAND << "\n";
						std::cout << "  GameBlind: " << msg_[i].BLIND << "\n";
						std::cout << "//************TABLE******************//" << "\n";
						std::cout << "  TableName: " << msg_[i].TITLE<< "\n";
						std::cout << "  TableChairs#: " << msg_[i].CHAIRS<< "\n";
						std::cout << "  TableDealer: " << msg_[i].DEALER << "\n";
						std::cout << "  TableSmallBlind#: " << msg_[i].SB << "\n";
						std::cout << "  TableBigBlind#: " << msg_[i].BB << "\n";
						std::cout << "  TablePot#: " << msg_[i].POT_NO << "\n";
						std::cout << "  TablePotValue#: " << msg_[i].POT_VALUE << "\n";
						std::cout << "//************PLAYER******************//" << "\n";
						std::cout << "  PlayerName: " << msg_[i].NAME << "\n";
						std::cout << "  PlayerPosition: " << msg_[i].POSITION << "\n";
						std::cout << "  PlayerBalance: " << msg_[i].BALANCE << "\n";
						std::cout << "  PlayerPocket#1: " << msg_[i].POCKET_CARD_1 << "\n";
						std::cout << "  PlayerPocket#2: " << msg_[i].POCKET_CARD_2 << "\n";
						std::cout << "  PlayerAction: " << msg_[i].ACTION << "\n";
						std::cout << "  PlayerBet: " << msg_[i].BET << "\n";
						//std::cout << "//	ACTION_REQUEST;
						std::cout << "  PlayerIsSeated: " << msg_[i].IS_SEATED << "\n";
						std::cout << "  PlayerIsActive: " << msg_[i].IS_ACTIVE << "\n";
						std::cout << "  PlayerIsMyTurn: " << msg_[i].IS_MY_TURN << "\n";
						std::cout << "//************MISC******************//" << "\n";
						std::cout << "  JSON_String#: " << msg_[i].JSON_STRING << "\n";
						std::cout << "  MsgSummary: " << msg_[i].SUMMARY << "\n";

						std::cout << "\n";


      }
	  //call async_read cycle again
	  async_recieve(e, conn);
    }
    else
    {
      // An error occurred.
      std::cerr << e.message() << std::endl;
    }

    // Since we are not starting a new operation the io_service will run out of
    // work to do and the client will exit.
  }


  void Server::load_dummy_msgs(){
	   // Create the data to be sent to each client.
   Msg s;
			//s.VERSION = 1.00 ; is a constant cannot modify
			s.TO = 1234 ;
			s.FROM = 1234 ;
			s.COMMAND_KEY = COMMAND_KEY::COMMENT ;
			s.COMMAND_VALUE =COMMAND_VALUE::MESSAGE ;
			s.LAST_MSG = 0;
			// ************GAME****************** //
			s.TYPE = 1 ;
			s.ROUND =2 ;
			s.HAND = 1234568 ;
			s.BLIND = 0.25 ;
			// ************TABLE****************** //
			s.TITLE = "Harries Poker Stars Table" ;
			s.CHAIRS = 6 ;
			s.DEALER = 2 ;
			s.SB = 3 ;
			s.BB = 5 ;
			s.POT_NO = 0 ;
			s.POT_VALUE = 5.00 ;
			// ************PLAYER****************** //
			s.NAME = "Hero is" ;
			s.POSITION = 1 ;
			s.BALANCE = 3.00 ;
			s.POCKET_CARD_1 = 12 ;
			s.POCKET_CARD_2 = 25 ;
			s.ACTION = 2 ;
			s.BET = 1.00 ;
			s.IS_SEATED = 1 ;
			s.IS_ACTIVE = 1 ;
			s.IS_MY_TURN = 0 ;
			// ************MISC****************** //
			s.JSON_STRING = "JSON {String::Value}" ;
			s.SUMMARY = "This is a message from summary" ;

			msg_.push_back(s);

			s.TO = 1234 ;
			s.FROM = 1234 ;
			s.COMMAND_KEY = COMMAND_KEY::COMMENT ;
			s.COMMAND_VALUE =COMMAND_VALUE::MESSAGE ;
			s.LAST_MSG = 1;
			// ************GAME****************** //
			s.TYPE = 1 ;
			s.ROUND =2 ;
			s.HAND = 1234568 ;
			s.BLIND = 0.25 ;
			//************TABLE******************//
			s.TITLE = "Harries Poker Stars Table" ;
			s.CHAIRS = 6 ;
			s.DEALER = 2 ;
			s.SB = 3 ;
			s.BB = 5 ;
			s.POT_NO = 0 ;
			s.POT_VALUE = 5.00 ;
			// ************PLAYER****************** //
			s.NAME = "Hero is" ;
			s.POSITION = 1 ;
			s.BALANCE = 3.00 ;
			s.POCKET_CARD_1 = 12 ;
			s.POCKET_CARD_2 = 25 ;
			s.ACTION = 2 ;
			s.BET = 1.00 ;
			s.IS_SEATED = 1 ;
			s.IS_ACTIVE = 1 ;
			s.IS_MY_TURN = 0 ;
			// ************MISC****************** //
			s.JSON_STRING = "JSON {String::Value}" ;
			s.SUMMARY = "Second message from summary" ;
			msg_.push_back(s);
  }
  