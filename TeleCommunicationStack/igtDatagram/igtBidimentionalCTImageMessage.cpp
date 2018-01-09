#include "igtBidimentionalCTImageMessage.h"

igtBidimentionalCTImageMessage::igtBidimentionalCTImageMessage()
{

}

void igtBidimentionalCTImageMessage::setWidth(int width){
    this->width = width;

}

int igtBidimentionalCTImageMessage::getWidth(){
    int ret = this->width;
    return ret;
}

void igtBidimentionalCTImageMessage::setHeight(int height){
    this->height = height;

}

int igtBidimentionalCTImageMessage::getHeight(){
    int ret = this->height;
    return ret;
}

void igtBidimentionalCTImageMessage::setDepth(int depth){
    this->depth = depth;

}

int igtBidimentionalCTImageMessage::getDepth(){
    int ret = this->depth;
    return ret;
}

void igtBidimentionalCTImageMessage::setXSpacing(int xSpacing){
    this->xSpacing = xSpacing;

}

int igtBidimentionalCTImageMessage::getXSpacing(){
    int ret = this->xSpacing;
    return ret;
}

void igtBidimentionalCTImageMessage::setYSpacing(int ySpacing){
    this->ySpacing = ySpacing;

}

int igtBidimentionalCTImageMessage::getYSpacing(){
    int ret = ySpacing;
    return ret;
}

void igtBidimentionalCTImageMessage::setZSpacing(int zSpacing){
    this->zSpacing = zSpacing;

}

int igtBidimentionalCTImageMessage::getZSpacing(){
    int ret = zSpacing;
    return ret;
}

void igtBidimentionalCTImageMessage::setDatatype(int datatype){
    this->datatype = datatype;

}

int igtBidimentionalCTImageMessage::getDatatype(){
    int ret = this->datatype;
    return ret;
}

void igtBidimentionalCTImageMessage::setMSB(int MSB){
    this->MSB = MSB;

}

int igtBidimentionalCTImageMessage::getMSB(){
    int ret = this->MSB;
    return ret;
}

void igtBidimentionalCTImageMessage::setCTImageMessageBody(QString CTImagePath){
    QFile *CTImageMessage = new QFile(CTImagePath);
    CTImageMessageBody = CTImageMessage->readAll();

}

QByteArray igtBidimentionalCTImageMessage::getCTImageMessageBody(){
    QByteArray ret = CTImageMessageBody;
    return ret;
}

QList<QByteArray> igtBidimentionalCTImageMessage::encode(){
    QList<QByteArray> encodeByteArray;
    int temp = CTImageMessageBody.size()/1003;
    for(int i=0; i<temp; i++){
        encodeByteArray[i][0] = (uchar)(0x000000ff&this->getDataType());
        encodeByteArray[i][1] = (uchar)((0x0000ff00&this->getDataType())>>8);

        encodeByteArray[i][2] = (uchar)((0x0000000ff&this->getOrigineId()));

        encodeByteArray[i][3] = (uchar)((0x000000ff&this->getTargetId()));

        encodeByteArray[i][4] = (uchar)((0x000000ff&this->getTimeStampes()));
        encodeByteArray[i][5] = (uchar)((0x0000ff00&this->getTimeStampes())>>8);
        encodeByteArray[i][6] = (uchar)((0x00ff0000&this->getTimeStampes())>>16);
        encodeByteArray[i][7] = (uchar)((0xff000000&this->getTimeStampes())>>24);

        encodeByteArray[i][8] = (uchar)((0x000000ff&1003));
        encodeByteArray[i][9] = (uchar)((0x0000ff00&1003)>>8);

        encodeByteArray[i][10] = (uchar)(0x000000ff&this->getWidth());
        encodeByteArray[i][11] = (uchar)((0x0000ff00&this->getWidth())>>8);

        encodeByteArray[i][12] = (uchar)(0x000000ff&this->getHeight());
        encodeByteArray[i][13] = (uchar)((0x0000ff00&this->getHeight())>>8);

        encodeByteArray[i][14] = (uchar)(0x000000ff&this->getDepth());
        encodeByteArray[i][15] = (uchar)((0x0000ff00&this->getDepth()));

        encodeByteArray[i][16] = (uchar)(0x000000ff&this->getXSpacing());

        encodeByteArray[i][17] = (uchar)(0x000000ff&this->getYSpacing());

        encodeByteArray[i][18] = (uchar)(0x000000ff&this->getZSpacing());

        encodeByteArray[i][19] = (uchar)(0x000000ff&this->getDatatype());

        encodeByteArray[i][20] = (uchar)(0x000000ff&this->getMSB());
        for(int y=0; y<1003; y++)
        {
            encodeByteArray[i].append(1003*i+y);
        }
    }




    return encodeByteArray;
}
