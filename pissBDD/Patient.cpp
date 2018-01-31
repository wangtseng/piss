#include "Patient.h"
#include <QDebug>
#include <QFileInfo>
#include <QDir>


//!----------------------------------------------------------------------------
//!
//! \brief Patient::ThreadImageProcessing::run
//!
void Patient::ThreadImageProcessing::run(){

    //! check if patient's mraImage exist in the personnel mra path....
    QFileInfo checkMHDFile(patient->myPath + "/mra_tridimensionel__image/" + patient->name + "_T1.mhd");
    QString mraImagefilePath;

    //! check if file exists and if yes: Is it really a file and no directory?
    if((checkMHDFile.exists() && checkMHDFile.isFile()) ){
        mraImagefilePath = patient->myPath + "/mra_tridimensionel__image/" + patient->name + "_T1.mhd";
        patient->loadMRAImageFile(mraImagefilePath);

        patient->statisticsList = patient->imageProcessingFactory->getHistogramStatisticsFrom(patient->MriImageForDisplay);
        patient->maximumGrayScaleValue = patient->statisticsList[0].toInt(0, 10);
        patient->minimumGrayScaleValue = patient->statisticsList[1].toInt(0, 10);

        patient->frequencies = patient->imageProcessingFactory->getHistogramFrom(patient->MriImageForDisplay);
    }
    else{

    }

    QString targetFolder = patient->myPath + "/mra_tridimensionel__image/centerlines/cerebrovascular/";

    QDir bDir(targetFolder+"B/");
    QDir fDir(targetFolder+"F/");
    QDir lDir(targetFolder+"L/");
    QDir rDir(targetFolder+"R/");

    foreach (QString fileName, bDir.entryList(QDir::Files)){
        patient->loadVesselByPath(fileName, targetFolder+"B/"+fileName);
    }

    foreach (QString fileName, fDir.entryList(QDir::Files)){
        patient->loadVesselByPath(fileName, targetFolder+"F/"+fileName);
    }

    foreach (QString fileName, lDir.entryList(QDir::Files)){
        patient->loadVesselByPath(fileName, targetFolder+"L/"+fileName);
    }

    foreach (QString fileName, rDir.entryList(QDir::Files)){
        patient->loadVesselByPath(fileName, targetFolder+"R/"+fileName);
    }

//    qDebug()<<patient->vesselCenterlines.keys();
//    qDebug()<<patient->vessels.keys();

    patient->MraImageReadComplete = true;
}

//!----------------------------------------------------------------------------
//!
//! \brief Patient::Patient
//! \param myPath
//!
Patient::Patient(QString myPath, int id){
    this->id = id;
    this->myPath = myPath;

    MraImageReadComplete = false;

    //! fetch name and birth of the patient
    QStringList temp = myPath.split("/");
    QStringList patientInfo = temp[temp.size()-1].split("__");
    name = patientInfo[0];

    QStringList name_temp = name.split("_");
    this->firstName = name_temp[1];
    this->lastName = name_temp[0];

    birthday = patientInfo[1];
    myPhotoPath = myPath + "/personnelInfoPath/" + name +".png";
    mraTridimensionelPath = myPath + "/mra_tridimensionel__image/" + name + ".mhd";
    centerLineFolderPath = myPath + "/mra_tridimensionel__image/centerlines/cerebrovascular/";

    maximumGrayScaleValue = 0;
    minimumGrayScaleValue = 0;

    originImage = new IgssImage();

    centerReader = new CenterLineReader();

    centerLinePoints = vtkPoints::New();

    this->cImageSequence.clear();
    this->controlTarget = "Guidewire";
    this->echantionnage = false;
}

//! --------------------------------------------------------------------------------------------------------------
//!
//! \brief Patient::configurerEchantionnageEtat
//! \param echantionnage
//!
void Patient::configurerEchantionnageEtat(bool echantionnage){
    this->echantionnage = echantionnage;
}

//! --------------------------------------------------------------------------------------------------------------
void Patient::setTargetObjectHandling(QString obj){
    this->controlTarget = obj;
}

//! --------------------------------------------------------------------------------------------------------------
//!
//! \brief Patient::getTargetObjectHandling
//! \return
//!
QString Patient::getTargetObjectHandling(){
    return this->controlTarget;
}

//! ---------------------------------------------------------------------------------------------------------------
//!
//! \brief Patient::doReadReconstructedResult
//!
bool Patient::doReadReconstructedResult(){
    //qDebug()<<myPath + "/bidimensionel__images/reconstruct/result.raw";
    QFileInfo checkMHDFile(myPath + "/bidimensionel__images/reconstruct/result.raw");
    QString mraImagefilePath;

    //! check if file exists and if yes: Is it really a file and no directory?
    if((checkMHDFile.exists() && checkMHDFile.isFile()) ){

        mraImagefilePath = myPath + "/bidimensionel__images/reconstruct/result.mhd";
        //qDebug()<<mraImagefilePath;
        loadMRAImageFile(mraImagefilePath);

        statisticsList =imageProcessingFactory->getHistogramStatisticsFrom(MriImageForDisplay);
        maximumGrayScaleValue = statisticsList[0].toInt(0, 10);
        minimumGrayScaleValue = statisticsList[1].toInt(0, 10);

        frequencies = imageProcessingFactory->getHistogramFrom(MriImageForDisplay);

        //QFile::remove(myPath + "/bidimensionel__images/reconstruct/result.raw");

        return true;
    }

    return false;
}

