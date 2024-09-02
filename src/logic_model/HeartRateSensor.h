#ifndef HEARTRATESENSOR_H
#define HEARTRATESENSOR_H

#include <iostream>
#include <string>
#include <random>
#include <vector>
#include <iomanip>
#include "AbstractSensor.h"

class HeartRateSensor : public AbstractSensor {
public:
    int meters;
    double restingHeartRate;
    double maximumHeartRate;
    double heartRateDrift;
    double injuryProbability;
    HeartRateSensor(const unsigned int &, const std::string &, const std::string & ="", const int & = 800, const double & = 0, const double & = 0, const double & = 5, const double & = 20);
    HeartRateSensor(const HeartRateSensor &);
    ~HeartRateSensor();

    HeartRateSensor * clonePattern() const override;
    std::vector<double> simulateData() override;
    void accept(SensorVisitor & visitor) const override;
    void acceptForDialog( SensorVisitor & visitor) override;
};

#endif