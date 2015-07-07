#include "Task.h"

Task::Task(){

}

void Task::run(){
	qDebug() << "Task start \n" ;
	int iNumber = 0;
	for (int i = 0; i < 100;		i++){
		iNumber += i;
	}
	qDebug() << "Task complete \n";	
	emit Result(iNumber);

}