//! ----------------------------------------------------------------------------------------------------------------
//!
//! \brief Patient::getCenterLines
//! \return
//!
QList<QString> Patient::getCenterLines(){
    return vesselNames.keys();
}

//! ------------------------------------------------------------------------------------------------------------------
//!
//! \brief Patient::getCenterLineFolderPath
//! \return
//!
QString Patient::getCenterLineFolderPath(){
    return this->centerLineFolderPath;
}

//! ------------------------------------------------------------------------------------------------------------------
//!
//! \brief Patient::getPoints
//! \return
//!
vtkPoints *Patient::getCenterLinePoints(){
    return this->centerLinePoints;
}

//! ------------------------------------------------------------------------------------------------------------------
//!
//! \brief Patient::getCenterLinePointsCount
//! \return
//!
int Patient::getCenterLinePointsCount(){
    return this->centerLinePointsCount;
}

//!-------------------------------------------------------------------------------------------------------------------
//!
//! \brief Patient::vtkImageDataToQImage
//! \param imageData
//! \return
//!
QImage Patient::vtkImageDataToQImage(vtkImageData* imageData){
  if (!imageData){
    return QImage();
  }

  /// \todo retrieve just the UpdateExtent
  int width = imageData->GetDimensions()[0];
  int height = imageData->GetDimensions()[1];

  QImage image(width, height, QImage::Format_RGB32);

  QRgb* rgbPtr = reinterpret_cast<QRgb*>(image.bits()) + width * (height-1);

  unsigned char* colorsPtr = reinterpret_cast<unsigned char*>(imageData->GetScalarPointer());
  // mirror vertically

  for(int row = 0; row < height; ++row){
    for (int col = 0; col < width; ++col){
      // Swap rgb
      *(rgbPtr++) = QColor(colorsPtr[0], colorsPtr[1], colorsPtr[2]).rgb();
      colorsPtr +=  3;
    }
        rgbPtr -= width * 2;
    }
  return image;
}

//--------------------------------------------------------------------------------------------------------------------------------
//!
//! \brief Patient::pushCTImage
//! \param img
//!
void Patient::pushCTImage(BidimensionnelImage *img){
    this->cImageSequence.append(img);
    this->cImageSequenceForDisplay.append(this->vtkImageDataToQImage(img->getImage()));
}

//--------------------------------------------------------------------------------------------------------------------------------
//!
//! \brief Patient::fetchCTImage
//! \return
//!
QImage Patient::fetchLatestCTImageForDisplay(){

    this->cImageSequence.takeLast();
    return this->cImageSequenceForDisplay.takeLast();
}

//--------------------------------------------------------------------------------------------------------------------------------
//!
//! \brief Patient::getCTImageSequenceLength
//! \return
//!
int Patient::getCTImageSequenceLength(){
    return this->cImageSequence.size();
}

//--------------------------------------------------------------------------------------------------------------------------------
//!
//! \brief Patient::getGrayScaleValueByIndex
//! \return
//!
int Patient::getGrayScaleValueByIndex(int index, QString option){
    int grayScaleValue = 0;
     if(option == "opacity"){
         grayScaleValue =  this->opacityTransferPoints[index]->getAbscissaValue();
     }
     else if(option == "gradient"){
         grayScaleValue = this->gradientTransferPoints[index]->getAbscissaValue();
     }
     else{
         grayScaleValue = this->colorTransferPoints[index]->getAbscissaValue();
     }

     return grayScaleValue;

}

//--------------------------------------------------------------------------------------------------------------------------------
//!
//! \brief Patient::updateOpacityPoint
//! \param index
//! \param abscissa
//! \param ordinate
//!
void Patient::updateOpacityPoint(int index, int abscissa, double ordinate){
    this->opacityTransferPoints[index]->setAbscissaValue(abscissa);
    this->opacityTransferPoints[index]->setOrdinateValue(ordinate);
}

//--------------------------------------------------------------------------------------------------------------------------------
//!
//! \brief Patient::updateGradientPoint
//! \param index
//! \param abscissa
//! \param ordinate
//!
void Patient::updateGradientPoint(int index, int abscissa, double ordinate){
    this->gradientTransferPoints[index]->setAbscissaValue(abscissa);
    this->gradientTransferPoints[index]->setOrdinateValue(ordinate);
}

//--------------------------------------------------------------------------------------------------------------------------------
//!
//! \brief Patient::updateColorPoint
//! \param index
//! \param abscissa
//! \param ordinate
//!
void Patient::updateColorPoint(int index, int abscissa){
    this->colorTransferPoints[index]->setAbscissaValue(abscissa);
}

//--------------------------------------------------------------------------------------------------------------------------------
//!
//! \brief Patient::getOpacityValueByIndex
//! \return
//!
double Patient::getOpacityValueByIndex(int index){
    return this->opacityTransferPoints[index]->getOrdinateValue();
}

//--------------------------------------------------------------------------------------------------------------------------------
//!
//! \brief Patient::getGradientValueByIndex
//! \param index
//! \return
//!
double Patient::getGradientValueByIndex(int index){
    return this->gradientTransferPoints[index]->getOrdinateValue();
}

//--------------------------------------------------------------------------------------------------------------------------------
//!
//! \brief Patient::getRedValueByIndex
//! \param index
//! \return
//!
double Patient::getRedValueByIndex(int index){
    return this->colorTransferPoints[index]->getRedValue();
}

