#ifndef CADENCESENSOR_H
#define CADENCESENSOR_H

#include <iostream>
#include <string>
#include <random>
#include <vector>
#include <iomanip>
#include "AbstractSensor.h"

class CadenceSensor: public AbstractSensor  {
public:
    int meters;
    double averageCadence;
    double cadenceDrift;
    double injuryProbability;
    CadenceSensor(const unsigned int &, const std::string &, const std::string & ="", const int & =800, const double & =0, const double & =5, const double & =20);
    CadenceSensor(const CadenceSensor &);
    ~CadenceSensor();

    CadenceSensor * clonePattern() const override;
    std::vector<double> simulateData () override;
    void accept ( SensorVisitor &) const override;
    void acceptForDialog( SensorVisitor &) override;
};

#endif