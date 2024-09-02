#include "CadenceSensor.h"

CadenceSensor::CadenceSensor(const unsigned int & identifier_, const std::string & type_, const std::string & description_, const int & meters_, const double & averageCadence_, const double & cadenceDrift_, const double & injuryProbability_): AbstractSensor(identifier_, type_, description_), meters(meters_), averageCadence(averageCadence_), cadenceDrift(cadenceDrift_), injuryProbability(injuryProbability_){};
CadenceSensor::CadenceSensor(const CadenceSensor & ob): AbstractSensor(ob), meters(ob.meters), averageCadence(ob.averageCadence), cadenceDrift(ob.cadenceDrift), injuryProbability(ob.injuryProbability){};
CadenceSensor::~CadenceSensor() = default;

CadenceSensor * CadenceSensor::clonePattern () const {return new CadenceSensor(*this);};

std::vector<double> CadenceSensor::simulateData(){
    std::cout << std::fixed << std::setprecision(2);
    std::random_device rd;
    std::mt19937 gen(rd());
    bool flag=false;
    if (averageCadence == 0) {
        flag=true;
        std::uniform_real_distribution<> cadenceDis(150.0, 210.0);
        averageCadence = cadenceDis(gen);
    }
    double minCadence = averageCadence * (1 - (cadenceDrift / 100));
    double maxCadence = averageCadence * (1 + (cadenceDrift / 100));
    double currentCadence = std::uniform_real_distribution<>(minCadence, maxCadence)(gen);
    std::vector<double> cadences;
    cadences.push_back(currentCadence);
    std::normal_distribution<> variationDis(0.0, (cadenceDrift / 100));
    std::uniform_real_distribution<> injuryDis(0.0, 1.0);
    int injuryOccurred = 0;

    if (injuryDis(gen) < (injuryProbability / 100)) { injuryOccurred = std::uniform_real_distribution<>(1, (meters / 5))(gen); }
    for (int i = 1; i <= meters / 5; ++i) {
        if (injuryOccurred != 1) {
            if (injuryOccurred > 1) {
                injuryOccurred--;
            }
            double variation = variationDis(gen);
            currentCadence += variation;
            if (currentCadence <= minCadence) currentCadence = minCadence;
            if (currentCadence >= maxCadence) currentCadence = maxCadence;
            cadences.push_back(currentCadence);
        } else {
            cadences.push_back(0);
        }
    }

    if (flag==true) averageCadence=0;

    return cadences;
}

void CadenceSensor::accept(SensorVisitor & visitor) const {
    visitor.visit(*this);
};

void CadenceSensor::acceptForDialog(SensorVisitor & visitor) {
    visitor.visitForDialog(*this);
};