//--------------------------------------------------------------------------------------------------------------------------------
//!
//! \brief Patient::getGreenValueByIndex
//! \param index
//! \return
//!
double Patient::getGreenValueByIndex(int index){
    return this->colorTransferPoints[index]->getGreenValue();
}

//--------------------------------------------------------------------------------------------------------------------------------
//!
//! \brief Patient::getBlueValueByIndex
//! \param index
//! \return
//!
double Patient::getBlueValueByIndex(int index){
    return this->colorTransferPoints[index]->getBlueValue();
}

//--------------------------------------------------------------------------------------------------------------------------------
//!
//! \brief Patient::findPointInTolerentArea
//! \param abscissa
//! \param ordinate
//! \param transformationFormat
//! \return
//!
int Patient::findPointInTolerentArea(double abscissa, double ordinate, QString transformationFormat){
    if(transformationFormat == "opacity"){
        for(unsigned char cpt = 0;  cpt < this->opacityTransferPoints.size(); cpt++){
            if(qAbs(abscissa - this->opacityTransferPoints[cpt]->getAbscissaValue())<15&&qAbs(ordinate - this->opacityTransferPoints[cpt]->getOrdinateValue())<0.1){
                return cpt;
            }
        }
    }
    else if(transformationFormat == "color"){
        for(unsigned char cpt = 0;  cpt < this->colorTransferPoints.size(); cpt++){
            if(qAbs(abscissa - this->colorTransferPoints[cpt]->getAbscissaValue())<10){
                return cpt;
            }
        }
    }
    else{
        for(unsigned char cpt = 0;  cpt < this->gradientTransferPoints.size(); cpt++){
            if(qAbs(abscissa - this->gradientTransferPoints[cpt]->getAbscissaValue())<15&&qAbs(ordinate - this->gradientTransferPoints[cpt]->getOrdinateValue())<0.1){
                return cpt;
            }

        }
    }
    return -1;
}

//--------------------------------------------------------------------------------------------------------------------------------
//!
//! \brief Patient::getOpacityTransferPoints
//! \return
//!
QVector<TransferPoint*> Patient::getOpacityTransferPoints(){
    return this->opacityTransferPoints;
}

//--------------------------------------------------------------------------------------------------------------------------------
//!
//! \brief Patient::getColorTransferPoints
//! \return
//!
QVector<ColorPoint*> Patient::getColorTransferPoints(){
    return this->colorTransferPoints;
}

//--------------------------------------------------------------------------------------------------------------------------------
//!
//! \brief Patient::getColorTransferPointsNumber
//! \return
//!
int Patient::getColorTransferPointsNumber(){
    return this->colorTransferPoints.size();
}

//--------------------------------------------------------------------------------------------------------------------------------
//!
//! \brief Patient::getGradientTransferPoints
//! \return
//!
QVector<TransferPoint*> Patient::getGradientTransferPoints(){
    return this->gradientTransferPoints;
}

//--------------------------------------------------------------------------------------------------------------------------------
//!
//! \brief Patient::setGrayScaleValueFrequencies
//! \param frequencies
//!
void Patient::setGrayScaleValueFrequencies(QVector<HistogramPoint*> frequencies){
    this->frequencies = frequencies;
}

//--------------------------------------------------------------------------------------------------------------------------------
//!
//! \brief Patient::setOpacityTransferPoint
//! \param opacityPoint
//!
void Patient::setOpacityTransferPoint(TransferPoint* opacityPoint){
    this->opacityTransferPoints.append(opacityPoint);

}

//--------------------------------------------------------------------------------------------------------------------------------
//!
//! \brief Patient::setColorTransferPoint
//! \param redPoint
//!
void Patient::setColorTransferPoint(ColorPoint* redPoint){
    //TODO
    this->colorTransferPoints.append(redPoint);
}

//--------------------------------------------------------------------------------------------------------------------------------
//!
//! \brief Patient::setGradientTransferPoint
//! \param opacityPoint
//!
void Patient::setGradientTransferPoint(TransferPoint* gradientPoint){
    this->gradientTransferPoints.append(gradientPoint);
}

//--------------------------------------------------------------------------------------------------------------------------------
//!
//! \brief Patient::getMaximumGrayscaleValue
//! \return
//!
int Patient::getMaximumGrayscaleValue(){
    return this->maximumGrayScaleValue;
}

//--------------------------------------------------------------------------------------------------------------------------------
//!
//! \brief Patient::getMinimumGrayscaleValue
//! \return
//!
int Patient::getMinimumGrayscaleValue(){
    return this->minimumGrayScaleValue;
}

//--------------------------------------------------------------------------------------------------------------------------------
//!
//! \brief Patient::getMriHistogramfrequencies
//! \return
//!
QVector<HistogramPoint*> Patient::getMriHistogramfrequencies(){
    return this->frequencies;
}

//--------------------------------------------------------------------------------------------------------------------------------
//!
//! \brief Patient::getMriStatisticsList
//! \return
//!
QStringList Patient::getMriStatisticsList(){
    return this->statisticsList;
}

//--------------------------------------------------------------------------------------------------------------------------------
//!
//! \brief Patient::getFirstName
//! \return
//!
QString Patient::getFirstName(){
    return this->firstName;
}

//--------------------------------------------------------------------------------------------------------------------------------
//!
//! \brief Patient::getLastName
//! \return
//!
QString Patient::getLastName(){
    return this->lastName;
}

//! --------------------------------------------------------------------------------------------------------------------------------
//!
//!
//! \brief Patient::getMraImageToBeDisplayed
//! \return
//!
vtkImageData *Patient::getMraImageToBeDisplayed(){
    return this->MriImageForDisplay;
}

