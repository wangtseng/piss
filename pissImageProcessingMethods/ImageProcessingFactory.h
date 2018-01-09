#ifndef IMAGEPROCESSINGFACTORY_H
#define IMAGEPROCESSINGFACTORY_H
#include "VesselEnhancementFilter.h"
#include "ErrorMessage.h"
#include <GeneralInformationAnalyser.h>
#include <QVector>

#include "vtkSmartPointer.h"
#include "vtkImageData.h"

#include <HistogramPoint.h>
#include "IgssVtkImageConverter.h"
#include "itkImageToVTKImageFilter.h"
#include "itkVTKImageToImageFilter.h"
#include<itkConfidenceConnectedImageFilter.h>
#include<itkCurvatureFlowImageFilter.h>
#include<itkCastImageFilter.h>
#include<vtkImageMask.h>
#include<itkImageFileReader.h>
#include<itkImageFileWriter.h>
#include<itkMetaImageIO.h>
#include<itkBinaryBallStructuringElement.h>
#include <vtkImageCast.h>
#include "vtkMetaImageWriter.h"

#include <QByteArray>


const unsigned int Dimension=3;
typedef float InternalPixelType;
typedef itk::Image<InternalPixelType,Dimension> InternalImageType;

typedef unsigned char BinartPixelType;
typedef itk::Image<BinartPixelType,Dimension> BinaryputImageType;

 typedef  itk::ImageFileWriter<  BinaryputImageType  > WriterType;

typedef itk::BinaryBallStructuringElement<BinartPixelType,Dimension> StructuringElementType;

typedef signed short OutputPixelType;
typedef itk::Image<OutputPixelType,Dimension> OutputImageType;

typedef itk::CurvatureFlowImageFilter< InternalImageType, InternalImageType > CurvatureFlowImageFilterType;

typedef itk::ConfidenceConnectedImageFilter<InternalImageType,InternalImageType> connnectedFilterType;


typedef  itk::VTKImageToImageFilter<OutputImageType> VTK2ITKFilterType;
typedef  itk::ImageToVTKImageFilter<BinaryputImageType> ITK2VTKFilterType;

typedef itk::CastImageFilter< InternalImageType, BinaryputImageType > CastingFilterType1;
typedef itk::CastImageFilter< OutputImageType, InternalImageType > CastingFilterType2;



class ImageProcessingFactory
{
public:
    ImageProcessingFactory();
    eProcessingErrorCode doProcessingByCommand(IgssImage *input, IgssImage *output, int dim, QString method);

    long getTotlaNumber(vtkImageData* mraImage);
    double getMean(vtkImageData* mraImage);
    QVector<HistogramPoint*> getHistogramFrom(vtkImageData* mraImage);
    QStringList getHistogramStatisticsFrom(vtkImageData* mraImage);

    GeneralInformationAnalyser* getGeneralInformationAnalyser();

    IgssVtkImageConverter *getIgssVtkImageConverter();

    bool extractBrainCortextFrom(vtkImageData *input, QString type, QString path);

private:
    VesselEnhancementFilter *vesselEnhancementFilter;
    GeneralInformationAnalyser* generalInformationAnalyser;
    IgssVtkImageConverter *igssVtkImageConverter;


};

#endif // IMAGEPROCESSINGFACTORY_H
