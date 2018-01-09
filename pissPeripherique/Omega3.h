#ifndef OMEGA3_H
#define OMEGA3_H


#include <QDebug>
#include <QThread>

#define _USE_MATH_DEFINES
#include <math.h>

#include "Eigen/Eigen"
using namespace Eigen;

#include "dhdc.h"
#include "drdc.h"

#define KP    100.0
#define KVP    10.0
#define MAXF    4.0
#define KR      0.3
#define KWR     0.02
#define MAXT    0.1
#define KG    100.0
#define KVG     0.5
#define MAXG    1.0

#define REFRESH_INTERVAL  0.8   // sec

#include "Patient.h"


/**
 * @brief The Omega3 class
 *
 *
 *
 *
 *
 */
class Omega3
{

private:
    class AcquisitionTask : public QThread{
        public:
            void run();
            Omega3 *omega;
            bool omegaDetected=false;
            int  done;
    };

public:
    void launch();
    void terminate();
    bool omegaAvailable();
    void setPatientHandling(Patient *patientHandling);

private:
    AcquisitionTask acquisitionTask;
    Patient *patientHandling;

    double p[DHD_MAX_DOF];
    double r[3][3];
    double v[DHD_MAX_DOF];
    double f[DHD_MAX_DOF];
    double normf, normt;
    int    done  = 0;
    double t0    = dhdGetTime ();
    double t1    = t0;
    bool   base  = false;
    bool   wrist = false;
    bool   grip  = false;
    int    count = 0;

    Matrix3d center;
    Matrix3d rotation;

public:
    Omega3();
};

#endif // OMEGA3_H
