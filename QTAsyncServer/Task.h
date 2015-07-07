#ifndef TASK_H
#define TASK_H

#include <QDebug>
#include <QObject>
#include <QRunnable>

class Task : public QObject, public QRunnable
{ Q_OBJECT
public:
	explicit Task();
	void SetSocket(int sockID);

signals:
	void Result(int Number);

public slots:



private:

protected:
	void run();

};
#endif  TASK_H