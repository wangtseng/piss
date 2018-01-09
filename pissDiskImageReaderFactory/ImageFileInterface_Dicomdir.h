#ifndef IMAGEFILEINTERFACE_DICOMDIR_H
#define IMAGEFILEINTERFACE_DICOMDIR_H

#include <ImageFileInterface.h>
#include <vtkImageData.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


typedef struct ImagePropertys
{
    int height;
    int width;
    long min;
    long max;
}iproperty;

typedef struct _Tag
{
    unsigned short   group;
    unsigned short   element;
}tag;

typedef enum{
        unknown=0,
        StudyDate,
        ImageTime,
        Manufacturer,
        InstitutionName,
        PatientsName,
        PatientID,
        PatientsWeight,
        SliceThickness,
        Gantry,
        SliceLocation,
        Rows,
        Columns,
        PixelSpacing,
        PixelAspectRatio,
        BitsAllocated,
        BitsStored,
        HighBit,
        PixelRepresentation,
        SmallestImagePixelValue,
        LargestImagePixelValue,
        WindowCenter,
        WindowWidth,
        PixelData,
        FileMetaInformationGroupLength,
        FilesetID,
        FilesetDescriptorFileID,
        SpecificCharacterSetOfFilesetDescriptorFile,
        OffsetOfTheFirstDirectoryRecordOfTheRootDirectoryEntity,
        OffsetOfTheLastDirectoryRecordOfTheRootDirectoryEntity,
        FilesetConsistencyFlag,
        DirectoryRecordSequence,
        OffsetOfTheNextDirectoryRecord,
        RecordInuseFlag,
        OffsetOfReferencedLowerLevelDirectoryEntry,
        DirectoryRecordType,
        PrivateRecordUID,
        ReferencedFileID,
        MRDRDirectoryRecordOffset,
        ReferencedSOPClassUIDInFile,
        ReferencedSOPInstanceUIDInFile,
        ReferencedTransferSyntaxUIDInFile,
        NumberOfReferences,

        FileMetaInformationVersion,
        MediaStorageSOPClassUID,
        MediaStorageSOPInstanceUID,
        TransferSyntaxUID,
        ImplementationClassUID,
        ImplementationVersionName,

        StudyTime,
        Modality,

        SamplesperPixel,
        PhotometricInterpretation,
        AccessionNumber,
        StudyDescription,
        ImageType,

        StudyInstanceUID,
        SeriesInstanceUID,
        StudyID,
        SeriesNumber,
        AcquisitionNumber,
        InstanceNumber,
        SourceApplicationEntityTitle,
        ReferencedRelatedGeneralSOPClassUIDInFile,
        PrivateInformationCreatorUID,
        PrivateInformation,

        InstanceCreationDate,/*0008*/
        InstanceCreationTime,
        SOPClassUID,
        SOPInstanceUID,
        SeriesDate,
        ContentDate,
        SeriesTime,
        ReferringPhysiciansName,
        ManufacturersModelName,
        SpecificCharacterSet,
        IdentifyingGroupLength,
        AcquisitionDate,
        AcquisitionTime,
        StationName,
        InstitutionalDepartmentName,
        PerformingPhysiciansName,
        OperatorsName,
        SeriesDescription,
        InstitutionAddress,
        ReferencedImageSequence,
        ProcedureCodeSequence,
        SourceImageSequence,

        PatientsBirthDate,/*0010*/
        PatientsBirthTime,
        PatientsSex,
        PatientGroupLength,
        PatientsAge,

        ScanOptions,/*0018*/
        KVP,
        DataCollectionDiameter,
        ReconstructionDiameter,
        TableHeight,
        RotationDirection,
        ExposureTime,
        XrayTubeCurrent,
        Exposure,
        ConvolutionKernel,
        PatientPosition,
        AcquisitionGroupLength,
        BodyPartExamined,
        DeviceSerialNumber,
        SpatialResolution,
        DistanceSourcetoDetector,
        DistanceSourcetoPatient,
        GeneratorPower,

        ImagePosition,/*0020*/
        ImageOrientation,
        FrameofReferenceUID,
        PositionReferenceIndicator,
        ImagesinAcquisition,
        RelationshipGroupLength,
        PatientOrientation,
        ImageComments,

        RescaleIntercept,/*0028*/
        RescaleSlope,
        RescaleType,
        PixelPaddingValue,
        PixelIntensityRelationship,

        PixelDataGroupLength,//(7FE0,0000)

        ScanningSequence, /*0018*/
        SequenceVariant,
        MRAcquisitionType,
        SequenceName,
        RepetitionTime,
        EchoTime,
        NumberofAverages,
        ImagingFrequency,
        EchoNumber,
        MagneticFieldStrength,
        SpacingBetweenSlices,
        NumberofPhaseEncodingSteps,
        EchoTrainLength,
        PixelBandwidth,
        SoftwareVersion,
        HeartRate,
        CardiacNumberofImages,
        TriggerWindow,
        ReceiveCoilName,
        AcquisitionMatrix,
        FlipAngle,
        SAR,
        RadiationSetting,
        IntensifierSize,
        PositionerPrimaryAngle,
        PositionerSecondaryAngle,

        CalibrationImage,//0050

        /*0038*/
        AdmissionID,
        AdmittingData,

}datatype;

typedef enum
{
        i16=0,
        i32,
        other
}datasize;


class ImageFileInterface_Dicomdir : public ImageFileInterface
{
public:
    ImageFileInterface_Dicomdir();
    ~ImageFileInterface_Dicomdir();

    //! Virtual methods inherited from the class ImageReaderInterface
    /*virtual*/ eImageErrorCode readFrom(const QString &filepath);
    /*virtual*/ vtkImageData* getImage();
    /*virtual*/ void getImageExtentInformation(int* extent);

private:
    void dicomdirFileAnalyser(const char *filepath);

    FILE *fp;
    FILE *filedir;
    iproperty ip;
    int fileNo;
    char **fileList;
};

#endif // IMAGEFILEINTERFACE_DICOMDIR_H
