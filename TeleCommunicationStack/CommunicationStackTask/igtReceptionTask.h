#ifndef IGTRECEPTIONTSAK_H
#define IGTRECEPTIONTSAK_H
#include <QThread>
#include <QTcpSocket>
#include "igtInputQueue.h"


class igtReceptionTask:public QThread
{
    Q_OBJECT

public:
    void run();
    void stop();

private:
    long received_frame_count;
    QTcpSocket *socket;
    igtInputQueue *inputQueueSave;
    int socketDescriptor;

signals:

public slots:
    void read();
    void disc();

public:
    igtReceptionTask(int ID,igtInputQueue* inputQueue);
};

#endif // IGTRECEPTIONTSAK_H
