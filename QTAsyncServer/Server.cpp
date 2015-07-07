#include "Server.h"

Server::Server (QObject *parent): QTcpServer(parent){

}

void Server::StartServer(){
		 if(listen(QHostAddress::Any, 1234)){
			 qDebug() << "Server Started \n";
		 } else {
			 qDebug() << "Server Not Started \n";

		}
}

void Server::incomingConnection(int handle){
	 qDebug() << "A Client Connected \n";
	Connection *connection = new Connection(this);
	connection->SetSocket(handle);
}