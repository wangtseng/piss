#ifndef VESSELENHANCEMENTFILTER_H
#define VESSELENHANCEMENTFILTER_H

#include "Options.h"
#include "IgssImage.h"
#include "MRAFileReader.h"
#include "Derivation.h"
#include "Convolution.h"
#include "ErrorMessage.h"
#include <QElapsedTimer>


class VesselEnhancementFilter
{
public:
    VesselEnhancementFilter();
    eProcessingErrorCode do_3D_VesselEnhancement(IgssImage *input,IgssImage *output);

private:
    Options *option;
    int sigma[3];
    IgssImage *mraImage;
    MRAFileReader *mraFileReader;
    Derivation* gaussDerivation;
    Convolution* hessionConvolution;
    QElapsedTimer myTimer;

};

#endif // VESSELENHANCEMENTFILTER_H
