#include "HeartRateSensor.h"

HeartRateSensor::HeartRateSensor(const unsigned int & identifier_, const std::string & type_, const std::string & description_, const int & meters_, const double & restingHeartRate_, const double & maximumHeartRate_, const double & heartRateDrift_, const double & injuryProbability_): AbstractSensor(identifier_, type_, description_), meters(meters_), restingHeartRate(restingHeartRate_), maximumHeartRate(maximumHeartRate_), heartRateDrift(heartRateDrift_), injuryProbability(injuryProbability_) {};
HeartRateSensor::HeartRateSensor(const HeartRateSensor & ob): AbstractSensor(ob), meters(ob.meters), restingHeartRate(ob.restingHeartRate), maximumHeartRate(ob.maximumHeartRate), heartRateDrift(ob.heartRateDrift), injuryProbability(ob.injuryProbability) {};
HeartRateSensor::~HeartRateSensor() = default;

HeartRateSensor * HeartRateSensor::clonePattern() const { return new HeartRateSensor(*this); };

std::vector<double> HeartRateSensor::simulateData() {
    std::cout << std::fixed << std::setprecision(2);
    std::random_device rd;
    std::mt19937 gen(rd());
    bool flag=false;
    if (restingHeartRate == 0) {
        flag=true;
        std::uniform_real_distribution<> restingHeartRateDis(45.0, 85.0);
        restingHeartRate = restingHeartRateDis(gen);
    }
    if (maximumHeartRate == 0) {
        std::uniform_real_distribution<> maximumHeartRateDis(140.0, 190.0);
        maximumHeartRate = maximumHeartRateDis(gen);
    }
    std::uniform_real_distribution<> metersToMaxHeartRateDis(30, 70);
    int metersToMaxHeartRate = metersToMaxHeartRateDis(gen);
    double lowerLimit = maximumHeartRate * (1 - heartRateDrift / 100);
    double upperLimit = maximumHeartRate * (1 + heartRateDrift / 100);
    double currentHeartRate = restingHeartRate;
    double heartRateIncreaseRate = (maximumHeartRate - restingHeartRate) / (metersToMaxHeartRate / 5);
    std::vector<double> heartRates;
    heartRates.push_back(currentHeartRate);
    std::uniform_real_distribution<> injuryDis(0.0, 1.0);
    std::normal_distribution<> variationDis(0.0, maximumHeartRate * (heartRateDrift / 100));
    int injuryOccurred = 0;

    if (injuryDis(gen) < (injuryProbability / 100)) { injuryOccurred = std::uniform_real_distribution<>(1, (meters / 5))(gen); }
    for (int i = 1; i <= meters / 5; ++i) {
        if (injuryOccurred != 1) {
            if (injuryOccurred > 1) {
                injuryOccurred--;
            }
            if (i * 5 <= metersToMaxHeartRate) {
                currentHeartRate += heartRateIncreaseRate;
            } else {
                double variation = variationDis(gen);
                currentHeartRate = maximumHeartRate + variation;

                if (currentHeartRate < lowerLimit) currentHeartRate = lowerLimit;
                if (currentHeartRate > upperLimit) currentHeartRate = upperLimit;
            }
            heartRates.push_back(currentHeartRate);
        } else {
            heartRates.push_back(0);
        }
    }
    if (flag==true) {restingHeartRate=0; maximumHeartRate=0;}
    return heartRates;
}

void HeartRateSensor::accept(SensorVisitor & visitor) const {
    visitor.visit(*this);
};

void HeartRateSensor::acceptForDialog(SensorVisitor & visitor) {
    visitor.visitForDialog(*this);
};
