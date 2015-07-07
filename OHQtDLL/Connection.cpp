#include "Connection.h"

Connection::Connection(QObject *parent) : QObject(parent){
	QThreadPool::globalInstance()->setMaxThreadCount(15);
}

void Connection::SetSocket(int sockID){
	socket = new QTcpSocket(this);

	connect(socket, SIGNAL(connected()), this, SLOT(connected()));
	connect(socket, SIGNAL(disconnected()), this, SLOT(disconnected()));
	connect(socket, SIGNAL(readyRead()), this, SLOT(readyRead()));
	connect(socket, SIGNAL(bytesWritten(qint64)), this, SLOT(bytesSent(qint64)));

	socket->setSocketDescriptor(sockID);
}


	void Connection::connected(){
		_cprintf( "Client connected event \n");

}

	void Connection::disconnected(){
		_cprintf(  "Client disconnected \n");

}

	
	void Connection::bytesSent(qint64 bytesWritten){
		_cprintf( "Sent data size of %d bytes \n" , bytesWritten );
	}

	void Connection::readyRead(){
		qDebug() << socket->readAll();

		
		Task *myTask = new Task();
		myTask->setAutoDelete(true);
		connect(myTask, SIGNAL(Result(int)), this , SLOT(TaskResult(int)), Qt::QueuedConnection );
		QThreadPool::globalInstance()->start(myTask);
		
}

	void Connection::TaskResult(int Number){
		QByteArray buffer;
		buffer.append("\r\nTask Result from client: ");
		buffer.append(QString::number(Number));
		//socket->write(buffer);
}

	//
 void Connection::SendMsg(Msg& msg){   	 
	QByteArray serializedData;
	 QDataStream dataStream(&serializedData, QIODevice::ReadWrite);
	dataStream.setVersion(QDataStream::Qt_5_4);
	/*
	//  unsigned char RE8K_ICDEF_SETTINGS_INOUT = (unsigned char)serializedData.size();
            re8k_ics_settings_inout structTemp;
            structTemp.header = 'A';

            structTemp.nominalVoltages.A = 400;
            structTemp.nominalVoltages.B = 9999;
            structTemp.nominalVoltages.C = 1;

            structTemp.FREQUENCY = re8k_ice_nominal_frequency::Frequency50Hz;    //0

            dataStream << structTemp.header
                       //<< quint32(structTemp.nominalVoltages.A )<< quint32(structTemp.nominalVoltages.B) << quint32(structTemp.nominalVoltages.C)
					   << structTemp.nominalVoltages.A << structTemp.nominalVoltages.B << structTemp.nominalVoltages.C
                       << structTemp.FREQUENCY;

            emit signalSendToSystem(serializedData);
			socket->write(serializedData);

            qDebug() << "Sent:" << serializedData.toHex() << sizeof(re8k_ics_settings_inout) << structTemp.header
                        << structTemp.nominalVoltages.A << structTemp.nominalVoltages.B << structTemp.nominalVoltages.C;
		*/
	
		dataStream << msg.TITLE << msg.VERSION << msg.BET << msg.BALANCE << msg.CHAIRS;
	_cprintf( "Serialized Data %x, Size: %d bytes \n", serializedData.toHex(), sizeof(Msg));
							
	emit signalSendToSystem(serializedData);
			socket->write(serializedData);
	_cprintf( "Send Dummy msg ");
	_cprintf( "\n Title: %s" , msg.TITLE );
	_cprintf( 	"\n Version: %.2f " , msg.VERSION );
	_cprintf( 	"\n Bet: %.2f" ,  msg.BET);
	_cprintf( 	"\n Balance: %.2f " , msg.BALANCE );
	_cprintf( 	"\n Chairs: %d" , msg.CHAIRS);
}

 void Connection::readySend(QByteArray& serializedData){
	 qDebug() << "triggered slot readySend";
 }
void Connection::RcvMsg(Msg& msg){
	/*
            //
			QByteArray serializedData = socket->readAll();
            const QByteArray tempBA = serializedData.mid(0, sizeof(re8k_ics_settings_inout));
            const void* tempPointer = tempBA.constData();
            const re8k_ics_settings_inout* const newStruct = static_cast< const re8k_ics_settings_inout* >(tempPointer);

            qDebug() << "Received:" << tempBA.toHex() << tempBA.size() << (tempBA.toHex() == serializedData.toHex()) << newStruct->nominalVoltages.A << newStruct->nominalVoltages.B << newStruct->nominalVoltages.C;
			
            //
            re8k_ics_settings_inout newStruct2;

            QDataStream deserialize(tempBA);
            deserialize.setVersion(QDataStream::Qt_5_4);

            deserialize >> newStruct2.header
                        >> newStruct2.nominalVoltages.A
                        >> newStruct2.nominalVoltages.B
                        >> newStruct2.nominalVoltages.C;

            qDebug() << "Received 2:" << (quint32) newStruct2.nominalVoltages.A << (quint32) newStruct2.nominalVoltages.B << (quint32) newStruct2.nominalVoltages.C;
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
				<< (QString) newStruct2.TITLE 
				<< (quint32) newStruct2.VERSION 
				<< (quint32) newStruct2.BET
				<< (quint32) newStruct2.BALANCE 
				<< (quint32) newStruct2.CHAIRS;
			


}
