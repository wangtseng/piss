#include "CollabIGTDataware.h"

CollabIGTDataware::CollabIGTDataware()
{

}


//!----------------------------------------------------------------------------------
//!
//! \brief CollabIGTDataware::appendAnCollabIGTCTImage
//! \param img
//!
void CollabIGTDataware::appendAnCollabIGTCTImage(CollabIGTCTImage *img){
    if(img == NULL){
        return;
    }
    imageSequenceToSend.append(img);
}


CollabIGTCTImage* CollabIGTDataware::fetchLatestImageToSend(){
    if(imageSequenceToSend.size() > 0){
        CollabIGTCTImage* ret = imageSequenceToSend[0];
        return ret;
    }
    return NULL;
}


void CollabIGTDataware::deleteLatest(){
    if(imageSequenceToSend.size() > 0){
        imageSequenceToSend.remove(0);
    }
}


int CollabIGTDataware::getCollabIGTDatawarelength(){
    int ret;
    ret = this->imageSequenceToSend.size();
    return ret;
}