//! --------------------------------------------------------------------------------------------------------------------------------
//!
//! \brief Patient::getVesselByName
//! \param name
//! \return
//!
vtkPoints* Patient::getCenterlineByName(QString name){
    return vesselCenterlines[name];
}

//! --------------------------------------------------------------------------------------------------------------------------------
//!
//! \brief Patient::getVesselByName
//! \param name
//! \return
//!
QVector<CenterLinePoint*> Patient::getVesselByName(QString name){
    return vessels[vesselNames[name]];
}

//! -------------------------------------------------------------------------------------------------------------------------------
//!
//! \brief Patient::loadVesselByPath
//! \param path
//!
void Patient::loadVesselByPath(QString filename, QString path){

//    vtkPoints *centerline = vtkPoints::New();
//    centerReader->doReadCenterLineFile(path, centerline);
//    vesselCenterlines[path] = centerline;

    vessels[path] = centerReader->doReadComplteCenterlineFile(path);
    vesselNames[filename] = path;
}

//! --------------------------------------------------------------------------------------------------------------------------------
//!
//! \brief Patient::loadCenterLineFiles
//! \param fileName
//! \return
//!
bool Patient::loadCenterLineFiles(QString fileName){
    CenterLineReader *centerReader = new CenterLineReader();
    centerReader->doReadCenterlineFolder(fileName);
    this->centerLinePointsCount = centerReader->get_vesselsPoints().size();
    for(int i = 0; i < this->centerLinePointsCount; i++){
        centerLinePoints->InsertPoint(i,
                            centerReader->get_vesselsPoints().at(i)->get_abscissa(),
                            centerReader->get_vesselsPoints().at(i)->get_ordinate(),
                            centerReader->get_vesselsPoints().at(i)->get_isometric());
    }
    return true;
}

//! --------------------------------------------------------------------------------------------------------------------------------
//!
//! \brief Patient::loadMRAImageFile
//! \param fileName
//! \return
//!
bool Patient::loadMRAImageFile(const QString &fileName){
    //! file type check
    eFileType ret =  ImageFileInterface::getFileType(fileName);

    if(ret == UNKOWN_FILE_TYPE){
        return false;
    }

    //! get the instance of related reader according to the file type
    ImageFileInterface *fileInterface = ImageFileInterface::getInstanceFileByType(ret);

    //! read the content of the image
    if(fileInterface->readFrom(fileName) != IMAGE_NO_ERRROR){
        return false;
    }

    //!Pass the reference of the image read to the pointer declared upon
    this->setMRAImage(fileInterface->getImage());

    fileInterface->getImageExtentInformation(imageExtents);

    return true;
}

//! -------------------------------------------------------------------------------------------------------------------------------
//!
//! \brief Patient::convertVolumeToUnsignedShort
//! \return
//!
vtkImageData* Patient::convertVolumeToUnsignedShort(vtkImageData* input){
    vtkImageData* ret = NULL;

    //! get input image information
    int* dims_input = input->GetDimensions();
    double* spacing_input = input->GetSpacing();

    //!initialize a new volume data
    ret = vtkImageData::New();
    ret->SetDimensions(dims_input[0], dims_input[1], dims_input[2]);
    ret->SetSpacing(spacing_input[0], spacing_input[1], spacing_input[2]);
    ret->AllocateScalars(VTK_UNSIGNED_SHORT, 1);

    //double* range = input->GetScalarRange();

    // Fill every entry of the image data with "2.0"
    for (int z = 0; z < dims_input[2]; z++){
        for (int y = 0; y < dims_input[1]; y++){
            for (int x = 0; x < dims_input[0]; x++){
                float* pixelOrigin = static_cast<float*>(input->GetScalarPointer(x, y, z));
                unsigned short* pixel = static_cast<unsigned short*>(ret->GetScalarPointer(x, y, z));

                if (x<10 || x>dims_input[0] - 10 || y<10 || y>dims_input[1] - 10 || z<10 || z>dims_input[2] - 10){
                     pixel[0] = 0;
                     continue;
                }

                pixel[0] = vtkMath::Round(pixelOrigin[0] * 1000);

                if(pixel[0]<100 || pixel[0]>450)
                    pixel[0] =0;
            }
        }
    }

    return ret;
}


//! --------------------------------------------------------------------------------------------------------------------------------
//!
//! \brief Patient::setMRAImage
//! \param personnelMRAImage
//!
void Patient::setMRAImage(vtkImageData *personnelMRAImage){
    QString type = QString(personnelMRAImage->GetScalarTypeAsString());

    if(type.contains("float")){
        qDebug()<<"hehe";
        this->MriImageForDisplay = convertVolumeToUnsignedShort(personnelMRAImage);
    }
    else{
        this->MriImageForDisplay = personnelMRAImage;
    }


}

//!---------------------------------------------------------------------------------------------------------------------------------
//!
//! \brief Patient::setImageProcessingFactory
//! \param imageProcessingFactory
//!
void Patient::setImageProcessingFactory(ImageProcessingFactory* imageProcessingFactory){
    this->imageProcessingFactory = imageProcessingFactory;
}

//!---------------------------------------------------------------------------------------------------------------------------------
//!
//! \brief Patient::readFinished
//! \return
//!
bool Patient::readFinished(){
    return this->MraImageReadComplete;
}

