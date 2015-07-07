
// main.cc
#include "Server.h"
#include <QtWidgets/QApplication.h>

int main(int argc, char** argv)
{
  QApplication app(argc, argv);
  Server server = new Server();
  return app.exec();
}