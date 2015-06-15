//
// client.cpp
// ~~~~~~~~~~
//
// Copyright (c) 2003-2010 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <iostream>
#include <vector>
#include "Connection.h" // Must come before boost/serialization headers.
#include <boost/serialization/vector.hpp>
#include "Msg.h"

namespace s11n_example {

/// Downloads stock quote information from a server.
class Client
{
public:
  /// Constructor starts the asynchronous connect operation.
  Client(boost::asio::io_service& io_service,
      const std::string& host, const std::string& service)
    : connection_(io_service)
  {
    // Resolve the host name into an IP address.
    boost::asio::ip::tcp::resolver resolver(io_service);
    boost::asio::ip::tcp::resolver::query query(host, service);
    boost::asio::ip::tcp::resolver::iterator endpoint_iterator =
      resolver.resolve(query);
    boost::asio::ip::tcp::endpoint endpoint = *endpoint_iterator;

    // Start an asynchronous connect operation.
    connection_.socket().async_connect(endpoint,
        boost::bind(&Client::handle_connect, this,
          boost::asio::placeholders::error, ++endpoint_iterator));
  }

  /// Handle completion of a connect operation.
  void handle_connect(const boost::system::error_code& e,
      boost::asio::ip::tcp::resolver::iterator endpoint_iterator)
  {
    if (!e)
    {
      // Successfully established connection. Start operation to read the list
      // of stocks. The connection::async_read() function will automatically
      // decode the data that is read from the underlying socket.
		std::printf("Client connected to Server at 1234");
      connection_.async_read(msg_, boost::bind(&Client::handle_read, this, boost::asio::placeholders::error));
    }
    else if (endpoint_iterator != boost::asio::ip::tcp::resolver::iterator())
    {
      // Try the next endpoint.
      connection_.socket().close();
      boost::asio::ip::tcp::endpoint endpoint = *endpoint_iterator;
      connection_.socket().async_connect(endpoint,
          boost::bind(&Client::handle_connect, this,
            boost::asio::placeholders::error, ++endpoint_iterator));
    }
    else
    {
      // An error occurred. Log it and return. Since we are not starting a new
      // operation the io_service will run out of work to do and the client will
      // exit.
      std::cerr << e.message() << std::endl;
    }
  }

  /// Handle completion of a read operation.
  void handle_read(const boost::system::error_code& e)
  {
    if (!e)
    {
      // Print out the data that was received.
      for (std::size_t i = 0; i < msg_.size(); ++i)
      {
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
    }
    else
    {
      // An error occurred.
      std::cerr << e.message() << std::endl;
    }

    // Since we are not starting a new operation the io_service will run out of
    // work to do and the client will exit.
  }

private:
  /// The connection to the server.
  Connection connection_;

  /// The data received from the server.
  std::vector<Msg> msg_;
};

} // namespace s11n_example

int main(int argc, char* argv[])
{
  try
  {
    
    boost::asio::io_service io_service;
    s11n_example::Client client(io_service, "localhost", "1234");
    io_service.run();

	std::cin.get();
  }
  catch (std::exception& e)
  {
    std::cerr << e.what() << std::endl;
  }

  return 0;
}