//!---------------------------------------------------------------------------------------------------------------------------------
//!
//! \brief Patient::getIdNumber
//! \return
//!
int Patient::getIdNumber(){
    return this->id;
}


//!---------------------------------------------------------------------------------------------------------------------------------
//!
//! \brief Patient::getBirthdayOfPatient
//! \return
//!
QString Patient::getBirthdayOfPatient(){
    return this->birthday;
}

//!---------------------------------------------------------------------------------------------------------------------------------
//!
//! \brief Patient::doImageProcessing
//!
void Patient::doImageFileLecture(){
    imageProcessingThread.patient = this;
    imageProcessingThread.start();
}

//!---------------------------------------------------------------------------------------------------------------------------------
//!
//! \brief Patient::getMagneticTackingDataPath
//! \return
//!
QString Patient::getMagneticTackingDataPath(){
    return this->myPath + "/magnetic_tracking_data/";
}

//!---------------------------------------------------------------------------------------------------------------------------------
//!
//! \brief Patient::getMRAPath
//! \return
//!
QString Patient::getCTImagePath(){
    return this->myPath + "/bidimensionel__images/";
}

//!---------------------------------------------------------------------------------------------------------------------------------
//!
//! \brief Patient::getPhotoPath
//! \return
//!
QString Patient::getPhotoPath(){
    return this->myPhotoPath;
}

//!---------------------------------------------------------------------------------------------------------------------------------
//!
//! \brief Patient::getTridimensionelPath
//! \return
//!
QString Patient::getTridimensionelPath(){
    return this->myPath + "/mra_tridimensionel__image/";
}

//!---------------------------------------------------------------------------------------------------------------------------------
//!
//! \brief getMraTridimensionelPath
//! \return
//!
QString Patient::getMraTridimensionelPath(){
    return this->mraTridimensionelPath;
}

//!---------------------------------------------------------------------------------------------------------------------------------
//!
//! \brief Patient::getName
//! \return
//!
QString Patient::getName(){
    return this->name;
}

//!---------------------------------------------------------------------------------------------------------------------------------
//!
//! \brief Patient::getSex
//! \return
//!
QString Patient::getSex(){
    return this->sex;
}

//!---------------------------------------------------------------------------------------------------------------------------------
//!
//! \brief Patient::getOriginImage
//! \return
//!
IgssImage *Patient::getOriginImage(){
    return this->originImage;
}

//! -------------------------------------------------------------------------------------------------------
//!
//! \brief Patient::getLatestProgressSpeed
//! \return
//!
bool Patient::getLatestProgressSpeed(double *speed){
    int len = this->omegaPositionSequence.length();
    if(len>2){
        double  current_x = this->omegaPositionSequence.at(len-1)->getPositionX();
        double  current_y = this->omegaPositionSequence.at(len-1)->getPositionY();
        double  current_z = this->omegaPositionSequence.at(len-1)->getPositionZ();

        this->omegaPositionSequence.pop_back();

        double zero_x = this->omegaPositionSequence.at(0)->getPositionX();
        double zero_y = this->omegaPositionSequence.at(0)->getPositionY();
        double zero_z = this->omegaPositionSequence.at(0)->getPositionZ();

        speed[0] = (current_x-zero_x)*1000;
        speed[1] = (current_y-zero_y)*1000;
        speed[2] = (current_z-zero_z)*1000;
        return true;
    }
    else{
        return false;
    }
}

//! -------------------------------------------------------------------------------------------------------
//!
//! \brief igtDataware::clearAllBuffers
//!
void Patient::clearAllBuffers(){
    this->omegaPositionSequence.clear();
    this->progressCommandsSequence.clear();
    this->rotateCommandsSequence.clear();
}

//! -------------------------------------------------------------------------------------------------------
//!
//! \brief igtDataware::getProgressCommandNumber
//! \return
//!
int Patient::getProgressCommandNumber(){
    int ret;
    progressMutex.lock();
    ret = progressCommandsSequence.size();
    progressMutex.unlock();
    return ret;
}

//! -------------------------------------------------------------------------------------------------------
//!
//! \brief igtDataware::getTotateCommandNumber
//! \return
//!
int Patient::getRotateCommandNumber(){
    int ret;
    rotateMutex.lock();
    ret = rotateCommandsSequence.size();
    rotateMutex.unlock();
    return ret;
}

//! -------------------------------------------------------------------------------------------------------
//!
//! \brief igtDataware::appendProgressCommand
//! \param currentRpmValue
//! \param currentSymbol
//! \param currentMotorType
//!
void Patient::appendProgressCommand(int currentRpmValue, int currentSymbol, int currentMotorType){
    if (this->echantionnage == false){
        return;
    }
    igtMotorParameter* motorParameter = new igtMotorParameter();
    motorParameter->setRPM(currentRpmValue);
    motorParameter->setSymbol(currentSymbol);
    motorParameter->setMotorType(currentMotorType);

    progressMutex.lock();
    progressCommandsSequence.append(motorParameter);
    progressMutex.unlock();
}

//! -------------------------------------------------------------------------------------------------------
//!
//! \brief Patient::appendMovingCommand
//! \param speed
//! \param valeur
//! \param unit
//!
void Patient::appendMovingCommand(int speed, int valeur, int unit){

}

//! -------------------------------------------------------------------------------------------------------
//!
//! \brief Patient::getCloseSessionCommandNumber
//! \return
//!
int Patient::getCloseSessionCommandNumber(){
    int ret;
    closeSessionMutex.lock();
    ret = closeSessionCommandsSequence.size();
    closeSessionMutex.unlock();
    return ret;
}

