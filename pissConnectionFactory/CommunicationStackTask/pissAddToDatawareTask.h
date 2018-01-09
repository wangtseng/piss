#ifndef ADDTODATAWARETASK_H
#define ADDTODATAWARETASK_H

#include <QThread>
#include "CollabIGTDataware.h"
#include "CollabIGTCTImage.h"
#include <QString>
#include <QDir>

class pissAddToDatawareTask : public QThread
{
    Q_OBJECT
    
private:
    CollabIGTDataware *igtDataware;
    QString filePath;
    
public:
    void run();
public:
    pissAddToDatawareTask(CollabIGTDataware *igtDataware);
};

#endif // ADDTODATAWARETASK_H
