#ifndef DERIVATION_H
#define DERIVATION_H



class Derivation
{
public:
    Derivation();
    void do_3D_Derivation(float sigma);
    void do_2D_Derivation(float sigma);

    void getKernelSize(int *dim);

    float getGaussKernel_xx(int index);
    float getGaussKernel_yy(int index);
    float getGaussKernel_zz(int index);
    float getGaussKernel_xy(int index);
    float getGaussKernel_xz(int index);
    float getGaussKernel_yz(int index);

private:

    int gaussKernel_x_size;
    int gaussKernel_y_size;
    int gaussKernel_z_size;

    float *gaussKernel_xx;
    float *gaussKernel_yy;
    float *gaussKernel_zz;
    float *gaussKernel_xy;
    float *gaussKernel_xz;
    float *gaussKernel_yz;

};

#endif // DERIVATION_H