//! -------------------------------------------------------------------------------------------------------
//!
//! \brief Patient::getInjectionCommandNumber
//! \return
//!
int  Patient::getInjectionCommandNumber(){
    int ret;
    injectionMutex.lock();
    ret = injectionCommandsSequence.size();
    injectionMutex.unlock();
    return ret;
}

//! -------------------------------------------------------------------------------------------------------
//!
//! \brief Patient::appendCloseSessionCommand
//!
void Patient::appendCloseSessionCommand(){
    if (this->echantionnage == false){
        return;
    }
    igtCloseSessionMessage* closeSessionMessage = new igtCloseSessionMessage();

    closeSessionMutex.lock();
    closeSessionCommandsSequence.append(closeSessionMessage);
    closeSessionMutex.unlock();
}

//! -------------------------------------------------------------------------------------------------------
//!
//! \brief Patient::appendInjectionCommand
//! \param volume
//! \param speed
//!
void Patient::appendInjectionCommand(double volume, double speed){

    if (this->echantionnage == false){
        return;
    }
    igtContrastMediaPush* contrastMediaPush = new igtContrastMediaPush();
    contrastMediaPush->setVolume(volume);
    contrastMediaPush->setSpeed(speed);

    injectionMutex.lock();
    injectionCommandsSequence.append(contrastMediaPush);
    injectionMutex.unlock();
}

//! -------------------------------------------------------------------------------------------------------
//!
//! \brief igtDataware::appendRotateCommand
//! \param currentRpmValue
//! \param currentSymbol
//! \param currentMotorType
//!
void Patient::appendRotateCommand(int currentRpmValue, bool currentSymbol, int currentMotorType){
    if (this->echantionnage == false){
        return;
    }
    igtMotorParameter* motorParameter = new igtMotorParameter();
    motorParameter->setRPM(currentRpmValue);
    //motorParameter->setRound(round);
    motorParameter->setSymbol(currentSymbol);
    motorParameter->setMotorType(currentMotorType);

    rotateMutex.lock();
    rotateCommandsSequence.append(motorParameter);
    rotateMutex.unlock();
}

//! --------------------------------------------------------------------------------------------------------
//!
//! \brief Patient::progressCommandsSequenceIsEmpty
//! \return
//!
bool Patient::progressCommandsSequenceIsEmpty(){
    int ret;
    this->progressMutex.lock();
    ret = this->progressCommandsSequence.size();
    this->progressMutex.unlock();
    if(ret != 0){
        return false;
    }
    return true;
}

//! -------------------------------------------------------------------------------------------------------
//!
//! \brief Patient::closeSessionCommandSequenceIsEmpty
//! \return
//!
bool Patient::closeSessionCommandSequenceIsEmpty(){
    int ret;
    this->closeSessionMutex.lock();
    ret = this->closeSessionCommandsSequence.size();
    this->closeSessionMutex.unlock();
    if(ret != 0){
        return false;
    }
    return true;
}

//! --------------------------------------------------------------------------------------------------------
//!
//! \brief Patient::injectionCommandsSequenceIsEmpty
//! \return
//!
bool Patient::injectionCommandsSequenceIsEmpty(){
    int ret;
    this->injectionMutex.lock();
    ret = this->injectionCommandsSequence.size();
    this->injectionMutex.unlock();
    if(ret != 0){
        return false;
    }
    return true;
}

//! --------------------------------------------------------------------------------------------------------
//!
//! \brief Patient::rotateCommandsSequenceIsEmpty
//! \return
//!
bool Patient::rotateCommandsSequenceIsEmpty(){
    int ret;
    this->rotateMutex.lock();
    ret = this->rotateCommandsSequence.size();
    this->rotateMutex.unlock();
    if(ret != 0){
        return false;
    }
    return true;
}

//! --------------------------------------------------------------------------------------------------------
//!
//! \brief Patient::fetchProgressMotorParameter
//! \return
//!
igtMotorParameter *Patient::fetchProgressMotorParameter(){
    igtMotorParameter *ret;
    this->progressMutex.lock();
    if(this->progressCommandsSequence.length() == 0){
        return NULL;
    }
    ret = this->progressCommandsSequence[0];
    this->progressMutex.unlock();
    return ret;
}

//! --------------------------------------------------------------------------------------------------------
//!
//! \brief Patient::removeFirstProgressMotorParameter
//!
void Patient::removeFirstProgressMotorParameter(){
    this->progressMutex.lock();
    if(this->progressCommandsSequence.length() == 0){
        return;
    }
    this->progressCommandsSequence.removeFirst();
    this->progressMutex.unlock();
}

//! --------------------------------------------------------------------------------------------------------
//!
//! \brief Patient::fetchCloseSessionMessage
//! \return
//!
igtCloseSessionMessage *Patient::fetchCloseSessionMessage(){
    igtCloseSessionMessage *ret;
    this->closeSessionMutex.lock();
    if(this->closeSessionCommandsSequence.length() == 0){
        return NULL;
    }
    ret = this->closeSessionCommandsSequence[0];
    this->closeSessionMutex.unlock();
    return ret;
}

//! ---------------------------------------------------------------------------------------------------------
//!
//! \brief Patient::fetchInjectionCommand
//! \return
//!
igtContrastMediaPush *Patient::fetchInjectionCommand(){
    igtContrastMediaPush *ret;
    this->injectionMutex.lock();
    if(this->injectionCommandsSequence.length() == 0){
        return NULL;
    }
    ret = this->injectionCommandsSequence[0];
    this->injectionMutex.unlock();
    return ret;
}

