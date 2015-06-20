#include "Client.h"

int main(int argc, char* argv[])
{
  try
  {
    
    boost::asio::io_service io_service;
    TCP::Client client(io_service, "localhost", "1234");
    io_service.run();

	std::cin.get();
  }
  catch (std::exception& e)
  {
    std::cerr << e.what() << std::endl;
  }

  return 0;
}