#ifndef CONNECTION_H
#define CONNECTION_H


#include <QObject>
#include <QTcpSocket>
#include <QDebug>
#include <QThreadPool>
#include <qiodevice.h>
#include "Task.h"
#include "Msg.h"

class Connection : public QObject
{
	Q_OBJECT

public:
	explicit Connection(QObject *parent = 0);
	void SetSocket(int sockID);
   
	
	void SendMsg();
	void RcvMsg();

	QTcpSocket* getSocket(){return socket;}

signals:
	void signalSendToSystem(QByteArray& serializedData);

public slots:
	void connected();
	void disconnected();
	void readyRead();
	void TaskResult(int Number);


private:
	QTcpSocket *socket;

};
#endif  CONNECTION_H