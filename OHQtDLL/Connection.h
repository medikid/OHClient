#ifndef CONNECTION_H
#define CONNECTION_H


#include <QObject>
#include <QTcpSocket>
#include <QDebug>
#include <QThreadPool>
#include <qiodevice.h>
#include "Task.h"
#include "Msg.h"
#include <conio.h>
#include <iostream>

class Connection : public QObject
{
	Q_OBJECT

public:
	explicit Connection(QObject *parent = 0);
	void SetSocket(int sockID);
	QTcpSocket* getSocket(){return socket;}
   
	void SendMsg(Msg& msg);
	void RcvMsg(Msg& msg);

signals:
	void signalSendToSystem(QByteArray& serializedData);

public slots:
	void connected();
	void disconnected();
	void readyRead();
	void TaskResult(int Number);
	void bytesSent(qint64 bytesWritten);
	void readySend(QByteArray& serializedData);


private:
	QTcpSocket *socket;

};
#endif  CONNECTION_H