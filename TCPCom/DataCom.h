#ifndef DATACOM_H
#define DATACOM_H


#include <stdio.h>
#include <boost\asio.hpp>
#include <boost\tuple\tuple.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>

class  DataCom { //: public boost::enable_shared_from_this<GConnection> {

private:
public:
	#pragma warning(disable:4251)
	  /// The size of a fixed length header.
	enum { FIXED_HEADER_SIZE = 8, FIXED_DATA_SIZE = 2000 };

	  /// Holds an outbound header.
	std::string outbound_header_;

	  /// Holds the outbound data.
	std::string outbound_data_;

	  /// Holds an inbound header.
	char inbound_header_[FIXED_HEADER_SIZE];

	  /// Holds the inbound data.
	std::vector<char> inbound_data_;

	//buffer to store data prior sending
	std::vector<boost::asio::const_buffer> buffers_;

	#pragma warning(disable:4251)

	DataCom(boost::asio::io_service& io_service){	/*setInboundDataPacketSize(0); */	inbound_data_.resize(FIXED_DATA_SIZE); }
	virtual ~DataCom(){}

	// boost::shared_ptr<GConnection> pointer(){ return shared_from_this(); }


//Serialize data and load it into outbound_data_. Prepare header, load into outbound_header
  template<typename T, typename Handler, typename U>
  void serializeData(const T& t, Handler handler, U& u){
	std::cout << "Calling DataCom->SerializeData. " <<  std::endl;

	std::ostringstream archive_stream;
    boost::archive::text_oarchive archive(archive_stream);
    archive << t;
	outbound_data_ = archive_stream.str();

	//std::cout << "Serialized Data("<<outbound_data_.size() <<"kb) ready to send: " << archive_stream.str() << std::endl;
  }

  //deserialize received data and load it into inbound_data_
  template<typename T, typename Handler, typename U>
  void deserializeData(T& t, Handler handler, U& u){
		std::cout << "Calling DataCom->DeSerializeData. " <<  std::endl;

		std::string archive_data(&inbound_data_[0], inbound_data_.size());
        std::istringstream archive_stream(archive_data);
        boost::archive::text_iarchive archive(archive_stream);
        archive >> t;

		std::cout << "DeSerialized Data ("<< inbound_data_.size() << "kb) ready to receive: " <<  archive_stream.str() << std::endl;
  }

  //prepare header for outbound data, size
  template<typename T, typename Handler, typename U>
  void prepareHeader(T& t, Handler handler, U& u){
	  std::cout << "Calling DataCom->prepareheader()" << std::endl;
	  // Format the header.
		std::ostringstream header_stream;
		header_stream << std::setw(FIXED_HEADER_SIZE) << std::hex << outbound_data_.size();

		if (!header_stream || header_stream.str().size() != FIXED_HEADER_SIZE)
		{
		  // Something went wrong, inform the caller.
		  boost::system::error_code error(boost::asio::error::invalid_argument);
		  u->socket_.get_io_service().post(boost::bind(handler, error));
		  return;
		}
		outbound_header_ = header_stream.str();

		std::cout << "Outbound_data is ready and has size" << outbound_data_.size() << std::endl;
		std::cout << "Serialized Header std hex" << header_stream.str() << std::endl;
	}



 //verifies the header, and resize inbound data size to receive data
	template <typename T, typename Handler, typename U>
  void verifyHeader(T& t, Handler handler, U& u){
	   std::cout << "Calling DataCom->verifyHeader()"  << std::endl;
	 // Determine the length of the serialized data.
      std::istringstream is(std::string(inbound_header_, FIXED_HEADER_SIZE));
      std::size_t inbound_data_size = 0;


	 if (!(is >> std::hex >> inbound_data_size)){
		 // Header doesn't seem to be valid. Inform the caller.
        boost::system::error_code error(boost::asio::error::invalid_argument);
        boost::get<0>(handler)(error);
        return ;
	 }
	  inbound_data_.resize(inbound_data_size);
	  std::cout << "Header received is verified and states size of kb" << inbound_data_size << std::endl;
  }

  template <typename T, typename Handler, typename U>
  void headerWriteHandler(T& t, Handler handler, U& u){
	  buffers_.push_back(boost::asio::buffer(outbound_header_));
  }

