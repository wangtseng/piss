#include "ImageProcessingFactory.h"


/**
 * @brief ImageProcessingFactory::ImageProcessingFactory
 *
 * @description
 *
 */
ImageProcessingFactory::ImageProcessingFactory()
{
    this->vesselEnhancementFilter = new VesselEnhancementFilter();
    this->generalInformationAnalyser = new GeneralInformationAnalyser();
    this->igssVtkImageConverter = new IgssVtkImageConverter();
}

//! ----------------------------------------------------------------------------------------------------------------------------------
//!
//! \brief ImageProcessingFactory::extractBrainCortextFrom
//! \param input
//! \return
//!

bool ImageProcessingFactory::extractBrainCortextFrom(vtkImageData *input, QString type, QString path){
    //!
    bool ret = false;

    //!
    if(type == "wm"){
        vtkSmartPointer<vtkImageCast> castFilter = vtkSmartPointer<vtkImageCast>::New();
        castFilter->SetOutputScalarTypeToShort();

        castFilter->SetInputData(input);
        castFilter->Update();

        VTK2ITKFilterType::Pointer vtkToitk=VTK2ITKFilterType::New();//O
        vtkToitk->SetInput(castFilter->GetOutput());

        CastingFilterType2::Pointer caster2=CastingFilterType2::New();//O-I
        caster2->SetInput(vtkToitk->GetOutput());

        CurvatureFlowImageFilterType::Pointer smoothing=CurvatureFlowImageFilterType::New();
        smoothing->SetInput(caster2->GetOutput());

        connnectedFilterType::Pointer confidenceConnected=connnectedFilterType::New();
        confidenceConnected->SetInput(smoothing->GetOutput());

        smoothing->SetNumberOfIterations( 2 );
        smoothing->SetTimeStep( 0.05 );
        confidenceConnected->SetMultiplier( 2.5 );
        confidenceConnected->SetNumberOfIterations( 5 );
        confidenceConnected->SetInitialNeighborhoodRadius( 2 );
        confidenceConnected->SetReplaceValue( 255 );

        InternalImageType::IndexType index1;
        index1[0] = 118;
        index1[1] = 133;
        index1[2] = 92;
        confidenceConnected->AddSeed( index1 );

        InternalImageType::IndexType index2;
        index2[0] = 63;
        index2[1] = 135;
        index2[2] = 94;
        confidenceConnected->AddSeed( index2 );

        InternalImageType::IndexType index3;
        index3[0] = 63;
        index3[1] = 157;
        index3[2] = 90;
        confidenceConnected->AddSeed( index3 );

        InternalImageType::IndexType index4;
        index4[0] = 111;
        index4[1] = 150;
        index4[2] = 90;
        confidenceConnected->AddSeed( index4 );

        InternalImageType::IndexType index5;
        index5[0] = 111;
        index5[1] = 50;
        index5[2] = 88;
        confidenceConnected->AddSeed( index5 );
        confidenceConnected->Update();

        CastingFilterType1::Pointer caster1=CastingFilterType1::New();//O-I
        caster1->SetInput( confidenceConnected->GetOutput() );

        ITK2VTKFilterType::Pointer filter = ITK2VTKFilterType::New();
        filter->SetInput(caster1->GetOutput());

        WriterType::Pointer writer=WriterType::New();
        writer->SetInput( caster1->GetOutput() );
        writer->SetImageIO(itk::MetaImageIO::New());
        writer->SetFileName(path.toLatin1().data());

        try
        {
            writer->Update();
            cout<<"ok"<<endl;
        }
        catch( itk::ExceptionObject & excep )
        {
            std::cerr << "Exception caught !" << std::endl;
            std::cerr << excep << std::endl;
        }
    }


    return !ret;
}



//! ----------------------------------------------------------------------------------------------------------------------------------
//!
//! \brief ImageProcessingFactory::doProcessingByCommand
//! \param input
//! \param output
//! \param dim
//! \param method
//! \return
//!
eProcessingErrorCode ImageProcessingFactory::doProcessingByCommand(IgssImage *input, IgssImage *output, int dim, QString method){
    eProcessingErrorCode ret = PROCESSING_NO_ERRROR;
    if(dim == 3){
        if(method == "vef"){
            ret = this->vesselEnhancementFilter->do_3D_VesselEnhancement(input,output);
        }
    }
    return ret;
}

//! ----------------------------------------------------------------------------------------------------------------------------------
//!
//! \brief ImageProcessingFactory::getIgssVtkImageConverter
//! \return
//!
IgssVtkImageConverter *ImageProcessingFactory::getIgssVtkImageConverter(){
    return this->igssVtkImageConverter;
}

//! ----------------------------------------------------------------------------------------------------------------------------------
//!
//! \brief ImageProcessingFactory::getGeneralInformationAnalyser
//! \return
//!
GeneralInformationAnalyser* ImageProcessingFactory::getGeneralInformationAnalyser(){
    return this->generalInformationAnalyser;
}

//! ----------------------------------------------------------------------------------------------------------------------------------
//!
//! \brief ImageProcessingFactory::getTotlaNumber
//! \param mraImage
//! \return
//!
long ImageProcessingFactory::getTotlaNumber(vtkImageData* mraImage){
    return this->generalInformationAnalyser->getTotalGrayscaleValueNumber(mraImage);
}

//! ----------------------------------------------------------------------------------------------------------------------------------
//!
//! \brief ImageProcessingFactory::getMean
//! \param mraImage
//! \return
//!
double ImageProcessingFactory::getMean(vtkImageData* mraImage){
    return this->generalInformationAnalyser->getGrayscaleValueMean(mraImage);
}

//! ----------------------------------------------------------------------------------------------------------------------------------
//!
//! \brief ImageProcessingFactory::getHistogramFrom
//! \param mraImage
//! \return
//!
QVector<HistogramPoint*> ImageProcessingFactory::getHistogramFrom(vtkImageData* mraImage){
    return this->getGeneralInformationAnalyser()->extractFrequency(mraImage);
}

//! ----------------------------------------------------------------------------------------------------------------------------------
//!
//! \brief ImageProcessingFactory::getHistogramStatisticsFrom
//! \param mraImage
//! \return
//!
QStringList ImageProcessingFactory::getHistogramStatisticsFrom(vtkImageData* mraImage){
    return this->getGeneralInformationAnalyser()->extractHistogramStatistics(mraImage);
}
