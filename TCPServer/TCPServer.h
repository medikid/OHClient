#ifndef TCPSERVER_H
#define TCPSERVER_H

#include "IServer.h"

//#include "Messenger.h"

namespace TCP {
	namespace SERVER {

		class TCPServer : public IServer {
		private:				
			boost::asio::ip::tcp::acceptor acceptor_; /// The acceptor object used to accept incoming socket connections.
			boost::asio::error::addrinfo_errors err;
			typedef boost::array<char, 512> buf;

		public:
			unsigned short myAddress;
			std::vector<Msg> Msgs_;
			MESSENGER::messenger_ptr messenger_;
			CONFIG* config_;

			TCPServer(boost::asio::io_service& io_service, unsigned short port)
				: config_(new CONFIG("Settings/ServerConfig.ini")),
				acceptor_(io_service, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), port)),
				messenger_(new MESSENGER::Messenger(io_service)){

				// load config.
				loadConfig();

				// Start an accept operation for a new connection.
				connection_ptr conn_(new TCPCom(acceptor_.get_io_service()));
				acceptor_.async_accept(conn_->socket_, boost::bind(&SERVER::TCPServer::HandleAccept, this, boost::asio::placeholders::error, conn_));
			}

			

			

			virtual bool loadConfig();

	 void setupIOService(boost::asio::io_service& IOService);
	 bool start(boost::asio::io_service& IOService);
	 bool stop(boost::asio::io_service& IOService);

	/// Handle completion of a accept operation.
	void HandleAccept(const boost::system::error_code& e, TCP::connection_ptr conn);

	 MESSENGER::messenger_ptr getMessenger();

	 void SendMsg( connection_ptr conn);
	 void asyncSendMsg( connection_ptr conn);
	 void RcvMsg( connection_ptr conn);

	 //this should be called only once per new connection in the handleAccept
	 void asyncRcvMsg( connection_ptr conn);

};

		}//namespace server
	} //namespace TCP

#endif //TCPSERVER_H