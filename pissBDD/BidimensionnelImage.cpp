#include "BidimensionnelImage.h"


BidimensionnelImage::BidimensionnelImage()
{

}

void BidimensionnelImage::setTimeStamp(qint16 timestamp){
    this->timeStamp = timestamp;
}

qint16 BidimensionnelImage::getTimeStamp(){
    return this->timeStamp;
}

void BidimensionnelImage::setImage(vtkImageData *img){
    this->img = img;
}

vtkImageData* BidimensionnelImage::getImage(){
    return this->img;
}
