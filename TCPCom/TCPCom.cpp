#include "TCPCom.h"

using namespace TCP;

TCPCOM_API void TCPCom::connect(boost::asio::ip::tcp::resolver::iterator endpoint_iter)
{
	std::cout << "Calling TCPCom->Connect()" << std::endl;
	if (endpoint_iter != boost::asio::ip::tcp::resolver::iterator())
	{
		std::cout << "Trying " << endpoint_iter->endpoint() << "...\n";

		// Set a deadline for the connect operation.
		deadline_.expires_from_now(boost::posix_time::seconds(60));

		// Start the asynchronous connect operation.
		//socket_.async_connect(endpoint_iter->endpoint(),boost::bind(&client::handle_connect, this, _1, endpoint_iter));
		socket_.async_connect(endpoint_iter->endpoint(), boost::bind(&TCPCom::HandleConnect, this, boost::asio::placeholders::error, ++endpoint_iter));			
	}
	else
	{
		// There are no more endpoints to try. Shut down the client.
	}
}


TCPCOM_API void TCPCom::HandleConnect(const boost::system::error_code& e,  boost::asio::ip::tcp::resolver::iterator endpoint_iterator)
{
	std::cout << "Calling TCPCom->HandleConnect()" << std::endl;
	if (!e)
	{
		// Successfully established connection. Start operation to read the list
		// of stocks. The connection::async_read() function will automatically
		// decode the data that is read from the underlying socket.
		//connection_.async_read(stocks_, boost::bind(&GmTCPClient::handle_read, this, boost::asio::placeholders::error));
		is_connected_ = true;
	}
	else if (endpoint_iterator != boost::asio::ip::tcp::resolver::iterator())
	{
		// Try the next endpoint.
		disconnect();
		boost::asio::ip::tcp::endpoint endpoint = *endpoint_iterator;

		while( is_connected_ == false){
			socket_.async_connect(endpoint, boost::bind(&TCPCom::HandleConnect, this, boost::asio::placeholders::error, ++endpoint_iterator));
		}
	}
	else
	{
		// An error occurred. Log it and return. Since we are not starting a new
		// operation the io_service will run out of work to do and the client will
		// exit.
		std::cerr << e.message() << std::endl;
		is_connected_ = false;
	}
}

TCPCOM_API void TCPCom::disconnect(){
}


TCPCOM_API boost::asio::ip::tcp::socket& TCPCom::GetSocket(){
	return socket_;
}

TCPCOM_API void TCPCom::ResolveAddress(boost::asio::io_service& io_service, const std::string& host_ip, const std::string& host_port){
	// Resolve the host name into an IP address.
	boost::asio::ip::tcp::resolver resolver(io_service);
	boost::asio::ip::tcp::resolver::query query(host_ip, host_port);
	endpoint_iterator_ = resolver.resolve(query);
	endpoint_ = *endpoint_iterator_;
}

TCPCOM_API void TCPCom::CheckDeadline(){
			  std::cout << "Calling TCPCom->CheckDeadline()" << std::endl;
			if (!is_connected_)
			  return;

			// Check whether the deadline has passed. We compare the deadline against
			// the current time since a new asynchronous operation may have moved the
			// deadline before this actor had a chance to run.
			if ( deadline_.expires_at() <= boost::asio::deadline_timer::traits_type::now())
			{
			  // The deadline has passed. The socket is closed so that any outstanding
			  // asynchronous operations are cancelled.
			  socket_.close();

			  // There is no longer an active deadline. The expiry is set to positive
			  // infinity so that the actor takes no action until a new deadline is set.
			   deadline_.expires_at(boost::posix_time::pos_infin);
			}

			// Put the actor back to sleep.
			 deadline_.async_wait(boost::bind(&TCPCom::CheckDeadline, this));
}

//get port
	TCPCOM_API unsigned short TCPCom::GetPort(){ 
			return socket_.remote_endpoint().port();
		}

		//get port
	TCPCOM_API	std::string TCPCom::GetAddress(){
			return socket_.remote_endpoint().address().to_string() ;
		}


				