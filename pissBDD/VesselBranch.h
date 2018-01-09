#ifndef VESSELBRANCH_H
#define VESSELBRANCH_H

#include <QVector>
#include <VesselRing.h>

class VesselBranch
{
private:
    int level;
    double s[3];
    double e[3];

    QVector <VesselRing*> ringSequence;

public:
    VesselBranch();
};

#endif // VESSELBRANCH_H
