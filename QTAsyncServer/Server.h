#ifndef SERVER_H
#define SERVER_H

#include <QTcpServer>
#include <QTcpSocket>
#include <QAbstractSocket>
#include "Connection.h"

class Server : public QTcpServer
{
	Q_OBJECT

public:
	explicit Server(QObject *parent = 0);
	void StartServer();

protected:
	void incomingConnection(int handle);

signals:

public slots:


private:

};










#endif  SERVER_H