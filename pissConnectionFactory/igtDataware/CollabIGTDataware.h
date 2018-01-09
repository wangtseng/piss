#ifndef COLLABIGTDATAWARE_H
#define COLLABIGTDATAWARE_H

#include <QVector>
#include "CollabIGTCTImage.h"


class CollabIGTDataware
{

public:

    void appendAnCollabIGTCTImage(CollabIGTCTImage* img);
    CollabIGTCTImage* fetchLatestImageToSend();
    void deleteLatest();
    int getCollabIGTDatawarelength();

private:
    QVector<CollabIGTCTImage*> imageSequenceToSend;

public:
    CollabIGTDataware();
};

#endif // COLLABIGTDATAWARE_H
