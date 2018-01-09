#include "Convolution.h"

Convolution::Convolution(){

}

bool Convolution::do_3D_Convolution(IgssImage *igssImage, Derivation *gaussDerivation){

//    int x, y, z, m, n, l, mm, nn, ll;

//    int kCenterX,kCenterY,kCenterZ;

//    int abscissa_X, ordinate_Y,isometric_Z;

//    int dims[3];
//    int kernelSize[3];
//    float hession_3D[3][3];
//    float hxx = 0.0;
//    float hyy = 0.0;
//    float hzz = 0.0;
//    float hxy = 0.0;
//    float hxz = 0.0;
//    float hyz = 0.0;

//    igssImage->getDimensionSize(dims);
//    gaussDerivation->getKernelSize(kernelSize);

//    if(!igssImage || !gaussDerivation ) return false;
//    if(dims[0] <= 0 || kernelSize[0] <= 0) return false;

//    kCenterX = kernelSize[0] / 2;
//    kCenterY = kernelSize[1] / 2;
//    kCenterZ = kernelSize[2] / 2;

//    for(int voxels = 0; voxels < dims[0] * dims[1] * dims[2]; voxels++){

//        z = (int)(voxels/(dims[0] * dims[1]));
//        y = (int)((voxels%(dims[0] * dims[1]))/dims[1]);
//        x = (int)((voxels%(dims[0] * dims[1]))%dims[1]);

//        for(m = 0; m < kernelSize[2]; m++){
//            mm = kernelSize[2] - 1 - m;
//            for(n = 0; n < kernelSize[1]; n++){
//                nn = kernelSize[1] - 1 - n;
//                for(l = 0; l < kernelSize[0]; l++){
//                    ll = kernelSize[0] - 1 -l;
//                    abscissa_X = x + m - kCenterX;
//                    ordinate_Y = y + n - kCenterY;
//                    isometric_Z = z + l - kCenterZ;

//                    if(abscissa_X >= 0 && abscissa_X <= kernelSize[2] && ordinate_Y >= 0 && ordinate_Y <= kernelSize[1] && isometric_Z >= 0 && isometric_Z <= kernelSize[0]){
//                        hxx += (float)(igssImage->getValueByIndex((isometric_Z * dims[2] + ordinate_Y) * dims[1] + abscissa_X) * gaussDerivation->getGaussKernel_xx((mm * kernelSize[2] + nn) * kernelSize[1] + ll));
//                        hyy += (float)(igssImage->getValueByIndex((isometric_Z * dims[2] + ordinate_Y) * dims[1] + abscissa_X) * gaussDerivation->getGaussKernel_yy((mm * kernelSize[2] + nn) * kernelSize[1] + ll));
//                        hzz += (float)(igssImage->getValueByIndex((isometric_Z * dims[2] + ordinate_Y) * dims[1] + abscissa_X) * gaussDerivation->getGaussKernel_zz((mm * kernelSize[2] + nn) * kernelSize[1] + ll));
//                        hxy += (float)(igssImage->getValueByIndex((isometric_Z * dims[2] + ordinate_Y) * dims[1] + abscissa_X) * gaussDerivation->getGaussKernel_xy((mm * kernelSize[2] + nn) * kernelSize[1] + ll));
//                        hxz += (float)(igssImage->getValueByIndex((isometric_Z * dims[2] + ordinate_Y) * dims[1] + abscissa_X) * gaussDerivation->getGaussKernel_xz((mm * kernelSize[2] + nn) * kernelSize[1] + ll));
//                        hyz += (float)(igssImage->getValueByIndex((isometric_Z * dims[2] + ordinate_Y) * dims[1] + abscissa_X) * gaussDerivation->getGaussKernel_yz((mm * kernelSize[2] + nn) * kernelSize[1] + ll));
//                    }

//                }
//            }
//        }

//        hession_3D[0][0] = hxx;
//        hession_3D[0][1] = hxy;
//        hession_3D[0][2] = hxz;
//        hession_3D[1][0] = hession_3D[0][1];
//        hession_3D[1][1] = hyy;
//        hession_3D[1][2] = hyz;
//        hession_3D[2][0] = hession_3D[0][2];
//        hession_3D[2][1] = hession_3D[1][2];
//        hession_3D[2][2] = hzz;
//    }

    return true;
}


bool Convolution::do_2D_Convolution(IgssImage *input,Derivation *gaussDerivation){
    int i, j, m, n, mm, nn;
    int kCenterX, kCenterY;
    int rowIndex, colIndex;
    int dataSizeX, dataSizeY;
    int kernelSize[2];
    int indims[2];
    float hession_2D[2][2];
    float hxx = 0.0;
    float hyy = 0.0;
    float hxy = 0.0;

    input->getDimensionSize(indims);
    dataSizeX = indims[0];
    dataSizeY = indims[1];

    gaussDerivation->getKernelSize(kernelSize);
    int kernelSizeX = kernelSize[0];
    int kernelSizeY = kernelSize[1];

    // check validity of params
    if(!input) return false;
    if(dataSizeX <= 0 || kernelSize[0] <= 0) return false;

    // find center position of kernel (half of kernel size)
    kCenterX = kernelSizeX / 2;
    kCenterY = kernelSizeY / 2;

    for(int pixel = 0; pixel < dataSizeX * dataSizeY; ++pixel){
        j = (int)(pixel/dataSizeX);
        i = (int)(pixel%dataSizeX);
        for(m=0; m < kernelSizeY; ++m){
            mm = kernelSizeY - 1 - m;
            for(n=0; n < kernelSizeX; ++n){
                nn = kernelSizeX - 1 - n;

                // index of input signal, used for checking boundary
                rowIndex = j + m - kCenterY;
                colIndex = i + n - kCenterX;

                // ignore input samples which are out of bound
                if(rowIndex >= 0 && rowIndex < dataSizeY && colIndex >= 0 && colIndex < dataSizeX){
                    hxx += input->getValueByIndex(dataSizeX * rowIndex + colIndex) * gaussDerivation->getGaussKernel_xx(kernelSizeX * mm + nn);
                    hxy += input->getValueByIndex(dataSizeX * rowIndex + colIndex) * gaussDerivation->getGaussKernel_xy(kernelSizeX * mm + nn);
                    hyy += input->getValueByIndex(dataSizeX * rowIndex + colIndex) * gaussDerivation->getGaussKernel_yy(kernelSizeX * mm + nn);
                }

            }
        }
        hession_2D[0][0] = hxx;
        hession_2D[0][1] = hxy;
        hession_2D[1][0] = hession_2D[0][1];
        hession_2D[1][1] = hyy;
    }
    return true;
}
