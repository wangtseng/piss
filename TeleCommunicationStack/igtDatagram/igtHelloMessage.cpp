#include "igtHelloMessage.h"

igtHelloMessage::igtHelloMessage()
{

}

void igtHelloMessage::setCount(long count){
    this->count = count;
    return;
}

long igtHelloMessage::getCount(){
    long ret = this->count;
    return ret;
}

void igtHelloMessage::setConnectionState(int connectionState){
    this->connectionState = connectionState;
    return;
}

int igtHelloMessage::getConnectionState(){
    int ret = this->connectionState;
    return ret;
}

igtDatagram *igtHelloMessage::transformHelloMessageToIgtDatagram(){
    igtDatagram *datagram = new igtDatagram();
    QByteArray array;
    array.resize(1024);

    array[0] = (uchar)(0x000000ff&0);
    array[1] = (uchar)(0x000000ff&0);

    array[2] = (uchar)((0x0000000ff&1));

    array[3] = (uchar)((0x000000ff&1));

    array[4] = (uchar)(0x000000ff&15);
    array[5] = (uchar)(0x000000ff&00);
    array[6] = (uchar)(0x000000ff&00);
    array[7] = (uchar)(0xff000000&00);

    array[8] = (uchar)(0x000000ff&12);
    array[9] = (uchar)(0x000000ff&00);

    array[10] = (uchar)((0x000000ff&this->count));
    array[11] = (uchar)((0x0000ff00&this->count)>>8);
    array[12] = (uchar)((0x00ff0000&this->count)>>16);
    array[13] = (uchar)((0xff000000&this->count)>>24);

    array[14] = (uchar)((0x000000ff&this->connectionState));
    array[15] = (uchar)((0x0000ff00&this->connectionState)>>8);
    array[16] = (uchar)((0x00ff0000&this->connectionState)>>16);
    array[17] = (uchar)((0xff000000&this->connectionState)>>24);

    datagram->decode(array);
    return datagram;
}
