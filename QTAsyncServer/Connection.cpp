#include "Connection.h"

Connection::Connection(QObject *parent) : QObject(parent){
	QThreadPool::globalInstance()->setMaxThreadCount(15);
}

void Connection::SetSocket(int sockID){
	socket = new QTcpSocket(this);

	connect(socket, SIGNAL(connected()), this, SLOT(connected()));
	connect(socket, SIGNAL(disconnected()), this, SLOT(disconnected()));
	connect(socket, SIGNAL(readyRead()), this, SLOT(readyRead()));

	socket->setSocketDescriptor(sockID);
	
}


void Connection::connected(){
		qDebug() << "Client connected event \n";

}

	void Connection::disconnected(){
		qDebug() << "Client disconnected event \n";

}

	void Connection::readyRead(){
		qDebug() << "Received incoming data of "<< socket->bytesAvailable() << " bytes \n";	
		RcvMsg();
		//qDebug() << socket->readAll();
		//()
		
		Task *myTask = new Task();
		myTask->setAutoDelete(true);
		connect(myTask, SIGNAL(Result(int)), this , SLOT(TaskResult(int)), Qt::QueuedConnection );
		//connect(myTask, SIGNAL(Result(int)), this , SLOT(RcvMsg()), Qt::QueuedConnection );
		QThreadPool::globalInstance()->start(myTask);	
}

	void Connection::TaskResult(int Number){
		/*
		QByteArray buffer;
		buffer.append("\r\nTask Result from Server: ");
		buffer.append(QString::number(Number));
		socket->write(buffer);
		*/
		RcvMsg();
}

	//
            
 void Connection::SendMsg(){   	 
	QByteArray serializedData;
	 QDataStream dataStream(&serializedData, QIODevice::ReadWrite);
	dataStream.setVersion(QDataStream::Qt_5_4);

	 unsigned char RE8K_ICDEF_SETTINGS_INOUT = (unsigned char)serializedData.size();
            re8k_ics_settings_inout structTemp;
            structTemp.header = RE8K_ICDEF_SETTINGS_INOUT;

            structTemp.nominalVoltages.A = 400;
            structTemp.nominalVoltages.B = 9999;
            structTemp.nominalVoltages.C = 1;

            structTemp.FREQUENCY = re8k_ice_nominal_frequency::Frequency50Hz;    //0

            dataStream << structTemp.header
                       << quint32(structTemp.nominalVoltages.A) << quint32(structTemp.nominalVoltages.B) << quint32(structTemp.nominalVoltages.C)
                       << structTemp.FREQUENCY;

            emit signalSendToSystem(serializedData);

            qDebug() << "Sent:" << serializedData.toHex() << sizeof(re8k_ics_settings_inout)
                        << structTemp.nominalVoltages.A << structTemp.nominalVoltages.B << structTemp.nominalVoltages.C;
		}

 void signalSendToSystem(QByteArray& serializedData){
	 qDebug() << "Emitted send to system signal";
 }
void Connection::RcvMsg(){
	/*
    		QByteArray serializedData = socket->readAll();
			QDataStream incomingData(&serializedData, QIODevice::ReadWrite);
			incomingData.setVersion(QDataStream::Qt_5_4);
			//incomingData << socket->readAll();// socket->readData();
            const QByteArray tempBA = serializedData.mid(0, sizeof(re8k_ics_settings_inout));
            const void* tempPointer = tempBA.constData();
            const re8k_ics_settings_inout* const newStruct = static_cast< const re8k_ics_settings_inout* >(tempPointer);

			qDebug() << "Received 1:" << tempBA.toHex() << tempBA.size() << (tempBA.toHex() == serializedData.toHex()) << quint32(newStruct->nominalVoltages.A) << qint32(newStruct->nominalVoltages.B) << newStruct->nominalVoltages.C;
			
            //
            re8k_ics_settings_inout newStruct2;

            QDataStream deserialize(tempBA);
			
            deserialize.setVersion(QDataStream::Qt_5_4);
            deserialize >> newStruct2.header
                        >> newStruct2.nominalVoltages.A
                        >> newStruct2.nominalVoltages.B
                        >> newStruct2.nominalVoltages.C;

            qDebug() << "Received 2:" << newStruct2.nominalVoltages.A <<  newStruct2.nominalVoltages.B << newStruct2.nominalVoltages.C;
			*/
				QByteArray serializedData = socket->readAll();
            const QByteArray tempBA = serializedData.mid(0, sizeof(Msg));
            const void* tempPointer = tempBA.constData();
            const Msg* const newStruct = static_cast< const Msg* >(tempPointer);

            qDebug() << "Received:" << tempBA.toHex() << tempBA.size() << (tempBA.toHex() == serializedData.toHex()); //;<< newStruct->nominalVoltages.A << newStruct->nominalVoltages.B << newStruct->nominalVoltages.C;
			
            //
            Msg newStruct2;

            QDataStream deserialize(tempBA);
            deserialize.setVersion(QDataStream::Qt_5_4);

            deserialize >> newStruct2.TITLE
                        >> newStruct2.VERSION
                        >> newStruct2.BET
                        >> newStruct2.BALANCE
						>> newStruct2.CHAIRS;

            qDebug() << "Received 2:" 
				<< "\n Title: " << (QString) newStruct2.TITLE 
				<< "\n Version: " << newStruct2.VERSION 
				<< "\n Bet: %.2f" << newStruct2.BET
				<< "\n Balance: %.2f" << newStruct2.BALANCE 
				<< "\n Chairs: " << (quint32) newStruct2.CHAIRS;
				
	}
