#include "SpeedSensor.h"

SpeedSensor::SpeedSensor(const unsigned int & identifier_, const std::string & type_, const std::string & description_, const int & meters_, const double & averageSpeed_,  const double & speedDrift_, const double & injuryProbability_)
    :AbstractSensor(identifier_, type_, description_), meters(meters_), averageSpeed(averageSpeed_), speedDrift(speedDrift_), injuryProbability(injuryProbability_){};
SpeedSensor::SpeedSensor(const SpeedSensor & ob)
    : AbstractSensor(ob), meters(ob.meters), averageSpeed(ob.averageSpeed), speedDrift(ob.speedDrift), injuryProbability(ob.injuryProbability) {};
SpeedSensor::~SpeedSensor() = default;

SpeedSensor * SpeedSensor::clonePattern () const { return new SpeedSensor(*this); };

std::vector<double>  SpeedSensor::simulateData(){
    std::cout << std::fixed << std::setprecision(2);
    std::random_device rd;
    std::mt19937 gen(rd());
    bool flag=false;
    if (averageSpeed == 0) {
        flag=true;
        std::uniform_real_distribution<> speedDis(20.0, 35.0);
        averageSpeed = speedDis(gen);
    }

    double minSpeed = averageSpeed * (1 - (speedDrift / 100));
    double maxSpeed = averageSpeed * (1 + (speedDrift / 100));
    double currentSpeed = std::uniform_real_distribution<>(minSpeed, maxSpeed)(gen);
    std::vector<double> speeds;
    speeds.push_back(currentSpeed);
    std::normal_distribution<> variationDis(0.0, (speedDrift / 100));
    std::uniform_real_distribution<> injuryDis(0.0, 1.0);
    int injuryOccurred = 0;

    if (injuryDis(gen) < (injuryProbability /100)) { injuryOccurred = std::uniform_real_distribution<> (1, (meters/5))(gen); }
    for (int i = 1; i <= meters / 5; ++i) {
        if(injuryOccurred!=1){
            if(injuryOccurred>1) {injuryOccurred--;}
            double variation = variationDis(gen);
            currentSpeed += variation;
            if (currentSpeed <= minSpeed) currentSpeed = minSpeed;
            if (currentSpeed >= maxSpeed) currentSpeed = maxSpeed;
            speeds.push_back(currentSpeed);
        } 
        else 
            speeds.push_back(0);
    }
    if (flag==true) averageSpeed=0;
    return speeds;

};

void SpeedSensor::accept(SensorVisitor & visitor) const {
    visitor.visit(*this);
};

void SpeedSensor::acceptForDialog(SensorVisitor & visitor) {
    visitor.visitForDialog(*this);
};
