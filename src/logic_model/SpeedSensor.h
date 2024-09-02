#ifndef SPEEDSENSOR_H
#define SPEEDSENSOR_H

#include <iostream>
#include <string>
#include <random>
#include <vector>
#include <iomanip>
#include "AbstractSensor.h"


class SpeedSensor: public AbstractSensor {
public:
    int meters;
    double averageSpeed;
    double speedDrift;
    double injuryProbability;
    SpeedSensor(const unsigned int &, const std::string &, const std::string & ="", const int & =800, const double & =0, const double & =5, const double & =20);
    SpeedSensor(const SpeedSensor &);
    ~SpeedSensor();

    SpeedSensor * clonePattern () const override;
    std::vector<double> simulateData () override;
    void accept ( SensorVisitor &) const  override;
    void acceptForDialog( SensorVisitor &) override;
};

#endif