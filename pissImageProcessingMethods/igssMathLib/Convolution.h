#ifndef CONVOLUTION_H
#define CONVOLUTION_H

#include "IgssImage.h"
#include "Derivation.h"


class Convolution
{
public:
    Convolution();
    bool do_3D_Convolution(IgssImage *igssImage,Derivation *gaussDerivation);
    bool do_2D_Convolution(IgssImage *input,Derivation *gaussDerivation);

private:
};

#endif // CONVOLUTION_H
