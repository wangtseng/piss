#ifndef BIDIMENSIONNELIMAGE_H
#define BIDIMENSIONNELIMAGE_H

#include <vtkImageData.h>
#include <IgssImage.h>
#include <vtkSmartPointer.h>
#include <vtkDataArray.h>
#include <vtkPointData.h>
#include <QTime>
#include <QBitArray>
#include <vtkImageViewer2.h>
#include <vtkBMPReader.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkRenderer.h>


class BidimensionnelImage
{
public:
   BidimensionnelImage();

   void setTimeStamp(qint16 timestamp);
   qint16 getTimeStamp();
   void setImage(vtkImageData *img);
   vtkImageData *getImage();

private:
   qint16 timeStamp;
   vtkImageData *img;
   
};

#endif // BIDIMENSIONNELIMAGE_H