  template <typename T, typename Handler, typename U>
  void dataWriteHandler(T& t, Handler handler, U& u){
	buffers_.push_back(boost::asio::buffer(outbound_data_));
  }

  template <typename T, typename Handler, typename U>
  void sendData(T& t, Handler handler, U& u){
	buffers_.clear();//clear this before writing data
	headerWriteHandler(t, handler, u);
	dataWriteHandler(t,handler,u);
    boost::asio::async_write(u->socket_, buffers_, handler);
	std::cout << "Sent data packet of size " << outbound_data_.size() << "KB" << std::endl;
  }

  template <typename T, typename Handler, typename U>
  void Receive(T& t, Handler handler, U& u){
  }

 template <typename T, typename Handler, typename U>
 void asyncSender(const T& t, Handler handler, U& u)
  {
	  std::cout << "Calling DataCom->asyncSender()" << std::endl;
	  // Serialize the data(t) and store it in outboundData storage
	  serializeData(t, handler, u);

	  //prepare headerData_ from the outboundData storage
	  prepareHeader(t, handler, u);

		// Write the serialized data to the socket. We use "gather-write" to send
		// both the header and the data in a single write operation.
		sendData(t, handler, u);
 }

 template <typename T, typename Handler, typename U>
 void asyncReceiver(T& t, Handler handler, U& u){
	// std::cout << "Calling DataCom->asyncReceiver()" << std::endl;

	 // Start an asynchronous call to receive the header.
	  readHeader(t, handler, u);
 }

 template <typename T, typename Handler, typename U>
 void readHeader(T& t, Handler handler, U& u){

      void (DataCom::*f)(const boost::system::error_code&, T&, boost::tuple<Handler>, U&) = &DataCom::headerReaderHandler<T, Handler, U>;
     boost::asio::async_read(u->socket_, boost::asio::buffer(inbound_header_), boost::bind(f, this, boost::asio::placeholders::error, boost::ref(t), boost::make_tuple(handler), u));
 }

   template <typename T, typename Handler, typename U>
 void readData(T& t, boost::tuple<Handler> handler, U& u){
	  void (DataCom::*f)(const boost::system::error_code&, T&, boost::tuple<Handler>, U&) = &DataCom::dataReaderHandler<T, Handler, U>;
      boost::asio::async_read(u->socket_, boost::asio::buffer(inbound_data_), boost::bind(f, this, boost::asio::placeholders::error, boost::ref(t), handler, u));
 }

	/// Handle a completed read of a message header. The handler is passed using
  /// a tuple since boost::bind seems to have trouble binding a function object
  /// created using boost::bind as a parameter.
  template <typename T, typename Handler, typename U>
 void headerReaderHandler(const boost::system::error_code& e, T& t, boost::tuple<Handler> handler, U& u)
  {
	 // std::cout << "Calling Gconnection.cpp->headerReaderHandler()" << std::endl;
	  if (!e){
		  //no error, go ahead and verify header, resize incoming data buffer
			verifyHeader(t, handler, u);
			readData(t, handler, u);
	  }
	  else {
		  //there is some error reading. inform sender.
		boost::get<0>(handler)(e);
		}
  }


 // Handle a completed read of message data.
  template <typename T, typename Handler, typename U>
 void dataReaderHandler(const boost::system::error_code& e, T& t, boost::tuple<Handler> handler, U& u)
  {
	  std::cout << "Calling DataCom->dataReaderHandle()" << std::endl;
    if (e) {
      boost::get<0>(handler)(e);
    }
    else  {
      // Extract the data structure from the data just received.
		  try {
			deserializeData(t, handler, u); //deserialize received data and load it into inbound_data_
			std::cout << "Received data packet of size " << inbound_data_.size() << "KB" << std::endl;
		  }
		  catch ( std::exception&){
			 std::cout << "Error in decoding data" << inbound_data_.size() << "KB" << std::endl;
			// Unable to decode data.
			boost::system::error_code error(boost::asio::error::invalid_argument);
			boost::get<0>(handler)(error);
			return;
		  }

      // Inform caller that data has been received ok.
      boost::get<0>(handler)(e);
    }
}




};//class

#endif //DATACOM_H
