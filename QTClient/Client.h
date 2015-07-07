#ifndef CLIENT_H
#define CLIENT_H
// client.h
#include <QtNetwork>
#include <QObject>
#include <QString>
#include <QTcpSocket>
#include "Connection.h"

class Client: public QObject
{
Q_OBJECT
public:
  Client(QObject* parent = 0);
  ~Client();
  void start(QString address, quint16 port);  
	void ConnectToServer();
	void DisconnectFromServer();
	bool isConnected();
	Msg getDummyMsg();
	void sendMsg(Msg msg);
public slots:
  void startTransfer();
private:
	Connection* conn_;
	QHostAddress servAddress_;
	quint16 servPort_;
	bool connected_;
};

#endif // CLIENT_H