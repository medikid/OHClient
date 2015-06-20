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

namespace TCP {

/// Serves stock quote information to any client that connects to it.
class Server
{
	
private:
  /// The acceptor object used to accept incoming socket connections.
  boost::asio::ip::tcp::acceptor acceptor_;

  /// The data to be sent to each client.
  std::vector<Msg> msg_;

public:
  /// Constructor opens the acceptor and starts waiting for the first incoming
  /// connection.
  Server(boost::asio::io_service& io_service, unsigned short port): acceptor_(io_service,boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), port))
  {
   
    // Start an accept operation for a new connection.
    connection_ptr new_conn(new Connection(acceptor_.get_io_service()));
    acceptor_.async_accept(new_conn->socket(),
        boost::bind(&Server::handle_accept, this,
          boost::asio::placeholders::error, new_conn));
  }

  /// Handle completion of a accept operation.
  void handle_accept(const boost::system::error_code& e, connection_ptr conn);

  void async_send(const boost::system::error_code& e, connection_ptr conn);

  /// Handle completion of a write operation.
  void handle_write(const boost::system::error_code& e, connection_ptr conn);

  void async_recieve(const boost::system::error_code& e, connection_ptr conn);

  /// Handle completion of a read operation.
  void handle_read(const boost::system::error_code& e,  connection_ptr conn) ;

  void load_dummy_msgs();

};

} // namespace s11n_example
