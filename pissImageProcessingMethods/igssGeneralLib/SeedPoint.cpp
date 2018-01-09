#include "SeedPoint.h"

SeedPoint::SeedPoint()
{

}

void SeedPoint::setAbscissa(int _abs){
    coordinates[0] = _abs;
}

void SeedPoint::setOrdinate(int _ord){
    coordinates[1] = _ord;
}

void SeedPoint::setIsometric(int _iso){
    coordinates[2] = _iso;
}

int SeedPoint::getAbscissa(){
    return coordinates[0];
}

int SeedPoint::getOrdinate(){
    return coordinates[1];
}

int SeedPoint::getIsometric(){
    return coordinates[2];
}
