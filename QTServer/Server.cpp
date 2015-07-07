
// server.cc
#include "Server.h"
#include <iostream>
#include <qstring>
#include <vector>

using namespace std;

Server::Server(QObject* parent): QObject(parent)
{
  connect(&server, SIGNAL(newConnection()),
    this, SLOT(acceptConnection()));

  server.listen(QHostAddress::Any, 1234);
}

Server::~Server()
{
  server.close();
}

void Server::acceptConnection()
{
  client = server.nextPendingConnection();

  connect(client, SIGNAL(readyRead()),
    this, SLOT(startRead()));
  qDebug() << "A client connected \n"<< endl;
}

void Server::startRead()
{
  char buffer[1024] = {0};
  client->read(buffer, client->bytesAvailable());
  qDebug() << buffer << endl;
  //cout << buffer << endl;
  client->close();
}