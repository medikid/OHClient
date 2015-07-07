// main.cc
#include "Client.h"
#include <QtWidgets/qapplication.h>

int main(int argc, char** argv)
{
  QApplication app(argc, argv);

  Client client = new Client();
  client.start("127.0.0.1", 1234);

  return app.exec();
}
