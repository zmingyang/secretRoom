#pragma once
#include "qthread.h"
#include "qimage.h"

typedef struct
{
    QString msg;

}signalMsg;

class signalThread : public QThread
{
	Q_OBJECT

public:
	//explicit VideoThread(QObject *parent = 0);
	void run();

private:

signals:
	// �Զ����ź�
    void Send2UI(signalMsg msg);
}; 