//! --------------------------------------------------------------------------------------------------------
//!
//! \brief Patient::fetchRotateMotorParameter
//! \return
//!
igtMotorParameter *Patient::fetchRotateMotorParameter(){
    igtMotorParameter *ret;
    this->rotateMutex.lock();
    if(this->rotateCommandsSequence.length() == 0){
        return NULL;
    }
    ret = this->rotateCommandsSequence[0];
    this->rotateMutex.unlock();
    return ret;
}

//! ---------------------------------------------------------------------------------------------------------
//!
//! \brief Patient::removeFirstInjectionCommand
//!
void Patient::removeFirstInjectionCommand(){
    this->injectionMutex.lock();
    if(this->injectionCommandsSequence.length() == 0){
        return;
    }
    this->injectionCommandsSequence.removeFirst();
    this->injectionMutex.unlock();
}

//! ---------------------------------------------------------------------------------------------------------
//!
//! \brief Patient::removeFirstCloseSessionMessage
//!
void Patient::removeFirstCloseSessionMessage(){
    this->closeSessionMutex.lock();
    if(this->closeSessionCommandsSequence.length() == 0){
        return;
    }
    this->closeSessionCommandsSequence.removeFirst();
    this->closeSessionMutex.unlock();
}

//! --------------------------------------------------------------------------------------------------------
//!
//! \brief Patient::removeFirstRotateMotorParameter
//!
void Patient::removeFirstRotateMotorParameter(){
    this->rotateMutex.lock();
    if(this->rotateCommandsSequence.length() == 0){
        return;
    }
    this->rotateCommandsSequence.removeFirst();
    this->rotateMutex.unlock();
}

//! --------------------------------------------------------------------------------------------------------
//!
//! \brief Patient::appendAdvancementTargetVelocity
//! \param advancementTargetVelocity
//!
void Patient::appendAdvancementTargetVelocity(igtAdvancementTargetVelocity* advancementTargetVelocity){
    if(advancementTargetVelocity == NULL){
        return;
    }
    this->AdvancementTargetVelocityMutex.lock();
    this->advancementTargetVelocitySequence.append(advancementTargetVelocity);
    this->AdvancementTargetVelocityMutex.unlock();
    return;
}

//! --------------------------------------------------------------------------------------------------------
//!
//! \brief Patient::getAdvancementTargetVelocityNumber
//! \return
//!
int Patient::getAdvancementTargetVelocityNumber(){
    int ret;
    this->AdvancementTargetVelocityMutex.lock();
    ret = this->advancementTargetVelocitySequence.length();
    this->AdvancementTargetVelocityMutex.unlock();
    return ret;
}

//! --------------------------------------------------------------------------------------------------------
//!
//! \brief Patient::AdvancementTargetVelocitySequenceIsEmpty
//! \return
//!
bool Patient::AdvancementTargetVelocitySequenceIsEmpty(){
    int ret;
    this->AdvancementTargetVelocityMutex.lock();
    ret = this->advancementTargetVelocitySequence.size();
    this->AdvancementTargetVelocityMutex.unlock();
    if(ret != 0){
        return false;
    }
    return true;
}

//! --------------------------------------------------------------------------------------------------------
//!
//! \brief Patient::fetchAdvancementTargetVelocity
//! \return
//!
igtAdvancementTargetVelocity *Patient::fetchAdvancementTargetVelocity(){
    igtAdvancementTargetVelocity *advancementTargetVelocity;
    this->AdvancementTargetVelocityMutex.lock();
    if(this->advancementTargetVelocitySequence.length() == 0){
        return NULL;
    }
    advancementTargetVelocity = this->advancementTargetVelocitySequence[0];
    this->AdvancementTargetVelocityMutex.unlock();
    return advancementTargetVelocity;
}

//! --------------------------------------------------------------------------------------------------------
//!
//! \brief Patient::removeFirstAdvancementTargetVelocity
//!
void Patient::removeFirstAdvancementTargetVelocity(){
    this->AdvancementTargetVelocityMutex.lock();
    if(this->advancementTargetVelocitySequence.length() == 0){
        return;
    }
    this->advancementTargetVelocitySequence.removeFirst();
    this->AdvancementTargetVelocityMutex.unlock();
    return;
}

//! --------------------------------------------------------------------------------------------------------
//!
//! \brief Patient::setOmegaPosition
//! \param px
//! \param py
//! \param pz
//!
void Patient::setOmegaPosition(double px, double py, double pz){
    //qDebug()<<"first"<<px<<py<<pz;
    OmegaPosition *omegaPosition = new OmegaPosition(px, py, pz);
    this->omegaPositionSequence.append(omegaPosition);
    return;
}

//! --------------------------------------------------------------------------------------------------------
//!
//! \brief Patient::appendForceTorqueValues
//! \param forceTorqueValues
//!
void Patient::appendForceTorqueValues(igtForceTorqueValues* forceTorqueValues){
    if(forceTorqueValues == NULL){
        return;
    }
    ForceTorqueValuesMutex.lock();
    forceTorqueValuesSequence.append(forceTorqueValues);
    ForceTorqueValuesMutex.unlock();
    return;
}

//! --------------------------------------------------------------------------------------------------------
//!
//! \brief Patient::getAForceTorqueNumber
//! \return
//!
int Patient::getForceTorqueNumber(){
    int ret;
    ForceTorqueValuesMutex.lock();
    ret = forceTorqueValuesSequence.length();
    ForceTorqueValuesMutex.unlock();
    return ret;
}

