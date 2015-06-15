//
// server.cpp
// ~~~~~~~~~~
//
// Copyright (c) 2003-2015 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/lexical_cast.hpp>
#include <iostream>
#include <vector>
#include "Connection.h" // Must come before boost/serialization headers.
#include <boost/serialization/vector.hpp>
#include "Msg.h"

namespace s11n_example {

/// Serves stock quote information to any client that connects to it.
class Server
{
public:
  /// Constructor opens the acceptor and starts waiting for the first incoming
  /// connection.
  Server(boost::asio::io_service& io_service, unsigned short port)
    : acceptor_(io_service,
        boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), port))
  {
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

    // Start an accept operation for a new connection.
    connection_ptr new_conn(new Connection(acceptor_.get_io_service()));
    acceptor_.async_accept(new_conn->socket(),
        boost::bind(&Server::handle_accept, this,
          boost::asio::placeholders::error, new_conn));
  }

  /// Handle completion of a accept operation.
  void handle_accept(const boost::system::error_code& e, connection_ptr conn)
  {
    if (!e)
    {
      // Successfully accepted a new connection. Send the list of stocks to the
      // client. The connection::async_write() function will automatically
      // serialize the data structure for us.

		for (std::size_t i = 0; i < msg_.size(); ++i)
      {
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

    // Start an accept operation for a new connection.
    connection_ptr new_conn(new Connection(acceptor_.get_io_service()));
    acceptor_.async_accept(new_conn->socket(), boost::bind(&Server::handle_accept, this, boost::asio::placeholders::error, new_conn));
  }

  /// Handle completion of a write operation.
  void handle_write(const boost::system::error_code& e, connection_ptr conn)
  {
    // Nothing to do. The socket will be closed automatically when the last
    // reference to the connection object goes away.
  }

private:
  /// The acceptor object used to accept incoming socket connections.
  boost::asio::ip::tcp::acceptor acceptor_;

  /// The data to be sent to each client.
  std::vector<Msg> msg_;
};

} // namespace s11n_example

int main(int argc, char* argv[])
{
  try
  {
    
    unsigned short port = boost::lexical_cast<unsigned short>("1234");

    boost::asio::io_service io_service;
    s11n_example::Server server(io_service, port);
    io_service.run();
	std::printf("Server started at port %d \n", port);

	std::cin.get();
  }
  catch (std::exception& e)
  {
    std::cerr << e.what() << std::endl;
  }

  return 0;
}