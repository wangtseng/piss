#include "IgssImage.h"

//!------------------------------------------------------------------------
//!
//! \brief MraImage::MraImage
//!
IgssImage::IgssImage()
{

}

//!------------------------------------------------------------------------
//!
//! \brief MraImage::setNumberOfDimension
//! \param NDims
//!
void IgssImage::setNumberOfDimension(int NDims){
    this->NDims = NDims;
}

//!------------------------------------------------------------------------
//!
//! \brief MraImage::setDimensionSize
//! \param x
//! \param y
//! \param z
//!
void IgssImage::setDimensionSize(int x, int y, int z){
    this->DimSize[0] = x;
    this->DimSize[1] = y;
    this->DimSize[2] = z;

}

//!------------------------------------------------------------------------
//!
//! \brief MraImage::setElementSpacing
//! \param x_spacing
//! \param y_spacing
//! \param z_spacing
//!
void IgssImage::setElementSpacing(float x_spacing,
                                 float y_spacing,
                                 float z_spacing){
    this->ElementSpacing[0] = x_spacing;
    this->ElementSpacing[1] = y_spacing;
    this->ElementSpacing[2] = z_spacing;
}

//!------------------------------------------------------------------------
//!
//! \brief MraImage::setElementByteOrderMSB
//! \param order
//!
void IgssImage::setElementByteOrderMSB(int order){
    this->ElementByteOrderMSB = order;
}

//!------------------------------------------------------------------------
//!
//! \brief MraImage::setElementType
//! \param type
//!
void IgssImage::setElementType(QString type){
    this->ElementType = type;
}

//!------------------------------------------------------------------------
//!
//! \brief MraImage::setRawFileName
//! \param rawFileName
//!
void IgssImage::setRawFileName(QString rawFileName){
    this->ElementDataFile = rawFileName;
}

//!------------------------------------------------------------------------
//!
//! \brief IgssImage::setPosition
//! \param x_position
//! \param y_position
//! \param z_position
//!
void IgssImage::setPosition(float x_position, float y_position, float z_position){
    this->Position[0] = x_position;
    this->Position[1] = y_position;
    this->Position[2] = z_position;
}

//!------------------------------------------------------------------------
//!
//! \brief MraImage::getDimensionSize
//! \param dims
//!
void IgssImage::getDimensionSize(int *dims){
    dims[0] = this->DimSize[0];
    dims[1] = this->DimSize[1];
    dims[2] = this->DimSize[2];
}

//!------------------------------------------------------------------------
//!
//! \brief MraImage::getElementSpacing
//! \param spaces
//!
void IgssImage::getElementSpacing(double *spaces){
    spaces[0] = this->ElementSpacing[0];
    spaces[1] = this->ElementSpacing[1];
    spaces[2] = this->ElementSpacing[2];
}

//!------------------------------------------------------------------------
//!
//! \brief MraImage::getNumberOfDimension
//! \return
//!
int IgssImage::getNumberOfDimension(){
    return this->NDims;
}

//!------------------------------------------------------------------------
//!
//! \brief MraImage::getElementByteOrderMSB
//! \return
//!
int IgssImage::getElementByteOrderMSB(){
    return this->ElementByteOrderMSB;
}

//!------------------------------------------------------------------------
//!
//! \brief IgssImage::getPosition
//! \param position
//!
void IgssImage::getPosition(float *position){
    position[0] = this->Position[0];
    position[1] = this->Position[1];
    position[2] = this->Position[2];
}

//!------------------------------------------------------------------------
//!
//! \brief MraImage::getRawFileName
//! \return
//!
QString IgssImage::getRawFileName(){
    return this->ElementDataFile;
}

//!------------------------------------------------------------------------
//!
//! \brief MraImage::setLengthOfDataSet
//!
void IgssImage::setLengthOfDataSet(){
    length = 1;

    for(int i = 0; i < this->NDims; i++){
        length *= DimSize[i];
    }

    values = (unsigned short*)malloc((length)*sizeof(unsigned short));

}

//!------------------------------------------------------------------------
//!
//! \brief MraImage::setValueByIndex
//! \param index
//! \param value
//!
void IgssImage::setValueByIndex(long index, unsigned short value){
    values[index] = value;
}

//!------------------------------------------------------------------------------
//! \brief IgssImage::getValueByIndex
//! \param i
//! \return
//!
unsigned short IgssImage::getValueByIndex(long i)
{
    return this->values[i];
}

//!
//! \brief IgssImage::getLengthOfDataset
//! \return
//!
long IgssImage::getLengthOfDataset()
{
    return this->length;
}

//!
//! \brief IgssImage::getIgssScalarType
//! \return
//!
int IgssImage::getIgssScalarType(){
    return IGSS_UNSIGNED_SHORT;
}