//! --------------------------------------------------------------------------------------------------------
//!
//! \brief Patient::forceTorqueSequenceIsEmpty
//! \return
//!
bool Patient::forceTorqueSequenceIsEmpty(){
    bool ret;
    ForceTorqueValuesMutex.lock();
    ret = forceTorqueValuesSequence.length();
    ForceTorqueValuesMutex.unlock();
    if(ret != 0){
        return false;
    }
    return true;
}

//! --------------------------------------------------------------------------------------------------------
//!
//! \brief Patient::fetchForceTorqueValues
//! \return
//!
igtForceTorqueValues *Patient::fetchForceTorqueValues(){
    if(this->forceTorqueValuesSequence.length() == 0){
        return NULL;
    }

    igtForceTorqueValues *forceTorqueValues;
    ForceTorqueValuesMutex.lock();
    forceTorqueValues = forceTorqueValuesSequence[0];
    ForceTorqueValuesMutex.unlock();
    return forceTorqueValues;
}

//! --------------------------------------------------------------------------------------------------------
//!
//! \brief Patient::removeFirstForceTorqueValues
//!
void Patient::removeFirstForceTorqueValues(){
    if(this->forceTorqueSequenceIsEmpty()){
        return;
    }

    ForceTorqueValuesMutex.lock();
    forceTorqueValuesSequence.removeFirst();
    ForceTorqueValuesMutex.unlock();
}

//!---------------------------------------------------------------------------------------------------------
//!
//! \brief Patient::appendNDIPosition
//! \param NDIPosition
//!
void Patient::appendGuidewireMovingDistanceSequence(int NDIPosition){
    if(NDIPosition == 0){
        return;
    }
    guidewireMovingDistanceMutex.lock();
    guidewireMovingDistanceSequence.append(NDIPosition);
    guidewireMovingDistanceMutex.unlock();
    return;
}

//! ----------------------------------------------------------------------------------------
//!
//! \brief Patient::getNDIPositionSequenceNumber
//! \return
//!
int Patient::getGuidewireMovingDistanceSequenceNumber(){
    int ret;
    guidewireMovingDistanceMutex.lock();
    ret = guidewireMovingDistanceSequence.length();
    guidewireMovingDistanceMutex.unlock();
    return ret;
}

//! ----------------------------------------------------------------------------------------
//!
//! \brief Patient::NDIPositionSequenceIsEmpty
//! \return
//!
bool Patient::GuidewireMovingDistanceSequenceIsEmpty(){
    bool ret;
    guidewireMovingDistanceMutex.lock();
    ret = guidewireMovingDistanceSequence.length();
    guidewireMovingDistanceMutex.unlock();
    if(ret != 0){
        return false;
    }
    return true;
}

double Patient::fetchGuidewireMovingDistance(){
    if(this->guidewireMovingDistanceSequence.length() == 0){
        return NULL;
    }

    double NDIPosition;
    guidewireMovingDistanceMutex.lock();
    NDIPosition = guidewireMovingDistanceSequence[0];
    guidewireMovingDistanceMutex.unlock();
    return NDIPosition;
}

void Patient::removeGuidewireMovingDistance(){
    guidewireMovingDistanceMutex.lock();
    guidewireMovingDistanceSequence.pop_front();
    guidewireMovingDistanceMutex.unlock();
}

//!---------------------------------------------------------------------------------------------------------
//!
//! \brief Patient::appendNDIPosition
//! \param NDIPosition
//!
void Patient::appendNDIPosition(igtNDIPosition* NDIPosition){
    if(NDIPosition == NULL){
        return;
    }
    NDIPositionMutex.lock();
    NDIPositionSequence.append(NDIPosition);
    NDIPositionMutex.unlock();
    return;
}

//! ----------------------------------------------------------------------------------------
//!
//! \brief Patient::getNDIPositionSequenceNumber
//! \return
//!
int Patient::getNDIPositionSequenceNumber(){
    int ret;
    NDIPositionMutex.lock();
    ret = NDIPositionSequence.length();
    NDIPositionMutex.unlock();
    return ret;
}

//! ----------------------------------------------------------------------------------------
//!
//! \brief Patient::NDIPositionSequenceIsEmpty
//! \return
//!
bool Patient::NDIPositionSequenceIsEmpty(){
    bool ret;
    NDIPositionMutex.lock();
    ret = NDIPositionSequence.length();
    NDIPositionMutex.unlock();
    if(ret != 0){
        return false;
    }
    return true;
}

//! ----------------------------------------------------------------------------------------
//!
//! \brief Patient::fetchNDIPosition
//! \return
//!
igtNDIPosition *Patient::fetchNDIPosition(){
    if(this->NDIPositionSequence.length() == 0){
        return NULL;
    }

    igtNDIPosition *NDIPosition;
    NDIPositionMutex.lock();
    NDIPosition = NDIPositionSequence[0];
    NDIPositionMutex.unlock();
    return NDIPosition;
}

//! -------------------------------------------------------------------------------------------
//!
//! \brief Patient::removeFirstNDIPosition
//!
void Patient::removeFirstNDIPosition(){
//    if(this->NDIPositionSequenceIsEmpty()){
//        return;
//    }

    NDIPositionMutex.lock();
    NDIPositionSequence.pop_front();
    NDIPositionMutex.unlock();
}
