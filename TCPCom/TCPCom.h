#include <boost\asio.hpp>
#include <boost\bind.hpp>
#include <boost\lexical_cast.hpp>
#include <boost\tuple\tuple.hpp>
#include <stdio.h>
#include "DataCom.h"

#ifdef TCPCOM_EXPORTS
#define TCPCOM_API __declspec(dllexport)
#else
#define TCPCOM_API __declspec(dllimport)
#endif

namespace TCP {

	class TCPCOM_API TCPCom{

	private:
		

	public:
#pragma warning(disable:4251) //we need this because class implementing this dll cannot use thse members
		DataCom dataCom_;
		bool is_connected_;
		//connection_ptr new_conn_;
		boost::asio::deadline_timer deadline_;
		boost::asio::deadline_timer heartbeat_timer_;
		boost::asio::ip::tcp::socket socket_;
		boost::asio::ip::tcp::resolver::iterator endpoint_iterator_;
		boost::asio::ip::tcp::endpoint endpoint_;
		boost::asio::streambuf input_buffer_;

		/// The size of a fixed length header.
		enum { FIXED_HEADER_LENGTH = 16 };

		/// Holds an outbound header.
		std::string outbound_header_;

		/// Holds the outbound data.
		std::string outbound_data_;

		/// Holds an inbound header.
		char inbound_header_[FIXED_HEADER_LENGTH];

		/// Holds the inbound data.
		std::vector<char> inbound_data_;
#pragma warning(default:4251)

		TCPCom(boost::asio::io_service& io_service) :
			socket_(io_service), deadline_(io_service), heartbeat_timer_(io_service), dataCom_(io_service){	}

		void connect(boost::asio::ip::tcp::resolver::iterator endpoint_iter);

		/// Handle completion of a connect operation.
		void HandleConnect(const boost::system::error_code& e,  boost::asio::ip::tcp::resolver::iterator endpoint_iterator);

		void disconnect();
		/**************** DO NOT USE GET SOCKET, USE SOCKET_ ********/
		// Get the underlying socket. Used for making a connection or for accepting an incoming connection.
		boost::asio::ip::tcp::socket& GetSocket();

		void ResolveAddress(boost::asio::io_service& io_service, const std::string& host_ip, const std::string& host_port);

		//check deadline timer
		void CheckDeadline();

		//get port
		unsigned short GetPort();

		//get port
		std::string GetAddress();

		// Handle a completed read of message data.
		template <typename T, typename Handler>
		void asyncSendMsg(T& t, Handler handler){
			dataCom_.asyncSender(t,handler,this);
			/*
			dataCom_.serializeData(t, handler, this);
			dataCom_.prepareHeader(t,handler, this);
			dataCom_.sendData(t, handler, this);
			*/

			std::cout <<"TCPCom->asyncSendMsg sent message"<< std::endl;

		}

		// Handle a completed read of message data.
		template <typename T, typename Handler>
		void asyncRcvMsg(T& t, Handler handler){
			dataCom_.asyncReceiver(t,handler,this);
			/*
			//call readheader to trigger this sequence: readHeader->readHeaderHandler->readData->readDataHandler
			dataCom_.readHeader(t, handler, this);
			//verifyHeader(t,handler, u);
			//dataCom_.readData(t, boost::make_tuple(handler), this);
			dataCom_.deserializeData(t, handler, this);
			*/
		}


	};
	typedef boost::shared_ptr<TCPCom> connection_ptr;
}//namespace TCP
