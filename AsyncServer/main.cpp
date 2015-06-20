#include "Server.h"

int main(int argc, char* argv[])
{
  try
  {
    
    unsigned short port = boost::lexical_cast<unsigned short>("1234");

    boost::asio::io_service io_service;
    TCP::Server server(io_service, port);
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