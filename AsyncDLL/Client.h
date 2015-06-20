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

namespace TCP {

/// Downloads stock quote information from a server.
class Client
{
	
public:
	/// The connection to the server.
  connection_ptr connection_;

  /// The data received from the server.
  std::vector<Msg> msg_;


  /// Constructor starts the asynchronous connect operation.
  Client(boost::asio::io_service& io_service, const std::string& host, const std::string& service) : connection_(new Connection(io_service))
  {
	 // connect(io_service, host, service); //this causes runtime error
  }

  void connect(boost::asio::io_service& io_service, const std::string& host, const std::string& service);

  /// Handle completion of a connect operation.
  void handle_connect(const boost::system::error_code& e, boost::asio::ip::tcp::resolver::iterator endpoint_iterator);

  void async_recieve(const boost::system::error_code& e, connection_ptr conn);
  
  /// Handle completion of a read operation.
  void handle_read(const boost::system::error_code& e,  connection_ptr conn);

  void async_send(const boost::system::error_code& e, connection_ptr conn);

  /// Handle completion of a write operation.
  void handle_write(const boost::system::error_code& e, connection_ptr conn);

  void load_dummy_msgs();

};

} // TCP
