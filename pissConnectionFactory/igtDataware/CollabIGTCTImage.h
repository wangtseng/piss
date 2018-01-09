#ifndef COLLABIGTCTIMAGE_H
#define COLLABIGTCTIMAGE_H

#include <QByteArray>
#include <QString>
#include <QFile>
#include <QDebug>
#include <cstring>

class CollabIGTCTImage
{
    
public:
    
    void setWidth(int width);       //! 2 Bytes
    int getWidth();

    void setHeight(int height);     //! 2 Bytes
    int getHeight();

    void setDepth(int depth);       //! 2 Bytes
    int getDepth();

    void setXSpacing(int xSpacing);     //! 1 Byte
    int getXSpacing();

    void setYSpacing(int ySpacing);     //! 1 Byte
    int getYSpacing();

    void setZSpacing(int zSpacing);     //! 1 Byte
    int getZSpacing();

    void setDatatype(int datatype);     //! 1 Byte
    int getDatatype();

    void setMSB(int MSB);               //! 1 Byte
    int getMSB();

    void doReadCTImageByPath(QString CTImagePath);
    const char* getCTImage();
    
    int getCTImageLength();
    
private:
    int width;
    int height;
    int depth;
    int xSpacing;
    int ySpacing;
    int zSpacing;
    int datatype;
    int MSB;

    char *ctImageMsgBody;
    qint64 ctImageSize;
public:
    CollabIGTCTImage();
};

#endif // COLLABIGTCTIMAGE_H
