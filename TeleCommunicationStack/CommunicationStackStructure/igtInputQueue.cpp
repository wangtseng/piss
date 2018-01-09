#include "igtInputQueue.h"

igtInputQueue::igtInputQueue()
{

}

int igtInputQueue::getLength(){

    int ret = 0;
    mutex.lock();
    ret = inputQueue.size();
    mutex.unlock();
    return ret;
}

void igtInputQueue::append(igtDatagram *byteArray){
    mutex.lock();
    inputQueue.append(byteArray);
    mutex.unlock();
}

igtDatagram *igtInputQueue::getLatestArray(){
    mutex.lock();
    igtDatagram *ret = inputQueue.at(0);
    mutex.unlock();
    return ret;
}

void igtInputQueue::deleteLatestArray(){
    mutex.lock();
    inputQueue.pop_front();
    mutex.unlock();
}

void igtInputQueue::print(){
//    for(int cpt = 0; cpt < inputQueue.size(); cpt++){
//        int array_size = inputQueue.at(cpt).size();

//        qDebug()<<"size:"<<array_size<<", value:"
//                <<(unsigned char)inputQueue.at(cpt).at(0)
//                <<(unsigned char)inputQueue.at(cpt).at(1)
//                <<(unsigned char)inputQueue.at(cpt).at(2)
//                <<(unsigned char)inputQueue.at(cpt).at(3)
//                <<(unsigned char)inputQueue.at(cpt).at(4)
//                <<(unsigned char)inputQueue.at(cpt).at(5)
//                <<(unsigned char)inputQueue.at(cpt).at(6)
//                <<(unsigned char)inputQueue.at(cpt).at(7)
//                <<(unsigned char)inputQueue.at(cpt).at(8)
//                <<(unsigned char)inputQueue.at(cpt).at(9);

//    }
}
